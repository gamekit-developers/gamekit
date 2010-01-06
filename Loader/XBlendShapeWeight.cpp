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

#include <assert.h>

#include "XBlendShapeWeight.h"

#ifdef WIN32
#else
#include "mm_malloc.h"
#endif

XBlendShapeWeight::XBlendShapeWeight(XTimeInfo* timeInfo)
{
	m_TimeInfo = timeInfo;
	m_Weights = NULL;
}

XBlendShapeWeight::~XBlendShapeWeight(void)
{
	if (m_Weights)
		//delete [] m_Weights;
		_mm_free( m_Weights );
}

std::istream &operator>>(std::istream& ifs,XBlendShapeWeight& w)
{
	char buffer[256];

	ifs >> buffer; assert( strcmp(buffer, "NumWeights") == 0 );
	ifs >> w.m_NumWeights;

	w.m_NumFrames = w.m_TimeInfo->m_iNumFrames;
	w.m_Weights = (float*)_mm_malloc( sizeof(float)*w.m_NumWeights*w.m_NumFrames, 16 );
	for ( unsigned int i = 0; i < w.m_NumFrames; i++ )
	{
		for ( unsigned int j = 0; j < w.m_NumWeights; j++ )
		{
			ifs >> w.m_Weights[ i * w.m_NumWeights + j ];
		}
	}

	return ifs;
}

void XBlendShapeWeight::getCurrentWeight( float *weights )
{
	unsigned int frame = m_TimeInfo->GetFrameNum();

	assert( frame >= 0 );

	if (frame < m_NumFrames - 1)
	{
		// linear interpolation of frame and frame+1
		float frameTime = m_TimeInfo->GetFrameT();
		assert( frameTime > 0 && frameTime < 1 );
		float oneMinusF = 1.0f - frameTime;

		for ( unsigned int i = 0; i < m_NumWeights; i++ )
		{
			float w0 = m_Weights[ frame * m_NumWeights + i ];
			float w1 = m_Weights[ (frame+1) * m_NumWeights + i ];

			weights[ i ] = w1 * frameTime + w0 * oneMinusF;
		}
	}
	else
	{
		for ( unsigned int i = 0; i < m_NumWeights; i++ )
		{
			weights[ i ] = m_Weights[ frame * m_NumWeights + i ];
		}
	}
}
