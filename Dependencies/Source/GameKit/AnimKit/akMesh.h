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

#ifndef AKMESH_H
#define AKMESH_H

#include "utCommon.h"
#include "utString.h"

#include "akCommon.h"
#include "akMathUtils.h"
#include "akBufferInfo.h"
#include "akGeometryDeformer.h"

#include "btAlignedObjectArray.h"

/// Contains the BufferInfo (description of the vertex data) and owns the memory
class akSubMesh
{
public:
	enum Type
	{
		ME_TRIANGLES,
		ME_TRIANGLE_STRIP,
		ME_TRIANGLE_FAN
	};
	
	typedef union VBufBrick
	{
		UTuint32 ui32;
		UTuint8  ui8[4];
		float    f;
	}VBufBrick;

	typedef utArray<akVertexGroup*> VertexGroups;
	typedef utArray<akMorphTarget*> MorphTargets;
	
private:
	Type            m_type;
	bool            m_hasNormals;
	bool            m_hasSecondPos;
	bool            m_hasVertexColor;
	bool            m_hasSkinningData;
	bool            m_vBufDirty, m_iBufDirty;
	UTuint32        m_uvLayerCount;
	
	// vertex buffer
	akBufferInfo                    m_vertexBuffer;
	
	btAlignedObjectArray<akVector3> m_posnor;
	btAlignedObjectArray<akVector3> m_posnor2;
	UTuint32                        m_posnorStride;
	UTuint32                        m_norOffset;
	
	utArray<VBufBrick>              m_static;
	UTuint32                        m_staticStride;
	UTuint32                        m_colorOffset;
	
	utArray<UTuint8>                m_boneIndices;
	utArray<float>                  m_boneWeights;
	
	// index buffer
	akBufferInfo                    m_elementBuffer;
	utArray<UTuint32>               m_elements;
	UTuint32                        m_elementStride;
	
	VertexGroups                    m_vertexGroups;
	MorphTargets                    m_morphTargets;

public:
	
	akSubMesh(Type type, bool hasNormals, bool hasColors, UTuint32 uvlayers);
	~akSubMesh();
	
	void addVertex(const akVector3& co, const akVector3& no, const UTuint32& color, utArray<float>& uv);
	
	void addIndex(UTuint32 idx);
	
	/// Allocate necessary data to store the skinning info ( bone indices and weights)
	/// and add them to the vertexbuffer
	void addSkinningDataBuffer(void);
	
	/// Alocate a second postion buffer to store positions of the deformed mesh
	void addSecondPositionBuffer(void);
	
	/// Populate skinning data buffer acording to skeleton and vgroups.
	void generateBoneWeightsFromVertexGroups(akSkeleton* skel, bool deleteVGroups);
	
	bool hasMorphTargets(void);
		
	const akBufferInfo* getVertexBuffer(void);
	
	const akBufferInfo* getIndexBuffer(void);
	
	void deform(akGeometryDeformer::SkinningOption method, 
				akGeometryDeformer::NormalsOption normalMethod, 
				const akPose* pose, const btAlignedObjectArray<akMatrix4> * mpalette,
				const btAlignedObjectArray<akDualQuat> * dqpalette =0);
	
	UT_INLINE bool hasNormals(void)
	{
		return m_hasNormals;
	}
	
	UT_INLINE bool hasVertexColor(void)
	{
		return m_hasVertexColor;
	}

	UT_INLINE UTuint32 getVertexCount(void)
	{
		return m_vertexBuffer.getSize();
	}
	
	UT_INLINE UTuint32 getIndexCount(void)
	{
		return m_elementBuffer.getSize();
	}
	
	UT_INLINE UTuint32 getUVLayerCount(void)
	{
		return m_uvLayerCount;
	}
	
	UT_INLINE akVector3* getPosNoDataPtr(void)
	{
		return &m_posnor[0];
	}

	UT_INLINE akVector3* getSecondPosNoDataPtr(void)
	{
		return &m_posnor2[0];
	}

	UT_INLINE float* getStaticVertexDataPtr(void)
	{
		return (float*)&m_static[0];
	}

	UT_INLINE UTuint32* getIndexDataPtr(void)
	{
		return &m_elements[0];
	}

	UT_INLINE UTuint32 getPosNoDataStride(void)
	{
		return m_posnorStride;
	}
	
	UT_INLINE UTuint32 getStaticVertexDataStride(void)
	{
		return m_staticStride;
	}
	
	UT_INLINE UTuint32 getIndexDataStride(void)
	{
		return m_elementStride;
	}

	UT_INLINE void addVertexGroup(akVertexGroup* v)
	{
		m_vertexGroups.push_back(v);
	}
	
	UT_INLINE void removeVertexGroup(akVertexGroup* v)
	{
		m_vertexGroups.erase(v);
	}
	
	UT_INLINE int getNumVertexGroups(void) const
	{
		return m_vertexGroups.size();
	}
	
	UT_INLINE akVertexGroup* getVertexGroup(int i)
	{
		return m_vertexGroups[i];
	}
	
	UT_INLINE void addMorphTarget(akMorphTarget* target)
	{
		m_morphTargets.push_back(target);
	}
	
	UT_INLINE int getNumMorphTargets(void) const
	{
		return m_morphTargets.size();
	}
	
	UT_INLINE akMorphTarget* getMorphTarget(int i)
	{
		return m_morphTargets[i];
	}
};

/// A triangle mesh
/// Contains several submeshes (or section or surfaces)
class akMesh
{
private:
	utArray<akSubMesh*> m_submeshes;
	

public:
	akMesh();
	~akMesh();
	
	void addSubMesh(akSubMesh* mesh);
	
	/// Allocate necessary data to store the skinning info ( bone indices and weights)
	/// and add them to the vertexbuffer
	void addSkinningDataBuffer(void);
	
	/// Alocate a second postion buffer to store positions of the deformed mesh
	void addSecondPositionBuffer(void);

	/// Populate skinning data buffer acording to skeleton and vgroups.
	void generateBoneWeightsFromVertexGroups(akSkeleton* skel, bool deleteVGroups);
	
	bool hasMorphTargets(void);
	
	void deform(akGeometryDeformer::SkinningOption method, 
				akGeometryDeformer::NormalsOption normalMethod, 
				const akPose* pose, const btAlignedObjectArray<akMatrix4> * mpalette=0,
				const btAlignedObjectArray<akDualQuat> * dqpalette =0);
	
	UT_INLINE int getNumSubMeshes(void) const
	{
		return m_submeshes.size();
	}
	
	UT_INLINE akSubMesh* getSubMesh(int i)
	{
		return m_submeshes[i];
	}
	
};


#endif // AKMESH_H
