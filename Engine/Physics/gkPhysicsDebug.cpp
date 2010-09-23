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
#include "gkPhysicsDebug.h"
#include "gkDynamicsWorld.h"
#include "gkScene.h"
#include "gkDebugScreen.h"



gkPhysicsDebug::gkPhysicsDebug(gkDynamicsWorld* wo)
	:       m_physics(wo), m_flags(0)
{
}

gkPhysicsDebug::~gkPhysicsDebug()
{
	gkDebugger* dbg = m_physics->getScene()->getDebugger();
	if (dbg)
		dbg->clear();
}

void gkPhysicsDebug::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	gkDebugger* dbg = m_physics->getScene()->getDebugger();
	if (dbg)
		dbg->drawLine(gkVector3(from.x(), from.y(), from.z()), gkVector3(to.x(), to.y(), to.z()), gkVector3(color.x(), color.y(), color.z()));

}

void gkPhysicsDebug::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
}

void gkPhysicsDebug::draw3dText(const btVector3& location, const char* textString)
{
}

void gkPhysicsDebug::reportErrorWarning(const char* warningString)
{
	dsPrintf("Warning: %s", warningString);
}


void gkPhysicsDebug::setDebugMode(int debugMode)
{
	m_flags = debugMode;
}

int gkPhysicsDebug::getDebugMode() const
{
	return m_flags;
}
