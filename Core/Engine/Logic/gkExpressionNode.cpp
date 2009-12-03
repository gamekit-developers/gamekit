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
#include "gkExpressionNode.h"
#include "gkScriptManager.h"


using namespace Ogre;


// ----------------------------------------------------------------------------
gkExpressionNode::gkExpressionNode(gkLogicTree *parent, size_t id) :
		gkLogicNode(parent, NT_EXPR, id), mNr(0), mExpr(0), mCode(""), mError(false)
{
	ADD_ISOCK(mSockets[0], this, gkLogicSocket::ST_BOOL);
	for (int i=1; i<11; i++)
		ADD_ISOCK(mSockets[i], this, gkLogicSocket::ST_REAL);

	ADD_OSOCK(mSockets[11], this, gkLogicSocket::ST_BOOL);
	ADD_OSOCK(mSockets[12], this, gkLogicSocket::ST_BOOL);
}

// ----------------------------------------------------------------------------
bool gkExpressionNode::evaluate(Real tick)
{
	if (mError || !mSockets[0].getValueBool())
	{
		mSockets[11].setValue(false);
		mSockets[12].setValue(false);
		mSockets[11].block(true);
		mSockets[12].block(true);
	}

	return !mError && mSockets[0].getValueBool();
}

// ----------------------------------------------------------------------------
void gkExpressionNode::update(Real tick)
{
	if (mExpr != 0 && mExpr->isOk())
	{
		int nr= 0;
		for (int i=1; i<11 && nr < mNr; i++, nr++)
			mExpr->addConstant(mSockets[i].getRealName(), mSockets[i].getValueReal());

		int result= mExpr->run();
		if (result == gkScriptExpression::EXPR_ERROR)
		{
			mError= true;
			mSockets[11].setValue(false);
			mSockets[12].setValue(false);
			mSockets[11].block(true);
			mSockets[12].block(true);
			return;
		}

		mSockets[11].setValue(result == gkScriptExpression::EXPR_TRUE);
		mSockets[12].setValue(result == gkScriptExpression::EXPR_FALSE);
		mSockets[11].block(result != gkScriptExpression::EXPR_TRUE);
		mSockets[12].block(result != gkScriptExpression::EXPR_FALSE);
	}
}

// ----------------------------------------------------------------------------
void gkExpressionNode::_initialize()
{
	if (mExpr == 0 && !mCode.empty())
		mExpr= gkScriptManager::getSingleton().createExpression(mCode);

	mSockets[11].setValue(false);
	mSockets[12].setValue(false);
	mSockets[11].block(true);
	mSockets[12].block(true);
}
