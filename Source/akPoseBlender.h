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

#ifndef AKPOSEBLENDER_H
#define AKPOSEBLENDER_H

#include "akMathUtils.h"
#include "akTransformState.h"
#include "akSkeletonPose.h"

/// Simple utility for animation blending
class akPoseBlender
{
public:
	enum BlendMode
	{
		PB_BM_LERP,
		PB_BM_ADD,
		PB_BM_SUB
	};
	
	enum RotMode
	{
		PB_RM_LERP,
		PB_RM_SLERP
	};

	/// Blend the postion of to joints according the blend mode and weight
	/// In the case of ADD and SUB, the order of a and b is important
	/// In the case of SUB there is no much use of having weight !=1.
	/// SUB should not be used at runtime but to create resources poses or animations for latter additive blending
	static void blendJoint(BlendMode bmode, RotMode rmode, akScalar weight, const akTransformState& a, const akTransformState& b, akTransformState& out);
	
	
	/// Blend 2 skeleton poses bone by bones
	static void blendSkeleton(BlendMode bmode, RotMode rmode, akScalar weight, const akSkeletonPose& a, const akSkeletonPose& b, akSkeletonPose& out);
};

#endif // AKPOSEBLENDER_H
