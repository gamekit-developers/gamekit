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
#include "gkObject.h"
#include "gkLogger.h"
#include "OgreException.h"



// ----------------------------------------------------------------------------
gkObject::gkObject(const gkString &name)
	:       m_name(name), m_instanceState(ST_DESTROYED)
{
}


// ----------------------------------------------------------------------------
gkObject::~gkObject()
{
}



// ----------------------------------------------------------------------------
void gkObject::createInstance(void)
{

	if (m_instanceState != ST_DESTROYED)
		return;

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
	}

	catch (Ogre::Exception &e)
	{
		m_instanceState = ST_ERROR;
		gkLogMessage("Object: Loading failed. \n\t" << e.getDescription());
	}

}


// ----------------------------------------------------------------------------
void gkObject::destroyInstance(void)
{

	if (m_instanceState != ST_CREATED)
		return;

	m_instanceState = ST_DESTROYING;

	try
	{
		preDstroyInstanceImpl();
		destroyInstanceImpl();
		m_instanceState |= ST_DESTROYED;
		postDestroyInstanceImpl();
		m_instanceState = ST_DESTROYED;
	}

	catch (Ogre::Exception &e)
	{
		m_instanceState = ST_ERROR;
		gkLogMessage("Object: Loading failed. \n\t" << e.getDescription());
	}
}


// ----------------------------------------------------------------------------
void gkObject::reinstance(void)
{
	destroyInstance();
	createInstance();
}
