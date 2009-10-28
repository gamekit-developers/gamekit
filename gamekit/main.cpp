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

///Note: this is based on Irrlicht Example 003 Custom SceneNode, so the copyright of the original parts belongs to Irrlicht

///you can disable custom node, to use irr::scene::SAnimatedMesh in Irrlicht 1.5
//#define USE_CUSTOM_NODE 1

///You can enable vertex buffer objects (VBO) for Irrlicht 1.5
#define USE_VBO 1

#include <irrlicht.h>
#include <iostream>

#include "BulletBlendReader.h"
#include "btBulletDynamicsCommon.h"
#include "readblend.h"
#include "blendtype.h"

using namespace irr;
IrrlichtDevice* device=0;
btScalar	physicsWorldScaling = 1.f;
bool gPause=false;
bool gWireframe = false;


#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

//Blender/OpenGL
//left/right = X axis 
//front/back = Y axis 
//top/bottom = Z axis 
//
//In DirectX's and Irrlicht's and many other coordinate systems it's like this 
//
//left/right = X axis 
//top/bottom = Y axis 
//front/back = Z axis 


///mapping between right handed and left-handed coordinate system
#define SWAP_COORDINATE_SYSTEMS
#ifdef SWAP_COORDINATE_SYSTEMS

#define IRR_X 0
#define IRR_Y 2
#define IRR_Z 1

#define IRR_X_M 1.f
#define IRR_Y_M 1.f
#define IRR_Z_M 1.f

///also winding is different
#define IRR_TRI_0_X 0
#define IRR_TRI_0_Y 2
#define IRR_TRI_0_Z 1

#define IRR_TRI_1_X 0
#define IRR_TRI_1_Y 3
#define IRR_TRI_1_Z 2
#else
#define IRR_X 0
#define IRR_Y 1
#define IRR_Z 2

#define IRR_X_M 1.f
#define IRR_Y_M 1.f
#define IRR_Z_M 1.f

///also winding is different
#define IRR_TRI_0_X 0
#define IRR_TRI_0_Y 1
#define IRR_TRI_0_Z 2

#define IRR_TRI_1_X 0
#define IRR_TRI_1_Y 2
#define IRR_TRI_1_Z 3
#endif

void	MatrixToEuler(const btMatrix3x3& mat,btVector3& TEuler)
{
	irr::core::matrix4 imat;
	float m[16];
	m[0] = mat[IRR_X][IRR_X];
	m[1] = IRR_X_M*IRR_Y_M*mat[IRR_Y][IRR_X];
	m[2] = IRR_X_M*IRR_Z_M*mat[IRR_Z][IRR_X];
	m[3] = 0;

	m[4] = IRR_X_M*IRR_Y_M*mat[IRR_X][IRR_Y];
	m[5] = mat[IRR_Y][IRR_Y];
	m[6] = IRR_Y_M*IRR_Z_M*mat[IRR_Z][IRR_Y];
	m[7] = 0;

	m[8] = IRR_X_M*IRR_Z_M*mat[IRR_X][IRR_Z];
	m[9] = IRR_Y_M*IRR_Z_M*mat[IRR_Y][IRR_Z];
	m[10] = mat[IRR_Z][IRR_Z];
	m[11] = 0;
	
	m[12] = 0;//IRR_X_M*btr.getOrigin()[IRR_X];
	m[13] = 0;//IRR_Y_M*btr.getOrigin()[IRR_Y];
	m[14] = 0;//IRR_Z_M*btr.getOrigin()[IRR_Z];
	m[15] = 0;

	imat.setM(m);
	
	irr::core::vector3df eulerrot = imat.getRotationDegrees();
	TEuler[0] = eulerrot.X;
	TEuler[1] = eulerrot.Y;
	TEuler[2] = eulerrot.Z;
}


///temporary support for custom node
#ifdef USE_CUSTOM_NODE
class CSampleSceneNode : public scene::ISceneNode
{

	/*
	First, we declare some member variables:
	The bounding box, 4 vertices, and the material of the tetraeder.
	*/
	core::aabbox3d<f32> m_boundingBox;
	video::S3DVertex*	m_vertices;
	int					m_numVertices;
	unsigned short int*			m_indices;
	int					m_numTriangles;
	video::SMaterial Material;

	/*
	The parameters of the constructor specify the parent of the scene node,
	a pointer to the scene manager, and an id of the scene node.
	In the constructor we call the parent class' constructor,
	set some properties of the material, and
	create the 4 vertices of the tetraeder we will draw later.
	*/

public:

	CSampleSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id,video::S3DVertex* vertices,int numVertices,unsigned short int* indices,int numTriangles,irr::video::ITexture* texture0)
		: scene::ISceneNode(parent, mgr, id),
		m_vertices(vertices),
		m_numVertices(numVertices),
		m_indices(indices),
		m_numTriangles(numTriangles)
	{
		Material.Wireframe = false;
		Material.Lighting = false;
		video::IVideoDriver* driver = SceneManager->getVideoDriver();

		if (!texture0)
		{
			//irr::video::ITexture* texture = driver->getTexture("../../media/bullet_logo.JPG");//driver->getTexture("../../media/faerie2.bmp"); // set diffuse texture
			irr::video::ITexture* texture = driver->getTexture("../../media/textures/AlternatingBrick-ColorMap.png");//driver->getTexture("../../media/faerie2.bmp"); // set diffuse texture
			setMaterialTexture(0,texture);
		} else
		{
			setMaterialTexture(0,texture0);
		}

	/*
	The Irrlicht Engine needs to know the bounding box of a scene node.
	It will use it for automatic culling and other things. Hence, we
	need to create a bounding box from the 4 vertices we use.
	If you do not want the engine to use the box for automatic culling,
	and/or don't want to create the box, you could also call
	irr::scene::ISceneNode::setAutomaticCulling() with irr::scene::EAC_OFF.
	*/
		irr::core::vector3df minBounds(1e30f,1e30f,1e30f);
		irr::core::vector3df maxBounds(-1e30f,-1e30f,-1e30f);
		
		m_boundingBox = core::aabbox3df(minBounds,maxBounds);
		//better to use Bullet for this

		////Box.set.reset(Vertices[0].Pos);
		for (s32 i=0; i<m_numVertices; ++i)
			m_boundingBox.addInternalPoint(m_vertices[i].Pos);
	}

	/*
	Before it is drawn, the irr::scene::ISceneNode::OnRegisterSceneNode()
	method of every scene node in the scene is called by the scene manager.
	If the scene node wishes to draw itself, it may register itself in the
	scene manager to be drawn. This is necessary to tell the scene manager
	when it should call irr::scene::ISceneNode::render(). For
	example, normal scene nodes render their content one after another,
	while stencil buffer shadows would like to be drawn after all other
	scene nodes. And camera or light scene nodes need to be rendered before
	all other scene nodes (if at all). So here we simply register the
	scene node to render normally. If we would like to let it be rendered
	like cameras or light, we would have to call
	SceneManager->registerNodeForRendering(this, SNRT_LIGHT_AND_CAMERA);
	After this, we call the actual
	irr::scene::ISceneNode::OnRegisterSceneNode() method of the base class,
	which simply lets also all the child scene nodes of this node register
	themselves.
	*/
	virtual void OnRegisterSceneNode()
	{
		if (IsVisible)
			SceneManager->registerNodeForRendering(this);

		ISceneNode::OnRegisterSceneNode();
	}

	/*
	In the render() method most of the interesting stuff happens: The
	Scene node renders itself. We override this method and draw the
	tetraeder.
	*/
	virtual void render()
	{
		
		video::IVideoDriver* driver = SceneManager->getVideoDriver();

		Material.Wireframe = gWireframe;
		driver->setMaterial(Material);
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		driver->drawIndexedTriangleList(m_vertices, m_numVertices, m_indices, m_numTriangles);
	}

	/*
	And finally we create three small additional methods.
	irr::scene::ISceneNode::getBoundingBox() returns the bounding box of
	this scene node, irr::scene::ISceneNode::getMaterialCount() returns the
	amount of materials in this scene node (our tetraeder only has one
	material), and irr::scene::ISceneNode::getMaterial() returns the
	material at an index. Because we have only one material here, we can
	return the only one material, assuming that no one ever calls
	getMaterial() with an index greater than 0.
	*/
	virtual const core::aabbox3d<f32>& getBoundingBox() const
	{
		return m_boundingBox;
	}

	virtual u32 getMaterialCount() const
	{
		return 1;
	}

	virtual video::SMaterial& getMaterial(u32 i)
	{
		return Material;
	}	
};



#endif //USE_CUSTOM_NODE




///This class sychronizes the world transform between Bullet rigid bodies and their accompanying Irrlicht nodes
class IrrMotionState : public btDefaultMotionState
{
	btAlignedObjectArray<scene::ISceneNode*> m_irrNodes;
public:

	IrrMotionState()
	{
	}
	///synchronizes world transform from user to physics
	virtual void	getWorldTransform(btTransform& centerOfMassWorldTrans ) const 
	{
		centerOfMassWorldTrans = m_graphicsWorldTrans;
	}

	void	addIrrlichtNode(scene::ISceneNode* node)
	{
		m_irrNodes.push_back(node);
	}

	///synchronizes world transform from physics to user
	///Bullet only calls the update of worldtransform for active objects
	virtual void	setWorldTransform(const btTransform& centerOfMassWorldTrans)
	{
		
		m_startWorldTrans = centerOfMassWorldTrans;
		m_graphicsWorldTrans = centerOfMassWorldTrans;

		const btVector3& Point = centerOfMassWorldTrans.getOrigin();
		// Set rotation
		btVector3 EulerRotation;
		MatrixToEuler(centerOfMassWorldTrans.getBasis(), EulerRotation);
	
		for (int i=0;i<m_irrNodes.size();i++)
		{
			m_irrNodes[i]->setPosition(core::vector3df((f32)Point[IRR_X], (f32)Point[IRR_Y], (f32)Point[IRR_Z])/physicsWorldScaling);
			m_irrNodes[i]->setRotation(core::vector3df(EulerRotation[0], EulerRotation[1], EulerRotation[2]));
			
		}
	}

};


struct	IrrlichtMeshContainer
{
	btAlignedObjectArray<scene::ISceneNode*>	m_sceneNodes;
};

class IrrlichtBulletBlendReader : public BulletBlendReader
{

	scene::ISceneManager* m_sceneManager;
	btLogicManager* m_logicManager;

	int	m_guid;

public:
	IrrlichtBulletBlendReader(scene::ISceneManager* irrlichtSceneManager, btDiscreteDynamicsWorld* bulletWorld, btLogicManager* logicManager)
		:BulletBlendReader(bulletWorld),
		m_sceneManager(irrlichtSceneManager),
		m_logicManager(logicManager),
		m_guid(666)
	{
	}
	
	virtual	void	addCamera(_bObj* tmpObject)
	{
		irr::scene::ICameraSceneNode* cam = m_sceneManager->getActiveCamera();
		if (cam)
		{
			cam->setPosition(irr::core::vector3df(tmpObject->location[IRR_X],tmpObject->location[IRR_Y],tmpObject->location[IRR_Z]));
			btMatrix3x3 mat;
			mat.setEulerZYX(tmpObject->rotphr[0],tmpObject->rotphr[1],tmpObject->rotphr[2]);
			btVector3 EulerRotation;
			MatrixToEuler(mat, EulerRotation);
			cam->setRotation(core::vector3df(EulerRotation[0], EulerRotation[1], EulerRotation[2]));
			cam->updateAbsolutePosition();
			
		}
	}

	virtual	void	addLight(_bObj* tmpObject)
	{

	}


	virtual void* createGraphicsObject(_bObj* tmpObject, class btCollisionObject* bulletObject)
	{

		IrrlichtMeshContainer* meshContainer = 0;

		
		btRigidBody* body = btRigidBody::upcast(bulletObject);
		IrrMotionState* newMotionState = 0;

		if (body)
		{
			if (!bulletObject->isStaticOrKinematicObject())
			{
				newMotionState = new IrrMotionState();
				newMotionState->setWorldTransform(body->getWorldTransform());
				body->setMotionState(newMotionState);
			}
		}
		

		if (tmpObject->data.mesh && tmpObject->data.mesh->vert_count && tmpObject->data.mesh->face_count)
		{
			if (tmpObject->data.mesh->vert_count> 16300)
			{
				printf("tmpObject->data.mesh->vert_count = %d\n",tmpObject->data.mesh->vert_count);
			}

			int maxVerts = btMin(16300,btMax(tmpObject->data.mesh->face_count*3*2,(tmpObject->data.mesh->vert_count-6)));

			video::S3DVertex* orgVertices= new video::S3DVertex[tmpObject->data.mesh->vert_count];
			video::S3DVertex* newVertices= new video::S3DVertex[maxVerts];
			
			for (int v=0;v<tmpObject->data.mesh->vert_count;v++)
			{
				float* vt3 = tmpObject->data.mesh->vert[v].xyz;
				orgVertices[v] = video::S3DVertex(	IRR_X_M*vt3[IRR_X],	IRR_Y_M*vt3[IRR_Y],	IRR_Z_M*vt3[IRR_Z], 1,1,0,		video::SColor(255,255,255,255), 0, 1);
			}

			
			int numVertices = 0;
			int numTriangles=0;
			int numIndices = 0;
			int currentIndex = 0;

			

			int maxNumIndices = tmpObject->data.mesh->face_count*4*2;

			unsigned short int* indices= new unsigned short int[maxNumIndices];

			for (int t=0;t<tmpObject->data.mesh->face_count;t++)
			{
				if (currentIndex>maxNumIndices-10)
					break;

				int originalIndex = tmpObject->data.mesh->face[t].v[IRR_TRI_0_X];
				indices[numIndices] = currentIndex;
				newVertices[indices[numIndices]] = orgVertices[originalIndex];
				newVertices[indices[numIndices]].TCoords.X = tmpObject->data.mesh->face[t].uv[IRR_TRI_0_X][0];
				newVertices[indices[numIndices]].TCoords.Y = tmpObject->data.mesh->face[t].uv[IRR_TRI_0_X][1];
				numIndices++;
				currentIndex++;
				numVertices++;

				originalIndex = tmpObject->data.mesh->face[t].v[IRR_TRI_0_Y];
				indices[numIndices] = currentIndex;
				newVertices[indices[numIndices]] = orgVertices[originalIndex];
				newVertices[indices[numIndices]].TCoords.X = tmpObject->data.mesh->face[t].uv[IRR_TRI_0_Y][0];
				newVertices[indices[numIndices]].TCoords.Y = tmpObject->data.mesh->face[t].uv[IRR_TRI_0_Y][1];
				numIndices++;
				currentIndex++;
				numVertices++;

				originalIndex = tmpObject->data.mesh->face[t].v[IRR_TRI_0_Z];
				indices[numIndices] = currentIndex;
				newVertices[indices[numIndices]] = orgVertices[originalIndex];
				newVertices[indices[numIndices]].TCoords.X = tmpObject->data.mesh->face[t].uv[IRR_TRI_0_Z][0];
				newVertices[indices[numIndices]].TCoords.Y = tmpObject->data.mesh->face[t].uv[IRR_TRI_0_Z][1];
				numIndices++;
				currentIndex++;
				numVertices++;
				numTriangles++;

				if (tmpObject->data.mesh->face[t].v[3])
				{
					originalIndex = tmpObject->data.mesh->face[t].v[IRR_TRI_1_X];
					indices[numIndices]= currentIndex;
					newVertices[currentIndex] = orgVertices[originalIndex];
					newVertices[currentIndex].TCoords.X = tmpObject->data.mesh->face[t].uv[IRR_TRI_1_X][0];
					newVertices[currentIndex].TCoords.Y = tmpObject->data.mesh->face[t].uv[IRR_TRI_1_X][1];
					numIndices++;
					numVertices++;
					currentIndex++;

					originalIndex = tmpObject->data.mesh->face[t].v[IRR_TRI_1_Y];
					indices[numIndices]= currentIndex;
					newVertices[currentIndex] = orgVertices[originalIndex];
					newVertices[currentIndex].TCoords.X = tmpObject->data.mesh->face[t].uv[IRR_TRI_1_Y][0];
					newVertices[currentIndex].TCoords.Y = tmpObject->data.mesh->face[t].uv[IRR_TRI_1_Y][1];
					numIndices++;
					numVertices++;
					currentIndex++;

					originalIndex = tmpObject->data.mesh->face[t].v[IRR_TRI_1_Z];
					indices[numIndices]= currentIndex;
					newVertices[currentIndex] = orgVertices[originalIndex];
					newVertices[currentIndex].TCoords.X = tmpObject->data.mesh->face[t].uv[IRR_TRI_1_Z][0];
					newVertices[currentIndex].TCoords.Y = tmpObject->data.mesh->face[t].uv[IRR_TRI_1_Z][1];
					
					numIndices++;
					numVertices++;
					currentIndex++;

					numTriangles++;
				}

				
				///subdivide the mesh into smaller parts, so that indices fit in short int
				if (numVertices>=maxVerts)
				{
					scene::ISceneNode* node = createMeshNode(newVertices,numVertices,indices,numIndices,numTriangles,bulletObject,tmpObject);
					if (!meshContainer)
						meshContainer = new IrrlichtMeshContainer();
					meshContainer->m_sceneNodes.push_back(node);

					if (newMotionState && node)
						newMotionState->addIrrlichtNode(node);
					
					numVertices = 0;
					numTriangles = 0;
					numIndices = 0;

					numVertices = 0;
					currentIndex = 0;

					indices= new unsigned short int[tmpObject->data.mesh->face_count*4*2];
				}
			}

			if (numTriangles>0)
			{
				scene::ISceneNode* node = createMeshNode(newVertices,numVertices,indices,numIndices,numTriangles,bulletObject,tmpObject);

				if (!meshContainer)
						meshContainer = new IrrlichtMeshContainer();
				meshContainer->m_sceneNodes.push_back(node);

				if (newMotionState && node)
					newMotionState->addIrrlichtNode(node);
				

			}
		}
	
		return meshContainer;
		
	}

	scene::ISceneNode*	createMeshNode(video::S3DVertex* vertices, int numVertices, unsigned short int* indices, int numIndices,int numTriangles,class btCollisionObject* bulletObject,_bObj* tmpObject)
	{
		scene::ISceneNode* myNode = 0;

		irr::video::ITexture* texture0=0;
		video::IVideoDriver* driver = m_sceneManager->getVideoDriver();

		if (tmpObject->data.mesh->face[0].m_image)
		{
			const char* fileName = tmpObject->data.mesh->face[0].m_image->m_imagePathName;
			
			texture0 = driver->findTexture(fileName);
			if (!texture0)
			{
				void* jpgData = tmpObject->data.mesh->face[0].m_image->m_packedImagePtr;
				int jpgSize = tmpObject->data.mesh->face[0].m_image->m_sizePackedImage;
				if (jpgSize)
				{
					io::IReadFile* file = device->getFileSystem()->createMemoryReadFile(jpgData,jpgSize,fileName,false);
					if (file)
					{
						texture0 = driver->getTexture(file);
						printf("width = %d, height = %d\n",texture0->getSize().Width,texture0->getSize().Height);
					}
				}
			}
		}

		if (!texture0)
		{
			static irr::video::ITexture* notFoundTexture = 0;
			if (!notFoundTexture)
			{
				unsigned char*	imageData=new unsigned char[256*256*3];
				for(int y=0;y<256;++y)
				{
					const int	t=y>>4;
					unsigned char*	pi=imageData+y*256*3;
					for(int x=0;x<256;++x)
					{
						const int		s=x>>4;
						const unsigned char	b=180;					
						unsigned char			c=b+((s+t&1)&1)*(255-b);
						pi[0]= 255;
						pi[1]=pi[2]=c;pi+=3;
					}
				}
				irr::core::dimension2d<unsigned int> dim(256,256);
				irr::video::IImage* image = driver->createImageFromData(irr::video::ECF_R8G8B8,dim,imageData);
				delete imageData;
				notFoundTexture = driver->addTexture("notFound",image);
				image->drop();
			}
			
			texture0 = notFoundTexture;
		}

		if (texture0)
		{

	
	#ifdef USE_CUSTOM_NODE
				myNode = new CSampleSceneNode(m_sceneManager->getRootSceneNode(), m_sceneManager, m_guid++,vertices,numVertices,indices,numTriangles,texture0);
	#else
				irr::scene::SMesh* mesh = new irr::scene::SMesh();
				irr::scene::SMeshBuffer* meshbuf = new irr::scene::SMeshBuffer();
				meshbuf->append(vertices,numVertices,indices,numTriangles*3);
				mesh->addMeshBuffer(meshbuf);
				irr::scene::SAnimatedMesh* animMesh = new irr::scene::SAnimatedMesh(mesh);
#ifdef USE_VBO
				animMesh->setHardwareMappingHint(scene::EHM_STATIC);
#endif //USE_VBO
				myNode = m_sceneManager->addMeshSceneNode(animMesh);
				myNode->setMaterialTexture(0,texture0);
				myNode->setMaterialFlag(irr::video::EMF_LIGHTING,false);
	#endif
			
				// Set rotation
				btVector3 EulerRotation;
				btTransform btr = bulletObject->getWorldTransform();

				btMatrix3x3 bmat33 = btr.getBasis();

				irr::core::matrix4 imat;
				float m[16];
				m[0] = btr.getBasis()[IRR_X][IRR_X];
				m[1] = IRR_X_M*IRR_Y_M*btr.getBasis()[IRR_Y][IRR_X];
				m[2] = IRR_X_M*IRR_Z_M*btr.getBasis()[IRR_Z][IRR_X];
				m[3] = 0;

				m[4] = IRR_X_M*IRR_Y_M*btr.getBasis()[IRR_X][IRR_Y];
				m[5] = btr.getBasis()[IRR_Y][IRR_Y];
				m[6] = IRR_Y_M*IRR_Z_M*btr.getBasis()[IRR_Z][IRR_Y];
				m[7] = 0;

				m[8] = IRR_X_M*IRR_Z_M*btr.getBasis()[IRR_X][IRR_Z];
				m[9] = IRR_Y_M*IRR_Z_M*btr.getBasis()[IRR_Y][IRR_Z];
				m[10] = btr.getBasis()[IRR_Z][IRR_Z];
				m[11] = 0;
				
				m[12] = IRR_X_M*btr.getOrigin()[IRR_X];
				m[13] = IRR_Y_M*btr.getOrigin()[IRR_Y];
				m[14] = IRR_Z_M*btr.getOrigin()[IRR_Z];
				m[15] = 0;

				imat.setM(m);
				
				irr::core::vector3df eulerrot = imat.getRotationDegrees();
				myNode->setRotation(eulerrot);
				myNode->setPosition(irr::core::vector3df(IRR_X_M*btr.getOrigin()[IRR_X],	IRR_Y_M*btr.getOrigin()[IRR_Y],	IRR_Z_M*btr.getOrigin()[IRR_Z]));



				myNode->setScale(core::vector3df(tmpObject->scaling[IRR_X],tmpObject->scaling[IRR_Y],tmpObject->scaling[IRR_Z]));
				myNode->getParent()->setPosition(irr::core::vector3df(0,0,0));
				myNode->getParent()->setRotation(irr::core::vector3df(0,0,0));
	#ifndef USE_CUSTOM_NODE
				mesh->recalculateBoundingBox();
				animMesh->recalculateBoundingBox();
				animMesh->setDirty();
	#endif
			}
		return myNode;
	}


	virtual	void	createParentChildHierarchy()
	{
		int i;
		for (i=0;i<m_visibleGameObjects.size();i++)
		{
			btDataObject* ob = m_visibleGameObjects[i];
			
			int parentBlendPtr = ob->getIntValue("*parent");
			if (parentBlendPtr)
			{
				btDataObject* parentPtr = *m_dataObjects[parentBlendPtr];

				btCollisionObject* colParent = (btCollisionObject*) parentPtr->m_userPointer;
				btCollisionObject* colObj = (btCollisionObject*)ob->m_userPointer;
				if (colObj && colParent)
				{
					IrrlichtMeshContainer* childNode = (IrrlichtMeshContainer*) colObj->getUserPointer();
					IrrlichtMeshContainer* parentNode = (IrrlichtMeshContainer*) colParent->getUserPointer();
					if (childNode && parentNode)
					{
						for (int i=0;i<childNode->m_sceneNodes.size();i++)
						{
							if (parentNode->m_sceneNodes.size() && childNode->m_sceneNodes.size())
							{
								
								//don't do physics with this
								//todo: add it to the collision compound (if flags are set)
								m_destinationWorld->removeCollisionObject(colObj);
								
								childNode->m_sceneNodes[i]->setParent(parentNode->m_sceneNodes[0]);

#if 0
								///todo: deal with 'parentinv'
								btDataValue* parentInverseOb = *ob->m_dataMap.find("parentinv[4][4]");
								btScalar m[16];

								{
								for (int i=0;i<4;i++)
									for (int j=0;j<4;j++)
									{
										m[i+j*4] = parentInverseOb->m_valueArray[i].m_valueArray[j].m_fValue;
									}
								}

								btTransform trans;
								trans.setFromOpenGLMatrix(m);
#endif
								///disconnect the rigidbody update for child objects
								colObj->setUserPointer(0);
								//childNode->m_sceneNodes[i]->setPosition(irr::core::vector3df(trans.getOrigin()[0],trans.getOrigin()[1],trans.getOrigin()[2]));
								//childNode->m_sceneNodes[i]->setRotation(irr::core::vector3df(0,0,0));

							}
						}
					}
				}
			}

			
				
		
		}
	}

	

#define SENS_ALWAYS		0
#define SENS_TOUCH		1
#define SENS_NEAR		2
#define SENS_KEYBOARD	3
#define SENS_PROPERTY	4
#define SENS_MOUSE		5
#define SENS_COLLISION	6
#define SENS_RADAR		7
#define SENS_RANDOM     8
#define SENS_RAY        9
#define SENS_MESSAGE   10
#define SENS_JOYSTICK  11
#define SENS_ACTUATOR  12
#define SENS_DELAY     13
#define SENS_ARMATURE  14

#define CONT_LOGIC_AND	0
#define CONT_LOGIC_OR	1
#define CONT_EXPRESSION	2
#define CONT_PYTHON		3
#define CONT_LOGIC_NAND	4
#define CONT_LOGIC_NOR	5
#define CONT_LOGIC_XOR	6
#define CONT_LOGIC_XNOR	7


#define ACT_OBJECT		0
#define ACT_IPO			1
#define ACT_LAMP		2
#define ACT_CAMERA		3
#define ACT_MATERIAL	4
#define ACT_SOUND		5
#define ACT_PROPERTY	6
	/* these two obsolete since 2.02 */
#define ACT_ADD_OBJECT	7
#define ACT_END_OBJECT	8

#define ACT_CONSTRAINT	9
#define ACT_EDIT_OBJECT	10
#define ACT_SCENE		11
#define ACT_GROUP		12
#define ACT_RANDOM      13
#define ACT_MESSAGE     14
#define ACT_ACTION		15	/* __ NLA */
#define ACT_GAME		17
#define ACT_VISIBILITY          18
#define ACT_2DFILTER	19
#define ACT_PARENT      20
#define ACT_SHAPEACTION 21
#define ACT_STATE		22
#define ACT_ARMATURE	23

	btDataObject* getObjectMemberPtrByName(btDataObject* ob, const char* ptrName)
	{
			unsigned int cPtr = ob->getIntValue(ptrName);
			if (cPtr)
			{
				btDataObject** dataPtr = m_dataObjects[cPtr];

				if (dataPtr&& *dataPtr)
				{
					btDataObject* dataObject = *dataPtr;
					return dataObject;
				}
			}
			return 0;
	}

	virtual	void	convertLogicBricks()
	{
		int i;
		for (i=0;i<m_visibleGameObjects.size();i++)
		{
			btDataObject* ob = m_visibleGameObjects[i];
			{
				btDataObject* sensorOb  = getObjectMemberPtrByName(ob,"sensors");
				while (sensorOb)
				{
					int sensType = sensorOb->getIntValue("type");
					switch (sensType)
					{
					case SENS_ALWAYS:
						{

							btSensor* sensor = new btSensor(ob,sensType);
							m_logicManager->m_sensors.push_back(sensor);
							break;
						}

					default:
						{
							printf("Unsupported sensor type %d\n",sensType);
						}
					}

					unsigned int nextIntPtr = sensorOb->getIntValue("*next");
					sensorOb = nextIntPtr ? *m_dataObjects[nextIntPtr] : 0;
				}
			}

			{
				btDataObject* controllerOb  = getObjectMemberPtrByName(ob,"controllers");
				while (controllerOb)
				{
					int conType = controllerOb->getIntValue("type");
					switch (conType)
					{
					case CONT_LOGIC_AND:
						{

							btController* controller = new btController(ob,conType);
							m_logicManager->m_controllers.push_back(controller);
							break;
						}

					default:
						{
							printf("Unsupported controller type %d\n",conType);
						}
					}

					unsigned int nextIntPtr = controllerOb->getIntValue("*next");
					controllerOb = nextIntPtr ? *m_dataObjects[nextIntPtr] : 0;
				}
			}
			
			{
				btDataObject* actuatorOb  = getObjectMemberPtrByName(ob,"actuators");
				while (actuatorOb )
				{
					int acType = actuatorOb ->getIntValue("type");
					switch (acType)
					{
					case ACT_SCENE:
					case ACT_IPO:
						{

							btActuator* actuator = new btActuator(ob,acType);
							m_logicManager->m_actuators.push_back(actuator);
							break;
						}

					
					default:
						{
							printf("Unsupported actuator type %d\n",acType);
						}
					}

					unsigned int nextIntPtr = actuatorOb ->getIntValue("*next");
					actuatorOb  = nextIntPtr ? *m_dataObjects[nextIntPtr] : 0;
				}
			}
			

		}
	}

};



class MyEventReceiver : public IEventReceiver
{
public:
	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event)
	{
		// Remember whether each key is down or up
		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
		{
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
			

			if (IsKeyDown(irr::KEY_KEY_Q))
			{
				exit(0);
			}
			if (IsKeyDown(irr::KEY_SPACE))
			{
				gPause = !gPause;
			}
			if (IsKeyDown(irr::KEY_KEY_W))
			{
				gWireframe = !gWireframe;
			}
		}

		return false;
	}

	// This is used to check whether a key is being held down
	virtual bool IsKeyDown(EKEY_CODE keyCode) const
	{
		return KeyIsDown[keyCode];
	}
	
	MyEventReceiver()
	{
		for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
			KeyIsDown[i] = false;
	}

private:
	// We use this array to store the current state of each key
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
};


#ifdef __APPLE__
#define MAXPATHLEN 512
char* AppleGetBundleDirectory(void) {
	CFURLRef bundleURL;
	CFStringRef pathStr;
	static char path[MAXPATHLEN];
	memset(path,MAXPATHLEN,0);
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	
	bundleURL = CFBundleCopyBundleURL(mainBundle);
	pathStr = CFURLCopyFileSystemPath(bundleURL, kCFURLPOSIXPathStyle);
	CFStringGetCString(pathStr, path, MAXPATHLEN, kCFStringEncodingASCII);
	CFRelease(pathStr);
	CFRelease(bundleURL);	
	return path;
}
#endif




/*
That's it. The Scene node is done. Now we simply have to start
the engine, create the scene node and a camera, and look at the result.
*/
int main(int argc,char** argv)
{

	
	if (argv[0])
		printf("argv[0]=%s\n",argv[0]);
	
	//const char* fileName = "clubsilo_packed.blend";
	const char* fileName = "PhysicsAnimationBakingDemo.blend";
	
	
	int verboseDumpAllTypes = false;
	int verboseDumpAllBlocks = false;

	printf("Usage:\nGameKit [-verbose] [blendfile.blend]\n");

	if (argc>1 && argv[1])
	{
		if (!strcmp(argv[1],"-verbose"))
		{
			verboseDumpAllTypes = true;
			verboseDumpAllBlocks = true;
			printf("enable verbose output: verboseDumpAllTypes and verboseDumpAllBlocks\n");
		}
	}

#if __APPLE__
	printf("Or double-click on the GameKit application. You can copy a file game.blend in the same directory as the GameKit application (not inside the bundle)\n");
#endif
	
	FILE* file = fopen(fileName,"rb");
	if (argc>1 || !file)
	{
#if __APPLE__

		char newName[1024];

		char* bundlePath = AppleGetBundleDirectory();
		//cut off the .app filename
		char* lastSlash=0;
		if( lastSlash = strrchr( bundlePath, '/' ) )
			*lastSlash = '\0';
		sprintf(newName,"%s/%s",bundlePath,"game.blend");
		file = fopen(newName,"rb");
	
		// how do you debug the Bundle execution, without a console?		
		FILE* dump = fopen ("/out.txt","wb");
		fwrite(newName,1,strlen(newName),dump);
		fclose(dump);
		
		if (!file)
		{
			sprintf(newName,"%s/%s/%s",AppleGetBundleDirectory(),"Contents/Resources",fileName);
			file=fopen(newName,"rb");
			if (!file)
				printf("cannot open file %s.\n",newName);
		}


		
#else

		char newName[1024];
		newName[0] = 0;

		switch (argc)
		{
		case 2:
			{
				if (argv[1])
					fileName = argv[1];
				break;
			}
		case 3:
			{
				if (argv[2])
					fileName = argv[2];
			}

		default:
			{
			}
		}
		
		sprintf(newName,"%s",fileName);
		file = fopen(newName,"rb");
		//printf("cannot open file %s.\n",newName);

#endif//__APPLE__
		if (!file)
		{
			printf("cannot open file %s.\n",argv[1]);
			exit(0);
		}
	}


	// let user select driver type

	video::E_DRIVER_TYPE driverType;
#if 0
	printf("Please select the driver you want for this example:\n"\
		" (a) Direct3D 9.0c\n (b) Direct3D 8.1\n (c) OpenGL 1.5\n"\
		" (d) Software Renderer\n (e) Burning's Software Renderer\n"\
		" (f) NullDevice\n (otherKey) exit\n\n");

	char i;
	std::cin >> i;
	switch(i)
	{
		case 'a': driverType = video::EDT_DIRECT3D9;break;
		case 'b': driverType = video::EDT_DIRECT3D8;break;
		case 'c': driverType = video::EDT_OPENGL;   break;
		case 'd': driverType = video::EDT_SOFTWARE; break;
		case 'e': driverType = video::EDT_BURNINGSVIDEO;break;
		case 'f': driverType = video::EDT_NULL;     break;
		default: return 0;
	}
#else
	//driverType = video::EDT_BURNINGSVIDEO;
	driverType = video::EDT_OPENGL;
	
#endif
	// create device

	MyEventReceiver receiver;

	core::dimension2d<unsigned int>dim(640, 480);
	device = createDevice(driverType,		dim, 32, false,false,false,&receiver);
	//device = createDevice(driverType,		core::dimension2d<s32>(640, 480), 32, false,false,true,&receiver);
	//device = createDevice(driverType,		core::dimension2d<s32>(640, 480), 32, true,false,true,&receiver);
//	device->setResizeAble(true);



	//device = createDevice(driverType,		core::dimension2d<s32>(1024, 768), 32, true);
		
	if (device == 0)
		return 1; // could not create selected driver.

	// create engine and camera

	device->setWindowCaption(L"Custom Scene Node - Irrlicht Engine Demo");


	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	irr::scene::ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS(0,20,0.01,100);
	//irr::scene::ICameraSceneNode* cam = smgr->addCameraSceneNodeMaya(0, core::vector3df(0,-40,0), core::vector3df(0,0,0));
	//irr::scene::ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS(0,10,10);
	//irr::scene::ICameraSceneNode* cam = smgr->addCameraSceneNode(0, core::vector3df(0,50,30), core::vector3df(0,0,0));
	//cam->setUpVector(irr::core::vector3df(0,1,0));
	

	// add light 1 (nearly red)
	scene::ILightSceneNode* light1 =
		smgr->addLightSceneNode(0, core::vector3df(400,0,0),
		video::SColorf(0.5f, 1.0f, 0.5f, 0.0f), 800.0f);


	btCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btBroadphaseInterface* pairCache = new btDbvtBroadphase();
	btDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btConstraintSolver* constraintSolver = new btSequentialImpulseConstraintSolver();
	btDiscreteDynamicsWorld* physicsWorld = new btDiscreteDynamicsWorld(dispatcher,pairCache,constraintSolver,collisionConfiguration);

	btLogicManager* logicManager = new btLogicManager();

	
//#ifdef SWAP_COORDINATE_SYSTEMS
	physicsWorld->setGravity(btVector3(0,0,-10));
//#endif //SWAP_COORDINATE_SYSTEMS

	IrrlichtBulletBlendReader	bulletBlendReader(smgr,physicsWorld,logicManager);
	if (!bulletBlendReader.readFile(file,verboseDumpAllTypes))
	{
		printf("cannot read Blender file %s.\n",argv[1]);
		fclose(file);
		exit(0);
	}
	
//				//io::IReadFile* file = io::createMemoryReadFile(jpgData, jpgSize, fileName, false);
//				irr::io::CMemoryReadFile* file = new irr::io::CMemoryReadFile(jpgData, jpgSize, fileName, false);
				

	

	bulletBlendReader.convertAllObjects(verboseDumpAllBlocks);
	//now create graphics objects for each Bullet object?
	
	/*
	Now draw everything and finish.
	*/
	u32 frames=0;
	while(device->run())
	{

			static int ms = device->getTimer()->getTime();
		int newTime = device->getTimer()->getTime();
		int deltaTimeMs = newTime-ms;
		ms = newTime;
		btScalar deltaTime = deltaTimeMs*0.001f;

		
		if (!gPause)
			physicsWorld->stepSimulation(deltaTime);

		
		driver->beginScene(true, true, video::SColor(0,100,100,100));

		logicManager->processLogicBricks(deltaTime);

		smgr->drawAll();

		driver->endScene();
		if (++frames==100)
		{
			core::stringw str = L"Irrlicht Engine [";
			str += driver->getName();
			str += L"] FPS: ";
			str += (s32)driver->getFPS();
			str += L" (press q to quit)";

			device->setWindowCaption(str.c_str());
			frames=0;
		}
		

	}

	device->drop();
	
	return 0;
}
