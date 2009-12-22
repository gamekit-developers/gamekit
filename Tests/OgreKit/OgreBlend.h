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

class gkEngine;
class gkSceneObject;
class gkGameObject;
class gkBlendFile;


class OgreBlend : public BulletBlendReaderNew
{
protected:

	gkEngine			*m_engine;
	gkSceneObject		*m_scene;
	gkBlendFile			*m_file;
	Blender::Scene		*m_blenScene;
	Ogre::String		m_curFile;

	void	applyObjectProperties(Blender::Object *tmpObject, gkGameObject *obj);

	// for non physics constraints
	void	_convertConstraints(Blender::Object *tmpObject, gkGameObject *obj);


public:
	OgreBlend(class btDynamicsWorld *destinationWorld);
	virtual ~OgreBlend();

	void	beginScene(Blender::Scene *scene);
	void	convertLogicBricks();
	void	createParentChildHierarchy();
	void*	createGraphicsObject(Blender::Object *tmpObject, class btCollisionObject *bulletObject);



	void	addCamera(Blender::Object *tmpObject);
	void	addArmature(Blender::Object *tmpObject);
	void	addEmpty(Blender::Object *tmpObject);
	void	addLight(Blender::Object *tmpObject);

	void	load();

	// setup minimal blend file
	int		_readFile(char *fname);

};


#endif//_OgreBlend_h_
