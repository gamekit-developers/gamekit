/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): Nestor Silveira.
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
#ifndef _gkVariableNode_h_
#define _gkVariableNode_h_

#include "gkLogicNode.h"

class gkVariable;

// these need to be reevaluated
class gkVariableNode : public gkLogicNode
{
public:

	enum
	{
		SET,
		VALUE,
		RESULT
	};

	DECLARE_SOCKET_TYPE(SET, bool);
	DECLARE_SOCKET_TYPE(VALUE, gkVariable);
	DECLARE_SOCKET_TYPE(RESULT, gkVariable);

    gkVariableNode(gkLogicTree *parent, size_t id);
    virtual ~gkVariableNode() {}

    bool evaluate(gkScalar tick);
    void initialize();

    GK_INLINE void setName(const gkString &varname) {m_varName = varname;}
    GK_INLINE void setDebug(void)                   {m_debug = true;}

private:
    gkString        m_varName;
    gkVariable*     m_prop;
    bool            m_debug;
};


class VariableOpNode : public gkLogicNode
{
public:

	enum
	{
		SET,
		VALUE,
		RESULT
	};

	DECLARE_SOCKET_TYPE(SET, bool);
	DECLARE_SOCKET_TYPE(VALUE, gkVariable);
	DECLARE_SOCKET_TYPE(RESULT, gkVariable);

    VariableOpNode(gkLogicTree *parent, size_t id);
    virtual ~VariableOpNode() {}

    bool evaluate(gkScalar tick);
    void initialize();

    GK_INLINE void setFunction(int func)              {m_function = func;}
    GK_INLINE void setName(const gkString &varname)   {m_varName = varname;}
    GK_INLINE void setToDegree(bool v)                {m_deg = v;}

private:
    int             m_function;
    gkString        m_varName;
    bool            m_deg;
    gkVariable*     m_prop;
};

#endif//_gkVariableNode_h_
