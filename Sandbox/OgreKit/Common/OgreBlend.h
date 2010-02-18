/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2009 Erwin Coumans & Charlie C
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#ifndef _OgreBlend_h_
#define _OgreBlend_h_

#include "BulletBlendReaderNew.h"
#include "OgreString.h"
#include "OgrePrerequisites.h"
#include "OgreDataStream.h"

class btCollisionConfiguration;
class btBroadphaseInterface;
class btDispatcher;
class btConstraintSolver;

namespace Ogre
{
class BlendConverter;
}

/// Ogre blend file loader. Builds the OgreKit scene tree
class OgreBlend : public BulletBlendReaderNew
{
private:


	// internal reading 
	char*		m_memBuf;
	int			readFile(char* memoryBuffer, int len, int verboseDumpAllTypes = false);

protected:

	Blender::Scene*					m_blenScene;
	Ogre::String					m_group;
	Ogre::SceneManager*				m_manager;
	Ogre::Camera*                   m_camera;
	Ogre::BlendConverter*			m_converter;

	btCollisionConfiguration*		m_collisionConfiguration;;
	btBroadphaseInterface*			m_pairCache;
	btDispatcher*					m_dispatcher;
	btConstraintSolver*				m_constraintSolver;


	void applyObjectProperties(Blender::Object *tmpObject, Ogre::SceneNode *obj);
	void setupPhysics(void);

public:
	OgreBlend();
	virtual ~OgreBlend();

	bool read(const Ogre::String& resource);


	void	beginScene(Blender::Scene *scene);
	void	convertLogicBricks();
	void	createParentChildHierarchy();
	void*	createGraphicsObject(Blender::Object *tmpObject, class btCollisionObject *bulletObject);

	void	addCamera(Blender::Object *tmpObject);
	void	addArmature(Blender::Object *tmpObject);
	void	addEmpty(Blender::Object *tmpObject);
	void	addLight(Blender::Object *tmpObject);
};


#endif//_OgreBlend_h_
