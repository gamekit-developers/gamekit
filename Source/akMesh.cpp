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

#include "akMesh.h"
#include "akMathUtils.h"
#include "akSkeleton.h"
#include "akVertexGroup.h"
#include "akMorphTarget.h"
#include "akPose.h"
#include "akAnimationChannel.h"
#include "akAnimationCurve.h"
#include "akGeometryDeformer.h"

#include "btAlignedAllocator.h"

akSubMesh::akSubMesh(Type type, bool hasNormals, bool hasColors, UTuint32 uvlayers)
	 : m_type(type), m_hasNormals(hasNormals), m_hasVertexColor(hasColors), m_uvLayerCount(uvlayers),
	   m_hasSecondPos(false), m_hasSkinningData(false), m_vBufDirty(true), m_iBufDirty(true)
{
	m_posnorStride = sizeof(akVector3);
	if(m_hasNormals)
	{
		m_posnorStride += sizeof(akVector3);
		m_norOffset = sizeof(akVector3);
	}
	
	m_staticStride = uvlayers * 2 * sizeof(float);
	if(hasColors)
	{
		m_staticStride += sizeof(UTuint32);
		m_colorOffset = uvlayers * 2 * sizeof(float);
	}
	
	m_elementStride = sizeof(UTuint32);
	
	m_material = new akMaterial();
}

akSubMesh::~akSubMesh()
{
	m_posnor.clear();
	m_posnor2.clear();
	m_static.clear();
	m_boneIndices.clear();
	m_boneWeights.clear();
	m_elements.clear();
		
	for(int i=0; i<m_vertexGroups.size(); i++)
	{
		delete m_vertexGroups[i];
	}
	m_vertexGroups.clear();
	
	for(int i=0; i<m_morphTargets.size(); i++)
	{
		delete m_morphTargets[i];
	}
	m_morphTargets.clear();
	
	delete m_material;
}

UTuint32 akSubMesh::addVertex(const akVector3 &co, const akVector3 &no, const UTuint32 &color, utArray<float>& uv)
{
	m_posnor.push_back(co);
	if(m_hasNormals)
		m_posnor.push_back(no);
	
	VBufBrick vbb;
	for(int i=0; i<m_uvLayerCount; i++)
	{
		if( (i+1)*2 <= uv.size() )
		{
			vbb.f = uv[i*2];
			m_static.push_back(vbb);
			vbb.f = uv[i*2+1];
			m_static.push_back(vbb);
		}
	}
	
	if(m_hasVertexColor)
	{
		vbb.ui32 = color;
		m_static.push_back(vbb);
	}
	
	// add whatever data to keep size in sync
	if(m_hasSecondPos)
	{
		m_posnor2.push_back(akVector3(0,0,0));
		if(m_hasNormals)
			m_posnor2.push_back(akVector3(1,0,0));
	}
	if(m_hasSkinningData)
	{
		for(int i=0; i<4; i++)
		{
			m_boneIndices.push_back(0);
			m_boneWeights.push_back(0.f);
		}
	}
	UTuint32 size = m_vertexBuffer.getSize();
	m_vertexBuffer.setSize(size+1);
	m_vBufDirty = true;
	return size;
}

void akSubMesh::addIndex(UTuint32 idx)
{
	m_elements.push_back(idx);
	m_elementBuffer.setSize(m_elementBuffer.getSize()+1);
	m_iBufDirty = true;
}

void akSubMesh::addSkinningDataBuffer(void)
{
	if(!m_hasSkinningData)
	{
		m_boneIndices.resize(4 * m_vertexBuffer.getSize());
		m_boneWeights.resize(4 * m_vertexBuffer.getSize());
		
		m_hasSkinningData = true;
		m_vBufDirty = true;
	}
}

void akSubMesh::addSecondPositionBuffer(void)
{
	if(!m_hasSecondPos)
	{
		m_posnor2.resize(m_vertexBuffer.getSize()*2);
		m_hasSecondPos = true;
		m_vBufDirty = true;
	}
}

void akSubMesh::generateBoneWeightsFromVertexGroups(akSkeleton* skel, bool deleteVGroups)
{
	addSkinningDataBuffer();
	
	utHashSet<akVertexGroup*> todelete;
	utArray<UTint32> bonevgmap;
	utArray<utArray<akScalar> > weightmaplist;
	
	bonevgmap.resize(skel->getNumJoints(), -1);
	weightmaplist.resize(skel->getNumJoints());
	
	for (int w = 0; w < getNumVertexGroups(); w++)
	{
		akVertexGroup* vg = getVertexGroup(w);
		int bi = skel->getIndex(vg->getName());
		if (bi != -1)
		{
			bonevgmap[bi]=w;
			utArray<akScalar> & weightmap = weightmaplist.at(bi);
			weightmap.resize(m_vertexBuffer.getSize(), AK_INFINITY);
			
			akVertexGroup* vg = getVertexGroup(w);
			for(int vi=0; vi<vg->getSize(); vi++)
			{
				weightmap[vg->getIndex(vi)] = vg->getWeight(vi);
			}
			
			if(deleteVGroups)
				todelete.insert(vg);
		}
	}
	
	
	UTuint32 vertnum = m_vertexBuffer.getSize();
	for(int j=0; j<vertnum; j++)
	{
		int bcount = 0;
		utArray<float> tmpweights;
		utArray<int> tmpbi;
		float wsum = 0;
		
		for (int bi = 0; bi< bonevgmap.size(); bi++)
		{
			int vgi = bonevgmap[bi];
			if (vgi != -1)
			{
				utArray<akScalar>& weightmap = weightmaplist[bi];
				akScalar weight = weightmap[j];
				if(weight != AK_INFINITY)
				{
					wsum += weight;
					tmpweights.push_back(weight);
					tmpbi.push_back(bi);
					bcount++;
				}
			}
		}
		
		while(bcount > 4)
		{
			float minw = 1;
			int mini;
			for(int b=0; b<bcount; b++)
			{
				if(tmpweights[b]<minw)
				{
					minw = tmpweights[b];
					mini = b;
				}
			}
			tmpweights.erase((UTsize)mini);
			tmpbi.erase((UTsize)mini);
			wsum -= minw;
			bcount--;
		}
		
		
		if( !akFuzzy(wsum, 1.0f))
		{
			for(int b=0; b<bcount; b++)
			{
				tmpweights[b] = tmpweights[b] / wsum;
			}
		}
		
		for(int b=0; b<4; b++)
		{
			if(b<bcount)
			{
				m_boneIndices[j*4+b] = tmpbi[b];
				m_boneWeights[j*4+b] = tmpweights[b];
			}
			else
			{
				m_boneIndices[j*4+b] = 0;
				m_boneWeights[j*4+b] = 0;
			}
		}
	}
	
	for(int i=0; i<todelete.size(); i++)
	{
		removeVertexGroup(todelete[i]);
		delete todelete[i];
	}
	todelete.clear();
	
	bonevgmap.clear();
	
	for(int i=0; i<weightmaplist.size(); i++)
	{
		weightmaplist[i].clear();
	}
	weightmaplist.clear();
}

const akBufferInfo* akSubMesh::getVertexBuffer()
{
	if(m_vBufDirty)
	{
		m_vertexBuffer.clear();
		
		m_vertexBuffer.addElement(akBufferInfo::BI_DU_VERTEX, akBufferInfo::VB_DT_3FLOAT32, m_posnorStride, &m_posnor[0], &m_posnor[0]);
		
		if(m_hasNormals)
		{
			void* nordata = (char*)(&m_posnor[0]) + m_norOffset;
			m_vertexBuffer.addElement(akBufferInfo::BI_DU_NORMAL, akBufferInfo::VB_DT_3FLOAT32, m_posnorStride, nordata, &m_posnor[0]);
		}

		if(m_hasSecondPos)
		{
			m_vertexBuffer.addElement(akBufferInfo::BI_DU_VERTEX, akBufferInfo::VB_DT_3FLOAT32, m_posnorStride, &m_posnor2[0], &m_posnor2[0]);
			if(m_hasNormals)
			{
				void* nordata = (char*)(&m_posnor2[0]) + m_norOffset;
				m_vertexBuffer.addElement(akBufferInfo::BI_DU_NORMAL, akBufferInfo::VB_DT_3FLOAT32, m_posnorStride, nordata, &m_posnor2[0]);
			}
		}
		
		for(int i=0; i<m_uvLayerCount; i++)
		{
			void* uvdata = (char*)(&m_static[0]) + i * 2 * sizeof(float);
			m_vertexBuffer.addElement(akBufferInfo::BI_DU_UV, akBufferInfo::VB_DT_2FLOAT32, m_staticStride, uvdata, &m_static[0]);
		}
		
		if(m_hasVertexColor)
		{
			void* vcoldata = (char*)(&m_static[0]) + m_colorOffset;
			m_vertexBuffer.addElement(akBufferInfo::BI_DU_COLOR, akBufferInfo::VB_DT_4UINT8, m_staticStride, vcoldata, &m_static[0]);
		}
		
		if(m_hasSkinningData)
		{
			m_vertexBuffer.addElement(akBufferInfo::BI_DU_BONE_IDX, akBufferInfo::VB_DT_4UINT8, 4*sizeof(UTuint8), &m_boneIndices[0], &m_boneIndices[0]);
			m_vertexBuffer.addElement(akBufferInfo::BI_DU_BONE_WEIGHT, akBufferInfo::VB_DT_4FLOAT32, 4*sizeof(float), &m_boneWeights[0], &m_boneIndices[0]);
		}
		
		m_vBufDirty = false;
	}
	return &m_vertexBuffer;
}

const akBufferInfo* akSubMesh::getIndexBuffer()
{
	if(m_iBufDirty)
	{
		m_elementBuffer.clear();
		m_elementBuffer.addElement(akBufferInfo::BI_DU_ELEMENT, akBufferInfo::VB_DT_UINT32, sizeof(UTuint32), &m_elements[0], &m_elements[0]);
		m_iBufDirty = false;
	}
	return &m_elementBuffer;
}


UTuint32 akSubMesh::getTriangleCount(void)
{
	switch(m_type)
	{
	case ME_TRIANGLE_STRIP:
	case ME_TRIANGLE_FAN:
		return m_elementBuffer.getSize() - 2;
		break;
	case ME_TRIANGLES:
	default:
		return m_elementBuffer.getSize() / 3;
		break;
	}
}

bool akSubMesh::hasMorphTargets(void)
{
	return m_morphTargets.size()>0;
}

void akSubMesh::deform(akGeometryDeformer::SkinningOption method, 
					   akGeometryDeformer::NormalsOption normalMethod, 
						const akPose* pose, const btAlignedObjectArray<akMatrix4> * mpalette,
						const btAlignedObjectArray<akDualQuat> * dqpalette)
{
	akVector3 *posin, *posout;
	unsigned int posins, posouts;
	akVector3 *norin, *norout;
	unsigned int norins, norouts;
	
	//be sure vbuf is up to date
	getVertexBuffer();
	
	m_vertexBuffer.getElement(akBufferInfo::BI_DU_VERTEX, akBufferInfo::VB_DT_3FLOAT32, 1, (void**)&posin, &posins);
	m_vertexBuffer.getElement(akBufferInfo::BI_DU_VERTEX, akBufferInfo::VB_DT_3FLOAT32, 2, (void**)&posout, &posouts);
	
	if(m_hasNormals)
	{
		m_vertexBuffer.getElement(akBufferInfo::BI_DU_NORMAL, akBufferInfo::VB_DT_3FLOAT32, 1, (void**)&norin, &norins);
		m_vertexBuffer.getElement(akBufferInfo::BI_DU_NORMAL, akBufferInfo::VB_DT_3FLOAT32, 2, (void**)&norout, &norouts);
	}
	
	// Morphing
	if(pose)
	{
		akVector3* pi = posin;
		akVector3* po = posout;
		akVector3* ni = norin;
		akVector3* no = norout;
		for(int j=0; j<m_vertexBuffer.getSize(); j++)
		{
			*po = *pi;
			akAdvancePointer(pi, posins);
			akAdvancePointer(po, posouts);
			if(m_hasNormals)
			{
				*no = *ni;
				akAdvancePointer(ni, norins);
				akAdvancePointer(no, norouts);
			}
		}
		posin = posout;
		posins = posouts;
		norin = norout;
		norins = norouts;
		
		for(int j=0; j<m_morphTargets.size();j++)
		{
			akMorphTarget* target = m_morphTargets[j];
			const akPose::FloatResult* result = pose->getFloatResult(akAnimationChannel::AC_MORPH, 
																 target->getName().hash(), 
																 akAnimationCurve::AC_CODE_VALUE);
			if(result)
				akGeometryDeformer::Morphing(target, result->value, posout, posouts, norout, norouts);
		}
	}
	
	// Skinning
	if(mpalette)
	{
		UTuint8 *indices;
		float *weights;
		unsigned int indicess, weightss;
		
		m_vertexBuffer.getElement(akBufferInfo::BI_DU_BONE_IDX, akBufferInfo::VB_DT_4UINT8, 1, (void**)&indices, &indicess);
		m_vertexBuffer.getElement(akBufferInfo::BI_DU_BONE_WEIGHT, akBufferInfo::VB_DT_4FLOAT32, 1, (void**)&weights, &weightss);
		
		akGeometryDeformer::Skinning(method,
									 normalMethod,
									 mpalette, dqpalette,
									 m_vertexBuffer.getSize(),
									 weights, weightss,
									 indices, indicess,
									 posin, posins,
									 posout, posouts,
									 norin, norins,
									 norout, norouts);
		
	}
}

akMesh::akMesh()
{

}

akMesh::~akMesh()
{
	int i;
	for(i=0; i<m_submeshes.size(); i++)
	{
		delete m_submeshes[i];
	}
	m_submeshes.clear();
}

void akMesh::addSubMesh(akSubMesh* smesh)
{
	m_submeshes.push_back(smesh);
}

void akMesh::addSkinningDataBuffer(void)
{
	for(int i=0; i<m_submeshes.size(); i++)
	{
		m_submeshes[i]->addSkinningDataBuffer();
	}
}

void akMesh::addSecondPositionBuffer(void)
{
	for(int i=0; i<m_submeshes.size(); i++)
	{
		m_submeshes[i]->addSecondPositionBuffer();
	}
}

void akMesh::generateBoneWeightsFromVertexGroups(akSkeleton* skel, bool deleteVGroups)
{
	for(int i=0; i<m_submeshes.size(); i++)
	{
		m_submeshes[i]->generateBoneWeightsFromVertexGroups(skel, deleteVGroups);
	}
}

bool akMesh::hasMorphTargets(void)
{
	for(int i=0; i<m_submeshes.size(); i++)
	{
		if(m_submeshes[i]->hasMorphTargets())
			return true;
	}
	return false;
}

void akMesh::deform(akGeometryDeformer::SkinningOption method, 
					akGeometryDeformer::NormalsOption normalMethod, 
					const akPose* pose, const btAlignedObjectArray<akMatrix4>* palette, 
					const btAlignedObjectArray<akDualQuat>* dqpalette)
{
	for (int i=0; i<m_submeshes.size(); i++)
	{
		if(hasMorphTargets())
			m_submeshes[i]->deform(method, normalMethod, pose, palette, dqpalette);
		else
			m_submeshes[i]->deform(method, normalMethod, 0, palette, dqpalette);
	}
}

UTuint32 akMesh::getVertexCount(void)
{
	UTuint32 c = 0;
	for (int i=0; i<m_submeshes.size(); i++)
	{
		c += m_submeshes[i]->getVertexCount();
	}
	return c;
}

UTuint32 akMesh::getTriangleCount(void)
{
	UTuint32 c = 0;
	for (int i=0; i<m_submeshes.size(); i++)
	{
		c += m_submeshes[i]->getTriangleCount();
	}
	return c;
}

