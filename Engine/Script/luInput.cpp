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
#include "luInput.h"
#include "gkWindowSystem.h"
#include "gkInput.h"


// ----------------------------------------------------------------------------
luMouse::luMouse()
{
    m_mouse = gkWindowSystem::getSingleton().getMouse();
}

luMouse::~luMouse()
{
}

static int luMouse_constructor(luObject &L)
{
    new (L) luMouse();
    return 1;
}

static int luMouse_get(luObject &L)
{
    gkMouse &mse = L.getValueClassT<luMouse>(1)->mouse();
    if (L.isString(2))
    {
        const luString &str = L.getValueString(2);
        if (str == "xpos")
            return L.push((int)mse.position.x);
        if (str == "ypos")
            return L.push((int)mse.position.y);
        if (str == "xrel")
            return L.push((int)mse.relitave.x);
        if (str == "yrel")
            return L.push((int)mse.relitave.y);
        if (str == "wheel")
            return L.push((int)mse.wheelDelta);
        if (str == "moved")
            return L.push(mse.moved);
    }
    return 0;
}


static int luMouse_isButtonDown(luObject &L)
{
    gkMouse &mse = L.getValueClassT<luMouse>(1)->mouse();
    return L.push(mse.isButtonDown((gkMouse::Buttons)L.getValueInt(2)));
}


luMethodDef luMouse::Methods[] =
{
    {"constructor",     luMouse_constructor,    LU_NOPARAM, "."},
    {"isButtonDown",    luMouse_isButtonDown,   LU_PARAM,   ".e"},
    {"__getter",        luMouse_get,            LU_PARAM,   ".s"},
    {0, 0, 0}
};

luTypeDef luMouse::Type = { "Mouse", 0,  Methods};

// ----------------------------------------------------------------------------
luKeyboard::luKeyboard()
{
    m_keyboard = gkWindowSystem::getSingleton().getKeyboard();
}

luKeyboard::~luKeyboard()
{
}


static int luKeyboard_constructor(luObject &L)
{
    new (L) luKeyboard();
    return 1;
}


static int luKeyboard_isKeyDown(luObject &L)
{
    gkKeyboard &key = L.getValueClassT<luKeyboard>(1)->key();
    return L.push(key.isKeyDown((gkScanCode)L.getValueInt(2)));
}

luMethodDef luKeyboard::Methods[] =
{
    {"constructor",     luKeyboard_constructor,     LU_NOPARAM, "."},
    {"isKeyDown",       luKeyboard_isKeyDown,       LU_PARAM,   ".e"},
    {0, 0, 0}
};


luTypeDef luKeyboard::Type = { "Keyboard", 0,  Methods};



// ----------------------------------------------------------------------------
void luMouse::bind(luBinder &L)
{
    L.addConstant("MB_Left",    gkMouse::Left);
    L.addConstant("MB_Middle",  gkMouse::Middle);
    L.addConstant("MB_Right",   gkMouse::Right);
}

void luKeyboard::bind(luBinder &L)
{
	L.addConstant("KC_AKEY", KC_AKEY);
	L.addConstant("KC_BKEY", KC_BKEY);
	L.addConstant("KC_CKEY", KC_CKEY);
	L.addConstant("KC_DKEY", KC_DKEY);
	L.addConstant("KC_EKEY", KC_EKEY);
	L.addConstant("KC_FKEY", KC_FKEY);
	L.addConstant("KC_GKEY", KC_GKEY);
	L.addConstant("KC_HKEY", KC_HKEY);
	L.addConstant("KC_IKEY", KC_IKEY);
	L.addConstant("KC_JKEY", KC_JKEY);
	L.addConstant("KC_KKEY", KC_KKEY);
	L.addConstant("KC_LKEY", KC_LKEY);
	L.addConstant("KC_MKEY", KC_MKEY);
	L.addConstant("KC_NKEY", KC_NKEY);
	L.addConstant("KC_OKEY", KC_OKEY);
	L.addConstant("KC_PKEY", KC_PKEY);
	L.addConstant("KC_QKEY", KC_QKEY);
	L.addConstant("KC_RKEY", KC_RKEY);
	L.addConstant("KC_SKEY", KC_SKEY);
	L.addConstant("KC_TKEY", KC_TKEY);
	L.addConstant("KC_UKEY", KC_UKEY);
	L.addConstant("KC_VKEY", KC_VKEY);
	L.addConstant("KC_WKEY", KC_WKEY);
	L.addConstant("KC_XKEY", KC_XKEY);
	L.addConstant("KC_YKEY", KC_YKEY);
	L.addConstant("KC_ZKEY", KC_ZKEY);
	L.addConstant("KC_ZEROKEY", KC_ZEROKEY);
	L.addConstant("KC_ONEKEY", KC_ONEKEY);
	L.addConstant("KC_TWOKEY", KC_TWOKEY);
	L.addConstant("KC_THREEKEY", KC_THREEKEY);
	L.addConstant("KC_FOURKEY", KC_FOURKEY);
	L.addConstant("KC_FIVEKEY", KC_FIVEKEY);
	L.addConstant("KC_SIXKEY", KC_SIXKEY);
	L.addConstant("KC_SEVENKEY", KC_SEVENKEY);
	L.addConstant("KC_EIGHTKEY", KC_EIGHTKEY);
	L.addConstant("KC_NINEKEY", KC_NINEKEY);
	L.addConstant("KC_CAPSLOCKKEY", KC_CAPSLOCKKEY);
	L.addConstant("KC_LEFTCTRLKEY", KC_LEFTCTRLKEY);
	L.addConstant("KC_LEFTALTKEY", KC_LEFTALTKEY);
	L.addConstant("KC_RIGHTALTKEY", KC_RIGHTALTKEY);
	L.addConstant("KC_RIGHTCTRLKEY", KC_RIGHTCTRLKEY);
	L.addConstant("KC_RIGHTSHIFTKEY", KC_RIGHTSHIFTKEY);
	L.addConstant("KC_LEFTSHIFTKEY", KC_LEFTSHIFTKEY);
	L.addConstant("KC_ESCKEY", KC_ESCKEY);
	L.addConstant("KC_TABKEY", KC_TABKEY);
	L.addConstant("KC_RETKEY", KC_RETKEY);
	L.addConstant("KC_SPACEKEY", KC_SPACEKEY);
	L.addConstant("KC_LINEFEEDKEY", KC_LINEFEEDKEY);
	L.addConstant("KC_BACKSPACEKEY", KC_BACKSPACEKEY);
	L.addConstant("KC_DELKEY", KC_DELKEY);
	L.addConstant("KC_SEMICOLONKEY", KC_SEMICOLONKEY);
	L.addConstant("KC_PERIODKEY", KC_PERIODKEY);
	L.addConstant("KC_COMMAKEY", KC_COMMAKEY);
	L.addConstant("KC_QUOTEKEY", KC_QUOTEKEY);
	L.addConstant("KC_ACCENTGRAVEKEY", KC_ACCENTGRAVEKEY);
	L.addConstant("KC_MINUSKEY", KC_MINUSKEY);
	L.addConstant("KC_SLASHKEY", KC_SLASHKEY);
	L.addConstant("KC_BACKSLASHKEY", KC_BACKSLASHKEY);
	L.addConstant("KC_EQUALKEY", KC_EQUALKEY);
	L.addConstant("KC_LEFTBRACKETKEY", KC_LEFTBRACKETKEY);
	L.addConstant("KC_RIGHTBRACKETKEY", KC_RIGHTBRACKETKEY);
	L.addConstant("KC_LEFTARROWKEY", KC_LEFTARROWKEY);
	L.addConstant("KC_DOWNARROWKEY", KC_DOWNARROWKEY);
	L.addConstant("KC_RIGHTARROWKEY", KC_RIGHTARROWKEY);
	L.addConstant("KC_UPARROWKEY", KC_UPARROWKEY);
	L.addConstant("KC_PAD0", KC_PAD0);
	L.addConstant("KC_PAD1", KC_PAD1);
	L.addConstant("KC_PAD2", KC_PAD2);
	L.addConstant("KC_PAD3", KC_PAD3);
	L.addConstant("KC_PAD4", KC_PAD4);
	L.addConstant("KC_PAD5", KC_PAD5);
	L.addConstant("KC_PAD6", KC_PAD6);
	L.addConstant("KC_PAD7", KC_PAD7);
	L.addConstant("KC_PAD8", KC_PAD8);
	L.addConstant("KC_PAD9", KC_PAD9);
	L.addConstant("KC_PADEQUALS", KC_PADEQUALS);
	L.addConstant("KC_PADPERIOD", KC_PADPERIOD);
	L.addConstant("KC_PADSLASHKEY", KC_PADSLASHKEY);
	L.addConstant("KC_PADASTERKEY", KC_PADASTERKEY);
	L.addConstant("KC_PADMINUS", KC_PADMINUS);
	L.addConstant("KC_PADENTER", KC_PADENTER);
	L.addConstant("KC_PADPLUSKEY", KC_PADPLUSKEY);
	L.addConstant("KC_F1KEY", KC_F1KEY);
	L.addConstant("KC_F2KEY", KC_F2KEY);
	L.addConstant("KC_F3KEY", KC_F3KEY);
	L.addConstant("KC_F4KEY", KC_F4KEY);
	L.addConstant("KC_F5KEY", KC_F5KEY);
	L.addConstant("KC_F6KEY", KC_F6KEY);
	L.addConstant("KC_F7KEY", KC_F7KEY);
	L.addConstant("KC_F8KEY", KC_F8KEY);
	L.addConstant("KC_F9KEY", KC_F9KEY);
	L.addConstant("KC_F10KEY", KC_F10KEY);
	L.addConstant("KC_F11KEY", KC_F11KEY);
	L.addConstant("KC_F12KEY", KC_F12KEY);
	L.addConstant("KC_PAUSEKEY", KC_PAUSEKEY);
	L.addConstant("KC_INSERTKEY", KC_INSERTKEY);
	L.addConstant("KC_HOMEKEY", KC_HOMEKEY);
	L.addConstant("KC_PAGEUPKEY", KC_PAGEUPKEY);
	L.addConstant("KC_PAGEDOWNKEY", KC_PAGEDOWNKEY);
	L.addConstant("KC_ENDKEY", KC_ENDKEY);
	L.addConstant("KC_UNKNOWNKEY", KC_UNKNOWNKEY);
	L.addConstant("KC_COMMANDKEY", KC_COMMANDKEY);
	L.addConstant("KC_GRLESSKEY", KC_GRLESSKEY);
}
