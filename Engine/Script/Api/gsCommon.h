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
#ifndef _gsCommon_h_
#define _gsCommon_h_


// All include
#include "OgreKit.h"
#include "Script/Lua/gkLuaUtils.h"

// ----------------------------------------------------------------------------
//  Conversion Macros


#ifndef SWIG

#define OGRE_KIT_WRAP_CLASS(cls, membername)\
    public:\
        cls *membername;\
        inline cls* getClassPtr(void) {return membername;}\
        inline cls& getClassRef(void) {assert(membername); return *membername;}

#define OGRE_KIT_WRAP_BASE_COPY_CTOR(wrap, cls)\
        wrap(cls *oth);

#define OGRE_KIT_WRAP_BASE_DEF_CTOR(wrap, cls)\
        wrap();


#define OGRE_KIT_WRAP_CLASS_COPY_CTOR(wrap, cls, membername)\
        OGRE_KIT_WRAP_CLASS(cls, membername);\
        OGRE_KIT_WRAP_BASE_COPY_CTOR(wrap, cls)

#define OGRE_KIT_WRAP_CLASS_DEF_CTOR(wrap, cls, membername)\
        OGRE_KIT_WRAP_CLASS(cls, membername);\
        OGRE_KIT_WRAP_BASE_DEF_CTOR(wrap, cls)



#define OGRE_KIT_TEMPLATE_CAST(T, membername)\
    T *get(void) {return static_cast<T *>(membername);}


#define OGRE_KIT_INTERNAL_CAST(membername)\
    template<typename T> T *cast(void) {return static_cast<T *>(membername);}


#define OGRE_KIT_OBJECT(T, val) val ? val->get() : 0


#endif


// Object to pass as callbacks and self pointers
typedef gkLuaCurState gsFunction;
typedef gkLuaCurState gsSelf;


// Class decls
class gsProperty;
class gsUserDefs;
class gsEngine;
class gsEntity;
class gsCamera;
class gsLight;
class gsScene;
class gsGameObject;
class gsEngineEventObject;
class gsMouse;
class gsKeyboard;
class gsSkeleton;
class gsFSM;
class gsRayTest;
class gsSweptTest;
class gsConstraint;


#endif//_gsCommon_h_
