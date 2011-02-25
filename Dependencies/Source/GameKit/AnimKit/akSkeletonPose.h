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

#ifndef AKSKELETONPOSE_H
#define AKSKELETONPOSE_H

#include "utTypes.h"
#include "utString.h"

#include "akCommon.h"
#include "akMathUtils.h"
#include "akTransformState.h"

/// A skeleton pose is linked to a skeleton and contains a transformation state for each bone
class akSkeletonPose
{
public:
	enum Space
	{
		SP_LOCAL_SPACE,   // coord sys = parent
		SP_MODEL_SPACE,   // coord sys = model
		SP_BINDING_SPACE, // coord sys = binding pose
	};
	
	typedef utArray<akTransformState> JointPoses;
	typedef utArray<akTransformState>::Iterator JointPoseIterator;

private:
	int         m_space;
	akSkeleton* m_skeleton;
	JointPoses  m_jointPoses;
	
public:
	akSkeletonPose(akSkeleton* skel, int space=SP_LOCAL_SPACE);
	akSkeletonPose(const akSkeletonPose& o);
	~akSkeletonPose();
	
	UTuint8  getNumJoints(void) const;
	int getIndex(const utHashedString& name) const;
	akTransformState* getJointPose(const UTuint8 idx);
	const akTransformState* getJointPose(const UTuint8 idx) const;
	akTransformState* getByName(const utHashedString& name);
	
	void toLocalSpace(akSkeletonPose* dest) const;
	void toModelSpace(akSkeletonPose* dest) const;
	
	void setIdentity(void);
	
	/// Assume the array is allocated and his size is the number of bones in the skeleton corresponding to this pose
	/// Will fill the pallet with the inverse binding pose of the skeleton multiplied by the bone pose.
	/// If the pose is in model space than the palette can be used to transform vertex/object attached to bones
	/// You can also process the palette afterward to include world transformation.
	void fillMatrixPalette(Matrix4* palette)const ;
	
	
	UT_INLINE int getSpace(void)
	{
		return m_space;
	}
	
	UT_INLINE void setSpace(int v)
	{
		m_space = v;
	}
	
	UT_INLINE akSkeleton* getSkeleton(void) const
	{
		return m_skeleton;
	}
	
};

#endif // AKSKELETONPOSE_H
