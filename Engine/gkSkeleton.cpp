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
#include "OgreSceneNode.h"

#include "gkSkeleton.h"
#include "gkSkeletonResource.h"
#include "gkLogger.h"
#include "gkEntity.h"


gkSkeleton::gkSkeleton(gkInstancedManager* creator, const gkResourceName& name, const gkResourceHandle& handle)
	:    gkGameObject(creator, name, handle, GK_SKELETON),
	     m_controller(0),
	     m_resource(0)
{
}



gkSkeleton::~gkSkeleton()
{	
}


void gkSkeleton::_setInternalSkeleton(gkSkeletonResource* skr)
{
	m_resource = skr;
}


void gkSkeleton::createInstanceImpl(void)
{
	if (!m_resource)
	{
		gkPrintf("Skeleton: '%s' Has no internal data.\n", m_name.getName().c_str());
		m_instanceState = ST_ERROR;
		return;
	}

	gkGameObject::createInstanceImpl();

	if (m_baseProps.isInvisible())
		m_node->setVisible(false);
}



void gkSkeleton::destroyInstanceImpl(void)
{
	gkGameObject::destroyInstanceImpl();
}


void gkSkeleton::updateFromController(void)
{
	if (!m_resource)
	{
		gkPrintf("Skeleton: '%s' Has no internal data.\n", m_name.getName().c_str());
		return;
	}


	if (m_controller)
	{
		if (!m_controller->isInstanced())
			m_controller->createInstance();

		m_resource->makeManual(m_controller);
	}

}


gkGameObject* gkSkeleton::clone(const gkString& name)
{
	gkSkeleton* cl = new gkSkeleton(getInstanceCreator(), name, -1);
	gkGameObject::cloneImpl(cl);
	cl->m_resource = m_resource->clone();
	return cl;
}

gkBone* gkSkeleton::getBone(const gkHashedString& name)
{
	if(m_resource)
		return m_resource->getBone(name);
	return 0;
}
