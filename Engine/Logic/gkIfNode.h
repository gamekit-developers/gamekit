/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C & Nestor Silveira.

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

template<typename T, gkBoolStatement stmt>
class gkIfNode : public gkLogicNode
{
public:

	enum
	{
		A,
		B,
		IS_TRUE,
		IS_FALSE
	};

	DECLARE_SOCKET_TYPE(A, T);
	DECLARE_SOCKET_TYPE(B, T);
	DECLARE_SOCKET_TYPE(IS_TRUE, bool);
	DECLARE_SOCKET_TYPE(IS_FALSE, bool);

	gkIfNode(gkLogicTree* parent, size_t id)
		: gkLogicNode(parent, id)
	{
		ADD_ISOCK(A, T());
		ADD_ISOCK(B, T());
		ADD_OSOCK(IS_TRUE, false);
		ADD_OSOCK(IS_FALSE, false);
	}

	virtual ~gkIfNode() {}

	bool evaluate(gkScalar tick)
	{
		bool result = doIf(Int2Type<stmt>());

		SET_SOCKET_VALUE(IS_TRUE, result);
		SET_SOCKET_VALUE(IS_FALSE, !result);

		return false;
	}

	bool doIf(Int2Type<CMP_TRUE>)
	{
		if (GET_SOCKET(A)->isLinked())
		{
			return GET_SOCKET_VALUE(A) ? true : false;
		}
		else if (GET_SOCKET(B)->isLinked())
		{
			return GET_SOCKET_VALUE(B) ? true : false;
		}
		else
		{
			return GET_SOCKET_VALUE(A) ? true : false;
		}
	}

	bool doIf(Int2Type<CMP_FALSE>)
	{
		if (GET_SOCKET(A)->isLinked())
		{
			return !GET_SOCKET_VALUE(A);
		}
		else if (GET_SOCKET(B)->isLinked())
		{
			return !GET_SOCKET_VALUE(B);
		}
		else
		{
			return !GET_SOCKET_VALUE(A);
		}
	}

	bool doIf(Int2Type<CMP_NOT>)
	{
		if (GET_SOCKET(A)->isLinked())
		{
			return !GET_SOCKET_VALUE(A);
		}
		else if (GET_SOCKET(B)->isLinked())
		{
			return !GET_SOCKET_VALUE(B);
		}
		else
		{
			return !GET_SOCKET_VALUE(A);
		}
	}

	bool doIf(Int2Type<CMP_AND>)
	{
		return GET_SOCKET_VALUE(A) && GET_SOCKET_VALUE(B);
	}

	bool doIf(Int2Type<CMP_OR>)
	{
		return GET_SOCKET_VALUE(A) || GET_SOCKET_VALUE(B);
	}

	bool doIf(Int2Type<CMP_EQUALS>)
	{
		return GET_SOCKET_VALUE(A) == GET_SOCKET_VALUE(B);
	}

	bool doIf(Int2Type<CMP_NOT_EQUAL>)
	{
		return GET_SOCKET_VALUE(A) != GET_SOCKET_VALUE(B);
	}

	bool doIf(Int2Type<CMP_GREATER>)
	{
		return GET_SOCKET_VALUE(A) > GET_SOCKET_VALUE(B);
	}

	bool doIf(Int2Type<CMP_GTHAN>)
	{
		return GET_SOCKET_VALUE(A) >= GET_SOCKET_VALUE(B);
	}

	bool doIf(Int2Type<CMP_LESS>)
	{
		return GET_SOCKET_VALUE(A) < GET_SOCKET_VALUE(B);
	}

	bool doIf(Int2Type<CMP_LTHAN>)
	{
		return GET_SOCKET_VALUE(A) <= GET_SOCKET_VALUE(B);
	}

	bool doIf(Int2Type<CMP_FIND>)
	{
		return gkString::npos != GET_SOCKET_VALUE(A).find(GET_SOCKET_VALUE(B));
	}

};

#endif//_gkIfNode_h_
