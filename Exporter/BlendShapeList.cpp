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

#ifdef WIN32
	#ifndef NT_APP
		#define NT_APP
	#endif
	#pragma warning(disable:4786)
#endif

#include <vector>
#include <ostream>

#include <maya/MItDependencyNodes.h>

#include "BlendShapeList.h"

BlendShapeList::BlendShapeList(void)
{
	m_BlendShapes = NULL;
}

BlendShapeList::~BlendShapeList(void)
{
	if( m_BlendShapes )
		delete [] m_BlendShapes;
}


void BlendShapeList::extractAllBlendShapes()
{
	// create an iterator to go through all blend shapes
	MItDependencyNodes it(MFn::kBlendShape); 
	m_NumBlendShapes = 0;
	while(!it.isDone())
	{
		m_NumBlendShapes++;
		it.next();
	}

	if ( m_NumBlendShapes > 0 )
	{
		m_BlendShapes = new BlendShape[ m_NumBlendShapes ];

		it.reset(MFn::kBlendShape); 
		int i = 0;
		while(!it.isDone())
		{
			m_BlendShapes[i++].extractBlendShape( it.thisNode() );
			it.next();
		}
	}
}

BlendShapeBinding BlendShapeList::getBlendShape(const std::string& meshName, BlendShape** blendShape) const
{
	// find BlendShape with a mesh named meshName
	for( int i = 0; i < m_NumBlendShapes; i++ )
	{
		BlendShape *bs = &m_BlendShapes[i];
		if ( bs->m_BaseName.compare( meshName ) == 0 )
		{
			// found
			*blendShape = bs;
			return BASE;
		}
		// check if mesh is a target 
		for ( unsigned int j = 0; j < bs->m_NumWeights; j++)
		{
			BlendShapeWeight *bsw = &bs->m_WeightList[j];
			if ( bsw->m_TargetName.compare( meshName ) == 0 )
			{
				*blendShape = bs;
				return TARGET;
			}
		}

	}

	return NONE;
}

void BlendShapeList::outputBlendShapeAnims(std::ostream& ofs, int startFrame, int endFrame, SAnimation* anim)
{
	anim->numBlendAnims = m_NumBlendShapes;
	if ( anim->numBlendAnims > 0 )
	{
		anim->blendAnims = (SBlendAnim*)calloc(anim->numBlendAnims, sizeof(SBlendAnim));//new SBlendAnim[ anim->numBlendAnims ];
		for( int i = 0; i < m_NumBlendShapes; i++ )
		{
			BlendShape *bs = &m_BlendShapes[i];
			bs->outputBlendAnim(ofs, startFrame, endFrame, &anim->blendAnims[i]);
		}
	}
}
