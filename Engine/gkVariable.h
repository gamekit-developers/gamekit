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
#ifndef _gkVariable_h_
#define _gkVariable_h_

#include "gkString.h"
#include "OgreStringConverter.h"
#include "gkCommon.h"
#include "gkMathUtils.h"


// Game property, supports numerical types and string types.
// numerical types from bool to quaternion are stored in the
// gkVector4 structure. All types can be converted to all others.
class gkVariable
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

    gkVariable()
    :       m_numeric(gkVector4::ZERO), m_string(Ogre::StringUtil::BLANK),
            m_type(VAR_NULL), m_name(""), m_debug(false)
    {
    }


    gkVariable(const gkString &n, bool dbg)
    :       m_numeric(gkVector4::ZERO), m_string(n),
            m_type(VAR_NULL), m_name(n), m_debug(dbg)
    {
    }



    GK_INLINE void setValue(float v)                { m_type = VAR_REAL; m_numeric.w = (gkScalar)v; }
    GK_INLINE void setValue(double v)               { m_type = VAR_REAL; m_numeric.w = (gkScalar)v; }
    GK_INLINE void setValue(bool v)                 { m_type = VAR_BOOL; m_numeric.w = v ? 1.0 : 0.0; }
    GK_INLINE void setValue(int v)                  { m_type = VAR_INT; m_numeric.w = (int)v; }
    GK_INLINE void setValue(const gkString& v)      { m_type = VAR_STRING; m_string = v; }
    GK_INLINE void setValue(const gkVector2& v)     { m_type = VAR_VEC2; m_numeric.w = v.x; m_numeric.x = v.y; }
    GK_INLINE void setValue(const gkVector3& v)     { m_type = VAR_VEC3; m_numeric.w = v.x; m_numeric.x = v.y; m_numeric.y = v.z; }
    GK_INLINE void setValue(const gkVector4& v)     { m_type = VAR_VEC4; m_numeric.w = v.w; m_numeric.x = v.x; m_numeric.y = v.y; m_numeric.z = v.z; }
    GK_INLINE void setValue(const gkQuaternion& v)  { m_type = VAR_QUAT; m_numeric.w = v.w; m_numeric.x = v.x; m_numeric.y = v.y; m_numeric.z = v.z; }


    // copy 
    GK_INLINE void setValue(const gkVariable& v)
    {
        m_type = v.m_type;
        m_numeric.w = v.m_numeric.w;
        m_numeric.x = v.m_numeric.x;
        m_numeric.y = v.m_numeric.y;
        m_numeric.z = v.m_numeric.z;
        m_string = v.m_string;
    }


    GK_INLINE bool getValueBool(void) const
    {
        if (m_type == VAR_STRING)
            return Ogre::StringConverter::parseBool(m_string);
        return m_numeric.w != 0;
    }


    GK_INLINE gkScalar getValueReal(void) const
    {
        if (m_type == VAR_STRING)
            return Ogre::StringConverter::parseReal(m_string);
        return m_numeric.w;
    }


    GK_INLINE int getValueInt(void) const
    {
        if (m_type == VAR_STRING)
            return Ogre::StringConverter::parseInt(m_string);
        return (int)m_numeric.w;
    }


    const gkString getValueString(void) const
    {
        if (m_type != VAR_STRING)
        {
            if (m_type == VAR_BOOL)
                return Ogre::StringConverter::toString(m_numeric.w != 0);
            if (m_type == VAR_REAL)
                return Ogre::StringConverter::toString(m_numeric.w);
            if (m_type == VAR_INT)
                return Ogre::StringConverter::toString((int)m_numeric.w);
            if (m_type == VAR_VEC2)
                return Ogre::StringConverter::toString(gkVector2(m_numeric.w, m_numeric.x));
            if (m_type == VAR_VEC3)
                return Ogre::StringConverter::toString(gkVector3(m_numeric.w, m_numeric.x, m_numeric.y));
            if (m_type == VAR_VEC4)
                return Ogre::StringConverter::toString(gkVector4(m_numeric.w, m_numeric.x, m_numeric.y,  m_numeric.z));
            if (m_type == VAR_QUAT)
                return Ogre::StringConverter::toString(gkQuaternion(m_numeric.w, m_numeric.x, m_numeric.y, m_numeric.z));
            return Ogre::StringUtil::BLANK;
        }
        return m_string;
    }

    gkVector2 getValueVector2(void) const
    {
        if (m_type == VAR_STRING)
            return Ogre::StringConverter::parseVector2(m_string);
        return gkVector2(m_numeric.w, m_numeric.x);
    }

    gkVector3 getValueVector3(void) const
    {
        if (m_type == VAR_STRING)
            return Ogre::StringConverter::parseVector3(m_string);
        return gkVector3(m_numeric.w, m_numeric.x, m_numeric.y);
    }


    gkVector4 getValueVector4(void) const
    {
        if (m_type == VAR_STRING)
            return Ogre::StringConverter::parseVector4(m_string);
        return gkVector4(m_numeric.w, m_numeric.x, m_numeric.y, m_numeric.z);
    }

    GK_INLINE gkQuaternion getValueQuaternion(void) const
    {
        if (m_type == VAR_STRING)
            return Ogre::StringConverter::parseQuaternion(m_string);
        return gkQuaternion(m_numeric.w, m_numeric.x, m_numeric.y, m_numeric.z);
    }

    GK_INLINE bool isDebug(void) const      { return m_debug; }
    GK_INLINE const gkString& getName(void) { return m_name; }

private:

    // data types
    mutable gkVector4   m_numeric;
    mutable gkString    m_string;
    mutable int         m_type;
    gkString            m_name;
    bool                m_debug;
};

#endif//_gkVariable_h_
