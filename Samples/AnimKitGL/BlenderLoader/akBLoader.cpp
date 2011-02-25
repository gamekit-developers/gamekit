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

#include <iostream>

#include "akBLoader.h"
#include "akBlenderDefines.h"
#include "akMesh.h"
#include "akSkeleton.h"
#include "akSkeletonPose.h"

#include "fbtBlend.h"
#include "Blender.h"

#include "../akEntity.h"
#include "../akDemo.h"

#include "akAnimationLoader.h"

akBLoader::akBLoader(akDemo* demo)
{
	m_demo = demo;
}

void akBLoader::convertTriangle(unsigned int firstIndex, Vector3* positions, int* oldIndices, Blender::MVert* bverts, int idx1, int idx2, int idx3)
{
	positions[firstIndex] = Vector3(bverts[idx1].co[0], bverts[idx1].co[1], bverts[idx1].co[2]);
	positions[firstIndex+1] = Vector3(bverts[idx2].co[0], bverts[idx2].co[1], bverts[idx2].co[2]);
	positions[firstIndex+2] = Vector3(bverts[idx3].co[0], bverts[idx3].co[1], bverts[idx3].co[2]);
	oldIndices[firstIndex] = idx1;
	oldIndices[firstIndex+1] = idx2;
	oldIndices[firstIndex+2] = idx3;
}

void akBLoader::removeMeshTempData(akMesh* mesh)
{
	for (int i=0; i<mesh->getNumSubMeshes(); i++)
	{
		akSubMesh* smesh = mesh->getSubMesh(i);
		akVertexBuffer* vbuf = smesh->getVertexBuffer();
		int* oldindices;
	
		if( vbuf->getElement(akVertexBuffer::VB_DU_CUSTOM, akVertexBuffer::VB_DT_INT32,1 ,(void**)&oldindices, 0) )
		{
			vbuf->removeElement(oldindices);
			
			delete[] oldindices;
		}
	}
}

void akBLoader::convertMesh(Blender::Mesh *me)
{
	unsigned int tricount = 0 ;
	Blender::MFace*  mface = me->mface;
	Blender::MVert*  mvert = me->mvert;
	
	if (!mface || !mvert)
		return;
	
	//count total triangle faces needed
	for (int i = 0; i < me->totface; i++)
	{
		const Blender::MFace& curface = mface[i];
		
		// skip if face is not a triangle || quad
		if (!curface.v3)
			continue;
		
		tricount++;
		
		const bool isQuad = curface.v4 != 0;
		
		if (isQuad)
			tricount++;
	}
	
	akSubMesh* smesh = new akSubMesh(tricount * 3);
	akMesh* mesh = new akMesh();
	mesh->addSubMesh(smesh);
	m_demo->addMesh(AKB_IDNAME(me), mesh);

	akVertexBuffer* vbuf = smesh->getVertexBuffer();
	unsigned int stride;
	Vector3* positions;
	
	vbuf->getElement(akVertexBuffer::VB_DU_POSITION, akVertexBuffer::VB_DT_3FLOAT32, 1, (void**)&positions, &stride);

	// add buffer to save original vertex index to convert deform groups later
	int* oldindices = new int[tricount*3];
	vbuf->addElement(akVertexBuffer::VB_DU_CUSTOM, akVertexBuffer::VB_DT_INT32, sizeof(int), oldindices);
	
	
	tricount = 0;
	for (int i = 0; i < me->totface; i++)
	{
		const Blender::MFace& curface = mface[i];

		// skip if face is not a triangle or quad
		if (!curface.v3)
			continue;

		const bool isQuad = curface.v4 != 0;


		if (isQuad)
		{

			Vector3 e0, e1;

			e0 = (Vector3(mvert[curface.v1].co[0], mvert[curface.v1].co[1], mvert[curface.v1].co[2]) - Vector3(mvert[curface.v2].co[0], mvert[curface.v2].co[1], mvert[curface.v2].co[2]));
			e1 = (Vector3(mvert[curface.v3].co[0], mvert[curface.v3].co[1], mvert[curface.v3].co[2]) - Vector3(mvert[curface.v4].co[0], mvert[curface.v4].co[1], mvert[curface.v4].co[2]));
			
			if (lengthSqr(e0) <lengthSqr(e1))
			{
				convertTriangle(tricount*3, positions, oldindices, mvert, curface.v1, curface.v2, curface.v3);
				tricount++;
				
				convertTriangle(tricount*3, positions, oldindices, mvert, curface.v3, curface.v4, curface.v1);
				tricount++;
			}
			else
			{
				convertTriangle(tricount*3, positions, oldindices, mvert, curface.v1, curface.v2, curface.v4);
				tricount++;
				
				convertTriangle(tricount*3, positions, oldindices, mvert, curface.v4, curface.v2, curface.v3);
				tricount++;
			}
		}
		else
		{
			convertTriangle(tricount*3, positions, oldindices, mvert, curface.v1, curface.v2, curface.v3);
			tricount++;
		}
	}
	
}

int countBones(Blender::Bone* bone)
{
	int bcount = 1;
	
	Blender::Bone* chi = static_cast<Blender::Bone*>(bone->childbase.first);
	while (chi)
	{
		// recurse
		bcount += countBones(chi);
		chi = chi->next;
	}
	return bcount;
}

int buildBoneTree(akSkeleton* skel, akSkeletonPose* bindPose, unsigned int idx, Blender::Bone* bone, Blender::Bone* parent)
{
	int bcount = 1;
	
	akJoint* joint = skel->getJoint(idx);
	joint->m_name = bone->name;
	
	float* bmat = (float*)bone->arm_mat; 
	Matrix4 mat(Vector4(bmat[4*0+0], bmat[4*0+1], bmat[4*0+2], bmat[4*0+3]),
				Vector4(bmat[4*1+0], bmat[4*1+1], bmat[4*1+2], bmat[4*1+3]),
				Vector4(bmat[4*2+0], bmat[4*2+1], bmat[4*2+2], bmat[4*2+3]),
				Vector4(bmat[4*3+0], bmat[4*3+1], bmat[4*3+2], bmat[4*3+3]));
				
	//joint->m_inverseBindPose = inverse(mat);
	akTransformState* jp = bindPose->getJointPose(idx);
	*jp = akTransformState(mat);
	
	if(parent)
	{
		joint->m_parentId = skel->getIndex(parent->name);
	}
	
	Blender::Bone* chi = static_cast<Blender::Bone*>(bone->childbase.first);
	while (chi)
	{
		// recurse
		bcount += buildBoneTree(skel, bindPose, idx+bcount, chi, bone);
		chi = chi->next;
	}
	
	return bcount;
}

void akBLoader::convertSkeleton(Blender::bArmature *bskel)
{
	unsigned int bcount = 0;
	Blender::Bone* bone = static_cast<Blender::Bone*>(bskel->bonebase.first);
	while (bone)
	{
		bcount += countBones(bone);
		bone = bone->next;
	}
	
	akSkeleton* skel = new akSkeleton(bcount);
	m_demo->addSkeleton(AKB_IDNAME(bskel), skel);
	akSkeletonPose* bindPose = new akSkeletonPose(skel, akSkeletonPose::SP_MODEL_SPACE);
	
	bcount = 0;
	bone = static_cast<Blender::Bone*>(bskel->bonebase.first);
	while (bone)
	{
		// only process root bones
		if (bone->parent == 0)
			bcount += buildBoneTree(skel, bindPose, bcount, bone, 0);
		bone = bone->next;
	}
	
	skel->setBindingPose(bindPose);
}

void akBLoader::convertMeshSkinning(akMesh* mesh, Blender::Object *bobj, akSkeleton* skel)
{
	Blender::Mesh* bmesh = (Blender::Mesh*)bobj->data;
	
	utArray<int> bonedgimap;
	int numb = skel->getNumJoints();
	
	for(int i=0; i<numb; i++)
	{
		bonedgimap.push_back(-1);
	}
	
	int dgi = 0;
	for (Blender::bDeformGroup* dg = (Blender::bDeformGroup*)bobj->defbase.first; dg; dg = dg->next, ++dgi)
	{
		int bi = skel->getIndex(dg->name);
		if(bi != -1)
			bonedgimap[bi] = dgi;
	}
	
	for (int i=0; i<mesh->getNumSubMeshes(); i++)
	{
		akSubMesh* smesh = mesh->getSubMesh(i);	
		smesh->addSkinningDataBuffer();
		
		akVertexBuffer* vbuf = smesh->getVertexBuffer();
		int vertnum = vbuf->getVerticesNumber();
		
		int* oldindices;
		float* weights; 
		UTuint8* bindices;
		
		vbuf->getElement(akVertexBuffer::VB_DU_CUSTOM, akVertexBuffer::VB_DT_INT32, 1, (void**)&oldindices, 0);
		vbuf->getElement(akVertexBuffer::VB_DU_BONE_IDX, akVertexBuffer::VB_DT_4UINT8, 1, (void**)&bindices, 0);
		vbuf->getElement(akVertexBuffer::VB_DU_BONE_WEIGHT, akVertexBuffer::VB_DT_4FLOAT32, 1, (void**)&weights, 0);
		
		for(int j=0; j<vertnum; j++)
		{
			int oldi = oldindices[j];
			Blender::MDeformVert& dv = bmesh->dvert[oldi];
			int bcount = 0;
			utArray<float> tmpweights;
			utArray<int> tmpbi;
			float wsum = 0;
			
			for (int w = 0; w < dv.totweight; w++)
			{
				int bi = bonedgimap.find(dv.dw[w].def_nr);
				if (bi != UT_NPOS)
				{
					wsum += dv.dw[w].weight;
					tmpweights.push_back(dv.dw[w].weight);
					tmpbi.push_back(bi);
					bcount++;
				}
			}
			
			if( !akFuzzy(wsum, 1.0f))
			{
				//std::cout << "Warning vertex " << oldi << " bone weights sum is not 1: " << wsum << std::endl;
				
				for(int b=0; b<bcount; b++)
				{
					tmpweights[b] = tmpweights[b] / wsum;
				}
			}
			
			if(bcount > 4)
			{
				while(bcount > 4)
				{
					//std::cout << "Warning vertex " << oldi << " deformed by more than 4 bones" << std::endl;
					
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
					
					bcount--;
				}
				
				wsum = 0;
				for(int b=0; b<bcount; b++)
				{
					wsum += tmpweights[b];
				}
				for(int b=0; b<bcount; b++)
				{
					tmpweights[b] = tmpweights[b] / wsum;
				}
			}
			
			for(int b=0; b<4; b++)
			{
				if(bcount<=b)
				{
					bindices[j*4+b] = 0;
					weights[j*4+b] = 0;
				}
				else
				{
					bindices[j*4+b] = tmpbi[b];
					weights[j*4+b] = tmpweights[b];
				}
			}
					
		}
	}
	
	bonedgimap.clear();
}

void akBLoader::convertMeshObject(Blender::Object *bobj)
{
	if(!bobj->data)
		return;

	akEntity* entity = new akEntity();
	m_demo->addEntity(AKB_IDNAME(bobj), entity);
	
	float* bmat = (float*)bobj->obmat; 
	Matrix4 mat(Vector4(bmat[4*0+0], bmat[4*0+1], bmat[4*0+2], bmat[4*0+3]),
				Vector4(bmat[4*1+0], bmat[4*1+1], bmat[4*1+2], bmat[4*1+3]),
				Vector4(bmat[4*2+0], bmat[4*2+1], bmat[4*2+2], bmat[4*2+3]),
				Vector4(bmat[4*3+0], bmat[4*3+1], bmat[4*3+2], bmat[4*3+3]));
				
				
	akTransformState trans(mat);
	entity->setTransformState(trans);
	
	Blender::Mesh* bmesh =  (Blender::Mesh*)bobj->data;
	if (!m_demo->getMesh(AKB_IDNAME(bmesh)))
		convertMesh(bmesh);
		
	akMesh* mesh = m_demo->getMesh(AKB_IDNAME(bmesh));
	entity->setMesh(mesh);
	
	if(bobj->parent != 0 && bobj->parent->type == OB_ARMATURE)
	{
		Blender::bArmature* bskel = (Blender::bArmature*)bobj->parent->data;
		if(!m_demo->getSkeleton(AKB_IDNAME(bskel)))
			convertSkeleton(bskel);
		
		akSkeleton* skel = m_demo->getSkeleton(AKB_IDNAME(bskel));
		entity->setSkeleton(skel);
		
		// skining weights
		
		convertMeshSkinning(mesh, bobj, skel);
	}
	
	removeMeshTempData(mesh);
}


void akBLoader::convertCameraObject(Blender::Object *bobj)
{
	if(!bobj || !bobj->data)
		return;

	Blender::Camera* bcam =  (Blender::Camera*)bobj->data;
	akCamera* camera = m_demo->getCamera();
	float* bmat = (float*)bobj->obmat; 
	Matrix4 mat(Vector4(bmat[4*0+0], bmat[4*0+1], bmat[4*0+2], bmat[4*0+3]),
				Vector4(bmat[4*1+0], bmat[4*1+1], bmat[4*1+2], bmat[4*1+3]),
				Vector4(bmat[4*2+0], bmat[4*2+1], bmat[4*2+2], bmat[4*2+3]),
				Vector4(bmat[4*3+0], bmat[4*3+1], bmat[4*3+2], bmat[4*3+3]));
				
	camera->m_transform = akTransformState(mat);
	camera->m_clipStart = bcam->clipsta;
	camera->m_clipEnd = bcam->clipend;
	camera->m_fov = 360.0f * atanf(16.0f / bcam->lens) / akPi; //TODO fovx to fovy
}


void akBLoader::loadFile(const utString &filename)
{
	fbtBlend fp;

	if (fp.parse(filename.c_str(), fbtFile::PM_COMPRESSED) != fbtFile::FS_OK)
	{
		return;
	}
	
	// current scne
	Blender::Scene* bscene = fp.m_fg->curscene;
	
	//animations
	akAnimationLoader animLoader(m_demo);
	animLoader.convertActions(fp.m_action, fp.getVersion() <= 249, bscene->r.frs_sec);
	
	//convert camera
	convertCameraObject(bscene->camera);
	
	//objects of the active scene
	for (Blender::Base* base = (Blender::Base*)bscene->base.first; base; base = base->next)
	{
		if (!base->object)
			continue;

		Blender::Object* bobj = base->object;

		// test for usable object type
		if(bobj->type == OB_MESH)
		{
			convertMeshObject(bobj);
			
			akEntity* entity = m_demo->getEntity(AKB_IDNAME(bobj));
			animLoader.convertObject(entity, bobj, fp.getVersion() <= 249, bscene->r.frs_sec);
		}
		
		
	}
	
	//add animation players to objects (automagic)
//	UTsize i;
//	UTsize count = m_demo->getNumEntities();
//	akDemo::Entities::ConstPointer ptr = m_demo->getEntities();
	
//	for(i=0; i<count; i++)
//	{
//		akEntity* entity = ptr[i].second;
//		//TODO search for animation clip that corrrespond to the object
//	}

	
}


