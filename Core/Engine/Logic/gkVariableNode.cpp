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
#include "gkVariableNode.h"
#include "gkVariable.h"
#include "gkGameObject.h"


using namespace Ogre;


// ----------------------------------------------------------------------------
gkVariableNode::gkVariableNode(gkLogicTree *parent, size_t id):
		gkLogicNode(parent, NT_VARIABLE, id), mDebug(false), mProp(0), mVarName("")
{
	ADD_ISOCK(mSockets[0], this, gkLogicSocket::ST_BOOL);
	ADD_ISOCK(mSockets[1], this, gkLogicSocket::ST_VARIABLE);
	ADD_OSOCK(mSockets[2], this, gkLogicSocket::ST_VARIABLE);
}

// ----------------------------------------------------------------------------
void gkVariableNode::_initialize()
{
	if (!mVarName.empty())
	{
		gkGameObject *ob= mSockets[0].getGameObject();
		if (ob != 0)
		{
			if (!ob->hasVariable(mVarName))
				mProp= ob->createVariable(mVarName, mDebug);
			else
				mProp= ob->getVariable(mVarName);

			if (mProp && mSockets[0].getValueBool())
				mProp->setValue(mSockets[1].getValueVariable());
		}
	}
}

// ----------------------------------------------------------------------------
bool gkVariableNode::evaluate(Real tick)
{
	if (!mProp) return false;


	mSockets[2].setValue(mProp);
	if (!mSockets[0].isLinked() && !mSockets[1].isLinked())
	{
		/* variable will not change */

		/* TODO, tell tree to delete this */
		return false;
	}

	if (mSockets[0].getValueBool())
		mProp->setValue(mSockets[1].getValueVariable());
	return false;
}

// ----------------------------------------------------------------------------
VariableOpNode::VariableOpNode(gkLogicTree *parent, size_t id) :
		gkLogicNode(parent, NT_VARIABLE_OP, id),
		mFunction(MTH_NO_FUNC), mDeg(false), mProp(0), mVarName("")
{
	ADD_ISOCK(mSockets[0], this, gkLogicSocket::ST_BOOL);
	ADD_ISOCK(mSockets[1], this, gkLogicSocket::ST_VARIABLE);
	ADD_OSOCK(mSockets[2], this, gkLogicSocket::ST_VARIABLE);
}

// ----------------------------------------------------------------------------
void VariableOpNode::_initialize()
{
	if (!mVarName.empty())
	{
		gkGameObject *ob= mSockets[0].getGameObject();
		if (ob != 0)
			mProp= ob->getVariable(mVarName);
	}
}

// ----------------------------------------------------------------------------
bool VariableOpNode::evaluate(Real tick)
{
	if (!mProp || !mSockets[0].getValueBool())
		return false;

	Real a= mProp->getValueReal();
	Real b= mSockets[1].getValueReal();
	Real d= 0.0;

	switch (mFunction)
	{
	case MTH_NO_FUNC:
		d= 0.0;
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
		d= (b == 0.0) ? 0.0 : a / b;
		break;
	case MTH_SINE:
		{
			d= Math::Sin(Radian(a));
			if (mDeg) d= d * Math::fRad2Deg;
		}
		break;
	case MTH_COSINE:
		{
			d= Math::Cos(Radian(a));
			if (mDeg) d= d * Math::fRad2Deg;
		}
		break;
	case MTH_TANGENT:
		{
			d= Math::Tan(Radian(a));
			if (mDeg) d= d * Math::fRad2Deg;
		}
		break;
	case MTH_ARCSINE:
		{
			if (a <= 1 && a >= -1)
				d= Math::ASin(a).valueRadians();
			else
				d= 0.0;

			if (mDeg) d= d * Math::fRad2Deg;

		}
		break;
	case MTH_ARCCOSINE:
		{
			a= Radian(a).valueRadians();
			b= Radian(b).valueRadians();
			if (a <= 1 && a >= -1)
				d= Math::ACos(a).valueRadians();
			else
				d= 0.0;

			if (mDeg) d= d * Math::fRad2Deg;
		}
		break;
	case MTH_ARCTANGENT:
		{
			a= Radian(a).valueRadians();
			d= Math::ATan(a).valueRadians();

			if (mDeg) d= d * Math::fRad2Deg;
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
			Real base= Math::Floor(a);
			Real dec= base - a;
			d= dec > 0.5 ? base + 1 : base;
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
	case MTH_SQRT:
		{
			d= Math::Sqrt(a);
		}
		break;
	case MTH_SQR:
		{
			d= a*a;
		}
		break;
	default:
		break;
	}

	if (gkFuzzy(d))
		d= 0.0;
	else if (gkNan(d) || !gkFinite(d))
		return false;

	mProp->setValue(d);
	mSockets[2].setValue(mProp);
	return false;
}
