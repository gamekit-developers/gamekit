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
#include "gkGameObjectGroup.h"
#include "gkGameObject.h"
#include "gkEntity.h"
#include "OgreEntity.h"
#include "OgreStaticGeometry.h"
#include "OgreSceneManager.h"


gkGameObjectInstance::gkGameObjectInstance(gkGameObject *owner, gkGameObjectGroup *group, UTsize uid)
    :   gkObject(group->getName().str()), m_owner(owner), m_parent(group), m_handle(uid)
{
}


gkGameObjectInstance::~gkGameObjectInstance()
{
    // free all objects

    utHashTableIterator<InstanceObjects> iter(m_objects);
    while (iter.hasMoreElements())
    {
        gkGameObject *ob = iter.getNext().second;
        ob->unload();
        delete ob;
    }

    m_objects.clear();
}


gkGameObject *gkGameObjectInstance::getObject(const gkHashedString &name)
{
    UTsize pos;
    if ((pos = m_objects.find(name)) == UT_NPOS)
    {
        // ignore
        return 0;
    }

    return m_objects.at(pos);
}


void gkGameObjectInstance::unloadObject(const gkHashedString &name)
{
    UTsize pos;
    if ((pos = m_objects.find(name)) == UT_NPOS)
    {
        // ignore
        return;
    }
    gkGameObject* ob= m_objects.at(pos);
    ob->unload();
}


void gkGameObjectInstance::removeObject(const gkHashedString &name)
{
    UTsize pos;
    if ((pos = m_objects.find(name)) == UT_NPOS)
    {
        // ignore
        return;
    }


    // we own this object, so destroy 
    gkGameObject* ob= m_objects.at(pos);
    ob->unload();
    m_objects.remove(name);
    delete ob;

    if (m_objects.empty())
        m_objects.clear();
}


void gkGameObjectInstance::addObject(gkGameObject *v)
{
    gkHashedString name = m_parent->getName().str() + "/" + v->getName() + "/Handle_" +  Ogre::StringConverter::toString(m_handle);

    // ignore
    if (m_objects.find(name) != UT_NPOS)
        return;

    gkGameObject *ob = static_cast<gkGameObject*>(v->clone(name.str()));
    ob->attachToGroupInstance(this);
    ob->setActiveLayer(m_owner->isInActiveLayer());

    gkGameObjectProperties &props = ob->getProperties();


    gkMatrix4 c = props.m_transform.toMatrix();
    gkMatrix4 p = m_owner->getProperties().m_transform.toMatrix();

    
    if (ob->getParent() == 0)
    {
        c = p * c;
        gkMathUtils::extractTransform(c, props.m_transform.loc, props.m_transform.rot, props.m_transform.scl);
    }
    else
    {
        c = ob->getParent()->getProperties().m_transform.toMatrix().inverse() * c;
        gkMathUtils::extractTransform(c, props.m_transform.loc, props.m_transform.rot, props.m_transform.scl);
    }


    m_objects.insert(name, ob);
}


void gkGameObjectInstance::loadImpl(void)
{
    // call load on all objects

    if (!m_owner->isInActiveLayer())
        return;


    utHashTableIterator<InstanceObjects> iter(m_objects);
    while (iter.hasMoreElements())
        iter.getNext().second->load();
}

void gkGameObjectInstance::unloadImpl(void)
{
    // call unload on all objects
    utHashTableIterator<InstanceObjects> iter(m_objects);
    while (iter.hasMoreElements())
        iter.getNext().second->unload();
}



gkGameObjectGroup::gkGameObjectGroup(const gkHashedString &name) 
    :   m_name(name), m_handle(0), m_geom(0)
{
}

gkGameObjectGroup::~gkGameObjectGroup()
{
    clear();
}



void gkGameObjectGroup::addObject(gkGameObject *v)
{
     if (!v) return;

    gkHashedString name = v->getName();

    if (m_internal.find(name) != UT_NPOS)
    {
        // ignore duplicates
        return;
    }


    v->attachToGroup(this);
    m_internal.insert(name, v);
}



// destroy all instances
void gkGameObjectGroup::clear(void)
{
    gkGroupInstanceIterator iter = gkGroupInstanceIterator(m_instances);
    while (iter.hasMoreElements())
        delete iter.getNext();

    m_instances.clear();
}


// notify that the owning object to
// this group has been unloaded / destroyed
void gkGameObjectGroup::notifyObject(gkGameObject *ob)
{
    gkHashedString name = ob->getName();
    if (m_internal.find(name) != UT_NPOS)
    {
        m_internal.remove(name);
        if (m_internal.empty()) 
            m_internal.clear();


        // find object in instance
        gkGroupInstanceIterator iter = gkGroupInstanceIterator(m_instances);
        while (iter.hasMoreElements())
        {
            gkGameObjectInstance *inst = iter.getNext();

            if (inst->hasObject(name))
            {
                inst->removeObject(name);
                break;
            }
        }
    }
}


gkGameObjectInstance* gkGameObjectGroup::createInstance(gkGameObject *instPar)
{
    gkGameObjectInstance *newInst = new gkGameObjectInstance(instPar, this, m_handle++);
    m_instances.push_back(newInst);


    // push new objects
    utHashTableIterator<Objects> iter(m_internal);
    while (iter.hasMoreElements())
        newInst->addObject(iter.getNext().second);

    return newInst;
}


// build instanced geometry.
void gkGameObjectGroup::build(Ogre::SceneManager *mgr)
{
    if (m_geom)
    {
        mgr->destroyStaticGeometry(m_geom);
    }
    m_geom = 0;

    gkGroupInstanceIterator iter = gkGroupInstanceIterator(m_instances);
    while (iter.hasMoreElements())
    {
        gkGameObjectInstance *inst = iter.getNext();

        gkGameObjectInstance::InstanceObjectIterator instIt = inst->getObjectIterator();

        while (instIt.hasMoreElements())
        {
            gkGameObject *obj = instIt.getNext().second;
            obj->load();


            if (obj->getType()==GK_ENTITY)
            {
                const gkGameObjectProperties& props = obj->getProperties();

                if (!props.isPhysicsObject())
                {
                    gkEntity *ent = obj->getEntity();

                    if (!m_geom)
                        m_geom = mgr->createStaticGeometry(m_name.str());

                    m_geom->addEntity(ent->getEntity(), obj->getWorldPosition(), 
                                                        obj->getWorldOrientation(), 
                                                        obj->getWorldScale());
                    // no longer needed
                    ent->unload();
                }
            }
        }
    }

    if (m_geom)
    {
        m_geom->build();
        m_geom->setCastShadows(false);
    }

}


