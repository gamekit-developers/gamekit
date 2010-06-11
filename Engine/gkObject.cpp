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
#include "btBulletDynamicsCommon.h"

gkObject::gkObject(const gkString& name)
:       m_name(name), m_loaded(false), m_flags(0), m_sensorMaterial("")
{
}



gkObject::~gkObject()
{
}


// Simple load mechanizm
void gkObject::load(void)
{
    if (m_loaded)
        return;

    try
    {
        preLoadImpl();
        loadImpl();
        m_loaded = true;
        postLoadImpl();
    }
    catch (Ogre::Exception &e)
    {
        gkLogMessage(e.getDescription());
    }

}


// Simple unload mechanizm
void gkObject::unload(void)
{
    if (!m_loaded)
        return;

    try
    {
        preUnloadImpl();
        unloadImpl();
        m_loaded = false;
        postUnloadImpl();
    }

    catch (Ogre::Exception &e)
    {
        gkLogMessage(e.getDescription());
    }
}

void gkObject::reload(void)
{
    unload();
    load();
}

void gkObject::handleManifold(btPersistentManifold* manifold)
{
	if(!wantsContactInfo()) return;

	gkObject* colA = static_cast<gkObject*>(static_cast<btCollisionObject*>(manifold->getBody0())->getUserPointer());
	gkObject* colB = static_cast<gkObject*>(static_cast<btCollisionObject*>(manifold->getBody1())->getUserPointer());

	gkObject* collider = colB;

	if(collider == this)
	{
		collider = colA;
	}

	int nrc = manifold->getNumContacts();

	if (nrc)
	{
		for (int j = 0; j < nrc; ++j)
		{
			gkObject::ContactInfo cinf;
			btManifoldPoint &pt = manifold->getContactPoint(j);

			if (pt.getDistance() < 0.f)
			{
				cinf.collider = collider; 
				cinf.point = pt;
				getContacts().push_back(cinf); 
			}
		}
	}
}

void gkObject::resetContactInfo()
{
	if(wantsContactInfo())
	{
		getContacts().resize(0);
	}
}