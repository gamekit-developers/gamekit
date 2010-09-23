/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

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
#include "gkCollisionCameraConstraint.h"
#include "gkGameObject.h"
#include "Physics/gkRayTest.h"
#include "gkLogger.h"


gkCollisionCameraConstraint::gkCollisionCameraConstraint()
	:    m_target(0),
	     m_cast(0),
	     m_length(0.f),
	     m_yOffs(0.f),
	     m_zOffs(0.f),
	     m_lhf(0.f)
{
}


gkCollisionCameraConstraint::~gkCollisionCameraConstraint()
{
	delete m_cast;
	m_cast = 0;
}



bool gkCollisionCameraConstraint::update(gkScalar delta)
{
	if (!m_object || gkFuzzy(m_influence))
		return false;


	if (m_target != 0)
	{
		if (!m_cast)
			m_cast = new gkRayTest();
	}

	if (m_cast)
	{
		gkVector3 wPos;
		gkVector3 oPos = m_object->getWorldPosition() + gkVector3(0.f, -m_yOffs, -m_zOffs);
		gkVector3 tPos = m_target->getWorldPosition() + gkVector3(0.f, 0.f, -m_zOffs);;

		gkVector3 dir(tPos - oPos);
		dir.normalise();
		dir *= m_length;

		gkScalar fac, di;



		if (m_cast->collides(gkRay(oPos, dir)))
		{
			di = oPos.distance(m_cast->getHitPoint());
			fac = m_influence * di;


			if (m_cast->getObject() != m_target)
				m_object->translate(gkVector3(0.f, fac, 0.f));
			else
			{
				gkScalar dz = (oPos.z - wPos.z);
				if (dz > 0.f)
					m_object->translate(gkVector3(0.f, -fac / 60.f, 0.f));
			}

		}


		gkScalar diff = m_object->getPosition().y;
		if (diff >= 0.f)
			m_object->setPosition(gkVector3(0.f, -m_length / 2.f, 0.f));
	}

	return false;
}



gkConstraint* gkCollisionCameraConstraint::clone(gkGameObject* clob)
{
	gkCollisionCameraConstraint* c = new gkCollisionCameraConstraint(*this);
	c->m_cast = 0;
	return c;
}
