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

#include "XJointAnim.h"

#ifdef WIN32
#else
#include "mm_malloc.h"
#endif

#include <assert.h>

XJointAnim::XJointAnim(void)
{
	m_JointNameHashes = NULL;
	
	m_JointXforms = NULL;
}

XJointAnim::~XJointAnim(void)
{
	if ( m_JointNameHashes )
		delete [] m_JointNameHashes;

	if ( m_JointXforms )
		_mm_free( m_JointXforms );
}

std::istream& operator >> (std::istream& ifs, XJointAnim &ja)
{
	char buffer[256];

	ifs >> buffer;  assert( strcmp(buffer, "NumJointAnims") == 0 );
	ifs >> ja.m_NumJointAnims;

	if ( ja.m_NumJointAnims > 0 )
	{
		ja.m_JointNameHashes = new uint[ ja.m_NumJointAnims ];
		for ( uint j = 0; j < ja.m_NumJointAnims; j++ )
		{
			ifs >> buffer;
			ja.m_JointNames.push_back( string(buffer) );
			ifs >> ja.m_JointNameHashes[j];
		}

		assert( !ja.m_JointXforms );
		uint numKeys = ja.m_NumJointAnims * ja.m_NumFrames;
		ja.m_JointXforms = (XTransformation*)_mm_malloc( sizeof(XTransformation)*numKeys, 16 );
		for ( uint i = 0; i < ja.m_NumFrames; i++ )
		{
			for ( uint j = 0; j < ja.m_NumJointAnims; j++ )
			{
				ifs >> ja.m_JointXforms[ ja.m_NumJointAnims * i + j ];
			}
		}
	}

	return ifs;
}

void XJointAnim::updateJointXforms(const XTimeInfo& timeInfo, XSkeleton& skeleton)
{
	unsigned int frame = timeInfo.GetFrameNum();
	assert( frame >= 0 );
	assert( frame <= m_NumFrames );

	if ( frame < m_NumFrames - 1)
	{
		// linear interpolation of frame and frame+1
		float frameTime = timeInfo.GetFrameT();
		assert( frameTime > 0 && frameTime < 1 );

		XTransformation** xforms0 = new XTransformation*[m_NumJointAnims];
		XTransformation** xforms1 = new XTransformation*[m_NumJointAnims];
		for ( uint j = 0; j < m_NumJointAnims; j++ )
		{
			xforms0[j] = &m_JointXforms[ m_NumJointAnims * frame + j ];
		}
		for ( uint j = 0; j < m_NumJointAnims; j++ )
		{
			xforms1[j] = &m_JointXforms[ m_NumJointAnims * (frame+1) + j ];
		}

		for ( uint j = 0; j < m_NumJointAnims; j++ )
		{
			// update transform in skeleton
			XTransformation xform = LERP( frameTime, *xforms0[j], *xforms1[j] );
			xform.m_Visibility = xforms0[j]->m_Visibility;
			skeleton.updateXform( m_JointNameHashes[j], xform);
		}

		delete [] xforms0;
		delete [] xforms1;
	}
	else // frame == m_NumFrames - 1
	{
		for ( uint j = 0; j < m_NumJointAnims; j++ )
		{
			// update transform in skeleton
			XTransformation xform = m_JointXforms[ m_NumJointAnims * frame + j ];
			skeleton.updateXform( m_JointNameHashes[j], xform );
		}
	}
}
