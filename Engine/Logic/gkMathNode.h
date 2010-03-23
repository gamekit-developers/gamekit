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

	gkMathNode(gkLogicTree *parent, size_t id) 
		: gkLogicNode(parent, id) 
	{
		ADD_ISOCK(A, T());
		ADD_ISOCK(B, T());
		ADD_OSOCK(RESULT, T());
	}

    virtual ~gkMathNode() {}

    void update(gkScalar tick)
	{
		T a = GET_SOCKET_VALUE(A);
		T b = GET_SOCKET_VALUE(B);
		T d;

		d = doMath(Int2Type<func>());

		SET_SOCKET_VALUE(RESULT, d);
	}

    GK_INLINE gkILogicSocket* getA() {return GET_SOCKET(A);}
    GK_INLINE gkILogicSocket* getB() {return GET_SOCKET(B);}
    GK_INLINE gkILogicSocket* getResult() {return GET_SOCKET(RESULT);}

	T doMath(Int2Type<MTH_ADD>)
	{
		return a + b;
	}

	T doMath(Int2Type<MTH_SUBTRACT>)
	{
		return a - b;
	}

	T doMath(Int2Type<MTH_MULTIPLY>)
	{
		return a * b;
	}

	T doMath(Int2Type<MTH_DIVIDE>)
	{
		return a / b;
	}

	T doMath(Int2Type<MTH_SINE>)
	{
		if (GET_SOCKET(A)->isLinked())
			return gkMath::Sin(gkRadian(a));
		else
			return gkMath::Sin(gkRadian(b));
	}

	T doMath(Int2Type<MTH_COSINE>)
	{
		if (GET_SOCKET(A)->isLinked())
			return gkMath::Cos(gkRadian(a));
		else
			return gkMath::Cos(gkRadian(b));
	}

	T doMath(Int2Type<MTH_TANGENT>)
	{
		if (GET_SOCKET(A)->isLinked())
			return gkMath::Tan(gkRadian(a));
		else
			return gkMath::Tan(gkRadian(b));
	}

	T doMath(Int2Type<MTH_ARCSINE>)
	{
		a = gkRadian(a).valueRadians();
		b = gkRadian(b).valueRadians();

		if (GET_SOCKET(A)->isLinked())
		{
			if (a <= 1 && a >= -1)
				return gkMath::ASin(a).valueDegrees();
			else
				return 0;
		}
		else
		{
			if (b <= 1 && b >= -1)
				return gkMath::ASin(b).valueDegrees();
			else
				return 0;
		}
	}

	T doMath(Int2Type<MTH_ARCCOSINE>)
	{
		a = gkRadian(a).valueRadians();
		b = gkRadian(b).valueRadians();

		if (GET_SOCKET(A)->isLinked())
		{
			if (a <= 1 && a >= -1)
				return gkMath::ACos(a).valueDegrees();
			else
				return 0;
		}
		else
		{
			if (b <= 1 && b >= -1)
				return gkMath::ACos(b).valueDegrees();
			else
				return 0;
		}
	}

	T doMath(Int2Type<MTH_ARCTANGENT>)
	{
		a = gkRadian(a).valueRadians();
		b = gkRadian(b).valueRadians();

		if (GET_SOCKET(A)->isLinked())
		{
			if (a <= 1 && a >= -1)
				return gkMath::ATan(a).valueDegrees();
			else
				return 0;
		}
		else
		{
			if (b <= 1 && b >= -1)
				return gkMath::ATan(b).valueDegrees();
			else
				return 0;
		}
	}

	T doMath(Int2Type<MTH_POWER>)
	{
		return a > 0 ? gkMath::Pow(a, b) : 0;
	}

	T doMath(Int2Type<MTH_LOGARITHM>)
	{
		if (a > 0 && b > 0)
			return gkMath::Log(a) / gkMath::Log(b);
		else 
			return 0;
	}

	T doMath(Int2Type<MTH_MINIMUM>)
	{
		return (a < b) ? a : b;
	}

	T doMath(Int2Type<MTH_MAXIMUM>)
	{
		return (a > b) ? a : b;
	}

	T doMath(Int2Type<MTH_ROUND>)
	{
		if (GET_SOCKET(A)->isLinked())
		{
			gkScalar base = gkMath::Floor(a);
			gkScalar dec = base - a;
			return dec > 0.5 ? base + 1 : base;
		}
		else
		{
			gkScalar base = gkMath::Floor(b);
			gkScalar dec = base - b;
			return dec > 0.5 ? base + 1 : base;
		}
	}

	T doMath(Int2Type<MTH_LESS_THAN>)
	{
		return (a < b) ? 1 : 0;
	}

	T doMath(Int2Type<MTH_GREATER_THAN>)
	{
		return (a > b) ? 1 : 0;
	}
};


#endif//_gkMathNode_h_
