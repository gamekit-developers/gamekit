/*
Maya Character Exporter
Copyright (c) 2009 Jun Murakawa

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. 
   If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include "XAnimation.h"

XAnimation::XAnimation(void)
{
	m_NumBlendShapes = 0;
}

XAnimation::~XAnimation(void)
{
	// delete all blendshapes
	for(XItBlendShape it = m_BlendShapes.begin(); it < m_BlendShapes.end(); it++)
	{
		delete *it;
	}
}

std::istream& operator >> (std::istream& ifs, XAnimation &anim)
{
	char buffer[256];
	int num_frames=0;
	float fStart,fEnd;

	ifs >> buffer;
	anim.m_AnimName = buffer;

	// get start and end times
	ifs >> buffer >> fStart;
	ifs >> buffer >> fEnd;
	ifs >> buffer >> num_frames;

	// set the timing info for the cycle
	anim.m_Time.Initialise(fStart,fEnd,num_frames);

	ifs >> buffer >> anim.m_NumBlendShapes;
	
	// read in blendshapes
	for ( int i = 0; i < anim.m_NumBlendShapes; i++ )
	{			
		XBlendShape* bs = new XBlendShape(&anim.m_Time);
		ifs >> *bs;
		anim.m_BlendShapes.push_back(bs);
	}
				
	anim.m_JointAnim.m_NumFrames = num_frames;
	ifs >> buffer; assert( strcmp( buffer, "JOINTANIM" ) == 0 );
	ifs >> anim.m_JointAnim;

	return ifs;
}

void XAnimation::Update(float dt)
{
	m_Time.Update(dt);
	updateBlendShapes();
}

void XAnimation::updateBlendShapes(void)
{
	// update blendshaped weights
	//for( XItBlendShape it = m_BlendShapes.begin(); it != m_BlendShapes.end() ; ++it ) 
	for ( int j = 0; j < m_NumBlendShapes; j++ )
	{
		XBlendShape *bs = m_BlendShapes[j];//*it;
		bs->updateWeights();
	}
}

void XAnimation::updateSkeleton(XSkeleton& skeleton)
{
	m_JointAnim.updateJointXforms( m_Time, skeleton );
}
