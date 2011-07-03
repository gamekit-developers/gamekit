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

#include "akPoseBlender.h"
#include "akSkeleton.h"
#include "akMathUtils.h"

void akPoseBlender::blendJoint(BlendMode bmode, RotMode rmode, akScalar weight, const akTransformState& a, const akTransformState& b, akTransformState& out)
{
	switch(bmode)
	{
	case PB_BM_LERP:
		out.loc =  lerp(weight, a.loc, b.loc);
		
		if(rmode==PB_RM_SLERP)
			out.rot = slerp(weight, a.rot, b.rot);
		else
			out.rot = lerp(weight, a.rot, b.rot);
			
		out.scale = lerp(weight, a.scale, b.scale);
		break;
		
	case PB_BM_ADD:
		out.loc =  a.loc + b.loc * weight;
		
		if(rmode==PB_RM_SLERP)
			out.rot = slerp(weight, a.rot, a.rot*b.rot);
		else
			out.rot = lerp(weight, a.rot, a.rot*b.rot);
		
		out.scale = lerp(weight, a.scale, mulPerElem(a.scale, b.scale) );

//		{
//		akTransformState sum;
//		akMathUtils::extractTransform(a.toMatrix()*b.toMatrix(), sum.loc, sum.rot, sum.scale);
//		out.loc =  lerp(weight, a.loc, sum.loc);
		
//		if(rmode==PB_RM_SLERP)
//			out.rot = slerp(weight, a.rot, sum.rot);
//		else
//			out.rot = lerp(weight, a.rot, sum.rot);
			
//		out.scale = lerp(weight, a.scale, sum.scale);
//		}

		break;
		
	//TODO test this.
	case PB_BM_SUB:
		out.loc = a.loc - b.loc * weight;
		
		akQuat invrot = conj(b.rot);
		
		if(rmode==PB_RM_SLERP)
			out.rot = slerp(weight, a.rot, a.rot*invrot);
		else
			out.rot = lerp(weight, a.rot, a.rot*invrot);
		
		// Needs a check for dision by 0 (not likely to happen).
		out.scale = lerp(weight, a.scale, divPerElem(a.scale, b.scale) );
		break;
	}
}

void akPoseBlender::blendSkeleton(BlendMode bmode, RotMode rmode, akScalar weight, const akSkeletonPose& a, const akSkeletonPose& b, akSkeletonPose& out)
{
	const akSkeleton *as = a.getSkeleton();
	const akSkeleton *bs = b.getSkeleton();
	const akSkeleton *os = out.getSkeleton();
	
	if( as == bs && as == os)
	{
		for(int i=0; i< a.getNumJoints(); i++)
		{
			blendJoint(bmode, rmode, weight, *a.getJointPose(i), *b.getJointPose(i), *out.getJointPose(i));
		}
	}
}
