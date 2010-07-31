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
#ifndef _gkMeshManager_h_
#define _gkMeshManager_h_

#include "gkCommon.h"
#include "gkHashedString.h"
#include "gkMesh.h"
#include "OgreSingleton.h"


// Manager for loaded scenes
class gkMeshManager : public Ogre::Singleton<gkMeshManager>
{
public:
	typedef utHashTable<gkHashedString, gkMesh *> ObjectMap;

protected:
	ObjectMap m_objects;

public:

	gkMeshManager();
	virtual ~gkMeshManager();


	gkMesh *getMesh(const gkHashedString &name);
	gkMesh *create(const gkHashedString &name);

	void destroy(const gkHashedString &name);
	void destroy(gkMesh *ob);
	void destroyAll(void);
	bool hasMesh(const gkHashedString &name);

	static gkMeshManager &getSingleton(void);
	static gkMeshManager *getSingletonPtr(void);

};





#endif//_gkMeshManager_h_
