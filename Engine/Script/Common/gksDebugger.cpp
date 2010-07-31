/*
-------------------------------------------------------------------------------
--                                                                           --
--  This file is part of GameKit.                                            --
--  http://gamekit.googlecode.com/                                           --
--                                                                           --
--                                                                           --
--  Released under the zlib license.                                         --
--  http://www.gzip.org/zlib/zlib_license.html                               --
--                                                                           --
--                                                                           --
-- Generic Script API Binding                                                --
-------------------------------------------------------------------------------
*/
#include "gksScene.h"
#include "gkScene.h"
#include "gksGameObject.h"
#include "gkGameObject.h"
#include "gkDebugger.h"
#include "gkDebugScreen.h"
#include "gksDebugger.h"
#include "gkEngine.h"



gksDebugger::gksDebugger(const gksPointer<gksScene> &sc)
	:   m_debugger(0)
{
	if (!sc.isNull())
	{
		gkScene *scene = sc->cast<gkScene>();
		if (scene)
			m_debugger = scene->getDebugger();

	}
}


void gksDebugger::drawLine(const gksVector3 &from, const gksVector3 &to, const gksVector3 &color)
{
	if (m_debugger)
		m_debugger->drawLine(from, to, color);
}


void gksDebugger::drawObjectAxis(const gksPointer<gksGameObject> &ptr, float size)
{
	if (m_debugger && !ptr.isNull())
	{
		gkGameObject *ob = ptr->cast<gkGameObject>();
		if (ob)
		{
			const gkVector3     &axis   = ob->getWorldPosition();
			const gkQuaternion  &ori    = ob->getOrientation();

			gkVector3 x = (ori * gkVector3(size, 0, 0));
			gkVector3 y = (ori * gkVector3(0, size, 0));
			gkVector3 z = (ori * gkVector3(0, 0, size));


			m_debugger->drawLine(axis, axis + x, gkVector3(1,0,0));
			m_debugger->drawLine(axis, axis + y, gkVector3(0,1,0));
			m_debugger->drawLine(axis, axis + z, gkVector3(0,0,1));
		}
	}
}

void gksDebugger::print(const char *str)
{
	gkDebugScreen::printTo(str);
}

void gksDebugger::clear(void)
{
	if (m_debugger)
		m_debugger->clear();
}
