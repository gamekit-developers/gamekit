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
#ifndef _gkLuaInput_h_
#define _gkLuaInput_h_


#include "gkWindowSystem.h"

// ----------------------------------------------------------------------------
enum MouseButton
{
    LEFT = 0,
    RIGHT,
    MIDDLE,
};


// ----------------------------------------------------------------------------
class Mouse
{
public:
    Mouse() :
        xpos(0), ypos(0),
        xrel(0), yrel(0),
        winx(0), winy(0),
        wheel(0), moved(false)
    {
    }


    float xpos, ypos;
    float xrel, yrel;
    float winx, winy;
    float wheel;
    bool moved;


    void capture(void)
    {
        gkMouse *mse = gkWindowSystem::getSingleton().getMouse();
        xpos    = mse->position.x;
        ypos    = mse->position.y;
        xrel    = mse->relitave.x;
        yrel    = mse->relitave.y;
        winx    = mse->winsize.x;
        winy    = mse->winsize.y;
        wheel   = mse->wheelDelta;
        moved   = mse->moved;
    }

    bool isButtonDown(MouseButton btn) { return gkWindowSystem::getSingleton().getMouse()->isButtonDown(btn);}
};


// ----------------------------------------------------------------------------
class Keyboard
{
public:

    Keyboard()
    {
    }

    void capture(void) {}

    bool isKeyDown(gkScanCode sc)
    {
        return gkWindowSystem::getSingleton().getKeyboard()->isKeyDown(sc);
    }
};


#endif//_gkLuaInput_h_