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
#ifndef _gkMathNode_h_
#define _gkMathNode_h_

#include "gkLogicNode.h"

template<typename T, gkMathFunction func>
class gkMathNode : public gkLogicNode
{
public:

	enum
	{
		A,
		B,
		RESULT
	};

	DECLARE_SOCKET_TYPE(A, T);
	DECLARE_SOCKET_TYPE(B, T);
	DECLARE_SOCKET_TYPE(RESULT, T);

	gkMathNode(gkLogicTree* parent, size_t id)
		: gkLogicNode(parent, id)
	{
		ADD_ISOCK(A, T());
		ADD_ISOCK(B, T());
		ADD_OSOCK(RESULT, T());
	}

	virtual ~gkMathNode() {}

	void update(gkScalar tick)
	{
		m_a = GET_SOCKET_VALUE(A);
		m_b = GET_SOCKET_VALUE(B);

		T d = doMath(Int2Type<func>());

		SET_SOCKET_VALUE(RESULT, d);
	}

	T doMath(Int2Type<MTH_ADD>)
	{
		return m_a + m_b;
	}

	T doMath(Int2Type<MTH_SUBTRACT>)
	{
		return m_a - m_b;
	}

	T doMath(Int2Type<MTH_MULTIPLY>)
	{
		return m_a * m_b;
	}

	T doMath(Int2Type<MTH_DIVIDE>)
	{
		return m_a / m_b;
	}

	T doMath(Int2Type<MTH_SINE>)
	{
		if (GET_SOCKET(A)->isLinked())
			return gkMath::Sin(gkRadian(m_a));
		else
			return gkMath::Sin(gkRadian(m_b));
	}

	T doMath(Int2Type<MTH_COSINE>)
	{
		if (GET_SOCKET(A)->isLinked())
			return gkMath::Cos(gkRadian(m_a));
		else
			return gkMath::Cos(gkRadian(m_b));
	}

	T doMath(Int2Type<MTH_TANGENT>)
	{
		if (GET_SOCKET(A)->isLinked())
			return gkMath::Tan(gkRadian(m_a));
		else
			return gkMath::Tan(gkRadian(m_b));
	}

	T doMath(Int2Type<MTH_ARCSINE>)
	{
		m_a = gkRadian(m_a).valueRadians();
		m_b = gkRadian(m_b).valueRadians();

		if (GET_SOCKET(A)->isLinked())
		{
			if (m_a <= 1 && m_a >= -1)
				return gkMath::ASin(m_a).valueDegrees();
			else
				return 0;
		}
		else
		{
			if (m_b <= 1 && m_b >= -1)
				return gkMath::ASin(m_b).valueDegrees();
			else
				return 0;
		}
	}

	T doMath(Int2Type<MTH_ARCCOSINE>)
	{
		m_a = gkRadian(m_a).valueRadians();
		m_b = gkRadian(m_b).valueRadians();

		if (GET_SOCKET(A)->isLinked())
		{
			if (m_a <= 1 && m_a >= -1)
				return gkMath::ACos(m_a).valueDegrees();
			else
				return 0;
		}
		else
		{
			if (m_b <= 1 && m_b >= -1)
				return gkMath::ACos(m_b).valueDegrees();
			else
				return 0;
		}
	}

	T doMath(Int2Type<MTH_ARCTANGENT>)
	{
		m_a = gkRadian(m_a).valueRadians();
		m_b = gkRadian(m_b).valueRadians();

		if (GET_SOCKET(A)->isLinked())
		{
			if (m_a <= 1 && m_a >= -1)
				return gkMath::ATan(m_a).valueDegrees();
			else
				return 0;
		}
		else
		{
			if (m_b <= 1 && m_b >= -1)
				return gkMath::ATan(m_b).valueDegrees();
			else
				return 0;
		}
	}

	T doMath(Int2Type<MTH_POWER>)
	{
		return m_a > 0 ? gkMath::Pow(m_a, m_b) : 0;
	}

	T doMath(Int2Type<MTH_LOGARITHM>)
	{
		if (m_a > 0 && m_b > 0)
			return gkMath::Log(m_a) / gkMath::Log(m_b);
		else
			return 0;
	}

	T doMath(Int2Type<MTH_MINIMUM>)
	{
		return (m_a < m_b) ? m_a : m_b;
	}

	T doMath(Int2Type<MTH_MAXIMUM>)
	{
		return (m_a > m_b) ? m_a : m_b;
	}

	T doMath(Int2Type<MTH_ROUND>)
	{
		if (GET_SOCKET(A)->isLinked())
		{
			gkScalar base = gkMath::Floor(m_a);
			gkScalar dec = base - m_a;
			return dec > 0.5 ? base + 1 : base;
		}
		else
		{
			gkScalar base = gkMath::Floor(m_b);
			gkScalar dec = base - m_b;
			return dec > 0.5 ? base + 1 : base;
		}
	}

	T doMath(Int2Type<MTH_LESS_THAN>)
	{
		return (m_a < m_b) ? 1 : 0;
	}

	T doMath(Int2Type<MTH_GREATER_THAN>)
	{
		return (m_a > m_b) ? 1 : 0;
	}
private:
	T m_a;
	T m_b;
};


#endif//_gkMathNode_h_
