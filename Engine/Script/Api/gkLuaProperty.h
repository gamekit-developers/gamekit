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
#ifndef _gkLuaProperty_h_
#define _gkLuaProperty_h_

#include "gkVariable.h"


// ----------------------------------------------------------------------------
class Property
{
#ifndef SWIG
    public:
        Property(gkVariable *oth);
        gkVariable *getVar() const {return m_var;}
#endif

private:
    class gkVariable *m_var;

public:

    enum Types
    {
        BOOL, 
        INT,
        FLOAT,
        STRING
    };

    Property() : m_var(0) {}

    int         getType(void);

    bool        getValueBool(void);
    int         getValueInt(void);
    float       getValueReal(void);
    const char  *getValueString(void);


    void        setBool(bool v);
    void        setInt(int v);
    void        setFloat(float v);
    void        setString(const char* v);

};


#endif//_gkLuaProperty_h_
