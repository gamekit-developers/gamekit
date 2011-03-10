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

#include "akGeometryDeformer.h"


void akGeometryDeformer::vertexSkinning(
	const UTsize vtxCount,
	const btAlignedObjectArray<akMatrix4>& matrices,
	const float *         weights, UTsize weightsStride,
	const unsigned char * index,   UTsize indexStride,
	const akVector3 *         vtxSrc,  UTsize vtxSrcStride,
	akVector3 *               vtxDst,  UTsize vtxDstStride,
	const akVector3 *         normSrc, UTsize normSrcStride,
	akVector3 *               normDst, UTsize normDstStride)
{
	//loop trough all vertices
	for(unsigned int i=0; i<vtxCount; i++)
	{
		akVector4 outpos(0.f);
		akVector4 outnorm(0.f);
		
		const akVector4 pos(vtxSrc[0], 1.f);
		
		if (weights[0]) outpos = matrices[index[0]] * pos * weights[0];
		if (weights[1]) outpos += matrices[index[1]] * pos * weights[1];
		if (weights[2]) outpos += matrices[index[2]] * pos * weights[2];
		if (weights[3]) outpos += matrices[index[3]] * pos * weights[3];
		
		vtxDst[0] = outpos.getXYZ();
		
		if(normSrc)
		{
			const akVector4 norm(normSrc[0], 1.0f);
			
			if (weights[0]) outnorm = matrices[index[0]] * norm * weights[0];
			if (weights[1]) outnorm += matrices[index[1]] * norm * weights[1];
			if (weights[2]) outnorm += matrices[index[2]] * norm * weights[2];
			if (weights[3]) outnorm += matrices[index[3]] * norm * weights[3];
			
			normalize(outnorm);
			
			normDst[0] = outnorm.getXYZ();
			
			advancePointer(normSrc, normSrcStride);
			advancePointer(normDst, normDstStride);
		}

		advancePointer(weights, weightsStride);
		advancePointer(index, indexStride);
		advancePointer(vtxSrc, vtxSrcStride);
		advancePointer(vtxDst, vtxDstStride);
	}
}
