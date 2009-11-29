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

