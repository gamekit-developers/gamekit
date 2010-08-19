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
	:       m_name(name), m_loadingState(ST_UNLOADED)
{
}


// ----------------------------------------------------------------------------
gkObject::~gkObject()
{
}



// ----------------------------------------------------------------------------
void gkObject::load(void)
{
	// Simple load mechanizm

	if (m_loadingState != ST_UNLOADED)
		return;

	m_loadingState = ST_LOADING;


	try
	{
		preLoadImpl();
		loadImpl();
		if (m_loadingState != ST_LOADFAILED)
		{
			m_loadingState |= ST_LOADED;

			postLoadImpl();
			m_loadingState = ST_LOADED;
		}
	}

	catch (Ogre::Exception &e)
	{
		m_loadingState = ST_LOADFAILED;
		gkLogMessage("Object: Loading failed. \n\t" << e.getDescription());
	}

}


// ----------------------------------------------------------------------------
void gkObject::unload(void)
{
	// Simple unload mechanizm
	if (m_loadingState != ST_LOADED)
		return;

	m_loadingState = ST_UNLOADING;

	try
	{
		preUnloadImpl();
		unloadImpl();
		m_loadingState |= ST_UNLOADED;
		postUnloadImpl();
		m_loadingState = ST_UNLOADED;
	}

	catch (Ogre::Exception &e)
	{
		m_loadingState = ST_LOADFAILED;
		gkLogMessage("Object: Loading failed. \n\t" << e.getDescription());
	}
}


// ----------------------------------------------------------------------------
void gkObject::reload(void)
{
	unload();
	load();
}
