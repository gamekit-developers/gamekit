/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): Nestor Silveira.
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
#include "gkMotionNode.h"
#include "gkEngine.h"
#include "gkGameObject.h"
#include "gkScene.h"


using namespace Ogre;



gkMotionNode::gkMotionNode(gkLogicTree* parent, size_t id)
	:       gkLogicNode(parent, id), m_motionType(MT_ROTATION), m_space(TRANSFORM_LOCAL),
	        m_keep(false), m_flag(0), m_otherName(""), m_current(0)
{
	ADD_ISOCK(UPDATE, false);
	ADD_ISOCK(X, 0);
	ADD_ISOCK(Y, 0);
	ADD_ISOCK(Z, 0);
	ADD_ISOCK(DAMPING, 0);
}


bool gkMotionNode::evaluate(gkScalar tick)
{
	return GET_SOCKET_VALUE(UPDATE);
}

void gkMotionNode::initialize()
{
	if (!m_otherName.empty())
	{
		// look at global scope
		if (!m_other)
		{
			if (m_object)
			{
				gkScene* scene = m_object->getOwner();
				GK_ASSERT(scene);
				m_other = scene->getObject(m_otherName);
			}
		}
	}
}


void gkMotionNode::applyConstraints(int lrs)
{
	// loc, rot, size
	CHECK_RETV(!m_current || lrs < 0 && lrs > 2);

	gkVector3 vec;
	bool doApply = false;

	if (lrs == 0)
		vec = m_current->getPosition();
	else if (lrs == 2)
		vec = m_current->getScale();
	else
	{
		vec = m_current->getRotation().toVector3();

		// apply limit
		x[0] = gkClampf(x[0], -MAX_ROT, MAX_ROT);
		x[1] = gkClampf(x[1], -MAX_ROT, MAX_ROT);

		y[0] = gkClampf(y[0], -MAX_ROT, MAX_ROT);
		y[1] = gkClampf(y[1], -MAX_ROT, MAX_ROT);

		z[0] = gkClampf(z[0], -MAX_ROT, MAX_ROT);
		z[1] = gkClampf(z[1], -MAX_ROT, MAX_ROT);
	}
	if (m_flag & 1)  // X
	{
		if (vec.x < x[0])
		{
			vec.x = x[0];
			doApply = true;
		}
		if (vec.x > x[1])
		{
			vec.x = x[1];
			doApply = true;
		}
	}
	if (m_flag & 2) // Y
	{
		if (vec.y < y[0])
		{
			vec.y = y[0];
			doApply = true;
		}
		if (vec.y > y[1])
		{
			vec.y = y[1];
			doApply = true;
		}
	}

	if (m_flag & 4) // Z
	{
		if (vec.z < z[0])
		{
			vec.z = z[0];
			doApply = true;
		}
		if (vec.z > z[1])
		{
			vec.z = z[1];
			doApply = true;
		}
	}


	if (doApply)
	{
		if (lrs == 0)
			m_current->setPosition(vec);
		else if (lrs == 1)
			m_current->setOrientation(vec);
		else
			m_current->setScale(vec);
	}
}


void gkMotionNode::applyObject(gkVector3& vec)
{
	CHECK_RETV(!m_other);

	int lrs = getLRS();

	if (lrs == 0)
		vec = m_other->getWorldOrientation() * vec;
	else if (lrs == 1)
	{
		gkQuaternion q = gkMathUtils::getQuatFromEuler(vec);
		gkQuaternion w = m_other->getWorldOrientation();

		q =  q * w * q * w.Inverse();
		vec = gkMathUtils::getEulerFromQuat(q);
	}
	else if (lrs == 2)
		vec = m_other->getWorldScale() * vec;
}


int gkMotionNode::getLRS()
{
	if (m_motionType == MT_ROTATION)
		return 1;
	else if (m_motionType == MT_TRANSLATION)
		return 0;

	else if (m_motionType == MT_SCALE)
		return 2;

	else if (m_motionType == MT_FORCE)
		return 0;

	else if (m_motionType == MT_TORQUE)
		return 1;

	else if (m_motionType == MT_LINV)
		return 0;

	else if (m_motionType == MT_ANGV)
		return 1;

	return -1;
}


void gkMotionNode::update(gkScalar tick)
{
	CHECK_RETV(!m_object || m_motionType == MT_NONE);

	if (GET_SOCKET(UPDATE)->isConnected())
		m_current = GET_SOCKET(UPDATE)->getGameObject();
	if (!m_current) m_current = m_object;

	gkVector3 vec;

	gkScalar d = 1.0 - GET_SOCKET_VALUE(DAMPING);
	gkScalar damp = gkAbs(d);
	damp = gkClampf(damp, 0.0, 1.0);

	vec.x = (GET_SOCKET_VALUE(X) * damp);
	vec.y = (GET_SOCKET_VALUE(Y) * damp);
	vec.z = (GET_SOCKET_VALUE(Z) * damp);

	if (m_motionType == MT_ROTATION)
	{

		CHECK_RETV(gkFuzzyVec(vec));
		vec.x = gkClampf(vec.x, -MAX_ROT, MAX_ROT);
		vec.y = gkClampf(vec.y, -MAX_ROT, MAX_ROT);
		vec.z = gkClampf(vec.z, -MAX_ROT, MAX_ROT);

		if (m_other != 0) applyObject(vec);

		m_current->rotate(vec, m_space);
		if (m_flag) applyConstraints(1);
	}

	else if (m_motionType == MT_TRANSLATION)
	{

		CHECK_RETV(gkFuzzyVec(vec));
		vec.x = gkClampf(vec.x, -MAX_TRANSLATION, MAX_TRANSLATION);
		vec.y = gkClampf(vec.y, -MAX_TRANSLATION, MAX_TRANSLATION);
		vec.z = gkClampf(vec.z, -MAX_TRANSLATION, MAX_TRANSLATION);

		if (m_other != 0) applyObject(vec);

		m_current->translate(vec, m_space);

		if (m_flag) applyConstraints(0);
	}


	else if (m_motionType == MT_SCALE)
	{
		CHECK_RETV(gkFuzzyVec(vec));
		vec.x = gkClampf(vec.x, -MAX_TRANSLATION, MAX_TRANSLATION);
		vec.y = gkClampf(vec.y, -MAX_TRANSLATION, MAX_TRANSLATION);
		vec.z = gkClampf(vec.z, -MAX_TRANSLATION, MAX_TRANSLATION);

		if (m_other != 0) applyObject(vec);

		m_current->scale(vec);

		if (m_flag) applyConstraints(2);
	}


	else if (m_motionType == MT_FORCE)
	{
		vec.x = gkClampf(vec.x, -MAX_TRANSLATION, MAX_TRANSLATION);
		vec.y = gkClampf(vec.y, -MAX_TRANSLATION, MAX_TRANSLATION);
		vec.z = gkClampf(vec.z, -MAX_TRANSLATION, MAX_TRANSLATION);

		if (m_other != 0) applyObject(vec);
		m_current->applyForce(vec, m_space);
		if (m_flag) applyConstraints(0);
	}

	else if (m_motionType == MT_TORQUE)
	{
		vec.x = gkClampf(vec.x, -MAX_TRANSLATION, MAX_TRANSLATION);
		vec.y = gkClampf(vec.y, -MAX_TRANSLATION, MAX_TRANSLATION);
		vec.z = gkClampf(vec.z, -MAX_TRANSLATION, MAX_TRANSLATION);

		if (m_other != 0) applyObject(vec);

		m_current->applyTorque(vec, m_space);
		if (m_flag) applyConstraints(1);
	}


	else if (m_motionType == MT_LINV)
	{
		vec.x = gkClampf(vec.x, -MAX_TRANSLATION, MAX_TRANSLATION);
		vec.y = gkClampf(vec.y, -MAX_TRANSLATION, MAX_TRANSLATION);
		vec.z = gkClampf(vec.z, -MAX_TRANSLATION, MAX_TRANSLATION);

		if (m_keep)
		{
			gkVector3 ovel = m_current->getLinearVelocity();
			if (!GET_SOCKET(X)->isConnected()) vec.x = ovel.x;
			if (!GET_SOCKET(Y)->isConnected()) vec.y = ovel.y;
			if (!GET_SOCKET(Z)->isConnected()) vec.z = ovel.z;
		}

		if (m_other != 0) applyObject(vec);

		m_current->setLinearVelocity(vec, m_space);
		if (m_flag) applyConstraints(0);
	}

	else if (m_motionType == MT_ANGV)
	{
		vec.x = gkClampf(vec.x, -MAX_TRANSLATION, MAX_TRANSLATION);
		vec.y = gkClampf(vec.y, -MAX_TRANSLATION, MAX_TRANSLATION);
		vec.z = gkClampf(vec.z, -MAX_TRANSLATION, MAX_TRANSLATION);

		if (m_keep)
		{
			gkVector3 ovel = m_current->getAngularVelocity();
			if (!GET_SOCKET(X)->isConnected()) vec.x = ovel.x;
			if (!GET_SOCKET(Y)->isConnected()) vec.y = ovel.y;
			if (!GET_SOCKET(Z)->isConnected()) vec.z = ovel.z;
		}

		if (m_other != 0) applyObject(vec);

		m_current->setAngularVelocity(vec, m_space);
		if (m_flag) applyConstraints(1);
	}
}
