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

#ifndef AKPOSE_H
#define AKPOSE_H

#include "akCommon.h"
#include "akTransformState.h"
#include "akSkeletonPose.h"

#include "utTypes.h"

class akPose
{
public:
	struct FloatResult
	{
		UTuint32 channelType;
		UTuint32 channelNameHash;
		UTuint32 curveCode;
		akScalar value;
	};

private:
	akTransformState*    m_transform;
	akSkeletonPose*      m_skelpose;
	utArray<FloatResult> m_floats;

public:
	akPose(akSkeleton* skeleton =0);
	~akPose();
	
	void setSkeleton(akSkeleton* skeleton);
	void reset(void);
	FloatResult* getFloatResult(UTuint32 ctype, UTuint32 chash, UTuint32 code);
	const FloatResult* getFloatResult(UTuint32 ctype, UTuint32 chash, UTuint32 code) const;
	void addFloatResult(UTuint32 ctype, UTuint32 chash, UTuint32 code, float value);
	
	void fillMatrixPalette(btAlignedObjectArray<akMatrix4> &palette) const ;
	
	void fillDualQuatPalette(btAlignedObjectArray<akDualQuat> &palette,
							btAlignedObjectArray<akMatrix4> &mpalette) const ;
							
	UT_INLINE akTransformState& getTransform(void)
	{
		return *m_transform;
	}
	
	UT_INLINE akSkeletonPose* getSkeletonPose(void)
	{
		return m_skelpose;
	}
	
};

#endif // AKPOSE_H
