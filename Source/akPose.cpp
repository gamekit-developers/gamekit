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

#include "akPose.h"
akPose::akPose(): m_skelpose(0)
{
	m_transform = (akTransformState*)btAlignedAlloc(sizeof(akTransformState), 16);
}

akPose::akPose(akSkeleton* skeleton)
{
	m_transform = (akTransformState*)btAlignedAlloc(sizeof(akTransformState), 16);
	m_skelpose = new akSkeletonPose(skeleton);
}

akPose::~akPose()
{
	btAlignedFree(m_transform);
	if(m_skelpose)
		delete m_skelpose;
	m_floats.clear();
}

void akPose::reset(akSkeletonPose::Space space)
{
	*m_transform = akTransformState::identity();
	
	if(m_skelpose)
	{
		m_skelpose->setIdentity();
		m_skelpose->setSpace(space);
	}
	
	for(int i=0; i<m_floats.size(); i++)
	{
		m_floats[i].value=0;
	}
}

akPose::FloatResult* akPose::getFloatResult(UTuint32 ctype, UTuint32 chash, UTuint32 code)
{
	for(int i=0; i<m_floats.size(); i++)
	{
		if( m_floats[i].channelType == ctype &&
			m_floats[i].channelNameHash == chash &&
			m_floats[i].curveCode == code)
		{
			return &m_floats.at(i);
		}
	}
	return 0;
}

const akPose::FloatResult* akPose::getFloatResult(UTuint32 ctype, UTuint32 chash, UTuint32 code) const
{
	for(int i=0; i<m_floats.size(); i++)
	{
		if( m_floats[i].channelType == ctype &&
			m_floats[i].channelNameHash == chash &&
			m_floats[i].curveCode == code)
		{
			return &m_floats.at(i);
		}
	}
	return 0;
}

void akPose::addFloatResult(UTuint32 ctype, UTuint32 chash, UTuint32 code, akScalar value)
{
	FloatResult result;
	result.channelType = ctype;
	result.channelNameHash = chash;
	result.curveCode = code;
	result.value = value;
	m_floats.push_back(result);
}

void akPose::fillMatrixPalette(btAlignedObjectArray<akMatrix4> &palette) const
{
	if(m_skelpose)
	{
		m_skelpose->fillMatrixPalette(palette);
	}
}

void akPose::fillDualQuatPalette(btAlignedObjectArray<akDualQuat> &palette, btAlignedObjectArray<akMatrix4> &mpalette) const
{
	if(m_skelpose)
	{
		m_skelpose->fillDualQuatPalette(palette, mpalette);
	}
}
