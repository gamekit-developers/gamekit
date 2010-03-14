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
#ifndef _luScene_h_
#define _luScene_h_

#include "luUtils.h"
#include "luLoadable.h"

class gkScene;

class luScene : public luLoadable
{
    luClassHeader;
public:

    luScene(gkObject *ob);
    ~luScene();

    static int create(luObject &L, gkObject *ob);
    static UT_INLINE luScene& getArg(luObject &L, int v) { return L.toclassRefT<luScene>(v); }

    // bool Scene:hasObject(String)
    int hasObject(luClass *self, luObject &L);

    // GameObject Scene:getObject(String)
    int getObject(luClass *self, luObject &L);

    // GameObject Scene:createEmpty(String)
    int createEmpty(luClass *self, luObject &L);

    // GameObject Scene:createCamera(String)
    int createCamera(luClass *self, luObject &L);

    // GameObject Scene:createMesh(String, String)
    int createMesh(luClass *self, luObject &L);

    // GameObject Scene:createLamp(String)
    int createLamp(luClass *self, luObject &L);

};

UT_INLINE bool          LU_IsScene(luObject &L, int n)          { return L.typecheck(n, &luScene::Type); }
UT_INLINE gkScene&      LU_GetScene(luObject &L, int n)         { return L.toclassT<luScene>(n)->ref<gkScene>(); }
UT_INLINE int           LU_NewScene(luObject &L, gkObject *sc)  { UT_ASSERT(sc); new (&luScene::Type, L) luScene(sc); return 1;}

#endif//_luScene_h_
