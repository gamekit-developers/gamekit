/*
GameKit
Copyright (c) 2009 Erwin Coumans  http://gamekit.googlecode.com

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/


//#define DUMP_TYPEDEFS 1

#include "BulletBlendReader.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "abs-file.h"

#include "readblend.h"
#include "blendtype.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.h"
#include "BulletCollision/Gimpact/btGImpactShape.h"

BulletBlendReader::BulletBlendReader(btDynamicsWorld* destinationWorld)
:m_bf(0),
m_destinationWorld(destinationWorld)
{

}


int	BulletBlendReader::openFile(const char* fileName)
{
	MY_FILETYPE *file;

	file = MY_OPEN_FOR_READ(fileName);

	if (!file) {
		fprintf(stderr,"couldn't open file %s\n",fileName);
		return 0;
	}

	m_bf =  blend_read(file);

	if (!m_bf) {
		fprintf(stderr, "couldn't read blender file. :(\n");
		MY_CLOSE(file);
		m_bf=0;
		return 0;
	}


#ifdef DUMP_TYPEDEFS
	blend_dump_typedefs(m_bf);
#endif //DUMP_TYPEDEFS

	MY_CLOSE(file);
	return 1;

	
}

BulletBlendReader::~BulletBlendReader()
{
	///@todo: delete/close file/data structures
	
}



void	BulletBlendReader::convertAllObjects()
{

	if (!m_bf)
	{
		printf("ERROR: no file loaded, use openFile first\n");
		return;
	}

	int j;
	for (j=0; j<m_bf->blocks_count; ++j) 
	{
		{
			int entry_count = blend_block_get_entry_count(m_bf, &m_bf->blocks[j]);
			for (int i=0; i<entry_count; ++i) 
			{
				BlendObject obj = blend_block_get_object(m_bf, &m_bf->blocks[j], i);
				//crawl(blend_file, obj);
				BlendObject data_obj;
				BlendObject data_obj2;

				BlendBlock* tmpBlock=0;

				{
					const char* type_name = m_bf->types[obj.type].name;

					//printf("type_name=%s. ",type_name);
					//printf("block blenderptr = %lx\n",m_bf->blocks[j].blender_pointer);

					
					if (strcmp(type_name,"Object")==0)
					{
						bObj tmpObj;
						blend_acquire_obj_from_obj(m_bf,&obj,&tmpObj,0);
						
						convertSingleObject(&tmpObj);

					}

					if (strcmp(type_name,"Mesh")==0)
					{
						//printf("object type_name = %s\n",type_name);
						bMesh tmpMesh;
						blend_acquire_mesh_from_obj(m_bf, &obj, &tmpMesh);
						convertSingleMesh(&tmpMesh);
					}
				}
			}
		}
	}
}



///for each Blender Object, this method will be called to convert/retrieve data from the bObj
void BulletBlendReader::convertSingleObject(_bObj* object)
{
	switch (object->type)
	{
	case BOBJ_TYPE_MESH:
		break;

	case BOBJ_TYPE_CAMERA:
		addCamera(object);
		return;
	case BOBJ_TYPE_LAMP:
		addLight(object);
		return;
	default:
		{
		}

	};

	//let's try to create some static collision shapes from the triangle meshes
	if (object->data.mesh)
	{
		btTriangleMesh* meshInterface = new btTriangleMesh();
		
		btVector3 minVert(1e30f,1e3f,1e30f);
		btVector3 maxVert(-1e30f,-1e30f,-1e30f);
		for (int t=0;t<object->data.mesh->face_count;t++)
		{
			
			float* vt0 = object->data.mesh->vert[object->data.mesh->face[t].v[0]].xyz;
			btVector3 vtx0(vt0[0],vt0[1],vt0[2]);
			minVert.setMin(vtx0); maxVert.setMax(vtx0);
			float* vt1 = object->data.mesh->vert[object->data.mesh->face[t].v[1]].xyz;
			btVector3 vtx1(vt1[0],vt1[1],vt1[2]);
			minVert.setMin(vtx1); maxVert.setMax(vtx1);
			float* vt2 = object->data.mesh->vert[object->data.mesh->face[t].v[2]].xyz;
			btVector3 vtx2(vt2[0],vt2[1],vt2[2]);
			minVert.setMin(vtx2); maxVert.setMax(vtx2);
			meshInterface ->addTriangle(vtx0,vtx1,vtx2);
			
			if (object->data.mesh->face[t].v[3])
			{
				float* vt3 = object->data.mesh->vert[object->data.mesh->face[t].v[3]].xyz;
				btVector3 vtx3(vt3[0],vt3[1],vt3[2]);
				minVert.setMin(vtx3); maxVert.setMax(vtx3);
				meshInterface ->addTriangle(vtx0,vtx3,vtx2);//?
			}
		}
		if (!meshInterface->getNumTriangles())
			return;

/* boundtype */
#define OB_BOUND_BOX		0
#define OB_BOUND_SPHERE		1
#define OB_BOUND_CYLINDER	2
#define OB_BOUND_CONE		3
#define OB_BOUND_POLYH		4
#define OB_BOUND_POLYT		5
#define OB_BOUND_DYN_MESH   6

/* ob->gameflag */
#define OB_DYNAMIC		1
//#define OB_CHILD		2
//#define OB_ACTOR		4
//#define OB_INERTIA_LOCK_X	8
//#define OB_INERTIA_LOCK_Y	16
//#define OB_INERTIA_LOCK_Z	32
//#define OB_DO_FH			64
//#define OB_ROT_FH			128
//#define OB_ANISOTROPIC_FRICTION 256
//#define OB_GHOST			512
#define OB_RIGID_BODY		1024
//#define OB_BOUNDS		2048

//#define OB_COLLISION_RESPONSE	4096//??
//#define OB_COLLISION	65536
//#define OB_SOFT_BODY	0x20000

		btVector3 localPos = (minVert+maxVert)*0.5f;
		btVector3 localSize= (maxVert-minVert)*0.5f;

		
		btTransform	worldTrans;
		worldTrans.setIdentity();
		worldTrans.setOrigin(btVector3(object->location[0],object->location[1],object->location[2]));
		//		blenderobject->loc[0]+blenderobject->dloc[0]//??
		
		//btVector3 eulerXYZ(object->rotphr[0],object->rotphr[1],object->rotphr[2]);
		worldTrans.getBasis().setEulerZYX(object->rotphr[0],object->rotphr[1],object->rotphr[2]);
		btVector3 scale(object->scaling[0],object->scaling[1],object->scaling[2]);

		if ( (object->gameflag & OB_RIGID_BODY) || (object->gameflag & OB_DYNAMIC))
		{
			//m_destinationWorld->addRigidBody(
			btCollisionShape* colShape = 0;

			switch (object->boundtype)
			{
			case OB_BOUND_SPHERE:
				{
					btScalar radius = localSize[localSize.maxAxis()];
					colShape = new btSphereShape(radius);
					break;
				};
			case OB_BOUND_BOX:
				{
					colShape = new btBoxShape(localSize);
					break;
				}
			case OB_BOUND_CYLINDER:
				{
					colShape = new btCylinderShapeZ(localSize);
					break;
				}
			case OB_BOUND_CONE:
				{
					btScalar radius = btMax(localSize[0], localSize[1]);
					btScalar height = 2.f*localSize[2];
					colShape = new btConeShapeZ(radius,height);
					break;
				}
			case OB_BOUND_POLYT:
			
				{
					//better to approximate it, using btShapeHull
					colShape = new btConvexTriangleMeshShape(meshInterface);
					break;
				}
			case OB_BOUND_POLYH:
			case OB_BOUND_DYN_MESH:
				{
					btGImpactMeshShape* gimpact = new btGImpactMeshShape(meshInterface);
					gimpact->postUpdate();
					colShape = gimpact;
					break;
				}

			default:
				{

				}
			};
			
			if (colShape)
			{
				colShape->setLocalScaling(scale);
				btVector3 inertia;
				colShape->calculateLocalInertia(object->mass,inertia);
				btRigidBody* body = new btRigidBody(object->mass,0,colShape,inertia);
				if (!(object->gameflag & OB_RIGID_BODY))
				{
					body->setAngularFactor(0.f);
				}
				body->setWorldTransform(worldTrans);
				m_destinationWorld->addRigidBody(body);
				//body->setActivationState(DISABLE_DEACTIVATION);

				createGraphicsObject(object,body);
			}

		} else
		{
			btCollisionObject* colObj = new btCollisionObject();
			colObj->setWorldTransform(worldTrans);

			btCollisionShape* colShape =0;
			if (meshInterface->getNumTriangles()>0)
			{
				btBvhTriangleMeshShape* childShape = new btBvhTriangleMeshShape(meshInterface,true);
				
				if (scale[0]!=1. || scale[1]!=1. || scale[2]!=1.)
				{
					colShape = new btScaledBvhTriangleMeshShape(childShape,scale);
				} else
				{
					colShape = childShape;
				}
				colObj->setCollisionShape(colShape);

				m_destinationWorld->addCollisionObject(colObj);
			
				createGraphicsObject(object,colObj);
			}
			
		}
	}
}

void BulletBlendReader::convertSingleMesh(_bMesh* mesh)
{

}
