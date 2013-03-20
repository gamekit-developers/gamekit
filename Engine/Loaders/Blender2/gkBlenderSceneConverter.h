/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Charlie C.

    Contributor(s): Thomas Trocha(dertom)
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
#ifndef _gkBlenderSceneConverter_h_
#define _gkBlenderSceneConverter_h_


#include "gkLoaderCommon.h"
#include "gkMathUtils.h"

class gkLogicLoader;


class gkBlenderSceneConverter
{
public:
	gkBlenderSceneConverter(gkBlendFile* fp, Blender::Scene* sc);
	~gkBlenderSceneConverter();
	
	// if you want to create group-instances from groups that are created in another
	// scene you have to first create the scene's with the groups suing convert(false)
	// and afterwards create the instances with convertGroupInstances();
	// default value to true because of compatibilty reasons
	void convert(bool createGroupInstances=true);
	// create the group instances for the corresponding scene. CAUTION: running convert(false) is
	// mandatory
	void convertGroupInstances(void);

private:
	bool validObject(Blender::Object* ob);
	void applyParents(utArray<Blender::Object*> &children);

	void convertSoundScene(void);
	void convertWorld(void);
	void convertObject(Blender::Object* ob, gkGameObject* gobj = 0);
	void convertGroups();

	void convertObjectGroup(gkGameObjectGroup* gobj, Blender::Object* bobj);
	void convertObjectGeneral(gkGameObject* gobj, Blender::Object* bobj);

	void convertObjectAnimations(gkGameObject* gobj, Blender::Object* bobj, gkScalar animfps);
	void convertObjectLogic(gkGameObject* gobj, Blender::Object* bobj);
	void convertObjectProperties(gkGameObject* gobj, Blender::Object* bobj);
	void convertObjectConstraints(gkGameObject* gobj, Blender::Object* bobj);
	void convertObjectPhysics(gkGameObject* gobj, Blender::Object* bobj);
	void convertObjectCamera(gkGameObject* gobj, Blender::Object* bobj);
	void convertObjectLamp(gkGameObject* gobj, Blender::Object* bobj);
	void convertObjectMesh(gkGameObject* gobj, Blender::Object* bobj);
	void convertObjectArmature(gkGameObject* gobj, Blender::Object* bobj);
	void convertObjectSkeleton(gkSkeletonResource* gobj, Blender::Object* bobj);
	void convertObjectParticles(gkGameObject* gobj, Blender::Object* bobj);
	void convertObjectCurve(gkGameObject* gobj, Blender::Object* bobj);


	Blender::Scene*				m_bscene;
	gkScene*					m_gscene;
	gkLogicLoader*				m_logic;
	gkBlendFile*				m_file;
	const gkResourceNameString	m_groupName;
};

#endif//_gkBlenderSceneConverter_h_
