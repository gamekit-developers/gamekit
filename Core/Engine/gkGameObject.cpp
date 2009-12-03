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

using namespace Ogre;



// ----------------------------------------------------------------------------
gkGameObject::gkGameObject(gkSceneObject *scene, const String& name, gkGameObjectTypes type, gkManualObjectLoader* loader) :
		mName(name), mIsLoaded(false), mLoader(loader),
		mType(type), mBaseProps(), mPhyProps(), mParent(0),
		mSoundObject(0), mScene(scene), mStartPose(StringUtil::BLANK), mNode(0),
		mLogic(0), mActiveLayer(true),
		mGroupRef(0), mInstance(0), mOutOfDate(false)
{
}

// ----------------------------------------------------------------------------
gkGameObject::~gkGameObject()
{
	clearVariables();
	unloadPhysics();
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

	mCState.pos= mBaseProps.position;
	mCState.rot= mBaseProps.orientation;
	mCState.scl= mBaseProps.scale;
	mPState= mCState;

	mNode->setInitialState();
}

// ----------------------------------------------------------------------------
void gkGameObject::postLoadImpl(void)
{
	// only after loaded
	if (mPhyProps.enable && mNode)
		loadPhysics();

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
	unloadPhysics();
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
void gkGameObject::stateUpdated(void)
{
	gkTransformState cs;
	cs.rot= mNode->getOrientation();
	cs.pos= mNode->getPosition();
	cs.scl= mNode->getScale();

	if (mPTState != cs)
	{
		mOutOfDate= true;
		/// tell scene about this transform
		if (mScene)  mScene->_notifyObjectMoved(this);
	}
	mPTState= cs;
}

// ----------------------------------------------------------------------------
void gkGameObject::synchronizeMotion(Real interpol)
{
	if (mNode != 0)
	{
		static const Real step= gkEngine::getStepRate();
		static const Real fallout_rate= 1.0 / 9.0;

		if (mOutOfDate || !(step < fallout_rate))
		{
			mPState= mCState;
			mOutOfDate= false;
		}

		mCState.rot= mNode->getOrientation();
		mCState.pos= mNode->getPosition();
		mCState.scl= mNode->getScale();

		Vector3 diff_loc= gkMathUtils::calculateLinearVelocity(mPState.pos, mCState.pos,  1.0);
		Vector3 diff_rot= gkMathUtils::calculateAngularVelocity(mPState.rot, mCState.rot, 1.5);
		Vector3 diff_scl= gkMathUtils::calculateLinearVelocity(mPState.scl, mCState.scl,  1.0);

		if (gkFuzzyVec(diff_loc) && gkFuzzyVec(diff_rot) && gkFuzzyVec(diff_scl))
		{
			// short cut, no motion
			return;
		}

		/// scale to current tick
		diff_loc *= step;
		diff_rot *= step;
		diff_scl *= step;

		// find ammount moved
		gkTransformState state= mCState;
		state.pos= mCState.pos + diff_loc * interpol;
		state.scl= mCState.scl + diff_scl * interpol;
		state.rot= gkMathUtils::calculateRotation(mCState.rot, diff_rot, interpol);


		mNode->setPosition(state.pos);
		mNode->setOrientation(state.rot);
		mNode->setScale(state.scl);

#if 0
		if (mRigidBody != 0)
		{
			if (mRigidBody->isStaticObject())
				return;

			// apply to the body
			btTransform trans;
			trans.setIdentity();
			trans.setBasis(btMatrix3x3(btQuaternion(state.rot.x, state.rot.y, state.rot.z, state.rot.w)));
			trans.setOrigin(btVector3(state.pos.x, state.pos.y, state.pos.z));
			mRigidBody->setCenterOfMassTransform(trans);
		}
#endif
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

			if (c->update(this))
				setTransform(c->getMatrix());
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
#if 0

		if (mRigidBody != 0)
		{
			if (mRigidBody->isStaticObject())
				return;

			/// apply to the body
			btTransform trans;
			trans.setIdentity();
			trans.setBasis(btMatrix3x3(btQuaternion(rot.x, rot.y, rot.z, rot.w)));
			trans.setOrigin(btVector3(loc.x, loc.y, loc.z));
			mRigidBody->setCenterOfMassTransform(trans);
		}
#endif
		stateUpdated();

	}
}

// ----------------------------------------------------------------------------
void gkGameObject::setPosition(const Vector3 &v)
{
	if (mNode != 0)
	{
		mNode->setPosition(v);
#if 0

		if (mRigidBody != 0)
		{
			if (mRigidBody->isStaticObject())
				return;

			btTransform trans= mRigidBody->getCenterOfMassTransform();
			trans.setOrigin(btVector3(v.x, v.y, v.z));
			mRigidBody->setCenterOfMassTransform(trans);
		}
#endif
		stateUpdated();
	}
}


// ----------------------------------------------------------------------------
void gkGameObject::setScale(const Vector3 &v)
{
	if (mNode != 0)
	{
		mNode->setScale(v);
#if 0

		if (mRigidBody != 0)
			mRigidBody->setLocalScale(v);
#endif
		stateUpdated();
	}
}

// ----------------------------------------------------------------------------
void gkGameObject::setOrientation(const Quaternion& q)
{
	if (mNode != 0)
	{
		mNode->setOrientation(q);
#if 0

		if (mRigidBody != 0)
		{
			if (mRigidBody->isStaticObject())
				return;

			btTransform trans= mRigidBody->getCenterOfMassTransform();
			btMatrix3x3 matr(btQuaternion(q.x, q.y, q.z, q.w));
			trans.setBasis(matr);
			mRigidBody->setCenterOfMassTransform(trans);
		}
#endif
		stateUpdated();
	}
}

// ----------------------------------------------------------------------------
void gkGameObject::setOrientation(const Vector3& v)
{
	if (mNode != 0)
	{

		Quaternion q= gkMathUtils::getQuatFromEuler(v);
		mNode->setOrientation(q);
#if 0

		if (mRigidBody != 0)
		{
			if (mRigidBody->isStaticObject())
				return;
			btTransform trans= mRigidBody->getCenterOfMassTransform();
			btMatrix3x3 matr(btQuaternion(q.x, q.y, q.z, q.w));
			trans.setBasis(matr);
			mRigidBody->setCenterOfMassTransform(trans);
		}
#endif
		stateUpdated();
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
#if 0
		if (mRigidBody != 0)
		{
			Quaternion q= dq;
			switch (tspace)
			{
			case TRANSFORM_LOCAL:
				q= q * mRigidBody->getCenterOfMassOrientation();
				break;
			case TRANSFORM_PARENT:
				q= mRigidBody->getCenterOfMassOrientation() * q;
				break;
			case TRANSFORM_WORLD:
				q= q * mNode->_getDerivedOrientation().Inverse() * q * mNode->_getDerivedOrientation();
				break;
			default:
				break;
			}

			btTransform trans= mRigidBody->getCenterOfMassTransform();
			trans.setBasis(btMatrix3x3(btQuaternion(q.x, q.y, q.z, q.w)));
			mRigidBody->setCenterOfMassTransform(trans);
		}
#endif
		mNode->rotate(dq, (Node::TransformSpace)tspace);

		stateUpdated();
	}
}


// ----------------------------------------------------------------------------
void gkGameObject::translate(const Vector3 &dloc, int tspace)
{
	if (mNode != 0)
	{
		mNode->translate(dloc, (Node::TransformSpace)tspace);
#if 0

		if (mRigidBody != 0)
		{
			Vector3 loc= dloc;
			switch (tspace)
			{
			case TRANSFORM_LOCAL:
				loc= mRigidBody->getCenterOfMassOrientation() * loc;
				break;
			case TRANSFORM_WORLD:
			case TRANSFORM_PARENT:
			default:
				break;
			}

			mRigidBody->translate(loc);
		}
#endif
		stateUpdated();
	}
}


// ----------------------------------------------------------------------------
void gkGameObject::scale(const Vector3 &dscale)
{
	if (mNode != 0)
	{
		mNode->scale(dscale);
#if 0

		if (mRigidBody != 0)
			mRigidBody->setLocalScale(mNode->getScale());
#endif
		stateUpdated();
	}
}


// ----------------------------------------------------------------------------
void gkGameObject::setLinearVelocity(const Vector3 &v, int tspace)
{
#if 0
	if (mRigidBody != 0)
	{
		if (mRigidBody->isStaticObject())
			return;

		if (v.squaredLength() > GK_EPSILON * GK_EPSILON)
			mRigidBody->activate();

		Vector3 vel;
		switch (tspace)
		{
		case TRANSFORM_LOCAL:
			vel= mRigidBody->getCenterOfMassOrientation() * v;
			break;
		case TRANSFORM_WORLD:
		case TRANSFORM_PARENT:
		default:
			vel= v;
			break;
		}

		mRigidBody->setLinearVelocity(vel);

		stateUpdated();
	}
#endif
}

// ----------------------------------------------------------------------------
void gkGameObject::setAngualrVelocity(const Vector3& v, int tspace)
{
#if 0
	if (mRigidBody != 0)
	{
		if (mRigidBody->isStaticObject())
			return;

		if (v.squaredLength() > GK_EPSILON * GK_EPSILON)
			mRigidBody->activate();

		Vector3 vel;
		switch (tspace)
		{
		case TRANSFORM_LOCAL:
			vel= mRigidBody->getCenterOfMassOrientation() * v;
			break;
		case TRANSFORM_WORLD:
		case TRANSFORM_PARENT:
		default:
			vel= v;
			break;
		}

		mRigidBody->setAngularVelocity(vel);

		stateUpdated();
	}
#endif
}

// ----------------------------------------------------------------------------
void gkGameObject::applyTorque(const Vector3 &v, int tspace)
{
#if 0
	if (mRigidBody != 0)
	{
		if (mRigidBody->isStaticObject())
			return;

		if (v.squaredLength() > GK_EPSILON * GK_EPSILON)
			mRigidBody->activate();


		Vector3 vel;
		switch (tspace)
		{
		case TRANSFORM_LOCAL:
			vel= mRigidBody->getCenterOfMassOrientation() * v;
			break;
		case TRANSFORM_WORLD:
		case TRANSFORM_PARENT:
		default:
			vel= v;
			break;
		}
		mRigidBody->applyTorque(vel);

		stateUpdated();
	}
#endif
}

// ----------------------------------------------------------------------------
void gkGameObject::applyForce(const Vector3 &v, int tspace)
{
#if 0
	if (mRigidBody != 0)
	{
		if (mRigidBody->isStaticObject())
			return;

		if (v.squaredLength() > GK_EPSILON * GK_EPSILON)
			mRigidBody->activate();

		Vector3 vel;
		switch (tspace)
		{
		case TRANSFORM_LOCAL:
			vel= mRigidBody->getCenterOfMassOrientation() * v;
			break;
		case TRANSFORM_WORLD:
		case TRANSFORM_PARENT:
		default:
			vel= v;
			break;
		}

		mRigidBody->applyCentralForce(vel);

		stateUpdated();
	}
#endif
}

// ----------------------------------------------------------------------------
Vector3 gkGameObject::getLinearVelocity(void)
{
#if 0
	if (mRigidBody != 0)
		return mRigidBody->getLinearVelocity();
#endif
	return Vector3::ZERO;
}

// ----------------------------------------------------------------------------
Vector3 gkGameObject::getAngualrVelocity()
{
#if 0
	if (mRigidBody != 0)
		return mRigidBody->getAngularVelocity();
#endif
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
gkVariable* gkGameObject::createVariable(const String &name, bool debug)
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
gkVariable* gkGameObject::getVariable(const String &name)
{
	gkVariableMap::iterator it= mVariables.find(name);
	return it == mVariables.end() ? 0 : it->second;
}


// ----------------------------------------------------------------------------
bool gkGameObject::hasVariable(const String &name)
{
	return mVariables.find(name) != mVariables.end();
}


// ----------------------------------------------------------------------------
void gkGameObject::loadPhysics(void)
{
#if 0
	// TODO, upgrade Ogre Bullet -> Or roll my own
	if (!mPhyProps.enable)
		return;

	unloadPhysics();

	switch (mPhyProps.type)
	{
	case gkPhysicsProperties::SH_BOX:
		mShape= new OgreBulletCollisions::BoxCollisionShape(mPhyProps.size);
		break;
	case gkPhysicsProperties::SH_CONE:
		mShape= new OgreBulletCollisions::ConeCollisionShape(std::max(mPhyProps.size.x, mPhyProps.size.y),
				mPhyProps.size.z * 2.0, Vector3::UNIT_Z);
		break;
	case gkPhysicsProperties::SH_CYL:
		mShape= new OgreBulletCollisions::CylinderCollisionShape(mPhyProps.size, Vector3::UNIT_Z);
		break;
	case gkPhysicsProperties::SH_SMESH:
		{
			if (mType != GK_ENTITY)
				break;

			Entity *ent= getEntity()->getEntity();
			if (ent != 0)
			{

				if (ent->hasSkeleton())
					break;
				else
				{
					OgreBulletCollisions::StaticMeshToShapeConverter conv(ent);
					if (mPhyProps.rigid || mPhyProps.dyn)
						mShape= conv.createConvex();
					else
						mShape= conv.createTrimesh();
					break;
				}
			}
		}
	case gkPhysicsProperties::SH_SPH:
		mShape= new OgreBulletCollisions::SphereCollisionShape(mPhyProps.radius);
		break;
	}

	if (!mShape)
		return;

	mShape->getBulletShape()->setMargin(0.01);


	btRigidBody::btRigidBodyConstructionInfo info(0.0, 0, 0, btVector3(0,0,0));
	info.m_angularDamping=  mPhyProps.adamp;
	info.m_linearDamping=   mPhyProps.ldamp;

	info.m_friction=	 mPhyProps.friction;
	info.m_localInertia=   btVector3(mPhyProps.mass / Real(3.0), mPhyProps.mass / Real(3.0), mPhyProps.mass / Real(3.0));
	info.m_mass=	   mPhyProps.mass;
	info.m_restitution=	mPhyProps.restitution;

	info.m_additionalDamping= true;
	info.m_additionalDampingFactor= info.m_linearDamping * 0.01;
	info.m_additionalAngularDampingFactor= info.m_additionalDampingFactor;
	info.m_additionalAngularDampingThresholdSqr= 0.015;
	info.m_additionalLinearDampingThresholdSqr= info.m_additionalAngularDampingThresholdSqr;

	if (mPhyProps.rigid)
		info.m_localInertia *= (mPhyProps.inertia/0.4);
	else
		info.m_localInertia *= 0.0;

	mRigidBody= new OgreBulletDynamics::RigidBody(mName + "_physics", mScene->getWorld());
	if (mPhyProps.rigid || mPhyProps.dyn)
		mRigidBody->setShape(mNode, mShape, info, mBaseProps.position, mBaseProps.orientation);
	else
		mRigidBody->setStaticShape(mNode, mShape, info, mBaseProps.position, mBaseProps.orientation);


	/* listen for state change events */
	mRigidBody->getObjectState()->setListener(this);

	btRigidBody *bbody= mRigidBody->getBulletRigidBody();

	/* update some bullet stuff */
	mShape->getBulletShape()->setLocalScaling(btVector3(mBaseProps.scale.x, mBaseProps.scale.y, mBaseProps.scale.z));
	if (mPhyProps.nosleep)
		mRigidBody->disableDeactivation();

	if (!mPhyProps.rigid)
		bbody->setAngularFactor(Real(0.0));
#endif
}

// ----------------------------------------------------------------------------
void gkGameObject::unloadPhysics(void)
{
#if 0
	if (mRigidBody)
	{
		delete mRigidBody;
		mRigidBody= 0;
	}
#endif
}
