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

#include "akDemo.h"

#include "BlenderLoader/akBLoader.h"

#include "akMesh.h"
#include "akEntity.h"
#include "akSkeletonPose.h"
#include "akGeometryDeformer.h"

akDemo::akDemo() : akDemoBase()
{

}

akDemo::~akDemo()
{
	
}


void akDemo::init(void)
{
	akBLoader loader(this);
	loader.loadFile("Blu.blend");
	
	for(int i=0; i<m_objects.size(); i++)
	{
		akEntity* object = m_objects.at(i);
		
		
		// When an object is deformed we allocate a place in the vertexbuffer to store the deformed positions
		if(object->isMeshDeformed())
		{	
			akMesh* mesh = object->getMesh();
			for (int i=0; i<mesh->getNumSubMeshes(); i++)
			{
				akSubMesh* smesh = mesh->getSubMesh(i);
				smesh->addSecondPositionBuffer();
			}
		}
		
		// Activate the first action of each object
		akAnimationPlayer* player = m_objects.at(i)->getAnimationPlayers()->getAnimationPlayer(0);
		if(player)
		{
			player->setEnabled(true);
			player->setWeight(1.0f);
			player->setMode(akAnimationPlayer::AK_ACT_LOOP);
		}
	}
	

}

void akDemo::step(akScalar time)
{
	int i;
	for( i=0; i<m_objects.size(); i++)
	{		
		akEntity* object = m_objects.at(i);
		
		// advance the players time postions
		object->getAnimationPlayers()->stepTime(time);
	
		if(object->isPositionAnimated())
		{
			// evaluate the animation clip values for the object worlkd position
			akTransformState transform = akTransformState::identity();
			object->getAnimationPlayers()->evaluate(&transform);
			object->setTransformState(transform);
		}
		
		if(object->isMeshDeformed())
		{
			akSkeletonPose* pose;
			Matrix4* palette;
			
			pose = object->getPose();
			
			// evaluate the animation clip values for the skeleton pose
			pose->setIdentity();
			object->getAnimationPlayers()->evaluate(pose);
			pose->setSpace(akSkeletonPose::SP_BINDING_SPACE);
			pose->toModelSpace(pose);
			
			// get matrix pallete for the pose
			palette = object->getPalette();
			pose->fillMatrixPalette(palette);
			
			akMesh* mesh = object->getMesh();
			for (int i=0; i<mesh->getNumSubMeshes(); i++)
			{
				akSubMesh *smesh = mesh->getSubMesh(i);
				akVertexBuffer *vbuf = smesh->getVertexBuffer();
				
				UTuint8 *indices;
				float *weights;
				Vector3 *posin, *posout;
				unsigned int indicess, weightss, posins, posouts;
				
				
				// get vertex buffer info needed for vertex skinning
				vbuf->getElement(akVertexBuffer::VB_DU_BONE_IDX, akVertexBuffer::VB_DT_4UINT8, 1, (void**)&indices, &indicess);
				vbuf->getElement(akVertexBuffer::VB_DU_BONE_WEIGHT, akVertexBuffer::VB_DT_4FLOAT32, 1, (void**)&weights, &weightss);
				vbuf->getElement(akVertexBuffer::VB_DU_POSITION, akVertexBuffer::VB_DT_3FLOAT32, 1, (void**)&posin, &posins);
				vbuf->getElement(akVertexBuffer::VB_DU_POSITION, akVertexBuffer::VB_DT_3FLOAT32, 2, (void**)&posout, &posouts);
				
				
				// apply vertex skinning
				akGeometryDeformer::vertexSkinning(vbuf->getVerticesNumber(), palette,
													weights, weightss,
													indices, indicess,
													posin, posins,
													posout, posouts);
				
			}
			
		}
	}
}
