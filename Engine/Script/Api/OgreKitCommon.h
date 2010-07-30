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
-- Binding Utilities
*/
#ifndef _OgreKitCommon_h_
#define _OgreKitCommon_h_


// All include 
#include "OgreKit.h"
#include "Script/Lua/gkLuaUtils.h"


// ----------------------------------------------------------------------------
// 
//  Conversion Macros
// 
// ----------------------------------------------------------------------------
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


#define OGRE_KIT_INTERNAL_CAST(membername)\
    template<typename T> T *cast(void) {return static_cast<T *>(membername);}

#define OGRE_KIT_PUBLIC (cls) : public cls


#endif


namespace OgreKit {
    typedef gkString String;

    // Object to pass as callbacks and self pointers 
    typedef gkLuaCurState Function;
    typedef gkLuaCurState Self;

}

#endif//_OgreKitCommon_h_
