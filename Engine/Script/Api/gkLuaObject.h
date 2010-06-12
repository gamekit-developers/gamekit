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
#ifndef _gkLuaObject_h_
#define _gkLuaObject_h_


#include "gkLuaGameLogic.h"
#include "gkLuaPointer.h"


// ----------------------------------------------------------------------------
class Object
{
#ifndef SWIG
    // ------------------------------------
protected:
    class gkObject *m_object;


public:
    Object(gkObject *oth) : m_object(oth) {}


    template<typename T> T *cast(void)
    {return static_cast<T *>(m_object);}

#endif

public:
    Object() : m_object(0) {}
    virtual ~Object() {}


    // nil Object:load()
    void            load(void);

    // nil Object:unload()
    void            unload(void);

    // nil Object:reload()
    void            reload(void);

    // String Object:getName()
    const char     *getName(void);
};

#endif//_gkLuaObject_h_