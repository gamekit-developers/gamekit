/*
-------------------------------------------------------------------------------
	This file is part of the Ogre GameKit port.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
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
#include "gkSceneLoader.h"
#include "gkSceneObject.h"
#include "gkBlendFile.h"
#include "gkEngine.h"
#include "gkUserDefs.h"
#include "gkBlenderDefines.h"
#include "gkSceneObjectManager.h"
#include "gkObjectLoader.h"
#include "gkGameObject.h"
#include "gkLightObject.h"
#include "gkCameraObject.h"
#include "gkEntityObject.h"
#include "gkMeshLoader.h"
#include "gkSkeletonLoader.h"
#include "gkLoaderUtils.h"

#include "blender.h"

using namespace Ogre;




// ----------------------------------------------------------------------------
gkSceneObjectLoader::gkSceneObjectLoader(gkBlendFile *fp, Blender::Scene *scene) :
		mFile(fp), mScene(scene)
{
	GK_ASSERT(mFile);
	GK_ASSERT(mScene);
}


// ----------------------------------------------------------------------------
gkSceneObjectLoader::~gkSceneObjectLoader()
{
}


// ----------------------------------------------------------------------------
void gkSceneObjectLoader::loadResource(Resource* resource)
{
	GK_ASSERT(mFile);
	GK_ASSERT(mScene);
	GK_ASSERT(resource);

	/// convert/ load objects

	gkSceneObject *scene= static_cast<gkSceneObject*>(resource);

	gkSceneProperties& props= scene->getProperties();


	// Apply user defined animation fps
	gkLoaderUtils::blender_anim_rate= mScene->r.frs_sec;
	gkEngine::getSingleton().getUserDefs().animspeed= gkLoaderUtils::blender_anim_rate;


	if (mScene->world)
	{
		Blender::World *world= mScene->world;

		props.gravity= Vector3(0.f, 0.f, -world->gravity);
		props.ambient.r= world->ambr;
		props.ambient.g= world->ambg;
		props.ambient.b= world->ambb;

		props.world_color.r= world->horr;
		props.world_color.g= world->horg;
		props.world_color.b= world->horb;
	}

	for (Blender::Base *base= (Blender::Base*)mScene->base.first; base; base= base->next)
	{
		if (!base->object) continue;

		Blender::Object *ob= base->object;
		loadObject(scene, base->object);

		if (ob->parent)
		{
			if (scene->hasObject(GKB_IDNAME(ob->parent)))
			{
				gkGameObject *parob= scene->getObject(GKB_IDNAME(ob->parent));
				if (scene->hasObject(GKB_IDNAME(ob)))
					scene->getObject(GKB_IDNAME(ob))->setParent(parob);
			}
		}

		// TODO DupliGroups

	}

	scene->_loadManual();
}


// ----------------------------------------------------------------------------
void gkSceneObjectLoader::loadObject(gkSceneObject *current, Blender::Object *ob)
{
	/// only create if needed
	if (!current->hasObject(GKB_IDNAME(ob)))
	{
		gkGameObject *gob= 0;
		switch (ob->type)
		{
		case OB_EMPTY:
			{
				gkGameObjectLoader *loader= new gkGameObjectLoader(mFile, mScene, ob);
				mFile->_registerLoader(loader);
				gob= current->createObject(GKB_IDNAME(ob), loader);
			}
			break;
		case OB_LAMP:
			{
				gkGameObjectLoader *loader= new gkGameObjectLoader(mFile, mScene, ob);
				mFile->_registerLoader(loader);
				gob= current->createLight(GKB_IDNAME(ob), loader);
			}
			break;
		case OB_CAMERA:
			{
				gkGameObjectLoader *loader= new gkGameObjectLoader(mFile, mScene,  ob);
				mFile->_registerLoader(loader);
				gob= current->createCamera(GKB_IDNAME(ob), loader);
			}
			break;
		case OB_MESH:
			{
				gkGameObjectLoader *loader= new gkGameObjectLoader(mFile, mScene, ob);
				mFile->_registerLoader(loader);
				gob= current->createEntity(GKB_IDNAME(ob), loader);

				gkMeshLoader *client= new gkMeshLoader(mFile, ob);
				mFile->_registerLoader(client);
				current->createMesh(GKB_IDNAME(ob), client);
			}
			break;
		case OB_ARMATURE:
			{
				gkGameObjectLoader *loader= new gkGameObjectLoader(mFile,  mScene, ob);
				mFile->_registerLoader(loader);
				gob= current->createObject(GKB_IDNAME(ob), loader);

				gkSkeletonLoader *client= new gkSkeletonLoader(mFile, ob);
				mFile->_registerLoader(client);
				current->createSkeleton(GKB_IDNAME(ob), client);
			}
			break;
		}

		if (gob != 0)
			gob->setActiveLayer((mScene->lay & ob->lay) != 0);
	}
}
