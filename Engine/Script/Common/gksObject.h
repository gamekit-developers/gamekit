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
#ifndef _gksObject_h_
#define _gksObject_h_


#include "gksConstants.h"
#include "gksPointer.h"


// ----------------------------------------------------------------------------
class gksObject
{
#ifndef SWIG
    // ------------------------------------
protected:
    class gkObject *m_object;


public:
    gksObject(gkObject *oth) : m_object(oth) {}


    template<typename T> T *cast(void)
    {return static_cast<T *>(m_object);}

#endif

public:
    gksObject() : m_object(0) {}
    virtual ~gksObject() {}


    // nil gksObject:load()
    void            load(void);

    // nil gksObject:unload()
    void            unload(void);

    // nil gksObject:reload()
    void            reload(void);

    // String gksObject:getName()
    const char     *getName(void);
};

#endif//_gksObject_h_