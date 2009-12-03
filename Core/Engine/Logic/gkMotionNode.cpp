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
#include "gkMotionNode.h"
#include "gkEngine.h"
#include "gkGameObjectGroup.h"
#include "gkSceneObject.h"


using namespace Ogre;


// ----------------------------------------------------------------------------
gkMotionNode::gkMotionNode(gkLogicTree *parent, size_t id) :
		gkLogicNode(parent, NT_MOTION, id), mMotionType(MT_ROTATION), mSpace(TRANSFORM_LOCAL),
		mKeep(false), mFlag(0), mOtherObject(""), mCurrent(0)
{
	ADD_ISOCK(mSockets[0], this, gkLogicSocket::ST_BOOL);
	ADD_ISOCK(mSockets[1], this, gkLogicSocket::ST_REAL);
	ADD_ISOCK(mSockets[2], this, gkLogicSocket::ST_REAL);
	ADD_ISOCK(mSockets[3], this, gkLogicSocket::ST_REAL);
	ADD_ISOCK(mSockets[4], this, gkLogicSocket::ST_REAL);

	mSockets[0].setValue(true);
	mSockets[1].setValue(0.f);
	mSockets[2].setValue(0.f);
	mSockets[3].setValue(0.f);
	mSockets[4].setValue(0.f);
}

// ----------------------------------------------------------------------------
bool gkMotionNode::evaluate(Real tick)
{
	return mSockets[0].getValueBool();
}

// ----------------------------------------------------------------------------
void gkMotionNode::_initialize()
{
	if (!mOtherObject.empty())
	{
		/* look in this instance */
		if (mObject && mObject->isInstance())
		{
			gkGameObjectGroupInstance *inst= mObject->getGroupInstance();

			if (inst->hasObject(mOtherObject))
				mOther= inst->getObject(mOtherObject);
		}

		/* look at global scope */
		if (!mOther)
		{
			if (mObject)
			{
				gkSceneObject *scene= mObject->getOwner();
				GK_ASSERT(scene);
				mOther= scene->getObject(mOtherObject);
			}
		}
	}
}

// ----------------------------------------------------------------------------
void gkMotionNode::applyConstraints(int lrs)
{
	/* loc, rot, size */
	CHECK_RETV(!mCurrent || lrs < 0 && lrs > 2);

	Vector3 vec;
	bool doApply= false;

	if (lrs == 0)
		vec= mCurrent->getPosition();
	else if (lrs == 2)
		vec= mCurrent->getScale();
	else
	{
		vec= mCurrent->getRotation();

		/* apply limit */
		x[0]= gkClamp(x[0], -MAX_ROT, MAX_ROT);
		x[1]= gkClamp(x[1], -MAX_ROT, MAX_ROT);

		y[0]= gkClamp(y[0], -MAX_ROT, MAX_ROT);
		y[1]= gkClamp(y[1], -MAX_ROT, MAX_ROT);

		z[0]= gkClamp(z[0], -MAX_ROT, MAX_ROT);
		z[1]= gkClamp(z[1], -MAX_ROT, MAX_ROT);
	}



	if (mFlag &1)   /* X */
	{
		if (vec.x < x[0])
		{
			vec.x= x[0];
			doApply= true;
		}
		if (vec.x > x[1])
		{
			vec.x= x[1];
			doApply= true;
		}
	}
	if (mFlag &2)   /* Y */
	{
		if (vec.y < y[0])
		{
			vec.y= y[0];
			doApply= true;
		}
		if (vec.y > y[1])
		{
			vec.y= y[1];
			doApply= true;
		}
	}

	if (mFlag &4)   /* Z */
	{
		if (vec.z < z[0])
		{
			vec.z= z[0];
			doApply= true;
		}
		if (vec.z > z[1])
		{
			vec.z= z[1];
			doApply= true;
		}
	}


	if (doApply)
	{
		if (lrs == 0)
			mCurrent->setPosition(vec);
		else if (lrs == 1)
			mCurrent->setOrientation(vec);
		else
			mCurrent->setScale(vec);
	}
}

// ----------------------------------------------------------------------------
void gkMotionNode::applyObject(Vector3 &vec)
{
	CHECK_RETV(!mOther);

	int lrs= getLRS();

	if (lrs == 0)
		vec= mOther->getWorldOrientation() * vec;
	else if (lrs == 1)
	{
		Quaternion q= gkMathUtils::getQuatFromEuler(vec);
		Quaternion w= mOther->getWorldOrientation();

		q=  q * w * q * w.Inverse();
		vec= gkMathUtils::getEulerFromQuat(q);
	}
	else if (lrs == 2)
		vec= mOther->getWorldScale() * vec;
}

// ----------------------------------------------------------------------------
int gkMotionNode::getLRS()
{
	if (mMotionType == MT_ROTATION)
		return 1;
	else if (mMotionType == MT_TRANSLATION)
		return 0;

	else if (mMotionType == MT_SCALE)
		return 2;

	else if (mMotionType == MT_FORCE)
		return 0;

	else if (mMotionType == MT_TORQUE)
		return 1;

	else if (mMotionType == MT_LINV)
		return 0;

	else if (mMotionType == MT_ANGV)
		return 1;

	return -1;
}

// ----------------------------------------------------------------------------
void gkMotionNode::update(Real tick)
{
	CHECK_RETV(!mObject || mMotionType == MT_NONE);

	/* get linked object */

	if (mSockets[0].isConnected())
		mCurrent= mSockets[0].getGameObject();

	if (!mCurrent) mCurrent= mObject;


	Vector3 vec;

	Real d= 1.0 - mSockets[4].getValueReal();
	Real damp= gkAbs(d);
	damp= gkClamp(damp, 0.0, 1.0);

	vec.x= (mSockets[1].getValueReal() * damp);
	vec.y= (mSockets[2].getValueReal() * damp);
	vec.z= (mSockets[3].getValueReal() * damp);

	if (mMotionType == MT_ROTATION)
	{

		CHECK_RETV(gkFuzzyVec(vec));

		/* clamp min/max */
		vec.x= gkClamp(vec.x, -MAX_ROT, MAX_ROT);
		vec.y= gkClamp(vec.y, -MAX_ROT, MAX_ROT);
		vec.z= gkClamp(vec.z, -MAX_ROT, MAX_ROT);

		if (mOther != 0) applyObject(vec);

		mCurrent->rotate(vec, mSpace);
		if (mFlag) applyConstraints(1);
	}

	else if (mMotionType == MT_TRANSLATION)
	{

		CHECK_RETV(gkFuzzyVec(vec));

		/* clamp min/max */
		vec.x= gkClamp(vec.x, -MAX_TRANSLATION, MAX_TRANSLATION);
		vec.y= gkClamp(vec.y, -MAX_TRANSLATION, MAX_TRANSLATION);
		vec.z= gkClamp(vec.z, -MAX_TRANSLATION, MAX_TRANSLATION);

		if (mOther != 0) applyObject(vec);

		mCurrent->translate(vec, mSpace);

		if (mFlag) applyConstraints(0);
	}


	else if (mMotionType == MT_SCALE)
	{

		CHECK_RETV(gkFuzzyVec(vec));

		/* clamp min/max */
		vec.x= gkClamp(vec.x, -MAX_TRANSLATION, MAX_TRANSLATION);
		vec.y= gkClamp(vec.y, -MAX_TRANSLATION, MAX_TRANSLATION);
		vec.z= gkClamp(vec.z, -MAX_TRANSLATION, MAX_TRANSLATION);

		if (mOther != 0) applyObject(vec);

		mCurrent->scale(vec);

		if (mFlag) applyConstraints(2);
	}


	else if (mMotionType == MT_FORCE)
	{
		/* clamp min/max */
		vec.x= gkClamp(vec.x, -MAX_TRANSLATION, MAX_TRANSLATION);
		vec.y= gkClamp(vec.y, -MAX_TRANSLATION, MAX_TRANSLATION);
		vec.z= gkClamp(vec.z, -MAX_TRANSLATION, MAX_TRANSLATION);

		if (mOther != 0) applyObject(vec);

		mCurrent->applyForce(vec, mSpace);

		if (mFlag) applyConstraints(0);
	}

	else if (mMotionType == MT_TORQUE)
	{
		/* clamp min/max */
		vec.x= gkClamp(vec.x, -MAX_TRANSLATION, MAX_TRANSLATION);
		vec.y= gkClamp(vec.y, -MAX_TRANSLATION, MAX_TRANSLATION);
		vec.z= gkClamp(vec.z, -MAX_TRANSLATION, MAX_TRANSLATION);

		if (mOther != 0) applyObject(vec);

		mCurrent->applyTorque(vec, mSpace);
		if (mFlag) applyConstraints(1);
	}


	else if (mMotionType == MT_LINV)
	{
		/* clamp min/max */
		vec.x= gkClamp(vec.x, -MAX_TRANSLATION, MAX_TRANSLATION);
		vec.y= gkClamp(vec.y, -MAX_TRANSLATION, MAX_TRANSLATION);
		vec.z= gkClamp(vec.z, -MAX_TRANSLATION, MAX_TRANSLATION);

		if (mKeep)
		{
			Vector3 ovel= mCurrent->getLinearVelocity();
			if (!mSockets[1].isConnected()) vec.x= ovel.x;
			if (!mSockets[2].isConnected()) vec.y= ovel.y;
			if (!mSockets[3].isConnected()) vec.z= ovel.z;
		}

		if (mOther != 0) applyObject(vec);

		mCurrent->setLinearVelocity(vec, mSpace);
		if (mFlag) applyConstraints(0);
	}

	else if (mMotionType == MT_ANGV)
	{
		/* clamp min/max */
		vec.x= gkClamp(vec.x, -MAX_TRANSLATION, MAX_TRANSLATION);
		vec.y= gkClamp(vec.y, -MAX_TRANSLATION, MAX_TRANSLATION);
		vec.z= gkClamp(vec.z, -MAX_TRANSLATION, MAX_TRANSLATION);

		if (mKeep)
		{
			Vector3 ovel= mCurrent->getAngualrVelocity();
			if (!mSockets[1].isConnected()) vec.x= ovel.x;
			if (!mSockets[2].isConnected()) vec.y= ovel.y;
			if (!mSockets[3].isConnected()) vec.z= ovel.z;
		}

		if (mOther != 0) applyObject(vec);

		mCurrent->setAngualrVelocity(vec, mSpace);
		if (mFlag) applyConstraints(1);
	}
}
