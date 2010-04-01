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


// Game property, supports numerical types and string types.
// numerical types from bool to quaternion are stored in the
// NSvec4 structure. All types can be converted to all others.
class nsVariable
{
public:

    typedef enum PropertyTypes
    {
        VAR_NULL = 0,
        VAR_BOOL,
        VAR_REAL,
        VAR_INT,
        VAR_VEC2,
        VAR_VEC3,
        VAR_VEC4,
        VAR_QUAT,
        VAR_STRING,
    }PropertyTypes;

    nsVariable()
    :       m_numeric(NSvec4(0,0,0,0)), m_string(""),
            m_type(VAR_NULL), m_name(""), m_debug(false)
    {
    }


    nsVariable(const utString &n, bool dbg=false)
    :       m_numeric(NSvec4(0,0,0,0)), m_string(n),
            m_type(VAR_STRING), m_name(n), m_debug(dbg)
    {
    }



    UT_INLINE void setValue(NSfloat v)          { m_type = VAR_REAL; m_numeric.w = (NSfloat)v; }
    UT_INLINE void setValue(bool v)             { m_type = VAR_BOOL; m_numeric.w = v ? 1.f : 0.f; }
    UT_INLINE void setValue(int v)              { m_type = VAR_INT; m_numeric.w = (NSfloat)v; }
    UT_INLINE void setValue(const utString& v)  { m_type = VAR_STRING; m_string = v; }
    UT_INLINE void setValue(const NSvec2& v)    { m_type = VAR_VEC2; m_numeric.w = v.x; m_numeric.x = v.y; }
    UT_INLINE void setValue(const NSvec3& v)    { m_type = VAR_VEC3; m_numeric.w = v.x; m_numeric.x = v.y; m_numeric.y = v.z; }
    UT_INLINE void setValue(const NSvec4& v)    { m_type = VAR_VEC4; m_numeric.w = v.w; m_numeric.x = v.x; m_numeric.y = v.y; m_numeric.z = v.z; }
    UT_INLINE void setValue(const NSquat& v)    { m_type = VAR_QUAT; m_numeric.w = v.w; m_numeric.x = v.x; m_numeric.y = v.y; m_numeric.z = v.z; }


    // copy 
    UT_INLINE void setValue(const nsVariable& v)
    {
        m_type = v.m_type;
        m_numeric.w = v.m_numeric.w;
        m_numeric.x = v.m_numeric.x;
        m_numeric.y = v.m_numeric.y;
        m_numeric.z = v.m_numeric.z;
        m_string = v.m_string;
    }

    UT_INLINE nsVariable& operator = (const nsVariable& v)
    {
        m_type = v.m_type;
        m_numeric.w = v.m_numeric.w;
        m_numeric.x = v.m_numeric.x;
        m_numeric.y = v.m_numeric.y;
        m_numeric.z = v.m_numeric.z;
        m_string = v.m_string;
        return *this;
    }



    UT_INLINE bool getValueBool(void) const
    {
        if (m_type == VAR_STRING)
            return utStringConverter::toBool(m_string);
        return m_numeric.w != 0;
    }


    UT_INLINE NSfloat getValueReal(void) const
    {
        if (m_type == VAR_STRING)
            return utStringConverter::toFloat(m_string);
        return m_numeric.w;
    }


    UT_INLINE int getValueInt(void) const
    {
        if (m_type == VAR_STRING)
            return utStringConverter::toInt(m_string);
        return (int)m_numeric.w;
    }


    const utString getValueString(void) const
    {
        if (m_type != VAR_STRING)
        {
            if (m_type == VAR_BOOL)
                return utStringConverter::toString(m_numeric.w != 0);
            if (m_type == VAR_REAL)
                return utStringConverter::toString(m_numeric.w);
            if (m_type == VAR_INT)
                return utStringConverter::toString((int)m_numeric.w);
            if (m_type == VAR_VEC2)
                return utStringConverter::toString(NSvec2(m_numeric.w, m_numeric.x));
            if (m_type == VAR_VEC3)
                return utStringConverter::toString(NSvec3(m_numeric.w, m_numeric.x, m_numeric.y));
            if (m_type == VAR_VEC4)
                return utStringConverter::toString(NSvec4(m_numeric.w, m_numeric.x, m_numeric.y,  m_numeric.z));
            if (m_type == VAR_QUAT)
                return utStringConverter::toString(NSquat(m_numeric.w, m_numeric.x, m_numeric.y, m_numeric.z));
         
            return utStringConverter::toString(m_numeric.w != 0);
        }
        return m_string;
    }

    NSvec2 getValueVector2(void) const
    {
        if (m_type == VAR_STRING)
            return utStringConverter::toVec2(m_string);
        return NSvec2(m_numeric.w, m_numeric.x);
    }

    NSvec3 getValueVector3(void) const
    {
        if (m_type == VAR_STRING)
            return utStringConverter::toVec3(m_string);
        return NSvec3(m_numeric.w, m_numeric.x, m_numeric.y);
    }


    NSvec4 getValueVector4(void) const
    {
        if (m_type == VAR_STRING)
            return utStringConverter::toVec4(m_string);
        return NSvec4(m_numeric.w, m_numeric.x, m_numeric.y, m_numeric.z);
    }

    UT_INLINE NSquat getValueQuaternion(void) const
    {
        if (m_type == VAR_STRING)
            return utStringConverter::toQuat(m_string);
        return NSquat(m_numeric.w, m_numeric.x, m_numeric.y, m_numeric.z);
    }

    UT_INLINE bool isDebug(void) const      { return m_debug; }
    UT_INLINE const utString& getName(void) { return m_name; }

private:

    // data types
    mutable NSvec4      m_numeric;
    mutable utString    m_string;
    mutable int         m_type;
    utString            m_name;
    bool                m_debug;
};

#endif//_nsVariable_h_
