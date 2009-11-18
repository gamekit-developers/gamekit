

#ifndef __IRR_BLEND_NEW_H
#define __IRR_BLEND_NEW_H

#include "BulletBlendReaderNew.h"



#include <irrlicht.h>
#include <iostream>

#include "btBulletDynamicsCommon.h"
#include "BulletBlendReaderNew.h"
struct btLogicManager;
class btDiscreteDynamicsWorld;


class IrrBlendNew : public BulletBlendReaderNew
{

	irr::IrrlichtDevice* m_device;
	irr::scene::ISceneManager* m_sceneManager;
	btLogicManager* m_logicManager;

	int	m_guid;


public:
	IrrBlendNew(irr::IrrlichtDevice* device,irr::scene::ISceneManager* irrlichtSceneManager, btDiscreteDynamicsWorld* bulletWorld, btLogicManager* logicManager);

	virtual ~IrrBlendNew();
	
	virtual	void	addCamera(Blender::Object* tmpObject);
	
	virtual	void	addLight(Blender::Object* tmpObject);

	virtual void* createGraphicsObject(Blender::Object* tmpObject, class btCollisionObject* bulletObject);

	irr::scene::ISceneNode*	createMeshNode(irr::video::S3DVertex* vertices, int numVertices, unsigned short int* indices, int numIndices,int numTriangles,class btCollisionObject* bulletObject,Blender::Object* tmpObject);
	
	virtual	void	createParentChildHierarchy();

	virtual	void	convertLogicBricks();

};


#endif //__IRR_BLEND_NEW_H
