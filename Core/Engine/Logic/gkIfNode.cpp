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
#include "gkIfNode.h"


using namespace Ogre;



// ----------------------------------------------------------------------------
gkIfNode::gkIfNode(gkLogicTree *parent, size_t id) :
		gkLogicNode(parent, NT_IF, id), mStatement(CMP_NULL)
{
	ADD_ISOCK(mSockets[0], this, gkLogicSocket::ST_REAL);
	ADD_ISOCK(mSockets[1], this, gkLogicSocket::ST_REAL);
	ADD_OSOCK(mSockets[2], this, gkLogicSocket::ST_BOOL);
	ADD_OSOCK(mSockets[3], this, gkLogicSocket::ST_BOOL);
}


// ----------------------------------------------------------------------------
bool gkIfNode::evaluate(Real tick)
{
	bool result= false;
	switch (mStatement)
	{
	case CMP_TRUE:
		{
			if (mSockets[0].isLinked())
				result= mSockets[0].getValueBool();
			else if (mSockets[1].isLinked())
				result= mSockets[1].getValueBool();
			else
				result= mSockets[0].getValueBool();
		}
		break;
	case CMP_NOT:
	case CMP_FALSE:
		{
			if (mSockets[0].isLinked())
				result= !mSockets[0].getValueBool();
			else if (mSockets[1].isLinked())
				result= !mSockets[1].getValueBool();
			else
				result= !mSockets[0].getValueBool();
		}
		break;
	case CMP_AND:
		result= mSockets[0].getValueBool() && mSockets[1].getValueBool();
		break;
	case CMP_OR:
		result= mSockets[0].getValueBool() || mSockets[1].getValueBool();
		break;
	case CMP_EQUALS:
		result= mSockets[0].getValueReal() == mSockets[1].getValueReal();
		break;
	case CMP_NOT_EQUAL:
		result= mSockets[0].getValueReal() != mSockets[1].getValueReal();
		break;
	case CMP_GREATER:
		result= (mSockets[0].getValueReal() > mSockets[1].getValueReal());
		break;
	case CMP_GTHAN:
		result= (mSockets[0].getValueReal() >= mSockets[1].getValueReal());
		break;
	case CMP_LESS:
		result= (mSockets[0].getValueReal() < mSockets[1].getValueReal());
		break;
	case CMP_LTHAN:
		result= (mSockets[0].getValueReal() <= mSockets[1].getValueReal());
		break;
	case CMP_NULL:
	default:
		break;
	}


	/* do io blocking */
	if (mSockets[2].isConnected()) mSockets[2].block(!result);
	if (mSockets[3].isConnected()) mSockets[3].block(result);

	mSockets[2].setValue(result);
	mSockets[3].setValue(!result);
	return false;
}
