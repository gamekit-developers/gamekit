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
#include "OgreEntity.h"
#include "OgreBone.h"
#include "OgreSkeleton.h"
#include "OgreSkeletonInstance.h"

#include "gkSkeleton.h"
#include "gkAction.h"
#include "gkLogger.h"



// Manual Bone 


static gkTransformState ManualBone_GetConstMatrix(Ogre::Bone *bone)
{
    GK_ASSERT(bone);
    gkTransformState r;
    r.loc = bone->getInitialPosition();
    r.rot = bone->getInitialOrientation();
    r.scl = bone->getInitialScale();
    return r;
}

gkBone::gkBone(const gkString& name, Ogre::Bone *bone) 
:       m_name(name), m_bone(bone), m_bind(ManualBone_GetConstMatrix(bone)), m_parent(0)
{
    m_pose.setIdentity();
}

gkBone::~gkBone()
{
}



// Skeleton 


gkSkeleton::gkSkeleton(gkScene *scene, const gkString& name, gkObject::Loader* loader) 
:       gkGameObject(scene, name, GK_SKELETON, loader)
{
}


gkSkeleton::~gkSkeleton()
{
    utHashTableIterator<Actions> iter(m_actions);
    while (iter.hasMoreElements())
        delete iter.getNext().second;

    utHashTableIterator<Bones> biter(m_bones);
    while (biter.hasMoreElements())
        delete biter.getNext().second;
}


void gkSkeleton::loadImpl(void)
{
    gkGameObject::loadImpl();
}


void gkSkeleton::unloadImpl(void)
{
    gkGameObject::unloadImpl();
}


void gkSkeleton::setEntity(Ogre::Entity *ent)
{
    if (ent->hasSkeleton())
    {
        Ogre::SkeletonInstance *inst = ent->getSkeleton();

        if (!m_boneList.empty())
        {
            gkBone *bone = m_boneList.begin();

            while (bone)
            {
                if (inst->hasBone(bone->m_name))
                {
                    bone->m_bone = inst->getBone(bone->m_name);
                    bone->m_bone->setManuallyControlled(true);
                }
                bone = bone->getNext();
            }
        }
    }

}


gkBone* gkSkeleton::createBone(Ogre::Bone *bone)
{
    if (hasBone(bone->getName()))
        return 0;

    gkBone *manual = new gkBone(bone->getName(), bone);
    bone->setManuallyControlled(true);
    m_bones.insert(bone->getName(), manual);


    if (bone->getParent())
        manual->m_parent = getBone(bone->getParent()->getName());

    m_boneList.push_back(manual);
    return manual;
}

gkAction* gkSkeleton::createAction(const gkHashedString& name)
{
    if (m_actions.find(name) != GK_NPOS)
    {
        gkPrintf("Duplicate action '%s'\n", name.str().c_str());
        return 0;
    }

    gkAction *act = new gkAction(name.str());
    m_actions.insert(act->getName(), act);
    return act;
}


gkAction* gkSkeleton::getAction(const gkHashedString& name)
{
    size_t pos;
    if ((pos = m_actions.find(name)) == GK_NPOS)
        return 0;

    return m_actions.at(pos);
}


gkBone* gkSkeleton::getBone(const gkHashedString& name)
{
    size_t pos;
    if ((pos = m_bones.find(name)) == GK_NPOS)
        return 0;
    return m_bones.at(pos);
}


gkObject *gkSkeleton::clone(const gkString &name)
{
    // will need to set other properties in a bit!
    return new gkSkeleton(m_scene, name, m_manual ? m_manual->clone() : 0);
}

