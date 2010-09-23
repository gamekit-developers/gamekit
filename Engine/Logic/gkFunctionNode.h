/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Nestor Silveira.

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
#ifndef _gkFunctionNode_h_
#define _gkFunctionNode_h_

#include "gkLogicNode.h"

enum GK_FUNCTION_NODE_PARAM
{
	FUNCTION_NODE_PARAM_NONE,
	FUNCTION_NODE_PARAM_ONE,
	FUNCTION_NODE_PARAM_TWO
};

template < typename OBJ, typename R, GK_FUNCTION_NODE_PARAM parameter = FUNCTION_NODE_PARAM_NONE, typename PARAM = int >
class gkFunctionNode : public gkLogicNode
{
public:

	enum
	{
		OBJECT,
		FUNCTION_0, // without param
		FUNCTION_1, // with tick
		FUNCTION_2, // with tick and param
		PARAMETER,
		RESULT
	};

	typedef R (OBJ::*F0)();
	typedef R (OBJ::*F1)(gkScalar);
	typedef R (OBJ::*F2)(gkScalar, PARAM);

	DECLARE_SOCKET_TYPE(OBJECT, OBJ*);
	DECLARE_SOCKET_TYPE(FUNCTION_0, F0);
	DECLARE_SOCKET_TYPE(FUNCTION_1, F1);
	DECLARE_SOCKET_TYPE(FUNCTION_2, F2);
	DECLARE_SOCKET_TYPE(PARAMETER, PARAM);
	DECLARE_SOCKET_TYPE(RESULT, R);

	gkFunctionNode(gkLogicTree* parent, size_t id)
		: gkLogicNode(parent, id)
	{
		ADD_ISOCK(OBJECT, 0);
		ADD_ISOCK(FUNCTION_0, 0);
		ADD_ISOCK(FUNCTION_1, 0);
		ADD_ISOCK(FUNCTION_2, 0);
		ADD_ISOCK(PARAMETER, PARAM());
		ADD_OSOCK(RESULT, R());
	}

	virtual ~gkFunctionNode() {}

	bool evaluate(gkScalar tick, Int2Type<FUNCTION_NODE_PARAM_NONE>)
	{
		F0 f0 = GET_SOCKET_VALUE(FUNCTION_0);

		GK_ASSERT(f0);

		SET_SOCKET_VALUE(RESULT, (GET_SOCKET_VALUE(OBJECT)->*f0)());

		return false;
	}

	bool evaluate(gkScalar tick, Int2Type<FUNCTION_NODE_PARAM_ONE>)
	{
		F1 f1 = GET_SOCKET_VALUE(FUNCTION_1);

		GK_ASSERT(f1);

		SET_SOCKET_VALUE(RESULT, (GET_SOCKET_VALUE(OBJECT)->*f1)(tick));

		return false;
	}

	bool evaluate(gkScalar tick, Int2Type<FUNCTION_NODE_PARAM_TWO>)
	{
		F2 f2 = GET_SOCKET_VALUE(FUNCTION_2);

		GK_ASSERT(f2);

		SET_SOCKET_VALUE(RESULT, (GET_SOCKET_VALUE(OBJECT)->*f2)(tick, GET_SOCKET_VALUE(PARAMETER)));

		return false;
	}

	bool evaluate(gkScalar tick)
	{
		return evaluate(tick, Int2Type<parameter>());
	}
};

#endif//_gkFunctionNode_h_
