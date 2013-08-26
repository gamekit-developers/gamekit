/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Charlie C.

    Contributor(s): Thomas Trocha(dertom)
-------------------------------------------------------------------------------
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
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
#include "gkBone.h"
#include "OgreBone.h"
#include "gkGameObject.h"
#include "gkLogger.h"
#include "gkValue.h"


gkBone::gkBone(const gkString& name)
	:    m_name(name), m_bone(0), m_bind(), m_parent(0)
{
	m_bind.setIdentity();
	m_pose.setIdentity();
	m_manualControl = false;
}



gkBone::~gkBone()
{
}


void gkBone::setRestPosition(const gkTransformState& st)
{
	GK_ASSERT(!st.isNaN() && "Invalid transform supplied as parameter");

	m_bind = st;
	m_manualControl = false;	
}


void gkBone::applyRootTransform(const gkTransformState& root)
{
	gkMatrix4 bonemat = m_bind.toMatrix();
	gkMatrix4 objmat = root.toMatrix();

	gkTransformState trans(objmat * bonemat);

	//GK_ASSERT(!trans.isNaN() && "Invalid transform supplied as parameter");
	
	if (!trans.isNaN())
		m_bind = trans;
	else
		gkPrintf("Warning: root bone transform is invalid.");
}


void gkBone::applyChannelTransform(const gkTransformState& channel, gkScalar weight)
{

	m_tempBlendMat = m_pose;

	// combine relative to binding position
	m_pose.loc = m_bind.loc + m_bind.rot * channel.loc;
	m_pose.rot = m_bind.rot * channel.rot;
	m_pose.scl = m_bind.scl * channel.scl;

	if (weight < 1.0)
	{
		// blend poses
		m_pose.loc = gkMathUtils::interp(m_tempBlendMat.loc, m_pose.loc, weight);
		m_pose.rot = gkMathUtils::interp(m_tempBlendMat.rot, m_pose.rot, weight);
		m_pose.rot.normalise();
		m_pose.scl = gkMathUtils::interp(m_tempBlendMat.scl, m_pose.scl, weight);
	}

	if(m_bone) {
		m_bone->setPosition(m_pose.loc);
		m_bone->setOrientation(m_pose.rot);
		m_bone->setScale(m_pose.scl);
	}

	AttachedObjectList::Iterator iter(m_attachedObjects);
	while (iter.hasMoreElements())
	{
		const gkMatrix4& boneMat = getTransform();
		gkTransformState boneState(boneMat);

		gkGameObject* attachedObj = iter.getNext();

		gkTransformState newBoneState(attachedObj->_getBoneTransform()?   boneMat * attachedObj->_getBoneTransform()->toMatrix()
																		: boneMat);

		attachedObj->applyTransformState(newBoneState,1.0f);
	}

}

const gkMatrix4 gkBone::getTransform()
{
	if (!m_parent) {
		return m_pose.toMatrix();
	} else {
		return getParent()->getTransform() * m_pose.toMatrix();
	}
}

const gkMatrix4 gkBone::getRestTransform()
{
	if (!m_parent) {
		return m_bind.toMatrix();
	} else {
		return getParent()->getRestTransform() * m_bind.toMatrix();
	}
}


void gkBone::applyPoseTransform(const gkTransformState& pose)
{
	if (!m_bone)
		return;


	m_bone->setPosition(pose.loc);
	m_bone->setOrientation(pose.rot);
	m_bone->setScale(pose.scl);
}



void gkBone::setParent(gkBone* bone)
{
	if (!bone)
		return;

	if (!m_parent)
	{
		m_parent = bone;
		m_parent->m_children.push_back(this);
	}
}

bool gkBone::isManuallyControlled()
{
	return m_manualControl;
}  // gkBone::getManualControl

void gkBone::setManuallyControlled(bool pManualControl)
{
	m_manualControl = pManualControl;
}  // gkBone::setManualControl

void gkBone::_setOgreBone(Ogre::Bone* bone)
{
	m_bone = bone;
}



UTsize gkBone::_getBoneIndex(void)
{
	if (!m_bone)
		return UT_NPOS;

	return m_bone->getHandle();
}

void gkBone::attachObject(gkGameObject* gobj)
{
	m_attachedObjects.push_back(gobj);
}

void gkBone::detachObject(gkGameObject* gobj)
{
	m_attachedObjects.erase(gobj);
}
