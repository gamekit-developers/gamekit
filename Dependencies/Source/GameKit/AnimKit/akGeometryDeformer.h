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

#ifndef AKGEOMETRYDEFORMER_H
#define AKGEOMETRYDEFORMER_H

#include "akCommon.h"
#include "utTypes.h"
#include "akMathUtils.h"
#include "btAlignedObjectArray.h"

/// Simple utility to deform mesh geometry
class akGeometryDeformer
{
public:

	/// Apply vertex skinning to a vertex buffer using linear interpolation of matrices.
	/// vtxcount         Number of vertex to deform
	/// matrices         Matrix palette ( minimum length == number of bone in the skeleton)
	/// weights          Weights (minimum length == number of vertex * 4
	/// indices          Bones indices (minimum length == number of vertex * 4
	/// vtxSrc           Vertex position input
	/// vtxDts           Vertex position output
	/// normSrc          Vertex normals input (optional)
	/// normDst          Vertex normals output (optional)
	static void LBSkinning(const UTsize vtxCount,
									const btAlignedObjectArray<akMatrix4> &matrices,
	                                const float* weights,        const UTsize weightsStride,
	                                const UTuint8* indices,      const UTsize indicesStride,
	                                const akVector3* vtxSrc,     const UTsize vtxSrcStride,
	                                akVector3* vtxDst,           const UTsize vtxDstStride,
	                                const akVector3* normSrc =0, const UTsize normSrcStride =0,
	                                akVector3* normDst =0,       const UTsize normDstStride =0);


	/// Apply vertex skinning to a vertex buffer using linear interpolation of matrices.
	/// Does not support non uniform scaling.
	static void LBSkinningFast(const UTsize vtxCount,
									const btAlignedObjectArray<akMatrix4> &matrices,
	                                const float* weights,        const UTsize weightsStride,
	                                const UTuint8* indices,      const UTsize indicesStride,
	                                const akVector3* vtxSrc,     const UTsize vtxSrcStride,
	                                akVector3* vtxDst,           const UTsize vtxDstStride,
	                                const akVector3* normSrc =0, const UTsize normSrcStride =0,
	                                akVector3* normDst =0,       const UTsize normDstStride =0);


	/// Apply vertex skinning to a vertex buffer using linear interpolation of dual quaternion.
	/// This avoid many of the volume shrinking artifacts.
	static void DLBSkinning(const UTsize vtxCount,
									const btAlignedObjectArray<akDualQuat>& dquats, 
									const btAlignedObjectArray<akMatrix4> &matrices,
	                                const float* weights,        const UTsize weightsStride,
	                                const UTuint8* indices,      const UTsize indicesStride,
	                                const akVector3* vtxSrc,     const UTsize vtxSrcStride,
	                                akVector3* vtxDst,           const UTsize vtxDstStride,
	                                const akVector3* normSrc =0, const UTsize normSrcStride =0,
	                                akVector3* normDst =0,       const UTsize normDstStride =0);


	/// Apply vertex skinning to a vertex buffer using linear interpolation of dual quaternion.
	/// This avoid many of the volume shrinking artifacts.
	/// Does not support non uniform scaling.
	static void DLBSkinningFast(const UTsize vtxCount,
									const btAlignedObjectArray<akDualQuat>& dquats, 
									const btAlignedObjectArray<akMatrix4> &matrices,
	                                const float* weights,       const UTsize weightsStride,
	                                const unsigned char* indices, const UTsize indicesStride,
	                                const akVector3* vtxSrc,        const UTsize vtxSrcStride,
	                                akVector3* vtxDst,              const UTsize vtxDstStride,
	                                const akVector3* normSrc =0,    const UTsize normSrcStride =0,
	                                akVector3* normDst =0,          const UTsize normDstStride =0);
	
	/// Apply vertex skinning to a vertex buffer using linear interpolation of dual quaternion.
	/// This avoid many of the volume shrinking artifacts.
	/// Support rigid transformation only (rotation and translation but no scaling/shear).
	static void DLBSkinningMoreFast(const UTsize vtxCount,
									const btAlignedObjectArray<akDualQuat>& dquats, 
									const btAlignedObjectArray<akMatrix4> &matrices,
									const float* weights,       const UTsize weightsStride,
									const unsigned char* indices, const UTsize indicesStride,
									const akVector3* vtxSrc,        const UTsize vtxSrcStride,
									akVector3* vtxDst,              const UTsize vtxDstStride,
									const akVector3* normSrc =0,    const UTsize normSrcStride =0,
									akVector3* normDst =0,          const UTsize normDstStride =0);

	/// Apply vertex skinning to a vertex buffer using linear interpolation of dual quaternion.
	/// This avoid many of the volume shrinking artifacts.
	/// This function solves antipodality per vertex, it is more robust (specially in the cases 
	/// where bones rotates more than 180º relatively to its parent) but less efficient.
	/// Support rigid transformation only (rotation and translation but no scaling/shear).
	static void DLBSkinningAntipodality(const UTsize vtxCount, 
									const btAlignedObjectArray<akDualQuat>& dquats, 
									const btAlignedObjectArray<akMatrix4> &matrices, 
									const float *weights, const UTsize weightsStride, 
									const UTuint8 *indices, const UTsize indicesStride, 
									const akVector3 *vtxSrc, const UTsize vtxSrcStride, 
									akVector3 *vtxDst, const UTsize vtxDstStride, 
									const akVector3 *normSrc =0, const UTsize normSrcStride =0, 
									akVector3 *normDst =0, const UTsize normDstStride =0);
};

#endif // AKGEOMETRYDEFORMER_H
