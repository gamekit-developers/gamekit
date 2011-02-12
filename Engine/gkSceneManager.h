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
#ifndef _gkSceneObjectManager_h_
#define _gkSceneObjectManager_h_

#include "gkInstancedManager.h"
#include "utSingleton.h"



class gkSceneManager : public gkInstancedManager, public utSingleton<gkSceneManager>
{
public:
	UT_DECLARE_SINGLETON(gkSceneManager);

public:

	gkSceneManager();
	virtual ~gkSceneManager();


	gkResource* createImpl(const gkResourceName& name, const gkResourceHandle& handle);

	enum ObjType
	{
		OBJ_CAMERA	  = 1 << 0,
		OBJ_LIGHT	  = 1 << 1,
		OBJ_ENTITY	  = 1 << 2,
		OBJ_OBJECT	  = 1 << 3,
		OBJ_SKELETON  = 1 << 4,
		OBJ_PARTICLES = 1 << 5
	};

	void copyObjects(gkScene* fromScene, gkScene* toScene, int exceptObjectTypes = OBJ_CAMERA | OBJ_LIGHT);
	gkScene* createEmptyScene(const gkString& sceneName = "Scene", const gkString& cameraName = "Camera", const gkString& group = "");

private:
	void notifyResourceDestroyedImpl(gkResource* res);

};





#endif//_gkSceneObjectManager_h_
