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
#include "OgreRoot.h"
#include "OgreSceneManager.h"
#include "OgreRenderWindow.h"
#include "OgreViewport.h"
#include "OgreStringConverter.h"

#include "gkWindowSystem.h"
#include "gkCamera.h"
#include "gkEntity.h"
#include "gkLight.h"
#include "gkSkeleton.h"
#include "gkGameObjectGroup.h"
#include "gkEngine.h"
#include "gkScene.h"
#include "gkLogicManager.h"
#include "gkNodeManager.h"
#include "gkLuaManager.h"
#include "gkLogger.h"
#include "gkDynamicsWorld.h"
#include "gkRigidBody.h"
#include "gkUserDefs.h"
#include "gkDebugger.h"
#include "gkMeshManager.h"

using namespace Ogre;


gkScene::gkScene(const gkString &name)
    :       gkObject(name), 
            m_manager(0), 
            m_startCam(0),
            m_viewport(0), 
            m_baseProps(), 
            m_physicsWorld(0), 
            m_debugger(0),
            m_hasLights(false),
            m_markDBVT(false)
{
}

gkScene::~gkScene()
{
    gkGameObjectHashMapIterator it(m_objects);
    while (it.hasMoreElements())
        delete it.getNext().second;
    destroyGroups();

    gkMeshManager::getSingleton().destroyAll();
}


bool gkScene::hasObject(const gkHashedString &ob)
{
    return m_objects.find(ob) != GK_NPOS;
}

bool gkScene::hasObject(gkGameObject *ob)
{
    GK_ASSERT(ob);
    return m_objects.find(ob->getName()) != GK_NPOS;
}


gkGameObject *gkScene::getObject(const gkHashedString &name)
{
    UTsize pos = m_objects.find(name);
    if (pos != GK_NPOS)
        return m_objects.at(pos);
    return 0;
}

bool gkScene::hasMesh(const gkHashedString& ob)
{
    return gkMeshManager::getSingleton().hasMesh(ob);
}

gkMesh* gkScene::getMesh(const gkHashedString& name)
{
    return gkMeshManager::getSingleton().getMesh(name);
}

gkMesh* gkScene::createMesh(const gkHashedString &name)
{
    return gkMeshManager::getSingleton().create(name);
}


gkGameObjectGroup *gkScene::createGroup(const gkHashedString &name)
{
    // empty group

    if (m_groups.find(name) != UT_NPOS)
    {
        gkPrintf("duplicate group '%s' found\n", name.str().c_str());
        return 0;
    }

    gkGameObjectGroup *group = new gkGameObjectGroup(name);
    m_groups.insert(name, group);
    return group;
}

gkGameObjectGroup *gkScene::getGroup(const gkHashedString &name)
{
    UTsize pos;
    if ( (pos = m_groups.find(name)) == UT_NPOS)
    {
        gkPrintf("mossing group '%s'\n", name.str().c_str());
        return 0;
    }
    return m_groups.at(pos);

}

void gkScene::destroyGroup(const gkHashedString &name)
{
    destroyGroup(getGroup(name));
}


void gkScene::destroyGroup(gkGameObjectGroup *group)
{
    if (group && hasGroup(group->getName()))
    {
        m_groups.remove(group->getName());
        delete group;

        if (m_groups.empty())
            m_groups.clear();
    }
}

void gkScene::destroyGroups(void)
{
    gkGroupTableIterator it(m_groups);
    while (it.hasMoreElements())
        delete it.getNext().second;

    m_groups.clear();
}


gkGameObject *gkScene::createObject(const gkHashedString &name)
{
    if (m_objects.find(name) != GK_NPOS)
    {
        gkPrintf("duplicate object '%s' found\n", name.str().c_str());
        return 0;
    }

    gkGameObject *ob = new gkGameObject(this, name.str(), GK_OBJECT);
    m_objects.insert(name, ob);
    return ob;
}


gkLight *gkScene::createLight(const gkHashedString &name)
{
    if (m_objects.find(name) != GK_NPOS)
    {
        gkPrintf("duplicate object '%s' found\n", name.str().c_str());
        return 0;
    }

    gkLight *ob = new gkLight(this, name.str());
    m_objects.insert(name, ob);
    m_hasLights = true;
    return ob;
}


gkCamera *gkScene::createCamera(const gkHashedString &name)
{
    if (m_objects.find(name) != GK_NPOS)
    {
        gkPrintf("duplicate object '%s' found\n", name.str().c_str());
        return 0;
    }

    gkHashedString nameHash = name;
    gkCamera *ob = new gkCamera(this, name.str());

    m_objects.insert(nameHash, ob);
    return ob;
}

gkEntity *gkScene::createEntity(const gkHashedString &name)
{
    if (m_objects.find(name) != GK_NPOS)
    {
        gkPrintf("duplicate object '%s' found\n", name.str().c_str());
        return 0;
    }

    gkEntity *ob = new gkEntity(this, name.str());
    m_objects.insert(name, ob);
    return ob;
}

gkSkeleton *gkScene::createSkeleton(const gkHashedString &name)
{
    if (m_objects.find(name) != GK_NPOS)
    {
        gkPrintf("duplicate object '%s' found\n", name.str().c_str());
        return 0;
    }

    gkSkeleton *ob = new gkSkeleton(this, name.str());
    m_objects.insert(name, ob);
    return ob;
}


gkDebugger *gkScene::getDebugger(void)
{
    if (isLoaded() && !m_debugger)
        m_debugger = new gkDebugger(this);
    return m_debugger;
}


gkGameObjectInstance *gkScene::createInstance(gkGameObject *owner, gkGameObjectGroup *group)
{
    gkGameObjectInstance *inst = group->createInstance(owner);
    m_instances.push_back(inst);
    return inst;
}


void gkScene::applyViewport(Viewport *vp)
{
    const gkVector2 &size = gkWindowSystem::getSingleton().getMouse()->winsize;
    vp->setDimensions(0, 0, 1, 1);

    Camera *c = vp->getCamera();
    if (c != 0) c->setAspectRatio(size.x / size.y);
}

void gkScene::setMainCamera(gkCamera *cam)
{
    if (!m_startCam)
        return;

    m_startCam = cam;
    Camera *main = m_startCam->getCamera();

    GK_ASSERT(main);

    if (!m_viewport)
    {
        // create a new viewport
        RenderWindow *window = gkWindowSystem::getSingleton().getMainWindow();
        if (window)
            m_viewport = window->addViewport(main);
    }
    else
        m_viewport->setCamera(main);

    GK_ASSERT(m_viewport);
    applyViewport(m_viewport);
}


void gkScene::loadImpl(void)
{
 
    if (m_objects.empty())
    {
        gkPrintf("no objects in scene '%s' to load\n", m_name.c_str());
        return;
    }


    // generic for now, but later scene properties will be used
    // to extract more detailed management information

    m_manager = Root::getSingleton().createSceneManager(ST_GENERIC, m_name);
    if (!m_baseProps.m_skyMat.empty())
        m_manager->setSkyBox(true, m_baseProps.m_skyMat, m_baseProps.m_skyDist, true, m_baseProps.m_skyOri);


    // create the world
    m_physicsWorld = new gkDynamicsWorld(m_name, this);


    gkGameObjectHashMapIterator it(m_objects);
    while (it.hasMoreElements())
    {
        gkGameObject *obptr = it.getNext().second;

        // Skip load of inactive layers
        if (!obptr->isLoaded() && obptr->isInActiveLayer() && !obptr->isGroupOwner())
        {
            // If there is one camera in the scene use it.
            // The start camera may not have been specified.
            if (!m_startCam && obptr->getType() == GK_CAMERA)
                m_startCam = (gkCamera *)obptr;

            // call builder
            obptr->load();

            if(obptr->getProperties().isStatic())
            {
                if (obptr->getAttachedObject())
                    m_Limits.merge(obptr->getAttachedObject()->getAabb());
            }

        }
    }

    if (!m_instances.empty())
    {
        gkGroupInstanceIterator instit(m_instances);
        while (instit.hasMoreElements())
            instit.getNext()->load();

        if (0)//gkEngine::getSingleton().getUserDefs().buildInstances)
        {
            gkGroupTableIterator instGeom(m_groups);
            while (instGeom.hasMoreElements())
                instGeom.getNext().second->build(m_manager);
        }
    }

    if (!m_viewport)
    {
        // setMainCamera has not been called, try to call
        if (m_startCam)
            setMainCamera(m_startCam);
        else
        {
            m_startCam = createCamera(" -- No Camera -- ");

            m_startCam->getProperties().m_transform.set(
                gkVector3(0, -5, 0),
                gkEuler(90.f, 0.f, 0.f).toQuaternion(),
                gkVector3(1.f, 1.f, 1.f)
            );
            m_startCam->load();
            setMainCamera(m_startCam);
        }
    }


    GK_ASSERT(m_viewport);

    m_viewport->setBackgroundColour(m_baseProps.m_world);
    m_manager->setAmbientLight(m_baseProps.m_ambient);

    if (m_baseProps.m_fog.m_mode != gkFogParams::FM_NONE)
    {
        m_manager->setFog(  m_baseProps.m_fog.m_mode == gkFogParams::FM_QUAD ?  Ogre::FOG_EXP2 :
                            m_baseProps.m_fog.m_mode == gkFogParams::FM_LIN  ?  Ogre::FOG_LINEAR :
                            Ogre::FOG_EXP,
                            m_baseProps.m_fog.m_color,
                            m_baseProps.m_fog.m_intensity, 
                            m_baseProps.m_fog.m_start,
                            m_baseProps.m_fog.m_end);
    }

    //Enable Shadows
    setShadows();

    // notify main scene
    gkEngine::getSingleton().setActiveScene(this);

    if(gkNavMeshData::getSingletonPtr())
        gkNavMeshData::getSingletonPtr()->createNavigationMesh();
}


void gkScene::postLoadImpl(void)
{
    gkLuaScript *script = gkLuaManager::getSingleton().getScript("OnLoad.lua");

    // load user application
    if (script)
        script->execute();
}


void gkScene::unloadImpl()
{
    if (m_objects.empty())
        return;

    if(gkNavMeshData::getSingletonPtr())
        gkNavMeshData::getSingletonPtr()->unloadAll();

    // free scripts
    gkLuaManager::getSingleton().unload();

    // unload instances
    if (!m_instances.empty())
    {
        gkGroupInstanceIterator instit(m_instances);
        while (instit.hasMoreElements())
            instit.getNext()->unload();
    }

    // free cloned
    destroyClones();
    endObjects();

    gkGameObjectHashMapIterator it(m_objects);
    while (it.hasMoreElements())
    {
        gkGameObject *obptr = it.getNext().second;

        if (obptr->isLoaded())
            obptr->unload();
    }

    if (m_physicsWorld)
    {
        delete m_physicsWorld;
        m_physicsWorld = 0;
    }

    if (m_debugger)
    {
        delete m_debugger;
        m_debugger = 0;
    }

    m_startCam = 0;

    m_Limits = AxisAlignedBox::BOX_NULL;

    // clear ogre scene manager
    if (m_manager)
    {
        m_manager->destroyAllStaticGeometry();
        Root::getSingleton().destroySceneManager(m_manager);
    }

    m_manager = 0;

    // finalize vp
    if (m_viewport)
    {

        RenderWindow *window = gkWindowSystem::getSingleton().getMainWindow();
        if (window)
        {
            window->removeViewport(0);
            m_viewport = 0;
        }
    }

    gkEngine::getSingleton().setActiveScene(0);
}


static Ogre::ShadowTechnique ParseShadowTechnique(const gkString &technique)
{
    gkString techniqueLower = technique;
    Ogre::StringUtil::toLowerCase(techniqueLower);

    if (techniqueLower == "none")
        return Ogre::SHADOWTYPE_NONE;
    else if (techniqueLower == "stencilmodulative")
        return Ogre::SHADOWTYPE_STENCIL_MODULATIVE;
    else if (techniqueLower == "stenciladditive")
        return Ogre::SHADOWTYPE_STENCIL_ADDITIVE;
    else if (techniqueLower == "texturemodulative")
        return Ogre::SHADOWTYPE_TEXTURE_MODULATIVE;
    else if (techniqueLower == "textureadditive")
        return Ogre::SHADOWTYPE_TEXTURE_ADDITIVE;
    else if (techniqueLower == "texturemodulativeintegrated")
        return Ogre::SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED;
    else if (techniqueLower == "textureadditiveintegrated")
        return Ogre::SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED;

    Ogre::StringUtil::StrStreamType errorMessage;
    errorMessage << "Invalid shadow technique specified: " << technique;

    OGRE_EXCEPT
    (
        Ogre::Exception::ERR_INVALIDPARAMS,
        errorMessage.str(),
        "ParseShadowTechnique"
    );
}


void gkScene::setShadows()
{
    gkUserDefs &defs = gkEngine::getSingleton().getUserDefs();

    if(defs.enableshadows)
    {
        Ogre::ShadowTechnique shadowTechnique = ::ParseShadowTechnique(defs.shadowtechnique);

        m_manager->setShadowTechnique(shadowTechnique);

        m_manager->setShadowColour(defs.colourshadow);

        m_manager->setShadowFarDistance(defs.fardistanceshadow);
    }
}

void gkScene::notifyObjectLoaded(gkGameObject *gobject)
{
    std::pair<gkGameObjectSet::iterator, bool> result = m_loadedObjects.insert(gobject);

    GK_ASSERT(result.second);

    if(gkNavMeshData::getSingletonPtr())
        gkNavMeshData::getSingletonPtr()->updateOrLoad(gobject);

    // add to constraints
    if (gobject->hasConstraints())
        m_constraintObjects.push_back(gobject);
}

void gkScene::notifyObjectUnloaded(gkGameObject *gobject)
{
    m_loadedObjects.erase(gobject);

    if (m_transformObjects.find(gobject))
        m_transformObjects.erase(gobject);

    if(gkNavMeshData::getSingletonPtr())
        gkNavMeshData::getSingletonPtr()->unload(gobject);

    if (gobject->hasConstraints())
        m_constraintObjects.erase(gobject);
}

void gkScene::notifyObjectUpdate(gkGameObject *gobject)
{
    m_markDBVT = true;

    if(gkNavMeshData::getSingletonPtr())
        gkNavMeshData::getSingletonPtr()->updateOrLoad(gobject);
}

void gkScene::synchronizeMotion(gkScalar blend)
{

    if (!m_transformObjects.empty())
    {
        gkGameObjectArray::Pointer buffer = m_transformObjects.ptr();
        size_t size = m_transformObjects.size();
        size_t i = 0;
        while (i < size)
            (buffer[i++])->blendTransform(blend);

        m_transformObjects.clear(true);
    }

}

void gkScene::applyConstraints(void)
{
    if (!m_constraintObjects.empty())
    {
        gkGameObjectArray::Pointer buffer = m_constraintObjects.ptr();
        size_t size = m_constraintObjects.size();
        size_t i = 0;
        while (i < size)
            (buffer[i++])->applyConstraints();
    }
}

void gkScene::destroyClones(void)
{
    if (!m_clones.empty())
    {
        gkGameObjectArray::Pointer buffer = m_clones.ptr();
        size_t size = m_clones.size();
        size_t i = 0;
        while (i < size)
        {
            gkGameObject *obj = buffer[i++];
            obj->unload();
            delete obj;
        }

        m_clones.clear();
    }
    if (!m_tickClones.empty())
    {
        gkGameObjectArray::Pointer buffer = m_tickClones.ptr();
        size_t size = m_tickClones.size();
        size_t i = 0;
        while (i < size)
        {
            gkGameObject *obj = buffer[i++];
            obj->unload();
            delete obj;
        }

        m_tickClones.clear();
    }
}


void gkScene::tickClones(void)
{
    if (!m_tickClones.empty())
    {
        gkGameObjectArray removed;

        gkGameObjectArray::Pointer buffer = m_tickClones.ptr();
        UTsize size = m_tickClones.size();
        UTsize i = 0;
        while (i < size)
        {
            gkGameObject *obj = buffer[i++];

            gkGameObject::LifeSpan &life = obj->getLifeSpan();

            if ((life.tick++) > life.timeToLive)
                removed.push_back(obj);
        }

        if (!removed.empty())
        {
            buffer = removed.ptr();
            size = removed.size();
            i = 0;

            while (i < size)
            {
                gkGameObject *obj = buffer[i++];
                m_tickClones.erase(obj);

                obj->unload();
                delete obj;
            }

            if (m_tickClones.empty())
                m_tickClones.clear(true);
        }

    }
}

gkGameObject* gkScene::cloneObject(gkGameObject *obj, int lifeSpan)
{
    static int cloneNr = 0;

    char buf[72];
    sprintf(buf, "%s:Clone #%i", obj->getName().c_str(), cloneNr++);

    gkGameObject *nobj = (gkGameObject *)obj->clone(gkString(buf));
    nobj->setActiveLayer(true);

    gkGameObject::LifeSpan life = {0, lifeSpan};
    nobj->setLifeSpan(life);

    if (lifeSpan > 0)
        m_tickClones.push_back(nobj);
    else
        m_clones.push_back(nobj);

    return nobj;

}


void gkScene::endObject(gkGameObject *obj)
{
    m_endObjects.push_back(obj);
}


void gkScene::endObjects(void)
{
    if (!m_endObjects.empty())
    {
        gkGameObjectArray::Pointer buffer = m_endObjects.ptr();
        UTsize size = m_endObjects.size();
        UTsize i = 0;
        while (i < size)
        {
            gkGameObject *obj = buffer[i++];

            if (obj->isClone())
            {
                UTsize fnd = m_clones.find(obj);
                if (fnd != UT_NPOS)
                {
                    m_clones.erase(obj);
                    obj->unload();
                    delete obj;
                }
                else
                {
                    fnd = m_tickClones.find(obj);
                    if (fnd != UT_NPOS)
                    {
                        m_tickClones.erase(obj);
                        obj->unload();
                        delete obj;
                    }
                    else
                    {
                        gkGameObjectInstance *inst = obj->getGroupInstance();
                        //dsPrintf("Del Object %s : Inst ? %p", obj->getName().c_str(), inst);
                        if (inst != 0)
                            inst->unloadObject(obj->getName());
                        else
                            obj->unload();
                    }
                }
            }
            else
            {
                gkGameObjectInstance *inst = obj->getGroupInstance();
                //dsPrintf("Del Object %s : Inst ? %p", obj->getName(), inst);
                if (inst != 0)
                    inst->unloadObject(obj->getName());
                else
                    obj->unload();
            }
        }

        m_endObjects.clear(true);
    }

}


void gkScene::update(gkScalar tickRate)
{
    if (!isLoaded())
        return;

    GK_ASSERT(m_physicsWorld);

    // update simulation
    m_physicsWorld->step(tickRate);

    // update logic bricks
    gkLogicManager::getSingleton().update(tickRate);

    // update node trees
    gkNodeManager::getSingleton().update(tickRate);

    applyConstraints();

    // tick life span
    tickClones();
    endObjects();

    if (m_markDBVT)
    {
        m_markDBVT = false;
        m_physicsWorld->handleDbvt(m_startCam);
    }

    if (m_debugger)
    {
        m_physicsWorld->DrawDebug();
        m_debugger->flush();
    }
}
