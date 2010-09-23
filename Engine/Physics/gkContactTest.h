/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

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
#ifndef _gkContactTest_h_
#define _gkContactTest_h_

#include "utTypes.h"
#include "btBulletCollisionCommon.h"

class gkAllContactResultCallback : public btCollisionWorld::ContactResultCallback
{
private:
	bool m_hasHit;

public:
	utArray<const btCollisionObject*>  m_contactObjects;

	gkAllContactResultCallback() : btCollisionWorld::ContactResultCallback(), m_hasHit(false)
	{
	}

	~gkAllContactResultCallback()
	{
		m_contactObjects.empty();
	}

	bool hasHit(void) {return m_hasHit;}

	virtual    btScalar    addSingleResult(btManifoldPoint& cp, const btCollisionObject* colObj0, int partId0, int index0, const btCollisionObject* colObj1, int partId1, int index1)
	{
		m_hasHit = true;

		if (m_contactObjects.find(colObj1) == UT_NPOS)
			m_contactObjects.push_back(colObj1);

		return 0.;
	}
};

#endif // _gkContactTest_h_
