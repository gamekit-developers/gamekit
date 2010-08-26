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
	:       m_name(name), m_initState(ST_FINALIZED)
{
}


// ----------------------------------------------------------------------------
gkObject::~gkObject()
{
}



// ----------------------------------------------------------------------------
void gkObject::initialize(void)
{
	// Simple initialize mechanizm

	if (m_initState != ST_FINALIZED)
		return;

	m_initState = ST_INITIALIZING;


	try
	{
		preInitializeImpl();
		initializeImpl();
		if (m_initState != ST_INITFAILED)
		{
			m_initState |= ST_INITIALIZED;

			postInitializeImpl();
			m_initState = ST_INITIALIZED;
		}
	}

	catch (Ogre::Exception &e)
	{
		m_initState = ST_INITFAILED;
		gkLogMessage("Object: Initialize failed. \n\t" << e.getDescription());
	}

}


// ----------------------------------------------------------------------------
void gkObject::finalize(void)
{
	// Simple finalize mechanizm
	if (m_initState != ST_INITIALIZED)
		return;

	m_initState = ST_FINALIZING;

	try
	{
		preFinalizeImpl();
		finalizeImpl();
		m_initState |= ST_FINALIZED;
		postFinalizeImpl();
		m_initState = ST_FINALIZED;
	}

	catch (Ogre::Exception &e)
	{
		m_initState = ST_INITFAILED;
		gkLogMessage("Object: Finalize failed. \n\t" << e.getDescription());
	}
}


// ----------------------------------------------------------------------------
void gkObject::reinitialize(void)
{
	finalize();
	initialize();
}
