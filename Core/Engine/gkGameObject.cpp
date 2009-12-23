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
#include "OgreSceneManager.h"
#include "OgreSceneNode.h"
#include "OgreException.h"
#include "OgreEntity.h"
#include "OgreStringConverter.h"

#include "gkGameObject.h"
#include "gkSoundManager.h"
#include "gkSoundObject.h"
#include "gkSceneObject.h"
#include "gkSceneObjectManager.h"
#include "gkEngine.h"
#include "gkLogicManager.h"
#include "gkEntityObject.h"
#include "gkConsole.h"
#include "gkConstraint.h"
#include "gkGameObjectGroup.h"
#include "gkManualLoader.h"

#include "gkLogicTree.h"

#include "btBulletDynamicsCommon.h"

using namespace Ogre;


Ogre::Vector3 toVector3(const btVector3& v)
{
	return Ogre::Vector3(v.x(), v.y(), v.z());
}

btVector3 toVector3(const Ogre::Vector3& v)
{
	return btVector3(v.x, v.y, v.z);
}

btQuaternion toQuat(const Ogre::Quaternion& q)
{
	return btQuaternion(q.x, q.y, q.z, q.w);
}


Ogre::Quaternion toQuat(const btQuaternion& q)
{
	return Ogre::Quaternion(q.w(), q.x(), q.y(), q.z());
}

Ogre::Quaternion OgreGetBulletOrientation(btRigidBody *body)
{
	const btTransform &trans = body->getCenterOfMassTransform();
	btQuaternion q = trans.getRotation();
	return Ogre::Quaternion(q.w(), q.x(), q.y(), q.z());
}

Ogre::Vector3 OgreGetBulletPosition(btRigidBody *body)
{
	const btVector3 &v = body->getCenterOfMassPosition();
	return Ogre::Vector3(v.x(), v.y(), v.z());
}



// ----------------------------------------------------------------------------
gkGameObject::gkGameObject(gkSceneObject *scene, const String& name, gkGameObjectTypes type, gkManualObjectLoader *loader) :
		mName(name), mIsLoaded(false), mLoader(loader),
		mType(type), mBaseProps(), mPhyProps(), mParent(0),
		mSoundObject(0), mScene(scene), mStartPose(StringUtil::BLANK), mNode(0),
		mLogic(0), mActiveLayer(true),
		mGroupRef(0), mInstance(0), mOutOfDate(false),
		mRigidBody(0)
{
}

// ----------------------------------------------------------------------------
gkGameObject::~gkGameObject()
{
	clearVariables();
	destroyConstraints();
}

// ----------------------------------------------------------------------------
gkGameObject *gkGameObject::duplicate(const String &newName)
{
	GK_ASSERT(0 && "TODO Re-Implement Group Loading...");
	return 0;
}

// ---------------------------------------------------------------------------
void gkGameObject::attachLogic(gkLogicTree *tree)
{
	if (!mLogic)
	{
		mLogic= tree;
		if (mLogic)
			mLogic->attachObject(this);
	}
}


// ----------------------------------------------------------------------------
void gkGameObject::_attachToGroup(gkGameObjectGroup *g)
{
	if (mGroupRef != 0)
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE,
		            mName + " is part of another group, '" + mGroupRef->getName() + ";",
		            "gkGameObject::_attachToGroup");
	}

	mGroupRef= g;
}

// ----------------------------------------------------------------------------
void gkGameObject::_attachToGroupInstance(gkGameObjectGroupInstance *g)
{
	if (mInstance != 0)
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE,
		            mName + " is part of another group, '" + mInstance->getName() + ";",
		            "gkGameObject::_attachToGroupInstance");
	}

	mInstance= g;
}

// ----------------------------------------------------------------------------
void gkGameObject::_detachFromGroup(void)
{
	if (mGroupRef != 0)
		mGroupRef->_notifyUnloaded(this);
	mGroupRef= 0;
}

// ----------------------------------------------------------------------------
gkGameObject *gkGameObject::getGroupParent(void)
{
	return mInstance ? mInstance->getOwner() : 0;
}

// ---------------------------------------------------------------------------
void gkGameObject::addConstraint(gkConstraint *c)
{
	GK_ASSERT(c);
	mConstraints.push_back(c);
	if (mScene)
		mScene->_notifyObjectUpdate(this);
}


// ----------------------------------------------------------------------------
void gkGameObject::load(void)
{
	/// simple loading
	if (mIsLoaded)
		return;

	if (mLoader != 0)
	{
		/// call manual loaders
		mLoader->loadObject(this);
		/// load local
		loadImpl();
	}
	else
		loadImpl();

	mIsLoaded= true;
	postLoadImpl();
}

// ----------------------------------------------------------------------------
void gkGameObject::unload(void)
{
	/// simple loading
	if (!mIsLoaded)
		return;

	unloadImpl();
	postUnloadImpl();
	mIsLoaded= false;
}

// ----------------------------------------------------------------------------
void gkGameObject::loadImpl(void)
{
	/// sanity check
	GK_ASSERT(mScene);
	SceneManager *manager= mScene->getManager();
	GK_ASSERT(manager);

	SceneNode *parentNode= 0;
	if (mParent)
	{
		if (!mParent->isLoaded())
			mParent->load();
		parentNode= mParent->mNode;
	}

	// create from parent or root node
	mNode= parentNode ? parentNode->createChildSceneNode(mName) : manager->getRootSceneNode()->createChildSceneNode(mName);

	mNode->setPosition(mBaseProps.position);
	mNode->setOrientation(mBaseProps.orientation);
	mNode->setScale(mBaseProps.scale);
	mNode->showBoundingBox(mBaseProps.showAABB);

	mCState.loc= mBaseProps.position;
	mCState.rot= mBaseProps.orientation;
	mCState.scl= mBaseProps.scale;
	mPState= mCState;

	mNode->setInitialState();
}

// ----------------------------------------------------------------------------
void gkGameObject::postLoadImpl(void)
{
	// tell scene
	mScene->_notifyObjectLoaded(this);
	if (!mConstraints.empty())
		mScene->_notifyObjectUpdate(this);
}

// ----------------------------------------------------------------------------
void gkGameObject::unloadImpl(void)
{
	/// sanity check
	GK_ASSERT(mScene);
	SceneManager *manager= mScene->getManager();
	GK_ASSERT(manager);

	// per loaded object
	clearVariables();

	// destroy gamelogic
	if (mLogic != 0)
	{
		gkLogicManager::getSingleton().destroy(mLogic);
		mLogic= 0;
	}

	_detachFromGroup();
	destroyConstraints();

	if (mNode)
	{
		/// destroy and detach from scene graph
		mNode->detachAllObjects();
		mNode->removeAllChildren();
		manager->destroySceneNode(mNode->getName());
		mNode= 0;
	}
}

// ----------------------------------------------------------------------------
void gkGameObject::_stateUpdated(void)
{
	/// tell scene about this transform
	if (mScene && mNode != 0)
	{
		if (!mOutOfDate) 
		{
			mScene->_addTransformObject(this);
			mOutOfDate= true;
		}

		mPState = mCState;
		mCState.loc= mNode->getPosition();
		mCState.rot= mNode->getOrientation();
		mCState.scl= mNode->getScale();
	}
}


// ----------------------------------------------------------------------------
void gkGameObject::applyTransformState(const gkTransformState& newstate)
{
	if (mNode)
	{
		mNode->setPosition(newstate.loc);
		mNode->setOrientation(newstate.rot);
		mNode->setScale(newstate.scl);

		if (mRigidBody && !mRigidBody->isStaticOrKinematicObject())
		{
			btTransform t;
			t.setIdentity();
			t.setOrigin(toVector3(newstate.loc));
			t.setRotation(toQuat(newstate.rot));
			mRigidBody->setCenterOfMassTransform(t);
		}
	}
}

// ----------------------------------------------------------------------------
void gkGameObject::synchronizeMotion(Real interpol, Real timeStep)
{
	if (mScene && mNode && mOutOfDate)
	{
		Ogre::Vector3 dloc= gkMathUtils::interp(mPState.loc, mCState.loc, interpol);
		Ogre::Vector3 dscl= gkMathUtils::interp(mPState.scl, mCState.scl, interpol);
		Ogre::Quaternion drot= gkMathUtils::interp(mPState.rot, mCState.rot, interpol);

		if (interpol >= 0.9) mOutOfDate = false;
		applyTransformState(gkTransformState(dloc, drot, dscl));
	}
}


// ----------------------------------------------------------------------------
void gkGameObject::update(void)
{
	applyConstraints();
}


// ----------------------------------------------------------------------------
void gkGameObject::applyConstraints(void)
{
	if (!mConstraints.empty())
	{
		gkConstraintListIterator iter= getConstraintIterator();
		while (iter.hasMoreElements())
		{

			gkConstraint *c= iter.getNext();

			/// FIXME, world transforms.
			Matrix4 old= getLocalTransform();
			c->setMatrix(old);
			if (c->update(this)) setTransform(c->getMatrix());
		}
	}
}


// ----------------------------------------------------------------------------
void gkGameObject::destroyConstraints(void)
{
	gkConstraintListIterator iter= getConstraintIterator();
	while (iter.hasMoreElements())
	{
		gkConstraint *c= iter.getNext();
		delete c;
	}

	mConstraints.clear();
}

// ----------------------------------------------------------------------------
void gkGameObject::setTransform(const Matrix4 &v)
{
	if (mNode != 0)
	{
		Quaternion rot;
		Vector3 loc, scl;

		gkMathUtils::extractTransform(v, loc, rot, scl);

		mNode->setPosition(loc);
		mNode->setOrientation(rot);
		mNode->setScale(scl);


		if (mRigidBody != 0)
		{
			if (mRigidBody->isStaticOrKinematicObject())
				return;

			/// apply to the body
			btTransform trans;
			trans.setIdentity();
			trans.setBasis(btMatrix3x3(btQuaternion(rot.x, rot.y, rot.z, rot.w)));
			trans.setOrigin(btVector3(loc.x, loc.y, loc.z));
			mRigidBody->setCenterOfMassTransform(trans);
		}

		_stateUpdated();

	}
}

// ----------------------------------------------------------------------------
void gkGameObject::setPosition(const Vector3 &v)
{
	if (mNode != 0)
	{
		mNode->setPosition(v);

		if (mRigidBody != 0)
		{
			if (mRigidBody->isStaticOrKinematicObject())
				return;

			btTransform trans= mRigidBody->getCenterOfMassTransform();
			trans.setOrigin(btVector3(v.x, v.y, v.z));
			mRigidBody->setCenterOfMassTransform(trans);
		}
		_stateUpdated();
	}
}


// ----------------------------------------------------------------------------
void gkGameObject::setScale(const Vector3 &v)
{
	if (mNode != 0)
	{
		mNode->setScale(v);
		_stateUpdated();
	}
}

// ----------------------------------------------------------------------------
void gkGameObject::setOrientation(const Quaternion& q)
{
	if (mNode != 0)
	{
		mNode->setOrientation(q);

		if (mRigidBody != 0)
		{
			if (mRigidBody->isStaticOrKinematicObject())
				return;

			btTransform trans= mRigidBody->getCenterOfMassTransform();
			btMatrix3x3 matr(btQuaternion(q.x, q.y, q.z, q.w));
			trans.setBasis(matr);
			mRigidBody->setCenterOfMassTransform(trans);
		}
		_stateUpdated();
	}
}

// ----------------------------------------------------------------------------
void gkGameObject::setOrientation(const Vector3& v)
{
	if (mNode != 0)
	{

		Quaternion q= gkMathUtils::getQuatFromEuler(v);
		mNode->setOrientation(q);

		if (mRigidBody != 0)
		{
			if (mRigidBody->isStaticOrKinematicObject())
				return;
			btTransform trans= mRigidBody->getCenterOfMassTransform();
			btMatrix3x3 matr(btQuaternion(q.x, q.y, q.z, q.w));
			trans.setBasis(matr);
			mRigidBody->setCenterOfMassTransform(trans);
		}
		_stateUpdated();
	}
}

// ----------------------------------------------------------------------------
void gkGameObject::rotate(const Vector3 &drot, int tspace)
{
	/// going for (yaw=x, pitch=y, roll=z)
	this->rotate(gkMathUtils::getQuatFromEuler(drot), tspace);
}

// ----------------------------------------------------------------------------
void gkGameObject::rotate(const Quaternion &dq, int tspace)
{

	if (mNode != 0)
	{

		if (mRigidBody != 0)
		{

			Quaternion q= dq;
			switch (tspace)
			{
			case TRANSFORM_LOCAL:
				q= q *OgreGetBulletOrientation(mRigidBody);
				break;
			case TRANSFORM_PARENT:
				q= OgreGetBulletOrientation(mRigidBody)*q;
				break;
			case TRANSFORM_WORLD:
				q= q  *mNode->_getDerivedOrientation().Inverse()  *q  *mNode->_getDerivedOrientation();
				break;
			default:
				break;
			}

			btTransform trans= mRigidBody->getCenterOfMassTransform();
			trans.setBasis(btMatrix3x3(btQuaternion(q.x, q.y, q.z, q.w)));
			mRigidBody->setCenterOfMassTransform(trans);
		}
		mNode->rotate(dq, (Node::TransformSpace)tspace);
		_stateUpdated();
	}
}


// ----------------------------------------------------------------------------
void gkGameObject::translate(const Vector3 &dloc, int tspace)
{
	if (mNode != 0)
	{
		mNode->translate(dloc, (Node::TransformSpace)tspace);

		if (mRigidBody != 0)
		{
			Vector3 loc= dloc;
			switch (tspace)
			{
			case TRANSFORM_LOCAL:
				loc= OgreGetBulletOrientation(mRigidBody)  *loc;
				break;
			case TRANSFORM_WORLD:
			case TRANSFORM_PARENT:
			default:
				break;
			}

			mRigidBody->translate(toVector3(loc));
		}
		_stateUpdated();
	}
}

// ----------------------------------------------------------------------------
void gkGameObject::scale(const Vector3 &dscale)
{
	if (mNode != 0)
	{
		mNode->scale(dscale);
		_stateUpdated();
	}
}

// ----------------------------------------------------------------------------
void gkGameObject::setLinearVelocity(const Vector3 &v, int tspace)
{

	if (mRigidBody != 0)
	{
		if (mRigidBody->isStaticOrKinematicObject())
			return;

		if (v.squaredLength() > GK_EPSILON*GK_EPSILON)
			mRigidBody->activate();

		Vector3 vel;
		switch (tspace)
		{
		case TRANSFORM_LOCAL:
			vel= OgreGetBulletOrientation(mRigidBody)*v;
			break;
		case TRANSFORM_WORLD:
		case TRANSFORM_PARENT:
		default:
			vel= v;
			break;
		}

		mRigidBody->setLinearVelocity(toVector3(vel));
		_stateUpdated();
	}
}

// ----------------------------------------------------------------------------
void gkGameObject::setAngualrVelocity(const Vector3& v, int tspace)
{
	if (mRigidBody != 0)
	{
		if (mRigidBody->isStaticOrKinematicObject())
			return;

		if (v.squaredLength() > GK_EPSILON * GK_EPSILON)
			mRigidBody->activate();

		Vector3 vel;
		switch (tspace)
		{
		case TRANSFORM_LOCAL:
			vel= OgreGetBulletOrientation(mRigidBody) *v;
			break;
		case TRANSFORM_WORLD:
		case TRANSFORM_PARENT:
		default:
			vel= v;
			break;
		}

		mRigidBody->setAngularVelocity(toVector3(vel));

		_stateUpdated();
	}
}

// ----------------------------------------------------------------------------
void gkGameObject::applyTorque(const Vector3 &v, int tspace)
{
	if (mRigidBody != 0)
	{
		if (mRigidBody->isStaticOrKinematicObject())
			return;

		if (v.squaredLength() > GK_EPSILON*GK_EPSILON)
			mRigidBody->activate();


		Vector3 vel;
		switch (tspace)
		{
		case TRANSFORM_LOCAL:
			vel= OgreGetBulletOrientation(mRigidBody)*v;
			break;
		case TRANSFORM_WORLD:
		case TRANSFORM_PARENT:
		default:
			vel= v;
			break;
		}
		mRigidBody->applyTorque(toVector3(vel));

		_stateUpdated();
	}
}

// ----------------------------------------------------------------------------
void gkGameObject::applyForce(const Vector3 &v, int tspace)
{
	if (mRigidBody != 0)
	{
		if (mRigidBody->isStaticOrKinematicObject())
			return;

		if (v.squaredLength() > GK_EPSILON*GK_EPSILON)
			mRigidBody->activate();

		Vector3 vel;
		switch (tspace)
		{
		case TRANSFORM_LOCAL:
			vel= OgreGetBulletOrientation(mRigidBody)*v;
			break;
		case TRANSFORM_WORLD:
		case TRANSFORM_PARENT:
		default:
			vel= v;
			break;
		}

		mRigidBody->applyCentralForce(toVector3(vel));

		_stateUpdated();
	}
}

// ----------------------------------------------------------------------------
Vector3 gkGameObject::getLinearVelocity(void)
{
	if (mRigidBody != 0)
		return toVector3(mRigidBody->getLinearVelocity());
	return Vector3::ZERO;
}

// ----------------------------------------------------------------------------
Vector3 gkGameObject::getAngualrVelocity()
{
	if (mRigidBody != 0)
		return toVector3(mRigidBody->getAngularVelocity());
	return Vector3::ZERO;
}

// ----------------------------------------------------------------------------
void gkGameObject::clearVariables(void)
{
	gkEngine &eng= gkEngine::getSingleton();
	gkVariableMapIterator iter= gkVariableMapIterator(mVariables.begin(), mVariables.end());

	while (iter.hasMoreElements())
	{
		gkVariable *var= iter.getNext();
		eng.removeDebugProperty(var);
		delete var;
	}
	mVariables.clear();
}

// ----------------------------------------------------------------------------
gkVariable *gkGameObject::createVariable(const String &name, bool debug)
{

	gkVariableMap::iterator it= mVariables.find(name);

	if (it != mVariables.end())
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_DUPLICATE_ITEM,
		            "duplicate property",
		            "gkGameObject::createVariable");
	}

	gkVariable *prop= new gkVariable(name, debug);
	mVariables.insert(std::make_pair(name, prop));

	if (debug)
		gkEngine::getSingleton().addDebugProperty(prop);

	return prop;
}

// ----------------------------------------------------------------------------
gkVariable *gkGameObject::getVariable(const String &name)
{
	gkVariableMap::iterator it= mVariables.find(name);
	return it == mVariables.end() ? 0 : it->second;
}


// ----------------------------------------------------------------------------
bool gkGameObject::hasVariable(const String &name)
{
	return mVariables.find(name) != mVariables.end();
}
