/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): none yet.
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



gkBone::gkBone(const gkString& name)
	:    m_name(name), m_bone(0), m_bind(), m_parent(0)
{
	m_bind.setIdentity();
	m_pose.setIdentity();
}



gkBone::~gkBone()
{
}



void gkBone::setRestPosition(const gkTransformState& st)
{
	m_bind = st;
}


void gkBone::applyRootTransform(const gkTransformState& root)
{
	gkMatrix4 bonemat = m_bind.toMatrix();
	gkMatrix4 objmat = root.toMatrix();

	bonemat = objmat * bonemat;

	m_bind = gkTransformState(bonemat);
}


void gkBone::applyChannelTransform(const gkTransformState& channel, gkScalar weight)
{
	// save previous pose
	gkTransformState blendmat = m_pose;

	// combine relative to binding position
	m_pose.loc = m_bind.loc + m_bind.rot * channel.loc;
	m_pose.rot = m_bind.rot * channel.rot;
	m_pose.scl = m_bind.scl * channel.scl;

	if (weight < 1.0)
	{
		// blend poses
		m_pose.loc = gkMathUtils::interp(blendmat.loc, m_pose.loc, weight);
		m_pose.rot = gkMathUtils::interp(blendmat.rot, m_pose.rot, weight);
		m_pose.rot.normalise();
		m_pose.scl = gkMathUtils::interp(blendmat.scl, m_pose.scl, weight);
	}

	m_bone->setPosition(m_pose.loc);
	m_bone->setOrientation(m_pose.rot);
	m_bone->setScale(m_pose.scl);
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
