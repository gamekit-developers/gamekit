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

#include "akTransformState.h"
#include "akDualQuat.h"

const akMatrix4 inverseTranspose(const akMatrix4& mat)
{
	//TODO optimise
	akMatrix4 tmp = inverse(mat);
	return transpose(tmp);
}

void akGeometryDeformer::LBSkinning(
	const UTsize vtxCount, 
	const btAlignedObjectArray<akMatrix4>& matrices, 
	const float*     weights, const UTsize weightsStride, 
	const UTuint8*   indices, const UTsize indicesStride, 
	const akVector3* vtxSrc,  const UTsize vtxSrcStride, 
	akVector3*       vtxDst,  const UTsize vtxDstStride, 
	const akVector3* normSrc, const UTsize normSrcStride, 
	akVector3*       normDst, const UTsize normDstStride)
{
	for(unsigned int i=0; i<vtxCount; i++)
	{
		akMatrix4 mat(matrices[indices[0]] * weights[0]);
		if (weights[1]) mat += matrices[indices[1]] * weights[1];
		if (weights[2]) mat += matrices[indices[2]] * weights[2];
		if (weights[3]) mat += matrices[indices[3]] * weights[3];
		
		*vtxDst = (mat * akVector4(*vtxSrc, 1.f)).getXYZ();
		
		if(normSrc)
		{
			mat = inverseTranspose(mat);
			*normDst = (mat * akVector4(*normSrc, 0.0f)).getXYZ();
			normalize(*normDst);
			
			advancePointer(normSrc, normSrcStride);
			advancePointer(normDst, normDstStride);
		}

		advancePointer(weights, weightsStride);
		advancePointer(indices, indicesStride);
		advancePointer(vtxSrc, vtxSrcStride);
		advancePointer(vtxDst, vtxDstStride);
	}
}

void akGeometryDeformer::LBSkinningFast(
	const UTsize vtxCount, 
	const btAlignedObjectArray<akMatrix4>& matrices, 
	const float*     weights, const UTsize weightsStride, 
	const UTuint8*   indices, const UTsize indicesStride, 
	const akVector3* vtxSrc,  const UTsize vtxSrcStride, 
	akVector3*       vtxDst,  const UTsize vtxDstStride, 
	const akVector3* normSrc, const UTsize normSrcStride, 
	akVector3*       normDst, const UTsize normDstStride)
{
	for(unsigned int i=0; i<vtxCount; i++)
	{
		akMatrix4 mat(matrices[indices[0]] * weights[0]);
		if (weights[1]) mat += matrices[indices[1]] * weights[1];
		if (weights[2]) mat += matrices[indices[2]] * weights[2];
		if (weights[3]) mat += matrices[indices[3]] * weights[3];
		
		*vtxDst = (mat * akVector4(*vtxSrc, 1.f)).getXYZ();
		
		if(normSrc)
		{
			*normDst = (mat * akVector4(*normSrc, 0.0f)).getXYZ();
			normalize(*normDst);
			
			advancePointer(normSrc, normSrcStride);
			advancePointer(normDst, normDstStride);
		}

		advancePointer(weights, weightsStride);
		advancePointer(indices, indicesStride);
		advancePointer(vtxSrc, vtxSrcStride);
		advancePointer(vtxDst, vtxDstStride);
	}
}

/*
    The code following this comment is written by Xavier T. based on origninal code 
    under the followind lisence:
    (Note that this is a complete rewrite and not an altered copy of Kavan's code.)
    
    Version 1.0.0, February 7th, 2007

    Copyright (C) 2006-2007 University of Dublin, Trinity College, All Rights 
    Reserved

    This software is provided 'as-is', without any express or implied
    warranty.  In no event will the author(s) be held liable for any damages
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

    Author: Ladislav Kavan, kavanl@cs.tcd.ie

-------------------------------------------------------------------------------
*/

void akGeometryDeformer::DLBSkinning(
	const UTsize vtxCount, 
	const btAlignedObjectArray<akDualQuat>& dquats, 
	const btAlignedObjectArray<akMatrix4>& matrices, 
	const float*     weights, const UTsize weightsStride, 
	const UTuint8*   indices, const UTsize indicesStride, 
	const akVector3* vtxSrc,  const UTsize vtxSrcStride, 
	akVector3*       vtxDst,  const UTsize vtxDstStride, 
	const akVector3* normSrc, const UTsize normSrcStride, 
	akVector3*       normDst, const UTsize normDstStride)
{
	for(unsigned int i=0; i<vtxCount; i++)
	{
		// 1st pass for non rigid part of the transformation using matrices
		akMatrix4 mat(matrices[indices[0]] * weights[0]);
		if (weights[1]) mat += matrices[indices[1]] * weights[1];
		if (weights[2]) mat += matrices[indices[2]] * weights[2];
		if (weights[3]) mat += matrices[indices[3]] * weights[3];
		
		akVector3 tmpPos = (mat * akVector4(*vtxSrc, 1.f)).getXYZ();
		
		// 2nd pass for rigid transformation (rotaion & location) using dual quats
		akDualQuat dq = dquats[indices[0]] * weights[0];
		if (weights[1]) dq += dquats[indices[1]] * weights[1];
		if (weights[2]) dq += dquats[indices[2]] * weights[2];
		if (weights[3]) dq += dquats[indices[3]] * weights[3];
			
		dq /= length(dq.nondual);
	
		akVector3 ndxyz(dq.nondual.getXYZ());
		akVector3 dxyz(dq.dual.getXYZ());
		*vtxDst = tmpPos + 2.0 * cross( ndxyz, cross(ndxyz, tmpPos) + dq.nondual.getW() * tmpPos ) +
				2.0 * ( dq.nondual.getW() * dxyz - dq.dual.getW() * ndxyz + cross(ndxyz, dxyz) );
	
		if(normSrc)
		{
			// 1st pass
			mat = inverseTranspose(mat);
			akVector3 tmpNorm = (mat * akVector4(*normSrc, 0.0f)).getXYZ();
			normalize(*normDst);
			
			// 2nd pass
			*normDst = tmpNorm + 2.0 * cross( ndxyz, cross(ndxyz, tmpNorm) + dq.nondual.getW() * tmpNorm );
			
			advancePointer(normSrc, normSrcStride);
			advancePointer(normDst, normDstStride);
		}

		advancePointer(weights, weightsStride);
		advancePointer(indices, indicesStride);
		advancePointer(vtxSrc, vtxSrcStride);
		advancePointer(vtxDst, vtxDstStride);
	}
}

void akGeometryDeformer::DLBSkinningFast(
	const UTsize vtxCount, 
	const btAlignedObjectArray<akDualQuat>& dquats, 
	const btAlignedObjectArray<akMatrix4>& matrices, 
	const float*     weights, const UTsize weightsStride, 
	const UTuint8*   indices, const UTsize indicesStride, 
	const akVector3* vtxSrc,  const UTsize vtxSrcStride, 
	akVector3*       vtxDst,  const UTsize vtxDstStride, 
	const akVector3* normSrc, const UTsize normSrcStride, 
	akVector3*       normDst, const UTsize normDstStride)
{
	for(unsigned int i=0; i<vtxCount; i++)
	{
		// 1st pass for non rigid part of the transformation using matrices
		akMatrix4 mat(matrices[indices[0]] * weights[0]);
		if (weights[1]) mat += matrices[indices[1]] * weights[1];
		if (weights[2]) mat += matrices[indices[2]] * weights[2];
		if (weights[3]) mat += matrices[indices[3]] * weights[3];
		
		akVector3 tmpPos = (mat * akVector4(*vtxSrc, 1.f)).getXYZ();
		
		// 2nd pass for rigid transformation (rotaion & location) using dual quats
		akDualQuat dq = dquats[indices[0]] * weights[0];
		if (weights[1]) dq += dquats[indices[1]] * weights[1];
		if (weights[2]) dq += dquats[indices[2]] * weights[2];
		if (weights[3]) dq += dquats[indices[3]] * weights[3];
			
		dq /= length(dq.nondual);
	
		akVector3 ndxyz(dq.nondual.getXYZ());
		akVector3 dxyz(dq.dual.getXYZ());
		*vtxDst = tmpPos + 2.0 * cross( ndxyz, cross(ndxyz, tmpPos) + dq.nondual.getW() * tmpPos ) +
				2.0 * ( dq.nondual.getW() * dxyz - dq.dual.getW() * ndxyz + cross(ndxyz, dxyz) );
	
		if(normSrc)
		{
			// 1st pass
			akVector3 tmpNorm = (mat * akVector4(*normSrc, 0.0f)).getXYZ();
			normalize(*normDst);
			
			// 2nd pass
			*normDst = tmpNorm + 2.0 * cross( ndxyz, cross(ndxyz, tmpNorm) + dq.nondual.getW() * tmpNorm );
			
			advancePointer(normSrc, normSrcStride);
			advancePointer(normDst, normDstStride);
		}

		advancePointer(weights, weightsStride);
		advancePointer(indices, indicesStride);
		advancePointer(vtxSrc, vtxSrcStride);
		advancePointer(vtxDst, vtxDstStride);
	}
}

void akGeometryDeformer::DLBSkinningMoreFast(
	const UTsize vtxCount, 
	const btAlignedObjectArray<akDualQuat>& dquats, 
	const btAlignedObjectArray<akMatrix4>& matrices, 
	const float*     weights, const UTsize weightsStride, 
	const UTuint8*   indices, const UTsize indicesStride, 
	const akVector3* vtxSrc,  const UTsize vtxSrcStride, 
	akVector3*       vtxDst,  const UTsize vtxDstStride, 
	const akVector3* normSrc, const UTsize normSrcStride, 
	akVector3*       normDst, const UTsize normDstStride)
{
	for(unsigned int i=0; i<vtxCount; i++)
	{
		akDualQuat dq = dquats[indices[0]] * weights[0];
		if (weights[1]) dq += dquats[indices[1]] * weights[1];
		if (weights[2]) dq += dquats[indices[2]] * weights[2];
		if (weights[3]) dq += dquats[indices[3]] * weights[3];
			
		dq /= length(dq.nondual);
	
		akVector3 ndxyz(dq.nondual.getXYZ());
		akVector3 dxyz(dq.dual.getXYZ());
		akVector3 in(*vtxSrc);
		*vtxDst = in + 2.0 * cross( ndxyz, cross(ndxyz, in) + dq.nondual.getW() * in ) +
				2.0 * ( dq.nondual.getW() * dxyz - dq.dual.getW() * ndxyz + cross(ndxyz, dxyz) );
	
		if(normSrc)
		{
			akVector3 inn(*normSrc);
			*normDst = inn + 2.0 * cross( ndxyz, cross(ndxyz, inn) + dq.nondual.getW() * inn );
			
			advancePointer(normSrc, normSrcStride);
			advancePointer(normDst, normDstStride);
		}

		advancePointer(weights, weightsStride);
		advancePointer(indices, indicesStride);
		advancePointer(vtxSrc, vtxSrcStride);
		advancePointer(vtxDst, vtxDstStride);
	}
}

void akGeometryDeformer::DLBSkinningAntipodality(
	const UTsize vtxCount, 
	const btAlignedObjectArray<akDualQuat>& dquats, 
	const btAlignedObjectArray<akMatrix4>& matrices, 
	const float*     weights, const UTsize weightsStride, 
	const UTuint8*   indices, const UTsize indicesStride, 
	const akVector3* vtxSrc,  const UTsize vtxSrcStride, 
	akVector3*       vtxDst,  const UTsize vtxDstStride, 
	const akVector3* normSrc, const UTsize normSrcStride, 
	akVector3*       normDst, const UTsize normDstStride)
{
	for(unsigned int i=0; i<vtxCount; i++)
	{
		akDualQuat dq0 = dquats[indices[0]];
		akDualQuat dq1 = dquats[indices[1]];
		akDualQuat dq2 = dquats[indices[2]];
		akDualQuat dq3 = dquats[indices[3]];
		
		if( dot(dq0.nondual, dq1.nondual) < 0.0) dq1 *= -1.0;
		if( dot(dq0.nondual, dq2.nondual) < 0.0) dq2 *= -1.0;
		if( dot(dq0.nondual, dq3.nondual) < 0.0) dq3 *= -1.0;
		
		akDualQuat dq = dq0 * weights[0];
		if (weights[1]) dq += dq1 * weights[1];
		if (weights[2]) dq += dq2 * weights[2];
		if (weights[3]) dq += dq3 * weights[3];
			
		dq /= length(dq.nondual);
	
		akVector3 ndxyz(dq.nondual.getXYZ());
		akVector3 dxyz(dq.dual.getXYZ());
		akVector3 in(*vtxSrc);
		*vtxDst = in + 2.0 * cross( ndxyz, cross(ndxyz, in) + dq.nondual.getW() * in ) +
				2.0 * ( dq.nondual.getW() * dxyz - dq.dual.getW() * ndxyz + cross(ndxyz, dxyz) );
	
		if(normSrc)
		{
			akVector3 inn(*normSrc);
			*normDst = inn + 2.0 * cross( ndxyz, cross(ndxyz, inn) + dq.nondual.getW() * inn );
			
			advancePointer(normSrc, normSrcStride);
			advancePointer(normDst, normDstStride);
		}

		advancePointer(weights, weightsStride);
		advancePointer(indices, indicesStride);
		advancePointer(vtxSrc, vtxSrcStride);
		advancePointer(vtxDst, vtxDstStride);
	}
}
