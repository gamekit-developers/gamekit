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
#include "gkLimitLocConstraint.h"
#include "gkGameObject.h"




gkLimitLocConstraint::gkLimitLocConstraint()
	:   gkConstraint()
{
	m_flag[0] = m_flag[1] = 0;
	x[0] = x[1] = 0.0;
	y[0] = y[1] = 0.0;
	z[0] = z[1] = 0.0;
}


gkConstraint* gkLimitLocConstraint::clone(gkGameObject* clob)
{
	gkLimitLocConstraint* cl = new gkLimitLocConstraint(*this);
	cl->setObject(clob);
	return cl;
}



bool gkLimitLocConstraint::update(gkScalar delta)
{
	if (!m_object) return false;

	m_matrix = m_object->getTransformState();


	gkVector3& position = m_matrix.loc;

	bool doupd = false;

	// min x
	if (m_flag[0] & 1)
	{
		if (position.x < x[0])
		{
			position.x = x[0];
			doupd = true;
		}
	}

	// max x
	if (m_flag[1] & 1)
	{
		if (position.x > x[1])
		{
			position.x = x[1];
			doupd = true;
		}
	}

	// min y
	if (m_flag[0] & 2)
	{
		if (position.y < y[0])
		{
			position.y = y[0];
			doupd = true;
		}
	}

	// max y
	if (m_flag[1] & 2)
	{
		if (position.y > y[1])
		{
			position.y = y[1];
			doupd = true;
		}
	}

	// min z
	if (m_flag[0] & 4)
	{
		if (position.z < z[0])
		{
			position.z = z[0];
			doupd = true;
		}
	}

	// max z
	if (m_flag[1] & 4)
	{
		if (position.z > z[1])
		{
			position.z = z[1];
			doupd = true;
		}
	}


	return doupd;
}
