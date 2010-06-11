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
#include "gkLuaProperty.h"

// ----------------------------------------------------------------------------
Property::Property(gkVariable *oth)
    :   m_var(oth)
{

}

int Property::getType(void)
{

    if (m_var)
    {
        switch (m_var->getType())
        {
        case gkVariable::VAR_BOOL:      return BOOL;
        case gkVariable::VAR_INT:       return INT;
        case gkVariable::VAR_REAL:      return FLOAT;
        case gkVariable::VAR_STRING:    return STRING;
        }
    }

    return -1;
}

bool Property::getValueBool(void)
{
    if (m_var)
        return m_var->getValueBool();
    return false;
}

int Property::getValueInt(void)
{
    if (m_var)
        return m_var->getValueInt();
    return -1;
}

float Property::getValueReal(void)
{
    if (m_var)
        return m_var->getValueReal();
    return 0.f;
}

const char *Property::getValueString(void)
{
    static char buffer[128];

    if (m_var)
    {
        sprintf(buffer, "%s", m_var->getValueString().c_str());
    }
    else
        buffer[0] = 0;
    return buffer;
}


void Property::setBool(bool v)
{
    if (m_var)
        m_var->setValue(v);
}

void Property::setInt(int v)
{
    if (m_var)
        m_var->setValue(v);
}

void Property::setFloat(float v)
{
    if (m_var)
        m_var->setValue(v);
}

void Property::setString(const char* v)
{
    if (m_var)
        m_var->setValue(gkString(v));
}

