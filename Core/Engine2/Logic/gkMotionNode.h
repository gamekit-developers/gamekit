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
#ifndef _gkMotionNode_h_
#define _gkMotionNode_h_

#include "gkLogicNode.h"




class gkMotionNode : public gkLogicNode
{
public:
    gkMotionNode(gkLogicTree *parent, size_t id);
    virtual ~gkMotionNode() {}
    void _initialize();

    void update(gkScalar tick);
    bool evaluate(gkScalar tick);

    void setMotionType(gkMotionTypes t) {m_motionType = t;}
    void setSpace(int ts) {m_space = ts;}

    void keepVelocity() {m_keep = true;}

    void setMinX(gkScalar v);
    void setMaxX(gkScalar v);

    void setMinY(gkScalar v);
    void setMaxY(gkScalar v);

    void setMinZ(gkScalar v);
    void setMaxZ(gkScalar v);

    void setOtherObject(const gkString &obname) {m_otherName = obname;}
private:
    int getLRS();
    void applyConstraints(int lrs);
    void applyObject(gkVector3 &vec);

    gkLogicSocket   m_sockets[5];
    gkMotionTypes   m_motionType;
    int             m_space;
    bool            m_keep;

    short           m_flag;
    gkScalar        x[2], y[2], z[2];
    gkGameObject*   m_current;
    gkString        m_otherName;
};



GK_INLINE void gkMotionNode::setMinX(gkScalar v)
{
    m_flag |= 1;
    x[0] = v;
}


GK_INLINE void gkMotionNode::setMaxX(gkScalar v)
{
    m_flag |= 1;
    x[1] = v;
}


GK_INLINE void gkMotionNode::setMinY(gkScalar v)
{
    m_flag |= 2;
    y[0] = v;
}


GK_INLINE void gkMotionNode::setMaxY(gkScalar v)
{
    m_flag |= 2;
    y[1] = v;
}


GK_INLINE void gkMotionNode::setMinZ(gkScalar v)
{
    m_flag |= 4;
    z[0] = v;
}


GK_INLINE void gkMotionNode::setMaxZ(gkScalar v)
{
    m_flag |= 4;
    z[1] = v;
}





#endif//_gkMotionNode_h_
