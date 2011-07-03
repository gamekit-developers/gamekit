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

#include "akMorphTarget.h"
#include "akTransformState.h"
#include "akDualQuat.h"


void inverseTranspose(akMatrix4& mat)
{
	//TODO optimise
	akMatrix4 tmp = inverse(mat);
	mat = transpose(tmp);
}

void akGeometryDeformer::Morphing(const akMorphTarget *morph, akScalar weight,
									akVector3 *vtxDst, const UTsize vtxDstStride,
									akVector3 *normDst, const UTsize normDstStride)
{
	for(unsigned int i=0; i<morph->getSize(); i++)
	{
		UTuint32 idx = morph->getIndex(i);
		
		// position
		const akVector3* src = (akVector3*)((char*)vtxDst + idx * vtxDstStride);
		akVector3* dst = (akVector3*)((char*)vtxDst + idx * vtxDstStride);
		*dst = *src + weight * morph->getVertexOffset(i);
		// normal
		src = (akVector3*)((char*)normDst + idx * normDstStride);
		dst = (akVector3*)((char*)normDst + idx * normDstStride);
		*dst = *src + weight * morph->getNormal(i);
	}
}

void akGeometryDeformer::Skinning(akGeometryDeformer::SkinningOption method, 
								  akGeometryDeformer::NormalsOption  normalOpt, 
								  const btAlignedObjectArray<akMatrix4>* matrices, 
								  const btAlignedObjectArray<akDualQuat>* dquats, 
								  const UTsize vtxCount, 
								  const float *weights,     UTsize weightsStride, 
								  const UTuint8 *indices,   UTsize indicesStride, 
								  const akVector3 *vtxSrc,  UTsize vtxSrcStride, 
								  akVector3 *vtxDst,        UTsize vtxDstStride, 
								  const akVector3 *normSrc, UTsize normSrcStride, 
								  akVector3 *normDst,       UTsize normDstStride)
{
	akGeometryDeformer::SkinningOption m = method;
	akGeometryDeformer::NormalsOption n = normalOpt;
	
	if(!dquats)
		m = GD_SO_MATRIX;
		
	if(!normSrc || !normDst)
		n = GD_NO_NONE;
	
	switch(m)
	{
	case GD_SO_DUAL_QUAT:
		switch(n)
		{
		case GD_NO_NONE:
			DLBSkinningNoNormals(matrices,
								 dquats,
								 vtxCount,
								 weights, weightsStride,
								 indices, indicesStride,
								 vtxSrc, vtxSrcStride,
								 vtxDst, vtxDstStride);
			break;
		case GD_NO_FULL:
			DLBSkinning(matrices,
						dquats,
						vtxCount,
						weights, weightsStride,
						indices, indicesStride,
						vtxSrc, vtxSrcStride,
						vtxDst, vtxDstStride,
						normSrc, normSrcStride,
						normDst, normDstStride);
			break;
		case GD_NO_UNIFORM_SCALE:
			DLBSkinningUniformScale(matrices,
									dquats,
									vtxCount,
									weights, weightsStride,
									indices, indicesStride,
									vtxSrc, vtxSrcStride,
									vtxDst, vtxDstStride,
									normSrc, normSrcStride,
									normDst, normDstStride);
			break;
		case GD_NO_NOSCALE:
		default:
			DLBSkinningNoScaling(matrices,
								 dquats,
								 vtxCount,
								 weights, weightsStride,
								 indices, indicesStride,
								 vtxSrc, vtxSrcStride,
								 vtxDst, vtxDstStride,
								 normSrc, normSrcStride,
								 normDst, normDstStride);
			break;
		}
		break;
	
	case GD_SO_DUAL_QUAT_ANTIPOD:
		switch(n)
		{
			case GD_NO_NONE:
				DLBAntipodalitySkinningNoNormals(matrices,
												 dquats,
												 vtxCount,
												 weights, weightsStride,
												 indices, indicesStride,
												 vtxSrc, vtxSrcStride,
												 vtxDst, vtxDstStride);
				break;
			case GD_NO_FULL:
				DLBAntipodalitySkinning(matrices,
												 dquats,
												 vtxCount,
												 weights, weightsStride,
												 indices, indicesStride,
												 vtxSrc, vtxSrcStride,
												 vtxDst, vtxDstStride,
												 normSrc, normSrcStride,
												 normDst, normDstStride);
				break;
			case GD_NO_UNIFORM_SCALE:
				DLBAntipodalitySkinningUniformScale(matrices,
												 dquats,
												 vtxCount,
												 weights, weightsStride,
												 indices, indicesStride,
												 vtxSrc, vtxSrcStride,
												 vtxDst, vtxDstStride,
												 normSrc, normSrcStride,
												 normDst, normDstStride);
				break;
			case GD_NO_NOSCALE:
			default:
				DLBAntipodalitySkinningNoScaling(matrices,
												 dquats,
												 vtxCount,
												 weights, weightsStride,
												 indices, indicesStride,
												 vtxSrc, vtxSrcStride,
												 vtxDst, vtxDstStride,
												 normSrc, normSrcStride,
												 normDst, normDstStride);
				break;
		}
		break;
	
	case GD_SO_MATRIX:
	default:
		switch(n)
		{
			case GD_NO_NONE:
				LBSkinningNoNormals(matrices,
								vtxCount,
								weights, weightsStride,
								indices, indicesStride,
								vtxSrc, vtxSrcStride,
								vtxDst, vtxDstStride);
				break;
			case GD_NO_FULL:
				LBSkinning(matrices,
								vtxCount,
								weights, weightsStride,
								indices, indicesStride,
								vtxSrc, vtxSrcStride,
								vtxDst, vtxDstStride,
								normSrc, normSrcStride,
								normDst, normDstStride);
				break;
			case GD_NO_NOSCALE:
			case GD_NO_UNIFORM_SCALE:
			default:
				LBSkinningUniformScale(matrices,
								vtxCount,
								weights, weightsStride,
								indices, indicesStride,
								vtxSrc, vtxSrcStride,
								vtxDst, vtxDstStride,
								normSrc, normSrcStride,
								normDst, normDstStride);
				break;
		}
		break;
	}
}


void akGeometryDeformer::LBSkinningNoNormals(
	const btAlignedObjectArray<akMatrix4>* mpalette, 
	const UTsize vtxCount, 
	const float*     weights, const UTsize weightsStride, 
	const UTuint8*   indices, const UTsize indicesStride, 
	const akVector3* vtxSrc,  const UTsize vtxSrcStride, 
	akVector3*       vtxDst,  const UTsize vtxDstStride)
{
	const btAlignedObjectArray<akMatrix4>& matrices = *mpalette;
	
	for(unsigned int i=0; i<vtxCount; i++)
	{
		akVector4 pos(vtxSrc[0].getX(), vtxSrc[0].getY(), vtxSrc[0].getZ(), 1);
		akVector4 posout(0,0,0,1);
		if (weights[0]) posout += matrices[indices[0]] * weights[0] * pos;
		if (weights[1]) posout += matrices[indices[1]] * weights[1] * pos;
		if (weights[2]) posout += matrices[indices[2]] * weights[2] * pos;
		if (weights[3]) posout += matrices[indices[3]] * weights[3] * pos;
		
		*vtxDst = posout.getXYZ();
		
		akAdvancePointer(weights, weightsStride);
		akAdvancePointer(indices, indicesStride);
		akAdvancePointer(vtxSrc, vtxSrcStride);
		akAdvancePointer(vtxDst, vtxDstStride);
	}
}

void akGeometryDeformer::LBSkinning(
	const btAlignedObjectArray<akMatrix4>* mpalette, 
	const UTsize vtxCount, 
	const float*     weights, const UTsize weightsStride, 
	const UTuint8*   indices, const UTsize indicesStride, 
	const akVector3* vtxSrc,  const UTsize vtxSrcStride, 
	akVector3*       vtxDst,  const UTsize vtxDstStride, 
	const akVector3* normSrc, const UTsize normSrcStride, 
	akVector3*       normDst, const UTsize normDstStride)
{
	const btAlignedObjectArray<akMatrix4>& matrices = *mpalette;
	
	for(unsigned int i=0; i<vtxCount; i++)
	{
		akMatrix4 mat(matrices[indices[0]] * weights[0]);
		if (weights[1]) mat += matrices[indices[1]] * weights[1];
		if (weights[2]) mat += matrices[indices[2]] * weights[2];
		if (weights[3]) mat += matrices[indices[3]] * weights[3];
		
		// position
		*vtxDst = (mat * akVector4(*vtxSrc, 1.f)).getXYZ();
		
		// normal
		inverseTranspose(mat);
		*normDst = (mat * akVector4(*normSrc, 0.0f)).getXYZ();
		*normDst = normalize(*normDst);
		
		akAdvancePointer(normSrc, normSrcStride);
		akAdvancePointer(normDst, normDstStride);
		akAdvancePointer(weights, weightsStride);
		akAdvancePointer(indices, indicesStride);
		akAdvancePointer(vtxSrc, vtxSrcStride);
		akAdvancePointer(vtxDst, vtxDstStride);
	}
}

void akGeometryDeformer::LBSkinningUniformScale(
	const btAlignedObjectArray<akMatrix4>* mpalette, 
	const UTsize vtxCount, 
	const float*     weights, const UTsize weightsStride, 
	const UTuint8*   indices, const UTsize indicesStride, 
	const akVector3* vtxSrc,  const UTsize vtxSrcStride, 
	akVector3*       vtxDst,  const UTsize vtxDstStride, 
	const akVector3* normSrc, const UTsize normSrcStride, 
	akVector3*       normDst, const UTsize normDstStride)
{
	const btAlignedObjectArray<akMatrix4>& matrices = *mpalette;
	
	for(unsigned int i=0; i<vtxCount; i++)
	{
		akMatrix4 mat(matrices[indices[0]] * weights[0]);
		if (weights[1]) mat += matrices[indices[1]] * weights[1];
		if (weights[2]) mat += matrices[indices[2]] * weights[2];
		if (weights[3]) mat += matrices[indices[3]] * weights[3];
		
		// position
		*vtxDst = (mat * akVector4(*vtxSrc, 1.f)).getXYZ();
		// normal
		*normDst = (mat * akVector4(*normSrc, 0.0f)).getXYZ();
		*normDst = normalize(*normDst);
		
		akAdvancePointer(normSrc, normSrcStride);
		akAdvancePointer(normDst, normDstStride);
		akAdvancePointer(weights, weightsStride);
		akAdvancePointer(indices, indicesStride);
		akAdvancePointer(vtxSrc, vtxSrcStride);
		akAdvancePointer(vtxDst, vtxDstStride);
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
	const btAlignedObjectArray<akMatrix4>* mpalette, 
	const btAlignedObjectArray<akDualQuat>* dqpalette, 
	const UTsize vtxCount, 
	const float*     weights, const UTsize weightsStride, 
	const UTuint8*   indices, const UTsize indicesStride, 
	const akVector3* vtxSrc,  const UTsize vtxSrcStride, 
	akVector3*       vtxDst,  const UTsize vtxDstStride, 
	const akVector3* normSrc, const UTsize normSrcStride, 
	akVector3*       normDst, const UTsize normDstStride)
{
	const btAlignedObjectArray<akMatrix4>& matrices = *mpalette;
	const btAlignedObjectArray<akDualQuat>& dquats = *dqpalette;
	
	for(unsigned int i=0; i<vtxCount; i++)
	{
		// position 1st pass for non rigid part of the transformation using matrices
		akMatrix4 mat(matrices[indices[0]] * weights[0]);
		if (weights[1]) mat += matrices[indices[1]] * weights[1];
		if (weights[2]) mat += matrices[indices[2]] * weights[2];
		if (weights[3]) mat += matrices[indices[3]] * weights[3];
		
		akVector3 tmpPos = (mat * akVector4(*vtxSrc, 1.f)).getXYZ();
		
		// position 2nd pass for rigid transformation (rotaion & location) using dual quats
		akDualQuat dq = dquats[indices[0]] * weights[0];
		if (weights[1]) dq += dquats[indices[1]] * weights[1];
		if (weights[2]) dq += dquats[indices[2]] * weights[2];
		if (weights[3]) dq += dquats[indices[3]] * weights[3];
			
		dq /= length(dq.n);
	
		akVector3 ndxyz(dq.n.getXYZ());
		akVector3 dxyz(dq.d.getXYZ());
		*vtxDst = tmpPos + 2.0 * cross( ndxyz, cross(ndxyz, tmpPos) + dq.n.getW() * tmpPos ) +
				2.0 * ( dq.n.getW() * dxyz - dq.d.getW() * ndxyz + cross(ndxyz, dxyz) );
	
		// normal 1st pass
		inverseTranspose(mat);
		akVector3 tmpNorm = (mat * akVector4(*normSrc, 0.0f)).getXYZ();
		
		// normal 2nd pass
		*normDst = tmpNorm + 2.0 * cross( ndxyz, cross(ndxyz, tmpNorm) + dq.n.getW() * tmpNorm );
		*normDst = normalize(*normDst);
		
		akAdvancePointer(normSrc, normSrcStride);
		akAdvancePointer(normDst, normDstStride);
		akAdvancePointer(weights, weightsStride);
		akAdvancePointer(indices, indicesStride);
		akAdvancePointer(vtxSrc, vtxSrcStride);
		akAdvancePointer(vtxDst, vtxDstStride);
	}
}

void akGeometryDeformer::DLBSkinningNoNormals(
	const btAlignedObjectArray<akMatrix4>* mpalette, 
	const btAlignedObjectArray<akDualQuat>* dqpalette, 
	const UTsize vtxCount, 
	const float*     weights, const UTsize weightsStride, 
	const UTuint8*   indices, const UTsize indicesStride, 
	const akVector3* vtxSrc,  const UTsize vtxSrcStride, 
	akVector3*       vtxDst,  const UTsize vtxDstStride)
{
	const btAlignedObjectArray<akMatrix4>& matrices = *mpalette;
	const btAlignedObjectArray<akDualQuat>& dquats = *dqpalette;
	
	for(unsigned int i=0; i<vtxCount; i++)
	{
		// position 1st pass for non rigid part of the transformation using matrices
		akVector4 pos(vtxSrc[0].getX(), vtxSrc[0].getY(), vtxSrc[0].getZ(), 1);
		akVector4 posout(0,0,0,1);
		if (weights[0]) posout += matrices[indices[0]] * weights[0] * pos;
		if (weights[1]) posout += matrices[indices[1]] * weights[1] * pos;
		if (weights[2]) posout += matrices[indices[2]] * weights[2] * pos;
		if (weights[3]) posout += matrices[indices[3]] * weights[3] * pos;
		
		// position 2nd pass for rigid transformation (rotaion & location) using dual quats
		akDualQuat dq = dquats[indices[0]] * weights[0];
		if (weights[1]) dq += dquats[indices[1]] * weights[1];
		if (weights[2]) dq += dquats[indices[2]] * weights[2];
		if (weights[3]) dq += dquats[indices[3]] * weights[3];
			
		dq /= length(dq.n);
	
		akVector3 tmpPos2(posout.getXYZ());
		akVector3 ndxyz(dq.n.getXYZ());
		akVector3 dxyz(dq.d.getXYZ());
		*vtxDst = tmpPos2 + 2.0 * cross( ndxyz, cross(ndxyz, tmpPos2) + dq.n.getW() * tmpPos2 ) +
				2.0 * ( dq.n.getW() * dxyz - dq.d.getW() * ndxyz + cross(ndxyz, dxyz) );
	
		akAdvancePointer(weights, weightsStride);
		akAdvancePointer(indices, indicesStride);
		akAdvancePointer(vtxSrc, vtxSrcStride);
		akAdvancePointer(vtxDst, vtxDstStride);
	}
}

void akGeometryDeformer::DLBSkinningUniformScale(
	const btAlignedObjectArray<akMatrix4>* mpalette, 
	const btAlignedObjectArray<akDualQuat>* dqpalette, 
	const UTsize vtxCount, 
	const float*     weights, const UTsize weightsStride, 
	const UTuint8*   indices, const UTsize indicesStride, 
	const akVector3* vtxSrc,  const UTsize vtxSrcStride, 
	akVector3*       vtxDst,  const UTsize vtxDstStride, 
	const akVector3* normSrc, const UTsize normSrcStride, 
	akVector3*       normDst, const UTsize normDstStride)
{
	const btAlignedObjectArray<akMatrix4>& matrices = *mpalette;
	const btAlignedObjectArray<akDualQuat>& dquats = *dqpalette;
	
	for(unsigned int i=0; i<vtxCount; i++)
	{
		// positions 1st pass for non rigid part of the transformation using matrices
		akMatrix4 mat(matrices[indices[0]] * weights[0]);
		if (weights[1]) mat += matrices[indices[1]] * weights[1];
		if (weights[2]) mat += matrices[indices[2]] * weights[2];
		if (weights[3]) mat += matrices[indices[3]] * weights[3];
		
		akVector3 tmpPos = (mat * akVector4(*vtxSrc, 1.f)).getXYZ();
		
		// position 2nd pass for rigid transformation (rotaion & location) using dual quats
		akDualQuat dq = dquats[indices[0]] * weights[0];
		if (weights[1]) dq += dquats[indices[1]] * weights[1];
		if (weights[2]) dq += dquats[indices[2]] * weights[2];
		if (weights[3]) dq += dquats[indices[3]] * weights[3];
			
		dq /= length(dq.n);
	
		akVector3 ndxyz(dq.n.getXYZ());
		akVector3 dxyz(dq.d.getXYZ());
		*vtxDst = tmpPos + 2.0 * cross( ndxyz, cross(ndxyz, tmpPos) + dq.n.getW() * tmpPos ) +
				2.0 * ( dq.n.getW() * dxyz - dq.d.getW() * ndxyz + cross(ndxyz, dxyz) );
	
		// normal 1st pass
		akVector3 tmpNorm = (mat * akVector4(*normSrc, 0.0f)).getXYZ();
		
		// normal 2nd pass
		*normDst = tmpNorm + 2.0 * cross( ndxyz, cross(ndxyz, tmpNorm) + dq.n.getW() * tmpNorm );
		*normDst = normalize(*normDst);
		
		akAdvancePointer(normSrc, normSrcStride);
		akAdvancePointer(normDst, normDstStride);
		akAdvancePointer(weights, weightsStride);
		akAdvancePointer(indices, indicesStride);
		akAdvancePointer(vtxSrc, vtxSrcStride);
		akAdvancePointer(vtxDst, vtxDstStride);
	}
}

void akGeometryDeformer::DLBSkinningNoScaling(
	const btAlignedObjectArray<akMatrix4>* mpalette,
	const btAlignedObjectArray<akDualQuat>* dqpalette, 
	const UTsize vtxCount,
	const float*     weights, const UTsize weightsStride, 
	const UTuint8*   indices, const UTsize indicesStride, 
	const akVector3* vtxSrc,  const UTsize vtxSrcStride, 
	akVector3*       vtxDst,  const UTsize vtxDstStride, 
	const akVector3* normSrc, const UTsize normSrcStride, 
	akVector3*       normDst, const UTsize normDstStride)
{
	const btAlignedObjectArray<akMatrix4>& matrices = *mpalette;
	const btAlignedObjectArray<akDualQuat>& dquats = *dqpalette;
	
	for(unsigned int i=0; i<vtxCount; i++)
	{
		akDualQuat dq = dquats[indices[0]] * weights[0];
		if (weights[1]) dq += dquats[indices[1]] * weights[1];
		if (weights[2]) dq += dquats[indices[2]] * weights[2];
		if (weights[3]) dq += dquats[indices[3]] * weights[3];
			
		dq /= length(dq.n);
	
		akVector3 ndxyz(dq.n.getXYZ());
		akVector3 dxyz(dq.d.getXYZ());
		akVector3 in(*vtxSrc);
		*vtxDst = in + 2.0 * cross( ndxyz, cross(ndxyz, in) + dq.n.getW() * in ) +
				2.0 * ( dq.n.getW() * dxyz - dq.d.getW() * ndxyz + cross(ndxyz, dxyz) );
	
		akVector3 inn(*normSrc);
		*normDst = inn + 2.0 * cross( ndxyz, cross(ndxyz, inn) + dq.n.getW() * inn );
		
		akAdvancePointer(normSrc, normSrcStride);
		akAdvancePointer(normDst, normDstStride);
		akAdvancePointer(weights, weightsStride);
		akAdvancePointer(indices, indicesStride);
		akAdvancePointer(vtxSrc, vtxSrcStride);
		akAdvancePointer(vtxDst, vtxDstStride);
	}
}

void akGeometryDeformer::DLBAntipodalitySkinning(
	const btAlignedObjectArray<akMatrix4>* mpalette, 
	const btAlignedObjectArray<akDualQuat>* dqpalette, 
	const UTsize vtxCount, 
	const float*     weights, const UTsize weightsStride, 
	const UTuint8*   indices, const UTsize indicesStride, 
	const akVector3* vtxSrc,  const UTsize vtxSrcStride, 
	akVector3*       vtxDst,  const UTsize vtxDstStride, 
	const akVector3* normSrc, const UTsize normSrcStride, 
	akVector3*       normDst, const UTsize normDstStride)
{
	const btAlignedObjectArray<akMatrix4>& matrices = *mpalette;
	const btAlignedObjectArray<akDualQuat>& dquats = *dqpalette;
	
	for(unsigned int i=0; i<vtxCount; i++)
	{
		// positions 1st pass for non rigid part of the transformation using matrices
		akMatrix4 mat(matrices[indices[0]] * weights[0]);
		if (weights[1]) mat += matrices[indices[1]] * weights[1];
		if (weights[2]) mat += matrices[indices[2]] * weights[2];
		if (weights[3]) mat += matrices[indices[3]] * weights[3];
		
		akVector3 tmpPos = (mat * akVector4(*vtxSrc, 1.f)).getXYZ();
		
		akDualQuat dq0 = dquats[indices[0]];
		akDualQuat dq1 = dquats[indices[1]];
		akDualQuat dq2 = dquats[indices[2]];
		akDualQuat dq3 = dquats[indices[3]];
		
		if( dot(dq0.n, dq1.n) < 0.0) dq1 *= -1.0;
		if( dot(dq0.n, dq2.n) < 0.0) dq2 *= -1.0;
		if( dot(dq0.n, dq3.n) < 0.0) dq3 *= -1.0;
		
		akDualQuat dq = dq0 * weights[0];
		if (weights[1]) dq += dq1 * weights[1];
		if (weights[2]) dq += dq2 * weights[2];
		if (weights[3]) dq += dq3 * weights[3];
		
		dq /= length(dq.n);
	
		akVector3 ndxyz(dq.n.getXYZ());
		akVector3 dxyz(dq.d.getXYZ());
		
		//position 2nd pass
		*vtxDst = tmpPos + 2.0 * cross( ndxyz, cross(ndxyz, tmpPos) + dq.n.getW() * tmpPos ) +
				2.0 * ( dq.n.getW() * dxyz - dq.d.getW() * ndxyz + cross(ndxyz, dxyz) );
	
		// normal 1rst pass
		inverseTranspose(mat);
		akVector3 tmpNor = (mat * akVector4(*normSrc, 1.f)).getXYZ();
		
		
		// normal 2nd pass
		*normDst = tmpNor + 2.0 * cross( ndxyz, cross(ndxyz, tmpNor) + dq.n.getW() * tmpNor );
		*normDst = normalize(*normDst);
		
		akAdvancePointer(normSrc, normSrcStride);
		akAdvancePointer(normDst, normDstStride);
		akAdvancePointer(weights, weightsStride);
		akAdvancePointer(indices, indicesStride);
		akAdvancePointer(vtxSrc, vtxSrcStride);
		akAdvancePointer(vtxDst, vtxDstStride);
	}
}
void akGeometryDeformer::DLBAntipodalitySkinningUniformScale(
	const btAlignedObjectArray<akMatrix4>* mpalette, 
	const btAlignedObjectArray<akDualQuat>* dqpalette, 
	const UTsize vtxCount, 
	const float*     weights, const UTsize weightsStride, 
	const UTuint8*   indices, const UTsize indicesStride, 
	const akVector3* vtxSrc,  const UTsize vtxSrcStride, 
	akVector3*       vtxDst,  const UTsize vtxDstStride, 
	const akVector3* normSrc, const UTsize normSrcStride, 
	akVector3*       normDst, const UTsize normDstStride)
{
	const btAlignedObjectArray<akMatrix4>& matrices = *mpalette;
	const btAlignedObjectArray<akDualQuat>& dquats = *dqpalette;
	
	for(unsigned int i=0; i<vtxCount; i++)
	{
		// positions 1st pass for non rigid part of the transformation using matrices
		akMatrix4 mat(matrices[indices[0]] * weights[0]);
		if (weights[1]) mat += matrices[indices[1]] * weights[1];
		if (weights[2]) mat += matrices[indices[2]] * weights[2];
		if (weights[3]) mat += matrices[indices[3]] * weights[3];
		
		akVector3 tmpPos = (mat * akVector4(*vtxSrc, 1.f)).getXYZ();
		
		akDualQuat dq0 = dquats[indices[0]];
		akDualQuat dq1 = dquats[indices[1]];
		akDualQuat dq2 = dquats[indices[2]];
		akDualQuat dq3 = dquats[indices[3]];
		
		if( dot(dq0.n, dq1.n) < 0.0) dq1 *= -1.0;
		if( dot(dq0.n, dq2.n) < 0.0) dq2 *= -1.0;
		if( dot(dq0.n, dq3.n) < 0.0) dq3 *= -1.0;
		
		akDualQuat dq = dq0 * weights[0];
		if (weights[1]) dq += dq1 * weights[1];
		if (weights[2]) dq += dq2 * weights[2];
		if (weights[3]) dq += dq3 * weights[3];
		
		dq /= length(dq.n);
	
		akVector3 ndxyz(dq.n.getXYZ());
		akVector3 dxyz(dq.d.getXYZ());
		
		//position 2nd pass
		*vtxDst = tmpPos + 2.0 * cross( ndxyz, cross(ndxyz, tmpPos) + dq.n.getW() * tmpPos ) +
				2.0 * ( dq.n.getW() * dxyz - dq.d.getW() * ndxyz + cross(ndxyz, dxyz) );
	
		// normal 1rst pass
		akVector3 tmpNor = (mat * akVector4(*normSrc, 1.f)).getXYZ();
		
		
		// normal 2nd pass
		*normDst = tmpNor + 2.0 * cross( ndxyz, cross(ndxyz, tmpNor) + dq.n.getW() * tmpNor );
		*normDst = normalize(*normDst);
		
		akAdvancePointer(normSrc, normSrcStride);
		akAdvancePointer(normDst, normDstStride);
		akAdvancePointer(weights, weightsStride);
		akAdvancePointer(indices, indicesStride);
		akAdvancePointer(vtxSrc, vtxSrcStride);
		akAdvancePointer(vtxDst, vtxDstStride);
	}
}

void akGeometryDeformer::DLBAntipodalitySkinningNoScaling(
	const btAlignedObjectArray<akMatrix4>* mpalette, 
	const btAlignedObjectArray<akDualQuat>* dqpalette, 
	const UTsize vtxCount, 
	const float*     weights, const UTsize weightsStride, 
	const UTuint8*   indices, const UTsize indicesStride, 
	const akVector3* vtxSrc,  const UTsize vtxSrcStride, 
	akVector3*       vtxDst,  const UTsize vtxDstStride, 
	const akVector3* normSrc, const UTsize normSrcStride, 
	akVector3*       normDst, const UTsize normDstStride)
{
	const btAlignedObjectArray<akMatrix4>& matrices = *mpalette;
	const btAlignedObjectArray<akDualQuat>& dquats = *dqpalette;
	
	for(unsigned int i=0; i<vtxCount; i++)
	{
		akDualQuat dq0 = dquats[indices[0]];
		akDualQuat dq1 = dquats[indices[1]];
		akDualQuat dq2 = dquats[indices[2]];
		akDualQuat dq3 = dquats[indices[3]];
		
		if( dot(dq0.n, dq1.n) < 0.0) dq1 *= -1.0;
		if( dot(dq0.n, dq2.n) < 0.0) dq2 *= -1.0;
		if( dot(dq0.n, dq3.n) < 0.0) dq3 *= -1.0;
		
		akDualQuat dq = dq0 * weights[0];
		if (weights[1]) dq += dq1 * weights[1];
		if (weights[2]) dq += dq2 * weights[2];
		if (weights[3]) dq += dq3 * weights[3];
		
		dq /= length(dq.n);
	
		akVector3 ndxyz(dq.n.getXYZ());
		akVector3 dxyz(dq.d.getXYZ());
		
		//position
		akVector3 in(*vtxSrc);
		*vtxDst = in + 2.0 * cross( ndxyz, cross(ndxyz, in) + dq.n.getW() * in ) +
				2.0 * ( dq.n.getW() * dxyz - dq.d.getW() * ndxyz + cross(ndxyz, dxyz) );
	
		// normal
		akVector3 inn(*normSrc);
		*normDst = inn + 2.0 * cross( ndxyz, cross(ndxyz, inn) + dq.n.getW() * inn );
		
		akAdvancePointer(normSrc, normSrcStride);
		akAdvancePointer(normDst, normDstStride);
		akAdvancePointer(weights, weightsStride);
		akAdvancePointer(indices, indicesStride);
		akAdvancePointer(vtxSrc, vtxSrcStride);
		akAdvancePointer(vtxDst, vtxDstStride);
	}
}

void akGeometryDeformer::DLBAntipodalitySkinningNoNormals(
	const btAlignedObjectArray<akMatrix4>* mpalette, 
	const btAlignedObjectArray<akDualQuat>* dqpalette, 
	const UTsize vtxCount, 
	const float*     weights, const UTsize weightsStride, 
	const UTuint8*   indices, const UTsize indicesStride, 
	const akVector3* vtxSrc,  const UTsize vtxSrcStride, 
	akVector3*       vtxDst,  const UTsize vtxDstStride)
{
	const btAlignedObjectArray<akMatrix4>& matrices = *mpalette;
	const btAlignedObjectArray<akDualQuat>& dquats = *dqpalette;
	
	for(unsigned int i=0; i<vtxCount; i++)
	{
	
		// position 1st pass for non rigid part of the transformation using matrices
		akVector4 pos(vtxSrc[0].getX(), vtxSrc[0].getY(), vtxSrc[0].getZ(), 1);
		akVector4 posout(0,0,0,1);
		if (weights[0]) posout += matrices[indices[0]] * weights[0] * pos;
		if (weights[1]) posout += matrices[indices[1]] * weights[1] * pos;
		if (weights[2]) posout += matrices[indices[2]] * weights[2] * pos;
		if (weights[3]) posout += matrices[indices[3]] * weights[3] * pos;
		
		akDualQuat dq0 = dquats[indices[0]];
		akDualQuat dq1 = dquats[indices[1]];
		akDualQuat dq2 = dquats[indices[2]];
		akDualQuat dq3 = dquats[indices[3]];
		
		if( dot(dq0.n, dq1.n) < 0.0) dq1 *= -1.0;
		if( dot(dq0.n, dq2.n) < 0.0) dq2 *= -1.0;
		if( dot(dq0.n, dq3.n) < 0.0) dq3 *= -1.0;
		
		akDualQuat dq = dq0 * weights[0];
		if (weights[1]) dq += dq1 * weights[1];
		if (weights[2]) dq += dq2 * weights[2];
		if (weights[3]) dq += dq3 * weights[3];
		
		dq /= length(dq.n);
	
		akVector3 ndxyz(dq.n.getXYZ());
		akVector3 dxyz(dq.d.getXYZ());
		
		//position
		akVector3 in(posout.getXYZ());
		*vtxDst = in + 2.0 * cross( ndxyz, cross(ndxyz, in) + dq.n.getW() * in ) +
				2.0 * ( dq.n.getW() * dxyz - dq.d.getW() * ndxyz + cross(ndxyz, dxyz) );
	
		akAdvancePointer(weights, weightsStride);
		akAdvancePointer(indices, indicesStride);
		akAdvancePointer(vtxSrc, vtxSrcStride);
		akAdvancePointer(vtxDst, vtxDstStride);
	}
}

