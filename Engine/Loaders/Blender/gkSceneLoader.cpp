/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): Nestor Silveira.
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
#include <sstream>
#include "OgreTextureManager.h"
#include "OgreMaterialManager.h"
#include "OgreMeshManager.h"
#include "OgreSkeletonManager.h"
#include "OgreMesh.h"
#include "OgreSkeleton.h"
#include "OgreTexture.h"
#include "OgreMaterial.h"
#include "OgreTechnique.h"
#include "OgrePass.h"

#include "gkSceneLoader.h"
#include "gkScene.h"
#include "gkBlendFile.h"
#include "gkEngine.h"
#include "gkUserDefs.h"
#include "gkBlenderDefines.h"
#include "gkSceneManager.h"
#include "gkObjectLoader.h"
#include "gkGameObject.h"
#include "gkLight.h"
#include "gkCamera.h"
#include "gkEntity.h"
#include "gkSkeleton.h"
#include "gkMeshLoader.h"
#include "gkSkeletonLoader.h"
#include "gkLoaderUtils.h"
#include "gkBlenderSkyBox.h"
#include "gkLogicLoader.h"
#include "gkLogicManager.h"

#include "gkDynamicsWorld.h"
#include "gkRigidBody.h"
#include "btBulletDynamicsCommon.h"

#include "bBlenderFile.h"
#include "bMain.h"

#include "blender.h"


gkSceneObjectLoader::gkSceneObjectLoader(gkBlendFile *fp, Blender::Scene *scene)
:       m_file(fp), m_scene(scene), m_grad(0)
{
    GK_ASSERT(m_file && m_scene);
}



gkSceneObjectLoader::~gkSceneObjectLoader()
{
    delete m_grad;
    m_grad = 0;
}


void gkSceneObjectLoader::load(gkObject* baseClass)
{
    GK_ASSERT(m_file && m_scene && baseClass);

    // convert/ load objects
    gkScene *scene = static_cast<gkScene*>(baseClass);



    gkSceneProperties& props = scene->getProperties();

    // Apply user defined animation fps
    gkLoaderUtils::blender_anim_rate = m_scene->r.frs_sec;
    gkEngine::getSingleton().getUserDefs().animspeed = gkLoaderUtils::blender_anim_rate;


    if (m_scene->world)
    {
        Blender::World *world = m_scene->world;

        props.gravity = gkVector3(0.f, 0.f, -world->gravity);
        props.ambient.r = world->ambr;
        props.ambient.g = world->ambg;
        props.ambient.b = world->ambb;

        props.world_color.r = world->horr;
        props.world_color.g = world->horg;
        props.world_color.b = world->horb;


		if ((world->skytype & WO_SKYBLEND) || (world->skytype & WO_SKYREAL))
		{
			// multi stop gradients are not implemented for now
			// so WO_SKYREAL is WO_SKYBLEND

            
            // take half far distance of the main camera
            props.skyDist = m_scene->camera && m_scene->camera->type == OB_CAMERA ?
                ((Blender::Camera*)m_scene->camera->data)->clipend * 0.5 : 10000.f;

            props.skyMat = GKB_IDNAME(world);
            loadSkyBox();
		}

    }

    // create the physics world
    if (scene->getDynamicsWorld() == 0)
        scene->createWorld();


    // clear on reload
    gkLogicManager::getSingleton().clear();
    gkLogicLoader gameLogicLoader;


    for (Blender::Base *base = (Blender::Base*)m_scene->base.first; base; base = base->next)
    {
        if (!base->object) continue;


        Blender::Object *ob = base->object;

        // since object adding from inactive layers
        // is not implementd yet, dont convert
        if (!(ob->lay & m_scene->lay)) continue;


        loadObject(gameLogicLoader, scene, base->object);


        if (ob->parent)
        {
            if (scene->hasObject(GKB_IDNAME(ob->parent)))
            {
                gkGameObject *parob = scene->getObject(GKB_IDNAME(ob->parent));
                if (scene->hasObject(GKB_IDNAME(ob)))
                    scene->getObject(GKB_IDNAME(ob))->setParent(parob);
            }
        }
        // TODO DupliGroups
    }

    // resolve crosslinks
    gameLogicLoader.resolveLinks();

}

void gkSceneObjectLoader::loadSkyBox()
{
	Blender::World *wo = m_scene->world;
    Ogre::MaterialPtr matptr = Ogre::MaterialManager::getSingleton().create(wo->id.name + 2, m_file->getGroup());

    if (m_grad)
        delete m_grad;

    m_grad = new gkSkyBoxGradient(wo);

    Ogre::TexturePtr txptr =  Ogre::TextureManager::getSingleton().create(wo->id.name + 2, m_file->getGroup(), true, m_grad);
	matptr->setLightingEnabled(false);

    Ogre::TextureUnitState *tus = matptr->getTechnique(0)->getPass(0)->createTextureUnitState();
	tus->setTextureName(wo->id.name + 2, Ogre::TEX_TYPE_CUBE_MAP);
	tus->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);

}


void gkSceneObjectLoader::loadObject(gkLogicLoader &gameLogicLoader, gkScene *current, Blender::Object *ob)
{
    gkHashedString str = GKB_IDNAME(ob);

    // only create if needed
    if (!current->hasObject(str))
    {
        gkGameObject *gob = 0;
        switch (ob->type)
        {
        case OB_EMPTY:
            gob = current->createObject(str, new gkGameObjectLoader(m_file, m_scene, ob));
            break;
        case OB_LAMP:
            gob = current->createLight(str, new gkGameObjectLoader(m_file, m_scene, ob));
            break;
        case OB_CAMERA:
            gob = current->createCamera(str, new gkGameObjectLoader(m_file, m_scene,  ob));
            break;
        case OB_MESH:
            {
                gob = current->createEntity(str, new gkGameObjectLoader(m_file, m_scene, ob));

                gkMeshLoader *client = new gkMeshLoader(m_file, ob);
                m_file->_registerLoader(client);

                // Create the destination Ogre mesh
                Ogre::MeshManager::getSingleton().create(GKB_IDNAME(ob), m_file->getGroup(), true, client);
            }
            break;
        case OB_ARMATURE:
            {
                gob = current->createSkeleton(str, new gkGameObjectLoader(m_file,  m_scene, ob));


                gkSkeletonLoader *client = new gkSkeletonLoader(m_file, ob);
                m_file->_registerLoader(client);

                // Create the destination Ogre skeleton
                Ogre::SkeletonManager::getSingleton().create(GKB_IDNAME(ob), m_file->getGroup(), true, client);
            }
            break;
        }

        if (gob != 0)
        {
            // for visibility
            gob->setActiveLayer((m_scene->lay & ob->lay) != 0);

        }
    }

    if (current->hasObject(str))
    {
        // apply physics  
        createRigidBody(current->getDynamicsWorld(), current->getObject(str), ob);
        gameLogicLoader.convertObject(ob, current->getObject(str));
    }
}


void gkSceneObjectLoader::createRigidBody(gkDynamicsWorld *dyn, gkGameObject *obj, Blender::Object *bobj)
{
    GK_ASSERT(dyn && obj && bobj);


    if (bobj->type != OB_MESH)
    {
        // exclude non actors if it's not a mesh
        if (!(bobj->gameflag & OB_ACTOR))
            return;
    }
    else 
    {
         Blender::Mesh *me = (Blender::Mesh*)bobj->data;
         if (!me->mface) 
             return;
    }

    if (bobj->gameflag & OB_GHOST)
    {
        dyn->createCharacter(obj, new gkCharacterLoader(m_file, m_scene, bobj, obj));
    }
	else
	{
	    dyn->createRigidBody(obj, new gkRigidBodyLoader(m_file, m_scene, bobj, obj));
	}
}
