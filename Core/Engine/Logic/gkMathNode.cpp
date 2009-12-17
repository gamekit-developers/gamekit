/*
-------------------------------------------------------------------------------
	This file is part of the Ogre GameKit port.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#include "gkMathNode.h"
#include "gkMathUtils.h"


using namespace Ogre;


// ----------------------------------------------------------------------------
gkMathNode::gkMathNode(gkLogicTree *parent, size_t id) :
		gkLogicNode(parent, NT_MATH, id),
		mFunction(MTH_NO_FUNC)
{
	ADD_ISOCK(mSockets[0], this, gkLogicSocket::ST_REAL);
	ADD_ISOCK(mSockets[1], this, gkLogicSocket::ST_REAL);
	ADD_OSOCK(mSockets[2], this, gkLogicSocket::ST_REAL);
}

// ----------------------------------------------------------------------------
void gkMathNode::update(Real tick)
{
	Real a= mSockets[0].getValueReal();
	Real b= mSockets[1].getValueReal();
	Real d= 0.0;

	switch (mFunction)
	{
	case MTH_NO_FUNC:
		d= Real(0.0);
		break;

	case MTH_ADD:
		d= a + b;
		break;

	case MTH_SUBTRACT:
		d= a - b;
		break;

	case MTH_MULTIPLY:
		d= a * b;
		break;

	case MTH_DIVIDE:
		if (b == 0.0)
			b= 1.0;
		d= a / b;
		break;

	case MTH_SINE:
		{
			if (mSockets[0].isLinked())
				d= Math::Sin(Radian(a));
			else
				d= Math::Sin(Radian(b));
		}
		break;
	case MTH_COSINE:
		{
			if (mSockets[0].isLinked())
				d= Math::Cos(Radian(a));
			else
				d= Math::Cos(Radian(b));
		}
		break;
	case MTH_TANGENT:
		{
			if (mSockets[0].isLinked())
				d= Math::Tan(Radian(a));
			else
				d= Math::Tan(Radian(b));
		}
		break;
	case MTH_ARCSINE:
		{
			if (mSockets[0].isLinked())
			{
				if (a <= 1 && a >= -1)
					d= Math::ASin(a).valueDegrees();
				else
					d= 0.0;
			}
			else
			{
				if (b <= 1 && b >= -1)
					d= Math::ASin(b).valueDegrees();
				else
					d= 0.0;
			}
		}
		break;
	case MTH_ARCCOSINE:
		{
			a= Radian(a).valueRadians();
			b= Radian(b).valueRadians();

			if (mSockets[0].isLinked())
			{
				if (a <= 1 && a >= -1)
					d= Math::ACos(a).valueDegrees();
				else
					d= 0.0;
			}
			else
			{
				if (b <= 1 && b >= -1)
					d= Math::ACos(b).valueDegrees();
				else
					d= 0.0;
			}
		}
		break;
	case MTH_ARCTANGENT:
		{
			a= Radian(a).valueRadians();
			b= Radian(b).valueRadians();

			if (mSockets[0].isLinked())
				d= Math::ATan(a).valueDegrees();
			else
				d= Math::ATan(b).valueDegrees();
		}
		break;
	case MTH_POWER:
		d= a > 0 ? Math::Pow(a, b) : 0.0;
		break;
	case MTH_LOGARITHM:
		{
			if (a > 0 && b > 0)
				d= Math::Log(a) / Math::Log(b);
			else d= 0.0;
		}
		break;
	case MTH_MINIMUM:
		{
			d= (a < b) ? a : b;
		}
		break;
	case MTH_MAXIMUM:
		{
			d= (a > b) ? a : b;
		}
		break;
	case MTH_ROUND:
		{
			if (mSockets[0].isLinked())
			{
				Real base= Math::Floor(a);
				Real dec= base - a;
				d= dec > 0.5 ? base + 1 : base;
			}
			else
			{
				Real base= Math::Floor(b);
				Real dec= base - b;
				d= dec > 0.5 ? base + 1 : base;
			}
		}
		break;
	case MTH_LESS_THAN:
		{
			d= (a < b) ? 1.0 : 0.0;
		}
		break;
	case MTH_GREATER_THAN:
		{
			d= (a > b) ? 1.0 : 0.0;
		}
		break;
	default:
		break;
	}

	if (gkFuzzy(d)) d= 0.0;
	//if (gkNan(d)) d= 0.0;
	if (!gkFinite(d)) d= 0.0;

	mSockets[2].setValue(d);
}
