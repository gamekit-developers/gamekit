/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C. & Nestor Silveira.

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

template<typename T>
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

	gkIfNode(gkLogicTree *parent, size_t id) 
		: gkLogicNode(parent, id), m_stmt(CMP_NULL)
	{
		ADD_ISOCK(A, false);
		ADD_ISOCK(B, false);
		ADD_OSOCK(IS_TRUE, false);
		ADD_OSOCK(IS_FALSE, false);
	}

    virtual ~gkIfNode() {}

    bool evaluate(gkScalar tick)
	{
		bool result = false;

		switch (m_stmt)
		{
			case CMP_TRUE:
			{
				if (GET_SOCKET(A)->isLinked())
				{
					result = GET_SOCKET_VALUE(A) ? true : false;
				}
				else if (GET_SOCKET(B)->isLinked())
				{
					result = GET_SOCKET_VALUE(B) ? true : false;
				}
				else
				{
					result = GET_SOCKET_VALUE(A) ? true : false;
				}
			}
			break;

			case CMP_NOT:
			case CMP_FALSE:
			{
				if (GET_SOCKET(A)->isLinked())
				{
					result = !GET_SOCKET_VALUE(A);
				}
				else if (GET_SOCKET(B)->isLinked())
				{
					result = !GET_SOCKET_VALUE(B);
				}
				else
				{
					result = !GET_SOCKET_VALUE(A);
				}
			}
			break;

		case CMP_AND:
			result = GET_SOCKET_VALUE(A) && GET_SOCKET_VALUE(B);
			break;
		case CMP_OR:
			result = GET_SOCKET_VALUE(A) || GET_SOCKET_VALUE(B);
			break;
		case CMP_EQUALS:
			result = GET_SOCKET_VALUE(A) == GET_SOCKET_VALUE(B);
			break;
		case CMP_NOT_EQUAL:
			result = GET_SOCKET_VALUE(A) != GET_SOCKET_VALUE(B);
			break;
		case CMP_GREATER:
			result = GET_SOCKET_VALUE(A) > GET_SOCKET_VALUE(B);
			break;
		case CMP_GTHAN:
			result = GET_SOCKET_VALUE(A) >= GET_SOCKET_VALUE(B);
			break;
		case CMP_LESS:
			result = GET_SOCKET_VALUE(A) < GET_SOCKET_VALUE(B);
			break;
		case CMP_LTHAN:
			result = GET_SOCKET_VALUE(A) <= GET_SOCKET_VALUE(B);
			break;
		case CMP_NULL:
		default:
			break;
		}

		SET_SOCKET_VALUE(IS_TRUE, result);
		SET_SOCKET_VALUE(IS_FALSE, !result);

		return false;
	}


    void setStatement(gkBoolStatement stmt) {m_stmt = stmt;}

private:

    gkBoolStatement m_stmt;
};

#endif//_gkIfNode_h_
