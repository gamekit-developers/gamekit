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

void akSkeletonPose::setIdentity(void)
{
	for(int i=0; i<getNumJoints(); i++)
	{
		m_jointPoses[i] = akTransformState::identity();
	}
}

void akSkeletonPose::toLocalSpace(akSkeletonPose* dest) const
{
	int i, count;
	count = getNumJoints();
	
	switch(m_space)
	{
	case SP_MODEL_SPACE:
		for(i=count-1; i>=0; i--)
		{
			UTuint8 pid = m_skeleton->getJoint(i)->m_parentId;
			
			if( pid != AK_JOINT_NO_PARENT)
			{
				akMatrix4 mat = inverse(m_jointPoses[pid].toMatrix()) * m_jointPoses[i].toMatrix();
				akTransformState& ts = dest->m_jointPoses[i];
				
				if(!m_skeleton->getJoint(i)->m_inheritScale)
					mat = akMathUtils::setScale(mat, ts.scale);
					
				akMathUtils::extractTransform(mat, ts.loc, ts.rot, ts.scale);
			}
			else
			{
				dest->m_jointPoses[i] = m_jointPoses[i];
			}
		}
		break;
		
	case SP_BINDING_SPACE:
		for(i=0; i<count; i++)
		{
			const akTransformState* jbp = m_skeleton->getLocalBindPose()->getJointPose(i);
			akMatrix4 mat = jbp->toMatrix() * m_jointPoses[i].toMatrix();
			akTransformState& ts = dest->m_jointPoses[i];
			akMathUtils::extractTransform(mat, ts.loc, ts.rot, ts.scale);
		}
		break;
		
	case SP_LOCAL_SPACE:
		for(i=0; i<count; i++)
		{
			dest->m_jointPoses[i] = m_jointPoses[i];
		}
		break;
	}
	
	dest->m_space = SP_LOCAL_SPACE;
}

void akSkeletonPose::toModelSpace(akSkeletonPose* dest) const
{
	int i, count;
	count = getNumJoints();
	
	switch(m_space)
	{
	case SP_BINDING_SPACE:
		for(i=0; i<count; i++)
		{
			UTuint8 pid = m_skeleton->getJoint(i)->m_parentId;
			const akTransformState* jbp = m_skeleton->getLocalBindPose()->getJointPose(i);
			akTransformState& ts = dest->m_jointPoses[i];
			akMatrix4 mat;
			
			if( pid != AK_JOINT_NO_PARENT)
			{
				akMatrix4 local = jbp->toMatrix() * m_jointPoses[i].toMatrix();
				mat = dest->m_jointPoses[pid].toMatrix() * local;
				
				if(!m_skeleton->getJoint(i)->m_inheritScale)
					mat = akMathUtils::setScale(mat, akMathUtils::getScale(local));
			}
			else
				mat = jbp->toMatrix() * m_jointPoses[i].toMatrix();
			
			akMathUtils::extractTransform(mat, ts.loc, ts.rot, ts.scale);
		}
		break;
	
	case SP_LOCAL_SPACE:
		for(i=0; i<count; i++)
		{
			UTuint8 pid = m_skeleton->getJoint(i)->m_parentId;
			
			if( pid != AK_JOINT_NO_PARENT)
			{
				akTransformState& ts = dest->m_jointPoses[i];
				akMatrix4 mat = dest->m_jointPoses[pid].toMatrix() * m_jointPoses[i].toMatrix();
				
				if(!m_skeleton->getJoint(i)->m_inheritScale)
					mat = akMathUtils::setScale(mat, ts.scale);
		
				akMathUtils::extractTransform(mat, ts.loc, ts.rot, ts.scale);
			}
			else
				dest->m_jointPoses[i] = m_jointPoses[i];
		}
		break;
	
	case SP_MODEL_SPACE:
		for(i=0; i<count; i++)
			dest->m_jointPoses[i] = m_jointPoses[i];
		break;
	}
	
	dest->m_space = SP_MODEL_SPACE;
}

void akSkeletonPose::fillMatrixPalette(btAlignedObjectArray<akMatrix4> &palette) const
{
	switch(m_space)
	{
	case SP_MODEL_SPACE:
		for(int i=0; i<getNumJoints(); i++)
		{
			palette[i] = m_jointPoses[i].toMatrix();
		}
		break;
	case SP_LOCAL_SPACE:
		for(int i=0; i<getNumJoints(); i++)
		{
			UTuint8 pid = m_skeleton->getJoint(i)->m_parentId;
			if( pid != AK_JOINT_NO_PARENT)
			{
				palette[i] = palette[pid] * m_jointPoses[i].toMatrix();
				
				if(!m_skeleton->getJoint(i)->m_inheritScale)
					palette[i] = akMathUtils::setScale(palette[i], m_jointPoses[i].scale);
			}
			else
				palette[i] = m_jointPoses[i].toMatrix();
		}
		break;
	case SP_BINDING_SPACE:
		for(int i=0; i<getNumJoints(); i++)
		{
			UTuint8 pid = m_skeleton->getJoint(i)->m_parentId;
			const akTransformState* jbp = m_skeleton->getLocalBindPose()->getJointPose(i);
			if( pid != AK_JOINT_NO_PARENT)
			{
				akMatrix4 parent = palette[pid];
				akMatrix4 local = jbp->toMatrix() * m_jointPoses[i].toMatrix();
				
				palette[i] = parent * local;
				
				if(!m_skeleton->getJoint(i)->m_inheritScale)
					palette[i] = akMathUtils::setScale(palette[i], akMathUtils::getScale(local));
			}
			else
				palette[i] = jbp->toMatrix() * m_jointPoses[i].toMatrix();
		}
		break;
	}
	
	for(int i=0; i<getNumJoints(); i++)
	{
		palette[i] = palette[i] * m_skeleton->getJointInverseBindPose(i);
	}
}

void akSkeletonPose::fillDualQuatPalette(btAlignedObjectArray<akDualQuat> &dqpalette,
										btAlignedObjectArray<akMatrix4> &mpalette) const
{
	fillMatrixPalette(mpalette);
	
	for(int i=0; i<getNumJoints(); i++)
	{
		akTransformState ts;
		akMathUtils::extractTransform(mpalette[i], ts.loc, ts.rot, ts.scale);
		
		dqpalette[i] = akDualQuat(ts.rot, ts.loc);
		mpalette[i] = akMatrix4::scale(ts.scale);
	}
	
	// antipodality
	for(int i=1; i<getNumJoints(); i++)
	{
		UTuint8 pid = m_skeleton->getJoint(i)->m_parentId;
		if(pid != AK_JOINT_NO_PARENT)
		{
			akDualQuat& dq(dqpalette[i]);
			akDualQuat& dqp(dqpalette[pid]);
			
			if( dot(dqp.n, dq.n) < 0 )
				dq *= -1.0;
		}
	}
	
}

