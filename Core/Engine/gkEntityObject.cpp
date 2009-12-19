/*
-------------------------------------------------------------------------------
	This file is part of the Ogre GameKit port.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
	 claim that you wrote the original software. If you use this software
	 in a product, an acknowledgment in the product documentation would be
	 appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
	 misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#include "OgreRoot.h"
#include "OgreRenderSystem.h"
#include "OgreSceneManager.h"
#include "OgreEntity.h"
#include "OgreSceneNode.h"
#include "OgreMaterialManager.h"
#include "OgreMesh.h"
#include "OgreSubMesh.h"

#include "gkEntityObject.h"
#include "gkGameObjectGroup.h"
#include "gkSceneObject.h"
#include "gkEngine.h"
#include "gkLogger.h"
#include "gkUserDefs.h"

using namespace Ogre;


// ----------------------------------------------------------------------------
gkEntityObject::gkEntityObject(gkSceneObject *scene, const String& name, gkManualObjectLoader* loader) :
		gkGameObject(scene, name, GK_ENTITY, loader),
		mEntityProps(), mEntity(0), mAnimProps(), mActive(0), mDefault(0),
		mAnimTime(-1.0), mBlendTime(0.0), mActiveName("")
{
}

// ----------------------------------------------------------------------------
void gkEntityObject::loadImpl(void)
{
	gkGameObject::loadImpl();

	if (mEntity != 0)
		return;

	/// sanity check
	GK_ASSERT(mScene);
	SceneManager *manager= mScene->getManager();
	GK_ASSERT(manager);

	if (mEntityProps.prefab != -1)
		mEntity= manager->createEntity(mName, (SceneManager::PrefabType)mEntityProps.prefab);
	else
	{
		if (mEntityProps.source.empty())
		{
			OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE,
						"No 'source' mesh provided, and no 'prefab' type",
						"gkGameObject::loadImpl");
		}

		mEntity= manager->createEntity(mName, mEntityProps.source);
	}

	if (!mStartPose.empty())
		_resetPose();

	mEntity->setCastShadows(mEntityProps.casts);
	mNode->attachObject(mEntity);


	/// use instances for groups
	if (mInstance != 0)
	{
		/// only do something if more than one object instance
		if (mInstance->getParent()->getInstanceCount() > 1)
		{
			if (!gkHAS_CAPABILITY(RSC_VERTEX_PROGRAM))
			{
				gkLogMessage("Your video card doesn't support batching; gkEntityObject::loadImpl");
				return;
			}

			/// create or retrieve instance
			InstancedGeometry *geom= 0;
			try
			{
				geom= manager->getInstancedGeometry(mEntityProps.source);
			}
			catch (Exception &)
			{
				geom= manager->createInstancedGeometry(mEntityProps.source);
			}
			if (geom) geom->addSceneNode(mNode);
		}
	}
}

// ----------------------------------------------------------------------------
void gkEntityObject::unloadImpl(void)
{
	if (mEntity != 0)
	{
		/// sanity check
		GK_ASSERT(mScene);
		SceneManager *manager= mScene->getManager();
		GK_ASSERT(manager);

		if (!mStartPose.empty())
		{
			_resetPose();
			mDefault = 0;
		}
		if (mNode)
			mNode->detachObject(mEntity);
		manager->destroyEntity(mEntity);
		mEntity= 0;
	}

	gkGameObject::unloadImpl();
}

// ----------------------------------------------------------------------------
void gkEntityObject::_resetPose(void)
{
	if (mActive != 0)
	{
		mActive->setWeight(0.0);
		mActive->setTimePosition(0.0);
		mActive->setEnabled(false);
		mActive= 0;
	}

	Ogre::AnimationStateSet *all_states= mEntity->getAllAnimationStates();

	if (!all_states)
		return;

	if (!mDefault)
	{
		if (all_states->hasAnimationState(mStartPose))
		{
			mDefault= all_states->getAnimationState(mStartPose);
			mDefault->setEnabled(true);
			mDefault->setWeight(1.0);

			// set start frame

			Real frame_pos= gkEngine::getSingleton().getUserDefs().startframe;

			Real anim= gkEngine::getSingleton().getUserDefs().animspeed;
			if (anim > 0.0)
				frame_pos *= Real(1.0) / anim;

			mDefault->setTimePosition(frame_pos);
		}
	}

	ConstEnabledAnimationStateIterator si= all_states->getEnabledAnimationStateIterator();
	while (si.hasMoreElements())
	{
		AnimationState *st= si.peekNext();

		if (st != mDefault)
		{
			st->setWeight(0.0);
			st->setTimePosition(0.0);
			st->setEnabled(false);
			si= all_states->getEnabledAnimationStateIterator();
		}
		else
			si.moveNext();
	}
}

// ----------------------------------------------------------------------------
void gkEntityObject::updateAnimations(void)
{
	if (!mEntity || mAnimProps.anim.empty()) return;
	if (!mEntity->_isAnimated()) return;

	static const Real aoffs= (1.0 / gkEngine::getAnimRate());
	static const Real foffs= gkEngine::getStepRate();

	// change to new state
	if (!mActive || mAnimProps.anim != mActiveName)
	{
		mActiveName= mAnimProps.anim;
		mActive= mEntity->getAnimationState(mAnimProps.anim);

		if (mActive != 0)
		{
			mActive->setEnabled(true);
			mActive->setWeight(mActive == mDefault ? mDefault->getWeight() : 0.0);

			if (mAnimProps.state == gkAnimProperties::APS_LOOP)
				mActive->setLoop(true);
			else
				mActive->setLoop(false);

			mBlendTime= 0.0;
			mAnimTime= mAnimProps.start * aoffs;
			mActive->setTimePosition(mAnimProps.start * foffs);
		}
	}

	if (!mActive)
		return;

	if (mAnimProps.start == mAnimProps.end)
	{
		mBlendTime= 0.0;
		mAnimTime= mAnimProps.start * aoffs;
		_resetPose();
		return;
	}

	mAnimProps.speed= aoffs * (mAnimProps.speed * foffs);

	Ogre::AnimationStateSet *all_states= mEntity->getAllAnimationStates();
	Real weight= mActive->getWeight();
	if (mAnimProps.blend != 0.0)
	{
		if (weight < 1.0)
		{
			mBlendTime += 1.0 / mAnimProps.blend;
			mActive->setWeight(mBlendTime);

			if (mBlendTime < 1.0)
			{
				// disable all others
				ConstEnabledAnimationStateIterator si= all_states->getEnabledAnimationStateIterator();

				while (si.hasMoreElements())
				{
					AnimationState *st= si.peekNext();
					if (st != mActive)
					{
						if (st->getWeight() > 0.0)
						{
							st->setWeight(1.0 - mBlendTime);
							if (st->getWeight() == 0.0)
							{
								st->setWeight(0.0);
								st->setTimePosition(0.0);
								st->setEnabled(false);
								si= all_states->getEnabledAnimationStateIterator();

							}
							else si.moveNext();
						}
						else
						{
							st->setWeight(0.0);
							st->setTimePosition(0.0);
							st->setEnabled(false);
							si= all_states->getEnabledAnimationStateIterator();
						}
					}
					else
						si.moveNext();
				}
			}
		}
		else
		{
			ConstEnabledAnimationStateIterator si= all_states->getEnabledAnimationStateIterator();
			while (si.hasMoreElements())
			{
				AnimationState *st= si.peekNext();
				if (st != mActive)
				{
					st->setWeight(0.0);
					st->setTimePosition(0.0);
					st->setEnabled(false);
					si= all_states->getEnabledAnimationStateIterator();
				}
				else
					si.moveNext();
			}
		}
	}
	else
	{
		mBlendTime= 0.0;
		if (weight < 1.0)
		{
			mActive->setWeight(1.0);

			// disable all others
			ConstEnabledAnimationStateIterator si= all_states->getEnabledAnimationStateIterator();
			while (si.hasMoreElements())
			{
				AnimationState *st= si.peekNext();
				if (st != mActive)
				{
					st->setWeight(0.0);
					st->setTimePosition(0.0);
					st->setEnabled(false);
					si= all_states->getEnabledAnimationStateIterator();
				}
				else
					si.moveNext();
			}
		}
	}

	Real len= (mAnimProps.end - mAnimProps.start) * aoffs;
	if (len > mActive->getLength())
		len= mActive->getLength();


	mAnimTime += mAnimProps.speed;
	if (mAnimProps.state != gkAnimProperties::APS_LOOP)
	{
		if ((mAnimTime > len) || mAnimTime < (mAnimProps.start*aoffs))
		{
			mAnimTime= mActive->getTimePosition();
			return;
		}
	}

	mActive->addTime(mAnimProps.speed);
}
