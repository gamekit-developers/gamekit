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
#ifndef _gkLogicSocket_h_
#define _gkLogicSocket_h_

#include "gkLogicCommon.h"
#include "gkMathUtils.h"
#include "OgreStringConverter.h"
#include "gkVariable.h"




class gkLogicSocket
{
public:

    typedef enum SocketType
    {
        ST_NULL = 0,
        ST_BOOL,
        ST_INT,
        ST_REAL,
        ST_VECTOR,
        ST_EULER,
        ST_QUAT,
        ST_STRING,
        ST_VARIABLE,
    }SocketType;

public:
    gkLogicSocket();
    gkLogicSocket(gkLogicNode* par, SocketType stype);

    void            link(gkLogicSocket *fsock);
    const           gkLogicSocket& getLink();

    gkLogicNode*        getParent() const;
    gkLogicSocket*      getFrom() const;
    const SocketType    getType() const;
    bool                isLinked() const;
    bool                isConnected() const;
    bool                isBlocked() const;

    void setValue(bool v);
    void setValue(int v);
    void setValue(gkScalar v);
    void setValue(const gkVector3& v);
    void setValue(const gkQuaternion& v);
    void setValue(const gkString& str);
    void setValue(const gkVariable& prop);
    void setValue(gkVariable* prop);

    bool            getValueBool() const;
    int             getValueInt() const;
    gkScalar        getValueReal() const;
    gkVector3       getValueVector3() const;
    gkQuaternion    getValueQuaternion() const;
    gkString        getValueString() const;
    gkVariable      getValueVariable() const;

    gkGameObject* getGameObject()const;
    void setConnected();

    void setName(const gkString& v);
    gkString getName();
    gkString getRealName();

    void copy(const gkLogicSocket &o);

    void block(bool truth);
    void push_link(gkLogicNode *link);
    bool has_link(gkLogicNode *link);

    gkNodeBaseIterator getNodeIterator();


private:
    bool                m_connected, m_blocked;
    gkVector4           m_data;      /* in w,x,y,z order */
    gkString            m_strData;   /* string pointer */
    gkVariable          m_var;       /* gkVariable data */
    SocketType          m_type;      /* base type*/
    gkLogicSocket*      m_from;      /* fron socket to 'this' */
    gkLogicNode*        m_parent;    /* owner node */
    gkString            m_name;      /* Blender name */
    gkLogicNodeBase     m_nodes;     /* Subtree for io blocking */

};


GK_INLINE gkLogicSocket::gkLogicSocket() :
        m_connected(false), m_blocked(false), m_data(gkVector4::ZERO), m_strData(""),
        m_var(), m_type(ST_NULL), m_from(0), m_parent(0), m_name("")
{
}


GK_INLINE gkLogicSocket::gkLogicSocket(gkLogicNode* par, SocketType stype) :
        m_connected(false), m_blocked(false), m_data(gkVector4::ZERO), m_strData(""),
        m_var(), m_type(stype), m_from(0), m_parent(par),  m_name("")
{
}


GK_INLINE const gkLogicSocket::SocketType gkLogicSocket::getType() const
{
    return m_type;
}


GK_INLINE gkLogicNode* gkLogicSocket::getParent() const
{
    return m_parent;
}


GK_INLINE bool gkLogicSocket::isLinked() const
{
    return m_from != 0;
}


GK_INLINE bool gkLogicSocket::isConnected() const
{
    return m_connected;
}


GK_INLINE gkLogicSocket* gkLogicSocket::getFrom() const
{
    return m_from;
}


GK_INLINE const gkLogicSocket& gkLogicSocket::getLink()
{
    /* const reference to linked m_data */
    if (m_from)
        return *m_from;
    /* self */
    return *this;
}


GK_INLINE void gkLogicSocket::setValue(bool v)
{
    m_data.w = v ? 1.0 : 0.0;
}


GK_INLINE void gkLogicSocket::setValue(int v)
{
    m_data.w = v;
}


GK_INLINE void gkLogicSocket::setValue(gkScalar v)
{
    m_data.w = v;
}


GK_INLINE void gkLogicSocket::setValue(const gkVector3& v)
{
    m_data.w = v.x;
    m_data.x = v.y;
    m_data.y = v.z;
}


GK_INLINE void gkLogicSocket::setValue(const gkQuaternion& v)
{
    m_data.w = v.w;
    m_data.x = v.x;
    m_data.y = v.y;
    m_data.z = v.z;
}


GK_INLINE void gkLogicSocket::setValue(const gkVariable &prop)
{
    m_var.setValue(prop);
}


GK_INLINE void gkLogicSocket::setValue(gkVariable *prop)
{
    if (prop)
        m_var.setValue(*prop);
}


GK_INLINE void gkLogicSocket::setValue(const gkString& str)
{
    m_strData = str;
}


GK_INLINE bool gkLogicSocket::getValueBool() const
{
    /* recursion should be OK, cannot have m_from->m_from .... */
    if (m_from)
        return m_from->getValueBool();

    if (m_type == ST_VARIABLE)
        return m_var.getValueBool();
    if (m_type == ST_STRING)
        return Ogre::StringConverter::parseBool(m_strData);
    return m_data.w != 0;
}


GK_INLINE int gkLogicSocket::getValueInt() const
{
    if (m_from)
        return m_from->getValueInt();

    if (m_type == ST_VARIABLE)
        return m_var.getValueInt();

    if (m_type == ST_STRING)
        return Ogre::StringConverter::parseInt(m_strData);
    return (int)m_data.w;
}


GK_INLINE gkScalar gkLogicSocket::getValueReal() const
{
    if (m_from)
        return m_from->getValueReal();

    if (m_type == ST_VARIABLE)
        return m_var.getValueReal();
    if (m_type == ST_STRING)
        return Ogre::StringConverter::parseReal(m_strData);

    return m_data.w;
}


GK_INLINE gkVector3 gkLogicSocket::getValueVector3() const
{
    if (m_from)
        return m_from->getValueVector3();


    if (m_type == ST_VARIABLE)
        return m_var.getValueVector3();
    if (m_type == ST_STRING)
        return Ogre::StringConverter::parseVector3(m_strData);
    return gkVector3(m_data.w, m_data.x, m_data.y);
}


GK_INLINE gkQuaternion gkLogicSocket::getValueQuaternion() const
{
    if (m_from)
        return m_from->getValueQuaternion();

    if (m_type == ST_VARIABLE)
        return m_var.getValueQuaternion();
    if (m_type == ST_STRING)
        return Ogre::StringConverter::parseQuaternion(m_strData);
    if (m_type == ST_EULER)
        return gkMathUtils::getQuatFromEuler(gkVector3(m_data.w, m_data.x, m_data.y), true);
    return gkQuaternion(m_data.w, m_data.x, m_data.y, m_data.z);
}


GK_INLINE gkString gkLogicSocket::getValueString() const
{
    if (m_from)
        return m_from->getValueString();

    if (m_type == ST_VARIABLE)
        return m_var.getValueString();

    if (m_type != ST_STRING)
    {
        if (m_type == ST_BOOL)
            return Ogre::StringConverter::toString(m_data.w != 0);
        if (m_type == ST_INT)
            return Ogre::StringConverter::toString((int)m_data.w);
        if (m_type == ST_REAL)
            return Ogre::StringConverter::toString(m_data.w);
        if (m_type == ST_VECTOR || m_type == ST_EULER)
            return Ogre::StringConverter::toString(gkVector3(m_data.w, m_data.x, m_data.y));
        if (m_type == ST_QUAT)
            return Ogre::StringConverter::toString(gkQuaternion(m_data.w, m_data.x, m_data.y, m_data.z));
    }
    return m_strData;
}


GK_INLINE gkVariable gkLogicSocket::getValueVariable() const
{
    if (m_from)
        return m_from->getValueVariable();

    if (m_type == ST_VARIABLE)
        return m_var;
    if (m_type == ST_BOOL)
    {
        gkVariable prop;
        prop.setValue(m_data.w != 0);
        return prop;
    }
    if (m_type == ST_INT)
    {
        gkVariable prop;
        prop.setValue((int)m_data.w);
        return prop;
    }
    if (m_type == ST_REAL)
    {
        gkVariable prop;
        prop.setValue(m_data.w);
        return prop;
    }
    if (m_type == ST_VECTOR || m_type == ST_EULER)
    {
        gkVariable prop;
        prop.setValue(gkVector3(m_data.w, m_data.x, m_data.y));
        return prop;
    }
    if (m_type == ST_QUAT)
    {
        gkVariable prop;
        prop.setValue(gkQuaternion(m_data.w, m_data.x, m_data.y, m_data.z));
        return prop;
    }

    /*gkString */
    gkVariable prop;
    prop.setValue(m_strData);
    return prop;
}


GK_INLINE void gkLogicSocket::copy(const gkLogicSocket &o)
{
    m_connected = o.m_connected;
    m_data = o.m_data;
    m_strData = o.m_strData;
    m_var = o.m_var;
    m_type = o.m_type;
    m_from = o.m_from;
    m_parent = o.m_parent;
}


GK_INLINE bool gkLogicSocket::isBlocked() const
{
    return m_blocked;
}


GK_INLINE void gkLogicSocket::setConnected()
{
    m_connected = true;
}


GK_INLINE void gkLogicSocket::setName(const gkString& v)
{
    m_name = v;
}


GK_INLINE gkString gkLogicSocket::getName()
{
    if (m_from)
        return m_from->m_name;
    return m_name;
}


GK_INLINE gkString gkLogicSocket::getRealName()
{
    return m_name;
}


GK_INLINE gkNodeBaseIterator gkLogicSocket::getNodeIterator()
{
    return gkNodeBaseIterator(m_nodes.begin(), m_nodes.end());
}




/* utility defines for nodes */
#define ADD_ISOCK(dest, parent, type){      \
    dest= gkLogicSocket(parent, type);      \
    m_inputs.push_back(&dest);              \
}

#define ADD_OSOCK(dest, parent, type){      \
    dest= gkLogicSocket(parent, type);      \
    m_outputs.push_back(&dest);             \
}

#define CHECK_RETV(cond) {                  \
    if ((cond)) return;                     \
}

#endif//_gkLogicSocket_h_
