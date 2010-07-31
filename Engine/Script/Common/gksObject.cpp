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
#include "gkObject.h"
#include "gksObject.h"
#include "gkEngine.h"


// ----------------------------------------------------------------------------
void gksObject::load(void)
{
	if (m_object)
		gkEngine::getSingleton().addLoadable(m_object, LQ_LOAD);
}

// ----------------------------------------------------------------------------
void gksObject::unload(void)
{
	if (m_object)
		gkEngine::getSingleton().addLoadable(m_object, LQ_UNLOAD);
}


// ----------------------------------------------------------------------------
void gksObject::reload(void)
{
	if (m_object)
		gkEngine::getSingleton().addLoadable(m_object, LQ_RELOAD);
}

// ----------------------------------------------------------------------------
const char *gksObject::getName(void)
{
	static char buf[72] = {0};
	if (m_object)
		sprintf(buf, "%s", m_object->getName().c_str());
	return buf;
}
