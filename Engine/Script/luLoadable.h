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
#ifndef _luLoadable_h_
#define _luLoadable_h_

#include "luUtils.h"


class luLoadable : public luClass
{
    luClassHeader;
protected:
    class gkObject *m_object;

public:
    luLoadable(gkObject *ob);
    ~luLoadable();


    // String Object:getName()
    int getName(luClass *self, luObject &L);

    // bool Object:isLoaded()
    int isLoaded(luClass *self, luObject &L);

    // nil Object:load()
    int load(luClass *self, luObject &L);

    // nil Object:unload()
    int unload(luClass *self, luObject &L);

    // nil Object:reload()
    int reload(luClass *self, luObject &L);

    template<typename T> T& ref(void) {UT_ASSERT(m_object); return *static_cast<T*>(m_object);}
};

#endif//_luLoadable_h_
