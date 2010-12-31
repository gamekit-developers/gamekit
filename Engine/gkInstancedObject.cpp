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
#include "gkInstancedObject.h"
#include "gkLogger.h"
#include "OgreException.h"




gkInstancedObject::gkInstancedObject(gkInstancedManager* creator, const gkResourceName& name, const gkResourceHandle& handle)
	:    gkResource(creator, name, handle),
	     m_instanceState(ST_DESTROYED)
{
}


gkInstancedObject::~gkInstancedObject()
{
}



void gkInstancedObject::createInstance(bool queue)
{

	if (!canCreateInstance())
	{
		gkLogMessage(m_creator->getResourceType() << ": Object '"
		             <<  m_name.getName() << "' \n\t Cannot create instances.");
		return;
	}

	if (m_instanceState != ST_DESTROYED)
		return;

	if (queue)
	{
		getInstanceCreator()->addCreateInstanceQueue(this);
		return;
	}


	m_instanceState = ST_CREATING;


	try
	{
		preCreateInstanceImpl();
		createInstanceImpl();
		if (m_instanceState != ST_ERROR)
		{
			m_instanceState |= ST_CREATED;

			postCreateInstanceImpl();
			m_instanceState = ST_CREATED;
		}
		else
		{
			gkLogMessage(m_creator->getResourceType() << ": Object '" <<  m_name.getName() << "' Instancing failed. \n\t" <<
			             (m_instanceError.empty() ? "Unknown error set" : m_instanceError));
		}
	}

	catch (Ogre::Exception& e)
	{
		m_instanceState = ST_ERROR;
		gkLogMessage(m_creator->getResourceType() << ": Object '" <<  m_name.getName()
		             << "' Instancing failed. \n\t" << e.getDescription());
	}


	if (m_instanceState == ST_CREATED)
		getInstanceCreator()->notifyInstanceCreated(this);

}



void gkInstancedObject::destroyInstance(bool queue)
{

	if (m_instanceState != ST_CREATED)
		return;

	if (queue)
	{
		getInstanceCreator()->addDestroyInstanceQueue(this);
		return;
	}


	m_instanceState = ST_DESTROYING;

	try
	{
		preDestroyInstanceImpl();
		destroyInstanceImpl();

		m_instanceState |= ST_DESTROYED;
		postDestroyInstanceImpl();
		m_instanceState = ST_DESTROYED;
	}

	catch (Ogre::Exception& e)
	{
		m_instanceState = ST_ERROR;
		gkLogMessage(m_creator->getResourceType() << ": Object '"
		             <<  m_name.getName() << "' Destruction failed. \n\t" << e.getDescription());
	}


	if (m_instanceState == ST_DESTROYED)
		getInstanceCreator()->notifyInstanceDestroyed(this);
}



void gkInstancedObject::reinstance(bool queue)
{
	if (queue)
	{
		getInstanceCreator()->addReInstanceQueue(this);
		return;
	}

	destroyInstance();
	createInstance();
}
