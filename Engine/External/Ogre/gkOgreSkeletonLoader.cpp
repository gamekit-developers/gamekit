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
#include "gkOgreSkeletonLoader.h"
#include "gkEntity.h"
#include "OgreSkeleton.h"
#include "OgreSkeletonInstance.h"
#include "OgreSkeletonManager.h"
#include "OgreSkeleton.h"
#include "OgreBone.h"
#include "OgreEntity.h"


gkSkeletonLoader::gkSkeletonLoader(gkSkeletonResource* skel)
	:   m_skeleton(skel)
{
	Ogre::SkeletonManager& mgr = Ogre::SkeletonManager::getSingleton();
	const gkString& name = m_skeleton->getResourceName().getName();



	Ogre::SkeletonPtr oskel = mgr.getByName(name);
	if (oskel.isNull())
		oskel = mgr.create(name, skel->getGroupName(), true, this); //GK_BUILTIN_GROUP
}



gkSkeletonLoader::~gkSkeletonLoader()
{
	Ogre::SkeletonManager& mgr = Ogre::SkeletonManager::getSingleton();
	mgr.remove(m_skeleton->getResourceName().getName());
}



void gkSkeletonLoader::makeManual(gkEntity* ent)
{

	Ogre::Entity* oent = ent->getEntity();
	if (!oent || !oent->hasSkeleton())
		return;


	Ogre::SkeletonInstance* inst = oent->getSkeleton();


	gkBone::BoneList::Iterator it = m_skeleton->getBoneList().iterator();
	while (it.hasMoreElements())
	{
		gkBone* bone = it.getNext();


		if (inst->hasBone(bone->getName()))
		{
			Ogre::Bone* obone = inst->getBone(bone->getName());
			bone->_setOgreBone(obone);

			obone->setManuallyControlled(true);
		}
	}
}



void gkSkeletonLoader::recurseBone(Ogre::Skeleton* skel, gkBone* cur, Ogre::Bone* par)
{
	GK_ASSERT(cur);

	Ogre::Bone* obone = skel->createBone(cur->getName());
	cur->_setOgreBone(obone);


	if (par != 0)
		par->addChild(obone);

	const gkTransformState& rest = cur->getRest();
	obone->setPosition(rest.loc);
	obone->setOrientation(rest.rot);
	obone->setScale(rest.scl);
	obone->setBindingPose();
	obone->setManuallyControlled(true);


	gkBone::BoneList& bl = cur->getChildren();
	for (UTsize i = 0; i < bl.size(); ++i)
		recurseBone(skel, bl[i], obone);
}



void gkSkeletonLoader::loadResource(Ogre::Resource* resource)
{
	Ogre::Skeleton* oskel = static_cast<Ogre::Skeleton*>(resource);

	gkBone::BoneList& bl = m_skeleton->getRootBoneList();
	for (UTsize i = 0; i < bl.size(); ++i)
	{
		gkBone* gbone = bl[i];
		recurseBone(oskel, gbone, 0);
	}
}
