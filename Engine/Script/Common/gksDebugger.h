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
#ifndef _gksDebugger_h_
#define _gksDebugger_h_

#include "gksPointer.h"
#include "gksMath.h"
class gksScene;
class gksGameObject;


class gksDebugger
{
protected:
	class gkDebugger *m_debugger;

public:

	gksDebugger(const gksPointer<gksScene>& sc);
	~gksDebugger() {}

	void drawLine(const gksVector3 &from, const gksVector3 &to, const gksVector3 &color);
	void drawObjectAxis(const gksPointer<gksGameObject> &ptr, float size);

	void print(const char *str);
	void clear(void);
};

#endif//_gksDebugger_h_
