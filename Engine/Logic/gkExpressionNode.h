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
#ifndef _gkExpressionNode_h_
#define _gkExpressionNode_h_

#include "gkLogicNode.h"

/*
#define EXPR_MAX 13


// not used for now.
// this needs either a custom lexer & parser or the
// use of the current scripting engine to handle
class gkExpressionNode : public gkLogicNode
{
public:
    gkExpressionNode(gkLogicTree *parent, size_t id);
    virtual ~gkExpressionNode() {}

    bool evaluate(gkScalar tick);
    void update(gkScalar tick);
    void initialize();

    // number of expressions to evaluate
    GK_INLINE void setNr(int nr)                  {m_nr = nr;}

    // expression string
    GK_INLINE void setExpr(const gkString &str)   {m_code = str;}

    // inputs
    GK_INLINE gkLogicSocket* getUpdate(void)    { return &m_sockets[0]; }
    GK_INLINE gkLogicSocket* getExpr(int idx) {
        idx = gkClamp<int>(idx, 0, 9);
        return &m_sockets[idx + 1];
    }

    // outputs
    GK_INLINE gkLogicSocket* getTrue(void)  {return &m_sockets[EXPR_MAX - 2];}
    GK_INLINE gkLogicSocket* getFalse(void) {return &m_sockets[EXPR_MAX - 1];}

private:
    class gkScriptExpression *m_expr;
    bool            m_err;
    int             m_nr;
    gkString        m_code;
    gkLogicSocket   m_sockets[EXPR_MAX];
};
*/


#endif//_gkExpressionNode_h_
