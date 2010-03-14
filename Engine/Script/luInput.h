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
#ifndef _luInput_h_
#define _luInput_h_

#include "luUtils.h"



class luMouse : public luClass
{
    luClassHeader;
private:
    class gkMouse *m_mouse;

public:
    luMouse();
    ~luMouse();

    int getAttribute(luClass *self, luObject &L);
    int isButtonDown(luClass *self, luObject &L);

    static void bind(luBinder &L);
};


class luKeyboard : public luClass
{
    luClassHeader;
private:
    class gkKeyboard *m_keyboard;

public:

    luKeyboard();
    ~luKeyboard();

    int isKeyDown(luClass *self, luObject &L);

    static void bind(luBinder &L);
};



#endif//_luInput_h_
