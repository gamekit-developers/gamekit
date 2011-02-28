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

#ifndef AKSKELETON_H
#define AKSKELETON_H

#include "utCommon.h"
#include "utTypes.h"
#include "utString.h"

#include "akCommon.h"
#include "akMathUtils.h"

#define AK_JOINT_NO_PARENT 0xFF

/// Base componemt of a skeleton
class akJoint
{
public:
	utHashedString m_name;
	UTuint8        m_parentId;
	
	akJoint() : m_parentId(AK_JOINT_NO_PARENT) {}
}
#ifdef __GNUC__
__attribute__ ((aligned(16)))
#endif
;


/// A skeleton is an articulated armature used to deforme a mesh.
/// Composed of joints organised in a hierachical way (parent/child)
/// PArent joints are assumed to be positioned in the array before any of their child
class akSkeleton
{
private:
	UTuint8 m_numJoints;
	akJoint* m_joints;
	
	akSkeletonPose* m_localBindPose;
	akSkeletonPose* m_modelBindPose;
	akMatrix4*      m_inverseBindPose;
	
public:
	akSkeleton(UTuint32 numJoints);
	~akSkeleton();
		
	int getIndex(const utHashedString& name) const;
	akJoint* getJoint(UTuint8 idx);
	akJoint* getByName(const utHashedString &name);
	
	bool setBindingPose(akSkeletonPose* pose);
	
	
	UT_INLINE UTuint8  getNumJoints(void) const
	{
		return m_numJoints;
	}
	
	UT_INLINE akSkeletonPose* getLocalBindPose(void)
	{
		return m_localBindPose;
	}
	
	UT_INLINE akSkeletonPose* getModelBindPose(void)
	{
		return m_modelBindPose;
	}
	
	UT_INLINE akMatrix4* getInverseBindPose()
	{
		return m_inverseBindPose;
	}
};


#endif // AKSKELETON_H
