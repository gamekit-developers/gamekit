


#include "IrrBlend.h"
#include "BulletIrrlichtUtils.h"


using namespace irr;



struct	IrrlichtMeshContainer
{
	btAlignedObjectArray<irr::scene::ISceneNode*>	m_sceneNodes;
};

IrrlichtBulletBlendReader::IrrlichtBulletBlendReader(irr::IrrlichtDevice* device,irr::scene::ISceneManager* irrlichtSceneManager, btDiscreteDynamicsWorld* bulletWorld, btLogicManager* logicManager)
		:BulletBlendReader(bulletWorld),
		m_device(device),
		m_sceneManager(irrlichtSceneManager),
		m_logicManager(logicManager),
		m_guid(666)
{
}
	
void	IrrlichtBulletBlendReader::addCamera(_bObj* tmpObject)
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

void	IrrlichtBulletBlendReader::addLight(_bObj* tmpObject)
{

}


void* IrrlichtBulletBlendReader::createGraphicsObject(_bObj* tmpObject, class btCollisionObject* bulletObject)
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
			if (currentIndex>maxNumIndices)
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

scene::ISceneNode*	IrrlichtBulletBlendReader::createMeshNode(video::S3DVertex* vertices, int numVertices, unsigned short int* indices, int numIndices,int numTriangles,class btCollisionObject* bulletObject,_bObj* tmpObject)
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
				io::IReadFile* file = m_device->getFileSystem()->createMemoryReadFile(jpgData,jpgSize,fileName,false);
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


void	IrrlichtBulletBlendReader::createParentChildHierarchy()
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

btDataObject* IrrlichtBulletBlendReader::getObjectMemberPtrByName(btDataObject* ob, const char* ptrName)
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

void	IrrlichtBulletBlendReader::convertLogicBricks()
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