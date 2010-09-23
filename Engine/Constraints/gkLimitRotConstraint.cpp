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
#include "gkLimitRotConstraint.h"
#include "gkGameObject.h"




gkLimitRotConstraint::gkLimitRotConstraint()
	:   gkConstraint(),
	    m_flag(0),
	    mXBounds(0.0, 0.0),
	    mYBounds(0.0, 0.0),
	    mZBounds(0.0, 0.0)
{
}



gkConstraint* gkLimitRotConstraint::clone(gkGameObject* clob)
{
	gkLimitRotConstraint* cl = new gkLimitRotConstraint(*this);
	cl->setObject(clob);
	return cl;
}



bool gkLimitRotConstraint::update(gkScalar delta)
{
	if (!m_object) return false;

	m_matrix = m_object->getTransformState();


	gkVector3 rotation = gkEuler(m_matrix.rot).toVector3();

	bool doupd = false;
	// x
	if (m_flag & 1)
	{
		if (rotation.x < mXBounds.x)
		{
			doupd = true;
			rotation.x = mXBounds.x;
		}

		if (rotation.x > mXBounds.y)
		{
			doupd = true;
			rotation.x = mXBounds.y;
		}
	}
	// y
	if (m_flag & 2)
	{
		if (rotation.y < mYBounds.x)
		{
			doupd = true;
			rotation.y = mYBounds.x;
		}

		if (rotation.y > mYBounds.y)
		{
			doupd = true;
			rotation.y = mYBounds.y;
		}
	}

	// z
	if (m_flag & 4)
	{
		if (rotation.z < mZBounds.x)
		{
			doupd = true;
			rotation.z = mZBounds.x;
		}

		if (rotation.z > mZBounds.y)
		{
			doupd = true;
			rotation.z = mZBounds.y;
		}
	}


	if (doupd)
		m_matrix.rot = gkEuler(rotation).toQuaternion();

	return doupd;
}
