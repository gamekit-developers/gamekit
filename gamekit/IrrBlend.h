#ifndef IRRLICHT_BULLET_BLEND_READER_H
#define IRRLICHT_BULLET_BLEND_READER_H

#include "BulletBlendReader.h"

#include <irrlicht.h>
#include "btBulletDynamicsCommon.h"


class IrrlichtBulletBlendReader : public BulletBlendReader
{

	irr::IrrlichtDevice* m_device;
	irr::scene::ISceneManager* m_sceneManager;
	btLogicManager* m_logicManager;

	int	m_guid;

public:
	IrrlichtBulletBlendReader(irr::IrrlichtDevice* device,irr::scene::ISceneManager* irrlichtSceneManager, btDiscreteDynamicsWorld* bulletWorld, btLogicManager* logicManager);
	
	virtual	void	addCamera(_bObj* tmpObject);
	
	virtual	void	addLight(_bObj* tmpObject);

	virtual void* createGraphicsObject(_bObj* tmpObject, class btCollisionObject* bulletObject);

	irr::scene::ISceneNode*	createMeshNode(irr::video::S3DVertex* vertices, int numVertices, unsigned short int* indices, int numIndices,int numTriangles,class btCollisionObject* bulletObject,_bObj* tmpObject);
	
	virtual	void	createParentChildHierarchy();

	btDataObject* getObjectMemberPtrByName(btDataObject* ob, const char* ptrName);

	virtual	void	convertLogicBricks();

};


#endif /IRRLICHT_BULLET_BLEND_READER_H

