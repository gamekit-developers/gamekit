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
#ifndef _gkGameObject_h_
#define _gkGameObject_h_

#include "gkCommon.h"
#include "OgrePrerequisites.h"
#include "OgreString.h"
#include "OgreSceneNode.h"

#include "gkGameObjectTypes.h"
#include "gkMathUtils.h"
#include "gkVariable.h"
#include "gkTransformState.h"



// ---------------------------------------------------------------------------
class gkGameObject
{
public:
	gkGameObject(gkSceneObject *scene, const Ogre::String& name, gkGameObjectTypes type, gkManualObjectLoader* loader= 0);
	virtual ~gkGameObject();

	const Ogre::String& getName(void);
	const gkHashedString &getHashedName(void);
	gkSceneObject* getOwner(void);

	bool isLoaded(void);

	gkVariable* createVariable(const Ogre::String &name, bool debug);
	gkVariable* getVariable(const Ogre::String &name);
	bool hasVariable(const Ogre::String &name);

	gkGameObjectTypes getType(void);
	gkGameObjectProperties& getProperties(void);
	gkPhysicsProperties& getPhysicsProperties(void);

	void setParent(gkGameObject *par);

	void load(void);
	void unload(void);

	// local, no parent
	Ogre::Matrix4	 getLocalTransform(void);
	Ogre::Vector3	 getPosition(void);
	Ogre::Vector3	 getScale(void);
	Ogre::Quaternion  getOrientation(void);
	Ogre::Vector3	 getRotation(void);

	// world, with parent
	Ogre::Matrix4	 getWorldTransform(void);
	Ogre::Vector3	 getWorldPosition(void);
	Ogre::Vector3	 getWorldScale(void);
	Ogre::Quaternion  getWorldOrientation(void);
	Ogre::Vector3	 getWorldRotation(void);


	// set local transforms
	void setTransform(const Ogre::Matrix4 &v);
	void setPosition(const Ogre::Vector3 &v);
	void setScale(const Ogre::Vector3 &v);
	void setOrientation(const Ogre::Quaternion& q);
	void setOrientation(const Ogre::Vector3& v);

	void rotate(const Ogre::Vector3 &drot, int tspace= TRANSFORM_PARENT);
	void rotate(const Ogre::Quaternion &q, int tspace= TRANSFORM_PARENT);
	void translate(const Ogre::Vector3 &dloc, int tspace= TRANSFORM_PARENT);
	void scale(const Ogre::Vector3 &dscale);

	void applyTorque(const Ogre::Vector3 &t, int tspace= TRANSFORM_PARENT);
	void applyForce(const Ogre::Vector3 &f, int tspace= TRANSFORM_PARENT);

	void setLinearVelocity(const Ogre::Vector3 &linv, int tspace= TRANSFORM_PARENT);
	void setAngualrVelocity(const Ogre::Vector3 &angv, int tspace= TRANSFORM_PARENT);

	Ogre::Vector3 getLinearVelocity(void);
	Ogre::Vector3 getAngualrVelocity(void);


	void _setStartPose(const Ogre::String& pose);
	void attachLogic(gkLogicTree *tree);

	gkEntityObject* getEntity(void);
	gkCameraObject* getCamera(void);
	gkLightObject*  getLight(void);
	gkArmatureObject* getArmature(void);


	void addConstraint(gkConstraint *c);

	gkConstraintListIterator getConstraintIterator(void);
	void destroyConstraints(void);

	// apply local constraints
	void update(void);

	gkGameObject *getParent(void);
	gkGameObject *duplicate(const Ogre::String &newName);

	void synchronizeMotion(Ogre::Real interp);

	void setActiveLayer(bool truth);
	bool isInActiveLayer(void);

	void _attachToGroup(gkGameObjectGroup *g);
	void _detachFromGroup(void);
	void _attachToGroupInstance(gkGameObjectGroupInstance *g);
	bool isInstance(void);

	gkGameObject *getGroupParent(void);
	gkGameObjectGroupInstance* getGroupInstance(void);

private:

	void stateUpdated(void);

	void clearVariables(void);
	void loadPhysics(void);
	void unloadPhysics(void);
	void applyConstraints(void);

protected:

	/// fixed name for now
	const gkHashedString mName;


	bool mIsLoaded;
	gkManualObjectLoader* mLoader;

	gkGameObjectTypes mType;
	gkGameObjectProperties mBaseProps;
	gkPhysicsProperties mPhyProps;
	gkGameObject* mParent;
	gkSoundReferenceObject* mSoundObject;
	gkSceneObject* mScene;
	Ogre::String mStartPose;
	gkVariableMap mVariables;
	Ogre::SceneNode* mNode;
	gkLogicTree* mLogic;

	gkTransformState mPState, mCState;
	gkTransformState mPTState;
	gkConstraintList mConstraints;

	gkGameObjectGroup* mGroupRef;		 //is owner
	gkGameObjectGroupInstance *mInstance; //is instance

	bool mActiveLayer, mOutOfDate;

	virtual void loadImpl(void);
	virtual void unloadImpl(void);
	virtual void postLoadImpl(void);
	virtual void postUnloadImpl(void) {}
};


// ---------------------------------------------------------------------------
GK_INLINE const Ogre::String& gkGameObject::getName(void)
{
	return mName;
}

// ---------------------------------------------------------------------------
GK_INLINE const gkHashedString &gkGameObject::getHashedName(void)
{
	return mName;
}

// ---------------------------------------------------------------------------
GK_INLINE gkSceneObject* gkGameObject::getOwner(void)
{
	// access to parent scene
	return mScene;
}

// ---------------------------------------------------------------------------
GK_INLINE bool gkGameObject::isLoaded(void)
{
	return mIsLoaded;
}

// ---------------------------------------------------------------------------
GK_INLINE gkGameObjectProperties& gkGameObject::getProperties(void)
{
	return mBaseProps;
}

// ---------------------------------------------------------------------------
GK_INLINE gkPhysicsProperties& gkGameObject::getPhysicsProperties(void)
{
	return mPhyProps;
}


// ---------------------------------------------------------------------------
GK_INLINE void gkGameObject::_setStartPose(const Ogre::String& pose)
{
	mStartPose= pose;
}

// ---------------------------------------------------------------------------
GK_INLINE gkGameObjectTypes gkGameObject::getType(void)
{
	return mType;
}

// ---------------------------------------------------------------------------
GK_INLINE gkEntityObject* gkGameObject::getEntity(void)
{
	if (mType != GK_ENTITY)
		return 0;
	return reinterpret_cast<gkEntityObject*>(this);
}

// ---------------------------------------------------------------------------
GK_INLINE gkCameraObject* gkGameObject::getCamera(void)
{
	if (mType != GK_CAMERA)
		return 0;
	return reinterpret_cast<gkCameraObject*>(this);
}

// ---------------------------------------------------------------------------
GK_INLINE gkLightObject* gkGameObject::getLight(void)
{
	if (mType != GK_LIGHT)
		return 0;
	return reinterpret_cast<gkLightObject*>(this);
}

// ---------------------------------------------------------------------------
GK_INLINE gkArmatureObject* gkGameObject::getArmature(void)
{
	if (mType != GK_ARMATURE)
		return 0;
	return reinterpret_cast<gkArmatureObject*>(this);
}

// ---------------------------------------------------------------------------
GK_INLINE gkConstraintListIterator gkGameObject::getConstraintIterator(void)
{
	return gkConstraintListIterator(mConstraints.begin(), mConstraints.end());
}

// ---------------------------------------------------------------------------
GK_INLINE gkGameObject *gkGameObject::getParent(void)
{
	return mParent;
}

// ---------------------------------------------------------------------------
GK_INLINE Ogre::Matrix4 gkGameObject::getLocalTransform(void)
{
	if (mNode != 0)
	{
		Ogre::Matrix4 mat;
		mat.makeTransform(mNode->getPosition(),
						  mNode->getScale(),
						  mNode->getOrientation());
		return mat;
	}
	return Ogre::Matrix4::IDENTITY;
}

// ---------------------------------------------------------------------------
GK_INLINE Ogre::Vector3 gkGameObject::getPosition(void)
{
	if (mNode != 0)
		return mNode->getPosition();
	return Ogre::Vector3::ZERO;
}

// ---------------------------------------------------------------------------
GK_INLINE Ogre::Vector3 gkGameObject::getScale(void)
{
	if (mNode != 0)
		return mNode->getScale();
	return Ogre::Vector3::UNIT_SCALE;
}

// ---------------------------------------------------------------------------
GK_INLINE Ogre::Quaternion gkGameObject::getOrientation(void)
{
	if (mNode != 0)
		return mNode->getOrientation();
	return Ogre::Quaternion::IDENTITY;
}

// ---------------------------------------------------------------------------
GK_INLINE Ogre::Vector3 gkGameObject::getRotation(void)
{
	if (mNode != 0)
		return gkMathUtils::getEulerFromQuat(mNode->getOrientation());
	return Ogre::Vector3::ZERO;
}

// ---------------------------------------------------------------------------
GK_INLINE Ogre::Matrix4 gkGameObject::getWorldTransform(void)
{
	if (mNode != 0)
		return mNode->_getFullTransform();
	return Ogre::Matrix4::IDENTITY;
}

// ---------------------------------------------------------------------------
GK_INLINE Ogre::Vector3 gkGameObject::getWorldPosition(void)
{
	if (mNode != 0)
		return mNode->_getDerivedPosition();
	return Ogre::Vector3::ZERO;
}

// ---------------------------------------------------------------------------
GK_INLINE Ogre::Vector3 gkGameObject::getWorldScale(void)
{
	if (mNode != 0)
		return mNode->_getDerivedScale();
	return Ogre::Vector3::UNIT_SCALE;
}

// ---------------------------------------------------------------------------
GK_INLINE Ogre::Quaternion gkGameObject::getWorldOrientation(void)
{
	if (mNode != 0)
		return mNode->_getDerivedOrientation();
	return Ogre::Quaternion::IDENTITY;
}

// ---------------------------------------------------------------------------
GK_INLINE Ogre::Vector3 gkGameObject::getWorldRotation(void)
{
	if (mNode != 0)
		return gkMathUtils::getEulerFromQuat(mNode->_getDerivedOrientation());
	return Ogre::Vector3::ZERO;
}

// ---------------------------------------------------------------------------
GK_INLINE void gkGameObject::setActiveLayer(bool truth)
{
	mActiveLayer= truth;
}

// ---------------------------------------------------------------------------
GK_INLINE bool gkGameObject::isInActiveLayer(void)
{
	return mActiveLayer;
}

// ---------------------------------------------------------------------------
GK_INLINE bool gkGameObject::isInstance(void)
{
	return mInstance != 0;
}

// ---------------------------------------------------------------------------
GK_INLINE gkGameObjectGroupInstance* gkGameObject::getGroupInstance(void)
{
	return mInstance;
}

// ---------------------------------------------------------------------------
GK_INLINE void gkGameObject::setParent(gkGameObject* par)
{
	if (par != 0)
		mParent= par;
}





#endif//_gkGameObject_h_
