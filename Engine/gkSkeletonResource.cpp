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
#include "gkSkeletonResource.h"
#include "gkAction.h"
#include "gkBone.h"
#include "gkLogger.h"

#if GK_EXTERNAL_RENDER == GK_EXTERNAL_RENDER_OGREKIT
#include "External/Ogre/gkOgreSkeletonLoader.h"
#endif



gkSkeletonResource::gkSkeletonResource(gkResourceManager* creator, const gkResourceName &name, const gkResourceHandle& handle)
	:   gkResource(creator, name, handle)
{
	m_externalLoader = new gkSkeletonLoader(this);
}


gkSkeletonResource::~gkSkeletonResource()
{
	delete m_externalLoader;

	Actions::Iterator iter = m_actions.iterator();
	while (iter.hasMoreElements())
		delete iter.getNext().second;

	Bones::Iterator biter = m_bones.iterator();
	while (biter.hasMoreElements())
		delete biter.getNext().second;
}


gkBone *gkSkeletonResource::createBone(const gkString &name)
{
	if (hasBone(name))
		return 0;

	gkBone *manual = new gkBone(name);
	m_bones.insert(name, manual);
	m_boneList.push_back(manual);
	return manual;
}


void gkSkeletonResource::makeManual(gkEntity *ent)
{
	m_externalLoader->makeManual(ent);
}


gkAction *gkSkeletonResource::createAction(const gkHashedString &name)
{
	if (m_actions.find(name) != GK_NPOS)
	{
		gkPrintf("SkeletonResource: Duplicate action '%s'\n", name.str().c_str());
		return 0;
	}

	gkAction *act = new gkAction(name.str());

	m_actions.insert(act->getName(), act);
	return act;
}




gkAction *gkSkeletonResource::getAction(const gkHashedString &name)
{
	size_t pos;
	if ((pos = m_actions.find(name)) == GK_NPOS)
		return 0;

	return m_actions.at(pos);
}



gkBone *gkSkeletonResource::getBone(const gkHashedString &name)
{
	size_t pos;
	if ((pos = m_bones.find(name)) == GK_NPOS)
		return 0;
	return m_bones.at(pos);
}



gkBone::BoneList &gkSkeletonResource::getRootBoneList(void)
{
	if (m_rootBoneList.empty())
	{
		for (UTsize i=0; i<m_boneList.size(); ++i)
		{
			gkBone *bone = m_boneList.at(i);

			if (bone->getParent() == 0)
				m_rootBoneList.push_back(bone);
		}
	}
	return m_rootBoneList;
}

