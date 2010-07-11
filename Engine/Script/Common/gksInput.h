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
#ifndef _gksInput_h_
#define _gksInput_h_



#include "gkWindowSystem.h"

// ----------------------------------------------------------------------------
enum gksMouseButton
{
    LEFT = 0,
    RIGHT,
    MIDDLE,
};


// ----------------------------------------------------------------------------
class gksMouse
{
public:
    gksMouse() :
        xpos(0), ypos(0),
        xrel(0), yrel(0),
        winx(0), winy(0),
        wheel(0), moved(false)
    {
    }

    float   xpos, ypos;
    float   xrel, yrel;
    float   winx, winy;
    float   wheel;
    bool    moved;


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

    bool isButtonDown(gksMouseButton btn) 
    { 
        return gkWindowSystem::getSingleton().getMouse()->isButtonDown(btn);
    }
};


// ----------------------------------------------------------------------------
class gksKeyboard
{
public:
    gksKeyboard() {}
    void capture(void) {}

    bool isKeyDown(gkScanCode sc)
    {
        return gkWindowSystem::getSingleton().getKeyboard()->isKeyDown(sc);
    }
};


#endif//_gksInput_h_