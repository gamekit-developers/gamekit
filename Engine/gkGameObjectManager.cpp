/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

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
#include "gkSettings.h"
#include "gkGameObjectManager.h"
#include "gkGameObject.h"
#include "gkCamera.h"
#include "gkEntity.h"
#include "gkLight.h"
#include "gkSkeleton.h"
#include "Ogre/gkOgreParticleObject.h"


gkGameObjectManager::gkGameObjectManager()
	:    gkInstancedManager("GameObjectManager", "GameObject"),
	     m_currentType(GK_OBJECT)
{
}

gkGameObjectManager::~gkGameObjectManager()
{
	destroyAllInstances();
	destroyAll();
}


gkGameObject* gkGameObjectManager::createObject(const gkResourceName& name)
{
	m_currentType = GK_OBJECT;
	return static_cast<gkGameObject*>(create(name));
}

gkLight* gkGameObjectManager::createLight(const gkResourceName& name)
{
	m_currentType = GK_LIGHT;
	return static_cast<gkLight*>(create(name));
}

gkCamera* gkGameObjectManager::createCamera(const gkResourceName& name)
{
	m_currentType = GK_CAMERA;
	return static_cast<gkCamera*>(create(name));
}


gkEntity* gkGameObjectManager::createEntity(const gkResourceName& name)
{
	m_currentType = GK_ENTITY;
	return static_cast<gkEntity*>(create(name));
}

gkSkeleton* gkGameObjectManager::createSkeleton(const gkResourceName& name)
{
	m_currentType = GK_SKELETON;
	return static_cast<gkSkeleton*>(create(name));
}

gkParticleObject* gkGameObjectManager::createParticleObject(const gkResourceName& name)
{
	m_currentType = GK_PARTICLES;
	return static_cast<gkParticleObject*>(create(name));
}


gkGameObject* gkGameObjectManager::getObject(const gkResourceName& name)
{
	return getByName<gkGameObject>(name);
}

gkLight* gkGameObjectManager::getLight(const gkResourceName& name)
{
	gkGameObject* ob = getObject(name);
	return ob ? ob->getLight() : 0;
}

gkCamera* gkGameObjectManager::getCamera(const gkResourceName& name)
{
	gkGameObject* ob = getObject(name);
	return ob ? ob->getCamera() : 0;
}

gkEntity* gkGameObjectManager::getEntity(const gkResourceName& name)
{
	gkGameObject* ob = getObject(name);
	return ob ? ob->getEntity() : 0;
}

gkSkeleton* gkGameObjectManager::getSkeleton(const gkResourceName& name)
{
	gkGameObject* ob = getObject(name);
	return ob ? ob->getSkeleton() : 0;
}

gkParticleObject * gkGameObjectManager::getParticleObject(const gkResourceName& name)
{
	gkGameObject* ob = getObject(name);
	return ob ? ob->getParticleObject() : 0;
}


gkResource* gkGameObjectManager::createImpl(const gkResourceName& name, const gkResourceHandle& handle)
{
	int curType = m_currentType;
	m_currentType = GK_OBJECT;

	switch (curType)
	{
	case GK_LIGHT:     return new gkLight(this, name, handle);
	case GK_CAMERA:    return new gkCamera(this, name, handle);
	case GK_ENTITY:    return new gkEntity(this, name, handle);
	case GK_SKELETON:  return new gkSkeleton(this, name, handle);
#ifdef OGREKIT_USE_PARTICLE
	case GK_PARTICLES: return new gkOgreParticleObject(this, name, handle);
#endif
	}
	return new gkGameObject(this, name, handle);
}



void gkGameObjectManager::notifyResourceDestroyedImpl(gkResource* res)
{
	gkGameObject* gobj = static_cast<gkGameObject*>(res);
	if (gobj)
		gobj->destroyInstance();
}


UT_IMPLEMENT_SINGLETON(gkGameObjectManager);
