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
#ifndef _gkIfNode_h_
#define _gkIfNode_h_

#include "gkLogicNode.h"


class gkVariable;

typedef enum gkBoolStatement
{
    CMP_NULL = 0,
    CMP_TRUE,
    CMP_FALSE,
    CMP_AND,
    CMP_OR,
    CMP_NOT,
    CMP_EQUALS,
    CMP_NOT_EQUAL,
    CMP_GREATER,
    CMP_LESS,
    CMP_GTHAN,
    CMP_LTHAN
} gkBoolStatement;

class gkIfNode : public gkLogicNode
{
public:
    gkIfNode(gkLogicTree *parent, size_t id);
    virtual ~gkIfNode() {}

    bool evaluate(gkScalar tick);

    void setStatement(gkBoolStatement stmt) {m_stmt = stmt;}


    // socket access

    // inputs
    GK_INLINE gkLogicSocket* getA(void)     {return &m_sockets[0];}
    GK_INLINE gkLogicSocket* getB(void)     {return &m_sockets[1];}

    // outputs
    GK_INLINE gkLogicSocket* getTrue(void)  {return &m_sockets[2];}
    GK_INLINE gkLogicSocket* getFalse(void) {return &m_sockets[3];}


private:
    gkLogicSocket   m_sockets[4];
    gkBoolStatement m_stmt;
};

#endif//_gkIfNode_h_
