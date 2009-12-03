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
#include "gkObjectNode.h"
#include "gkEngine.h"
#include "gkGameObjectGroup.h"
#include "gkSceneObject.h"


using namespace Ogre;


// ----------------------------------------------------------------------------
gkObjectNode::gkObjectNode(gkLogicTree *parent, size_t id) :
		gkLogicNode(parent, NT_OBJECT, id), mOtherName(""), mFunc(OB_FUNC_NONE)
{
	ADD_ISOCK(mSockets[0], this, gkLogicSocket::ST_BOOL);
	ADD_ISOCK(mSockets[1], this, gkLogicSocket::ST_VECTOR);
	ADD_ISOCK(mSockets[2], this, gkLogicSocket::ST_EULER);

	ADD_OSOCK(mSockets[3], this, gkLogicSocket::ST_BOOL);
	ADD_OSOCK(mSockets[4], this, gkLogicSocket::ST_VECTOR);
	ADD_OSOCK(mSockets[5], this, gkLogicSocket::ST_EULER);
}

// ----------------------------------------------------------------------------
bool gkObjectNode::evaluate(Real tick)
{
	mSockets[3].setValue(mSockets[0].getValueBool());
	return mSockets[0].getValueBool();
}

// ----------------------------------------------------------------------------
void gkObjectNode::_initialize()
{
	if (!mOtherName.empty())
	{
		/* look in this instance */
		if (mObject && mObject->isInstance())
		{
			gkGameObjectGroupInstance *inst= mObject->getGroupInstance();

			if (inst->hasObject(mOtherName))
				mOther= inst->getObject(mOtherName);
		}

		/* look at global scope */
		if (!mOther)
		{
			if (mObject)
			{
				gkSceneObject *scene= mObject->getOwner();
				GK_ASSERT(scene);
				mOther= scene->getObject(mOtherName);
			}
		}
	}
}

// ----------------------------------------------------------------------------
void gkObjectNode::update(Real tick)
{
	gkGameObject *ob= mOther ? mOther : mObject;
	if (!ob)
		return;

	if (mSockets[1].isLinked())
		ob->setPosition(mSockets[1].getValueVector3());
	else
		mSockets[4].setValue(ob->getPosition());

	if (mSockets[2].isLinked())
	{
		Quaternion q= gkMathUtils::getQuatFromEuler(mSockets[2].getValueVector3(), true);
		ob->setOrientation(q);
		mSockets[5].setValue(mSockets[2].getValueVector3());
	}
	else
		mSockets[5].setValue(gkMathUtils::getEulerFromQuat(ob->getOrientation(), true));
}
