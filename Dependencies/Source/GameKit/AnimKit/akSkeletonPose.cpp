/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

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

#include "akSkeletonPose.h"

#include "akSkeleton.h"
#include "akTransformState.h"
#include "akDualQuat.h"

akSkeletonPose::akSkeletonPose(akSkeleton* skel, int space) : m_skeleton(skel), m_space(space)
{
	m_jointPoses.reserve(m_skeleton->getNumJoints());
	m_jointPoses.resize(m_skeleton->getNumJoints());
}

akSkeletonPose::akSkeletonPose(const akSkeletonPose& o)
{
	m_space = o.m_space;
	m_skeleton = o.m_skeleton;
	
	m_jointPoses.reserve(m_skeleton->getNumJoints());
	m_jointPoses.resize(m_skeleton->getNumJoints());
	
	for (UTsize i = 0; i < o.m_jointPoses.size(); i++)
		m_jointPoses[i] = o.m_jointPoses[i];
}

akSkeletonPose::~akSkeletonPose()
{
	m_jointPoses.clear();
}

UTuint8 akSkeletonPose::getNumJoints(void) const
{
	return m_skeleton->getNumJoints();
}

int akSkeletonPose::getIndex(const utHashedString &name) const
{
	return m_skeleton->getIndex(name);
}

akTransformState* akSkeletonPose::getJointPose(const UTuint8 idx)
{
	if(idx < getNumJoints())
		return &m_jointPoses[idx];
	return 0;
}

const akTransformState* akSkeletonPose::getJointPose(const UTuint8 idx) const
{
	if(idx < getNumJoints())
		return &m_jointPoses[idx];
	return 0;
}

akTransformState* akSkeletonPose::getByName(const utHashedString& name)
{
	int idx = m_skeleton->getIndex(name);
	
	if (idx >=0)
		return &m_jointPoses[idx];
	else
		return 0;
}

void akSkeletonPose::toLocalSpace(akSkeletonPose* dest) const
{
	if(m_space == SP_MODEL_SPACE)
	{
		int i, count;
		count = getNumJoints();
		
		for(i=count-1; i>=0; i--)
		{
			UTuint8 pid = m_skeleton->getJoint(i)->m_parentId;
			
			if( pid != AK_JOINT_NO_PARENT)
			{
				dest->m_jointPoses[i] = akTransformState(inverse(m_jointPoses[pid].toMatrix()) * m_jointPoses[i].toMatrix());
			}
		}
	}
	
	else if(m_space == SP_BINDING_SPACE)
	{
		akSkeletonPose* bindPose;
		int i, count;
		
		count = getNumJoints();
		bindPose = m_skeleton->getLocalBindPose();
		
		for(i=0; i<count; i++)
		{
			akTransformState* jbp = bindPose->getJointPose(i);
			dest->m_jointPoses[i] = akTransformState(jbp->toMatrix() * m_jointPoses[i].toMatrix());
		}
	}
	
	dest->m_space = SP_LOCAL_SPACE;
}

void akSkeletonPose::toModelSpace(akSkeletonPose* dest) const
{
	if(m_space == SP_BINDING_SPACE)
	{
		toLocalSpace(dest);
		dest->toModelSpace(dest);
	}
	
	else if(m_space == SP_LOCAL_SPACE)
	{
		int i, count;
		count = getNumJoints();
		
		for(i=0; i<count; i++)
		{
			UTuint8 pid = m_skeleton->getJoint(i)->m_parentId;
			
			if( pid != AK_JOINT_NO_PARENT)
			{
				dest->m_jointPoses[i] = akTransformState(dest->m_jointPoses[pid].toMatrix() * m_jointPoses[i].toMatrix());
			}
		}
	}
	
	dest->m_space = SP_MODEL_SPACE;
}


void akSkeletonPose::setIdentity(void)
{
	for(int i=0; i<getNumJoints(); i++)
	{
		m_jointPoses[i] = akTransformState::identity();
	}
}


void akSkeletonPose::fillMatrixPalette(btAlignedObjectArray<akMatrix4> &palette) const
{
	for(int i=0; i<getNumJoints(); i++)
	{
		palette[i] = m_jointPoses[i].toMatrix() * m_skeleton->getJointInverseBindPose(i);
	}
}

void akSkeletonPose::fillDualQuatPalette(btAlignedObjectArray<akDualQuat> &dqpalette,
										btAlignedObjectArray<akMatrix4> &mpalette) const
{
	for(int i=0; i<getNumJoints(); i++)
	{
		akTransformState ts(m_jointPoses[i].toMatrix() * m_skeleton->getJointInverseBindPose(i));
		akDualQuat dq(ts.rot, ts.loc);
		akMatrix4 m = akMatrix4::identity();
		appendScale(m, ts.scale);
		dqpalette[i] = dq;
		mpalette[i] = m;
	}
	
	// antipodality
	for(int i=1; i<getNumJoints(); i++)
	{
		UTuint8 pid = m_skeleton->getJoint(i)->m_parentId;
		if(pid != AK_JOINT_NO_PARENT)
		{
			akDualQuat& dq(dqpalette[i]);
			akDualQuat& dqp(dqpalette[pid]);
			
			if( dot(dqp.nondual, dq.nondual) < 0 )
				dq *= -1.0;
		}
	}
	
}

