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
#include "../akDemoBase.h"

#include "akAnimationLoader.h"
#include "akMeshLoader.h"

#define BLENDER_ARM_DEF_QUATERNION (1<<2)  //form blender dna files

akBLoader::akBLoader(akDemoBase* demo)
{
	m_demo = demo;
}

void buildBoneTree(akSkeleton* skel, btAlignedObjectArray<akMatrix4>& bind, Blender::Bone* bone, UTuint8 parent)
{
	//if(!(bone->flag & BONE_NO_DEFORM) || parent == AK_JOINT_NO_PARENT)
	{
		utHashedString jname = bone->name;
		
		float* bmat = (float*)bone->arm_mat; 
		akMatrix4 mat(akVector4(bmat[4*0+0], bmat[4*0+1], bmat[4*0+2], bmat[4*0+3]),
					  akVector4(bmat[4*1+0], bmat[4*1+1], bmat[4*1+2], bmat[4*1+3]),
					  akVector4(bmat[4*2+0], bmat[4*2+1], bmat[4*2+2], bmat[4*2+3]),
					  akVector4(bmat[4*3+0], bmat[4*3+1], bmat[4*3+2], bmat[4*3+3]));
		
		bind.push_back(mat);
		parent = skel->addJoint(jname, parent);
		if(bone->flag & BONE_NO_SCALE)
			skel->getJoint(parent)->m_inheritScale = false;
	}
	
	Blender::Bone* chi = static_cast<Blender::Bone*>(bone->childbase.first);
	while (chi)
	{
		// recurse
		buildBoneTree(skel, bind, chi, parent);
		chi = chi->next;
	}
}

void akBLoader::convertSkeleton(Blender::bArmature *bskel)
{
	akSkeleton* skel = new akSkeleton();
	btAlignedObjectArray<akMatrix4> bind;
	
	Blender::Bone* bone = static_cast<Blender::Bone*>(bskel->bonebase.first);
	while (bone)
	{
		// only process root bones
		if (bone->parent == 0)
			buildBoneTree(skel, bind, bone, AK_JOINT_NO_PARENT);
		bone = bone->next;
	}

	akSkeletonPose* bindPose = new akSkeletonPose(skel, akSkeletonPose::SP_MODEL_SPACE);
	for(int i=0; i<skel->getNumJoints(); i++)
	{
		akTransformState t;
		akMathUtils::extractTransform(bind[i], t.loc, t.rot,t.scale );
		*bindPose->getJointPose(i) = t;
	}
	skel->setBindingPose(bindPose);
	
	m_demo->addSkeleton(AKB_IDNAME(bskel), skel);
}

void akBLoader::convertObjectMesh(Blender::Object *bobj)
{
	if(!bobj->data)
		return;

	akEntity* entity = new akEntity();
	m_demo->addEntity(AKB_IDNAME(bobj), entity);
	
	float* bmat = (float*)bobj->obmat; 
	akMatrix4 mat(akVector4(bmat[4*0+0], bmat[4*0+1], bmat[4*0+2], bmat[4*0+3]),
				akVector4(bmat[4*1+0], bmat[4*1+1], bmat[4*1+2], bmat[4*1+3]),
				akVector4(bmat[4*2+0], bmat[4*2+1], bmat[4*2+2], bmat[4*2+3]),
				akVector4(bmat[4*3+0], bmat[4*3+1], bmat[4*3+2], bmat[4*3+3]));
				
				
	akTransformState trans;
	akMathUtils::extractTransform(mat, trans.loc, trans.rot,trans.scale );
	entity->setTransformState(trans);
	
	Blender::Mesh* bmesh =  (Blender::Mesh*)bobj->data;
	
	if (!m_demo->getMesh(AKB_IDNAME(bmesh)))
	{
		akMesh* mesh = new akMesh();
		akMeshLoader meconv(m_demo, mesh, bobj, bmesh);
		meconv.convert(false, true);
		m_demo->addMesh(AKB_IDNAME(bmesh), mesh);
	}
	
	akMesh* mesh = m_demo->getMesh(AKB_IDNAME(bmesh));
	entity->setMesh(mesh);
	
	if(mesh && bobj->parent != 0 && bobj->parent->type == OB_ARMATURE)
	{
	
		Blender::bArmature* bskel = (Blender::bArmature*)bobj->parent->data;
		if(!m_demo->getSkeleton(AKB_IDNAME(bskel)))
			convertSkeleton(bskel);
		
		akSkeleton* skel = m_demo->getSkeleton(AKB_IDNAME(bskel));
		entity->setSkeleton(skel);
		
		if(bskel->deformflag & BLENDER_ARM_DEF_QUATERNION)
			entity->setUseDualQuatSkinning(true);

		mesh->generateBoneWeightsFromVertexGroups(skel, true);
	}
}

void akBLoader::convertCameraObject(Blender::Object *bobj)
{
	if(!bobj || !bobj->data)
		return;

	Blender::Camera* bcam =  (Blender::Camera*)bobj->data;
	akCamera* camera = m_demo->getCamera();
	float* bmat = (float*)bobj->obmat; 
	akMatrix4 mat(akVector4(bmat[4*0+0], bmat[4*0+1], bmat[4*0+2], bmat[4*0+3]),
				akVector4(bmat[4*1+0], bmat[4*1+1], bmat[4*1+2], bmat[4*1+3]),
				akVector4(bmat[4*2+0], bmat[4*2+1], bmat[4*2+2], bmat[4*2+3]),
				akVector4(bmat[4*3+0], bmat[4*3+1], bmat[4*3+2], bmat[4*3+3]));
				
	akTransformState trans;
	akMathUtils::extractTransform(mat, trans.loc, trans.rot,trans.scale );
	camera->m_transform = trans;
	camera->m_clipStart = bcam->clipsta;
	camera->m_clipEnd = bcam->clipend;
	camera->m_fov = 360.0f * atanf(16.0f / bcam->lens) / akPi; //TODO fovx to fovy
}


void akBLoader::loadFile(const utString &filename, bool sortByMat, bool openglVertexColor)
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
			convertObjectMesh(bobj);
			
			akEntity* entity = m_demo->getEntity(AKB_IDNAME(bobj));
			animLoader.convertObject(entity, bobj, fp.getVersion() <= 249, bscene->r.frs_sec);
		}
	}
}


