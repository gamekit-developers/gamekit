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
#ifndef _gkGameObjectManager_h_
#define _gkGameObjectManager_h_

#include "gkInstancedManager.h"
#include "utSingleton.h"



class gkGameObjectManager : public gkInstancedManager, public utSingleton<gkGameObjectManager>
{
public:
	UT_DECLARE_SINGLETON(gkGameObjectManager);

public:

	gkGameObjectManager();
	virtual ~gkGameObjectManager();

	gkGameObject* createObject(const gkResourceName& name);
	gkLight* createLight(const gkResourceName& name);
	gkCamera* createCamera(const gkResourceName& name);
	gkEntity* createEntity(const gkResourceName& name);
	gkSkeleton* createSkeleton(const gkResourceName& name);
	gkParticleObject* createParticleObject(const gkResourceName& name);

	gkGameObject* getObject(const gkResourceName& name);
	gkLight* getLight(const gkResourceName& name);
	gkCamera* getCamera(const gkResourceName& name);
	gkEntity* getEntity(const gkResourceName& name);
	gkSkeleton* getSkeleton(const gkResourceName& name);
	gkParticleObject* getParticleObject(const gkResourceName& name);

private:
	int m_currentType;

	void notifyResourceDestroyedImpl(gkResource* res);
	gkResource* createImpl(const gkResourceName& name, const gkResourceHandle& handle);
};


#endif//_gkGameObjectManager_h_
