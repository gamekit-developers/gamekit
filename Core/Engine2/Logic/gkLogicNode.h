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
#ifndef _gkLogicNode_h_
#define _gkLogicNode_h_

#include "gkLogicCommon.h"
#include "gkLogicSocket.h"
#include "gkString.h"



class gkGameObject;
class gkLogicTree;
class LogicLink;


class gkLogicNode
{
public:
    gkLogicNode(gkLogicTree *parent, const gkNodeTypes& nt, size_t handle);
    virtual ~gkLogicNode() {}

    // do the update logic
    virtual void update(gkScalar tick);
    // see if an update is wanted
    virtual bool evaluate(gkScalar tick);
    // do first run initialization
    virtual void _initialize();


    void block(bool truth);
    bool isBlocked();


    gkLogicSocket*    getInputSocket(size_t index);
    gkLogicSocket*    getOutputSocket(size_t index);

    void                attachObject(gkGameObject *);
    gkGameObject*       getAttachedObject();

    gkNodeTypes         getType();
    const size_t        getHandle();
    bool                hasLinks();
    void                setLinked();


    gkSocketBaseIterator getInputs();
    gkSocketBaseIterator getOutputs();

protected:
    bool            m_blocked;
    const size_t    m_handle;
    gkGameObject*   m_object, *m_other;
    gkNodeTypes     m_type;
    gkLogicTree*    m_parent;
    bool            m_hasLinks;
    gkSocketBase    m_inputs;
    gkSocketBase    m_outputs;
};


GK_INLINE gkNodeTypes gkLogicNode::getType()
{
    return m_type;
}


GK_INLINE void gkLogicNode::attachObject(gkGameObject * ob)
{
    m_object = ob;
}


GK_INLINE gkGameObject* gkLogicNode::getAttachedObject()
{
    return m_other != 0 ? m_other : m_object;
}


GK_INLINE const size_t gkLogicNode::getHandle()
{
    return m_handle;
}


GK_INLINE bool gkLogicNode::hasLinks()
{
    return m_hasLinks;
}


GK_INLINE void gkLogicNode::setLinked()
{
    m_hasLinks = true;
}


GK_INLINE bool gkLogicNode::evaluate(gkScalar tick)
{
    /* the default */
    return true;
}


GK_INLINE void gkLogicNode::_initialize()
{
    /* none needed */
}


GK_INLINE bool gkLogicNode::isBlocked()
{
    return m_blocked;
}


GK_INLINE void gkLogicNode::update(gkScalar tick)
{
    /* no update logic needed */
}


GK_INLINE gkSocketBaseIterator gkLogicNode::getInputs()
{
    return gkSocketBaseIterator(m_inputs.begin(), m_inputs.end());
}


GK_INLINE gkSocketBaseIterator gkLogicNode::getOutputs()
{
    return gkSocketBaseIterator(m_outputs.begin(), m_outputs.end());
}


GK_INLINE void gkLogicNode::block(bool truth)
{
    if (truth != m_blocked) m_blocked = truth;
}




#endif//_gkLogicNode_h_
