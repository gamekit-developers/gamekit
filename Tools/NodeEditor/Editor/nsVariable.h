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
#ifndef _nsVariable_h_
#define _nsVariable_h_

#include "nsCommon.h"
#include "nsMath.h"
#include "Utils/utString.h"


class nsVariable
{
public:
    typedef enum PropertyTypes
    {
        VAR_NULL = 0,
        VAR_BOOL,
        VAR_REAL,
        VAR_ENUM,
        VAR_INT,
        VAR_VEC2,
        VAR_VEC3,
        VAR_VEC4,
        VAR_QUAT,
        VAR_RECT,
        VAR_MAT3,
        VAR_MAT4,
        VAR_COLOR,
        VAR_STRING,
    }PropertyTypes;

    nsVariable()
    :       m_string(""),
            m_type(VAR_NULL)
    {
        memset(&m_numeric, 0, sizeof(NSfloat)*16);
    }

    nsVariable(const utString &v)
    :       m_string(v),
            m_type(VAR_STRING)
    {
        memset(&m_numeric, 0, sizeof(NSfloat)*16);
    }

    UT_INLINE int  getType(void)                { return m_type; }
    UT_INLINE void setType(PropertyTypes pt)    { m_type = pt;}

    UT_INLINE void setValue(NSfloat v)
    { 
        m_numeric[0] = (NSfloat)v; 
        if (!m_string.empty())
            m_string.clear();
    }

    UT_INLINE void setValue(bool v)
    { 
        m_numeric[0] = v ? 1.f : 0.f; 
        if (!m_string.empty())
            m_string.clear();
    }
    UT_INLINE void setValue(int v)              
    { 
        m_numeric[0] = (NSfloat)v; 
        if (!m_string.empty())
            m_string.clear();
    }
    UT_INLINE void setValue(const utString& v)  
    { 
        m_string = v; 
    }
    UT_INLINE void setValue(const NSvec2& v)
    { 
        m_numeric[0] = v.x; m_numeric[1] = v.y; 
        if (!m_string.empty())
            m_string.clear();
    }
    UT_INLINE void setValue(const NSvec3& v)    
    { 
        m_numeric[0] = v.x; m_numeric[1] = v.y; m_numeric[2] = v.z; 
        if (!m_string.empty())
            m_string.clear();
    }
    UT_INLINE void setValue(const NSvec4& v)    
    { 
        m_numeric[0] = v.w; m_numeric[1] = v.x; m_numeric[2] = v.y; m_numeric[3] = v.z; 
        if (!m_string.empty())
            m_string.clear();
    }
    UT_INLINE void setValue(const NSquat& v)    
    { 
        m_numeric[0] = v.w; m_numeric[1] = v.x; m_numeric[2] = v.y; m_numeric[3] = v.z; 
        if (!m_string.empty())
            m_string.clear();
    }

    // copy 
    UT_INLINE void setValue(const nsVariable& v)
    {
        m_type = v.m_type;
        m_numeric[0] = v.m_numeric[0];
        m_numeric[1] = v.m_numeric[1];
        m_numeric[2] = v.m_numeric[2];
        m_numeric[3] = v.m_numeric[3];
        m_string = v.m_string;
    }

    UT_INLINE nsVariable& operator = (const nsVariable& v)
    {
        m_type = v.m_type;
        m_numeric[0] = v.m_numeric[0];
        m_numeric[1] = v.m_numeric[1];
        m_numeric[2] = v.m_numeric[2];
        m_numeric[3] = v.m_numeric[3];
        m_string = v.m_string;
        return *this;
    }

    UT_INLINE bool getValueBool(void) const
    {
        if (!m_string.empty())
            return utStringConverter::toBool(m_string);
        return m_numeric[0] != 0;
    }

    UT_INLINE NSfloat getValueReal(void) const
    {
        if (!m_string.empty())
            return utStringConverter::toFloat(m_string);
        return m_numeric[0];
    }

    UT_INLINE int getValueInt(void) const
    {
        if (!m_string.empty())
            return utStringConverter::toInt(m_string);
        return (int)m_numeric[0];
    }

    const utString getValueString(void) const
    {
        if (m_type != VAR_STRING)
        {
            if (m_type == VAR_BOOL)
                return utStringConverter::toString(m_numeric[0] != 0);
            if (m_type == VAR_REAL)
                return utStringConverter::toString(m_numeric[0]);
            if (m_type == VAR_INT)
                return utStringConverter::toString((int)m_numeric[0]);
            if (m_type == VAR_VEC2)
                return utStringConverter::toString(NSvec2(m_numeric[0], m_numeric[1]));
            if (m_type == VAR_VEC3)
                return utStringConverter::toString(NSvec3(m_numeric[0], m_numeric[1], m_numeric[2]));
            if (m_type == VAR_VEC4)
                return utStringConverter::toString(NSvec4(m_numeric[0], m_numeric[1], m_numeric[2],  m_numeric[3]));
            if (m_type == VAR_QUAT)
                return utStringConverter::toString(NSquat(m_numeric[0], m_numeric[1], m_numeric[2], m_numeric[3]));
         
            return utStringConverter::toString(m_numeric[0] != 0);
        }
        return m_string;
    }

    UT_INLINE NSvec2 getValueVector2(void) const
    {
        if (!m_string.empty())
            return utStringConverter::toVec2(m_string);
        return NSvec2(m_numeric[0], m_numeric[1]);
    }

    UT_INLINE NSvec3 getValueVector3(void) const
    {
        if (!m_string.empty())
            return utStringConverter::toVec3(m_string);
        return NSvec3(m_numeric[0], m_numeric[1], m_numeric[2]);
    }

    UT_INLINE NSvec4 getValueVector4(void) const
    {
        if (!m_string.empty())
            return utStringConverter::toVec4(m_string);
        return NSvec4(m_numeric[0], m_numeric[1], m_numeric[2], m_numeric[3]);
    }

    UT_INLINE NSquat getValueQuaternion(void) const
    {
        if (!m_string.empty())
            return utStringConverter::toQuat(m_string);
        return NSquat(m_numeric[0], m_numeric[1], m_numeric[2], m_numeric[3]);
    }

    UT_INLINE operator const utString(void) const
    {
        return getValueString();
    }

private:

    // data types
    mutable NSfloat     m_numeric[16];
    mutable utString    m_string;
    mutable int         m_type;
};

#endif//_nsVariable_h_
