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
	
	enum SkinningOption
	{
		GD_SO_MATRIX,
		GD_SO_DUAL_QUAT,
		GD_SO_DUAL_QUAT_ANTIPOD,
	};
	
	enum NormalsOption
	{
		GD_NO_NONE,
		GD_NO_NOSCALE,
		GD_NO_UNIFORM_SCALE,
		GD_NO_FULL,
	};
	
	/// Apply a moprh target to a vertex buffer.
	static void Morphing(const akMorphTarget* morph, akScalar weight,
						 akVector3* vtxDst,  const UTsize vtxDstStride,
						 akVector3* normDst, const UTsize normDstStride);
	
	/// Apply vertex skinning to a vertex buffer
	/// vtxcount         Number of vertex to deform
	/// matrices         Matrix palette ( minimum length == number of bone in the skeleton)
	/// weights          Weights (minimum length == number of vertex * 4
	/// indices          Bones indices (minimum length == number of vertex * 4
	/// vtxSrc           Vertex position input
	/// vtxDts           Vertex position output
	/// normSrc          Vertex normals input (optional)
	/// normDst          Vertex normals output (optional)
	static void Skinning(akGeometryDeformer::SkinningOption method,
						 akGeometryDeformer::NormalsOption  normalOpt,
						 const btAlignedObjectArray<akMatrix4>* matrices,
						 const btAlignedObjectArray<akDualQuat>* dquats,
						 const UTsize vtxCount,
						 const float* weights,     UTsize weightsStride,
						 const UTuint8* indices,   UTsize indicesStride,
						 const akVector3* vtxSrc,  UTsize vtxSrcStride,
						 akVector3* vtxDst,        UTsize vtxDstStride,
						 const akVector3* normSrc, UTsize normSrcStride,
						 akVector3* normDst,       UTsize normDstStride);
	
	/// Apply vertex skinning to a vertex buffer using linear interpolation of matrices.
	/// vtxcount         Number of vertex to deform
	/// matrices         Matrix palette ( minimum length == number of bone in the skeleton)
	/// weights          Weights (minimum length == number of vertex * 4
	/// indices          Bones indices (minimum length == number of vertex * 4
	/// vtxSrc           Vertex position input
	/// vtxDts           Vertex position output
	/// normSrc          Vertex normals input (optional)
	/// normDst          Vertex normals output (optional)
	static void LBSkinning(const btAlignedObjectArray<akMatrix4>* matrices,
						   const UTsize vtxCount,
						   const float* weights,     const UTsize weightsStride,
						   const UTuint8* indices,   const UTsize indicesStride,
						   const akVector3* vtxSrc,  const UTsize vtxSrcStride,
						   akVector3* vtxDst,        const UTsize vtxDstStride,
						   const akVector3* normSrc, const UTsize normSrcStride,
						   akVector3* normDst,       const UTsize normDstStride);
	
	
	/// Apply vertex skinning to a vertex buffer using linear interpolation of matrices.
	/// Does not support non uniform scaling.
	static void LBSkinningUniformScale(const btAlignedObjectArray<akMatrix4>* matrices,
							   const UTsize vtxCount,
							   const float* weights,     const UTsize weightsStride,
							   const UTuint8* indices,   const UTsize indicesStride,
							   const akVector3* vtxSrc,  const UTsize vtxSrcStride,
							   akVector3* vtxDst,        const UTsize vtxDstStride,
							   const akVector3* normSrc, const UTsize normSrcStride,
							   akVector3* normDst,       const UTsize normDstStride);
	
	/// Apply vertex skinning to a vertex buffer using linear interpolation of matrices.
	static void LBSkinningNoNormals(const btAlignedObjectArray<akMatrix4>* matrices, 
							 const UTsize vtxCount, 
							 const float *weights, const UTsize weightsStride, 
							 const UTuint8 *indices, const UTsize indicesStride, 
							 const akVector3 *vtxSrc, const UTsize vtxSrcStride, 
							 akVector3 *vtxDst, const UTsize vtxDstStride);
	
	/// Apply vertex skinning to a vertex buffer using linear interpolation of dual quaternion.
	/// This avoid many of the volume shrinking artifacts.
	static void DLBSkinning(const btAlignedObjectArray<akMatrix4>* matrices,
							const btAlignedObjectArray<akDualQuat>* dquats,
							const UTsize vtxCount,
							const float* weights,     const UTsize weightsStride,
							const UTuint8* indices,   const UTsize indicesStride,
							const akVector3* vtxSrc,  const UTsize vtxSrcStride,
							akVector3* vtxDst,        const UTsize vtxDstStride,
							const akVector3* normSrc, const UTsize normSrcStride,
							akVector3* normDst,       const UTsize normDstStride);
	
	
	/// Apply vertex skinning to a vertex buffer using linear interpolation of dual quaternion.
	/// This avoid many of the volume shrinking artifacts.
	/// Does not support non uniform scaling.
	static void DLBSkinningUniformScale(const btAlignedObjectArray<akMatrix4>* matrices,
								const btAlignedObjectArray<akDualQuat>* dquats, 
								const UTsize vtxCount,
								const float* weights,         const UTsize weightsStride,
								const unsigned char* indices, const UTsize indicesStride,
								const akVector3* vtxSrc,      const UTsize vtxSrcStride,
								akVector3* vtxDst,            const UTsize vtxDstStride,
								const akVector3* normSrc,     const UTsize normSrcStride,
								akVector3* normDst,           const UTsize normDstStride);
	
	/// Apply vertex skinning to a vertex buffer using linear interpolation of dual quaternion.
	/// This avoid many of the volume shrinking artifacts.
	/// Support rigid transformation only (rotation and translation but no scaling/shear).
	static void DLBSkinningNoScaling(const btAlignedObjectArray<akMatrix4>* matrices,
									const btAlignedObjectArray<akDualQuat>* dquats, 
									const UTsize vtxCount,
									const float* weights,         const UTsize weightsStride,
									const unsigned char* indices, const UTsize indicesStride,
									const akVector3* vtxSrc,      const UTsize vtxSrcStride,
									akVector3* vtxDst,            const UTsize vtxDstStride,
									const akVector3* normSrc,     const UTsize normSrcStride,
									akVector3* normDst,           const UTsize normDstStride);
	
	static void DLBSkinningNoNormals(const btAlignedObjectArray<akMatrix4> *mpalette, 
									 const btAlignedObjectArray<akDualQuat> *dqpalette, 
									 const UTsize vtxCount, 
									 const float *weights, const UTsize weightsStride, 
									 const UTuint8 *indices, const UTsize indicesStride, 
									 const akVector3 *vtxSrc, const UTsize vtxSrcStride, 
									 akVector3 *vtxDst, const UTsize vtxDstStride);
		
	/// Apply vertex skinning to a vertex buffer using linear interpolation of dual quaternion.
	/// This avoid many of the volume shrinking artifacts.
	/// This function solves antipodality per vertex, it is more robust (specially in the cases 
	/// where bones rotates more than 180º relatively to its parent) but much less efficient
	/// and not advised for realtime.
	/// Support rigid transformation only (rotation and translation but no scaling/shear).
	static void DLBAntipodalitySkinningNoScaling(const btAlignedObjectArray<akMatrix4>* matrices, 
										const btAlignedObjectArray<akDualQuat>* dquats,
										const UTsize vtxCount, 
										const float *weights,     const UTsize weightsStride, 
										const UTuint8 *indices,   const UTsize indicesStride, 
										const akVector3 *vtxSrc,  const UTsize vtxSrcStride, 
										akVector3 *vtxDst,        const UTsize vtxDstStride, 
										const akVector3 *normSrc, const UTsize normSrcStride, 
												 akVector3 *normDst,       const UTsize normDstStride);
	
	static void DLBAntipodalitySkinningNoNormals(const btAlignedObjectArray<akMatrix4> *mpalette, 
										  const btAlignedObjectArray<akDualQuat> *dqpalette, 
										  const UTsize vtxCount, 
										  const float *weights, const UTsize weightsStride, 
										  const UTuint8 *indices, const UTsize indicesStride, 
										  const akVector3 *vtxSrc, const UTsize vtxSrcStride, 
										  akVector3 *vtxDst, const UTsize vtxDstStride);
										  
	static void DLBAntipodalitySkinning(const btAlignedObjectArray<akMatrix4> *mpalette, 
										const btAlignedObjectArray<akDualQuat> *dqpalette, 
										const UTsize vtxCount, 
										const float *weights, const UTsize weightsStride, 
										const UTuint8 *indices, const UTsize indicesStride, 
										const akVector3 *vtxSrc, const UTsize vtxSrcStride, 
										akVector3 *vtxDst, const UTsize vtxDstStride, 
										const akVector3 *normSrc, const UTsize normSrcStride, 
										akVector3 *normDst, const UTsize normDstStride);
										
	static void DLBAntipodalitySkinningUniformScale(const btAlignedObjectArray<akMatrix4> *mpalette, 
											 const btAlignedObjectArray<akDualQuat> *dqpalette, 
											 const UTsize vtxCount, 
											 const float *weights, const UTsize weightsStride, 
											 const UTuint8 *indices, const UTsize indicesStride, 
											 const akVector3 *vtxSrc, const UTsize vtxSrcStride, 
											 akVector3 *vtxDst, const UTsize vtxDstStride, 
											 const akVector3 *normSrc, const UTsize normSrcStride, 
											 akVector3 *normDst, const UTsize normDstStride);
};

#endif // AKGEOMETRYDEFORMER_H
