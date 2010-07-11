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
#ifndef _gksProperty_h_
#define _gksProperty_h_

#include "gkVariable.h"


// ----------------------------------------------------------------------------
class gksProperty
{
#ifndef SWIG
public:
    gksProperty(gkVariable *oth);
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

    gksProperty() : m_var(0) {}



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
