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
-------------------------------------------------------------------------------
*/
#ifndef _gkLuaDebugger_h_
#define _gkLuaDebugger_h_


#include "gkLuaGameLogic.h"


class Debugger
{
protected:
    class gkDebugger *m_debugger;

public:

    Debugger(const ScenePtr& sc);
    ~Debugger() {}

    void drawLine(const Vector3& from, const Vector3& to, const Vector3& color);
    void drawObjectAxis(const GameObjectPtr &ptr, float size);

    void print(const char *str);
    void clear(void);
};

#endif//_gkLuaDebugger_h_