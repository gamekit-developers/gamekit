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
#include "gkDbvt.h"
#include "gkVariable.h"
#include "gkGameObject.h"
#include "gkCamera.h"
#include "gkRigidBody.h"
#include "gkEngine.h"
#include "gkUserDefs.h"

#include "OgreCamera.h"



gkDbvt::gkDbvt()
	:    m_tvs(0),
	     m_tot(0),
	     m_debug(gkString("btDbvt"), false)
{
}



gkDbvt::~gkDbvt()
{
}



void gkDbvt::Process(const btDbvtNode* nd)
{
	btBroadphaseProxy* proxy = (btBroadphaseProxy*)nd->data;
	if (gkPhysicsController::castController(proxy->m_clientObject)->_markDbvt(true))
		m_tvs++;
}



void gkDbvt::mark(gkCamera* cam, btDbvtBroadphase* cullTree, gkPhysicsControllers& controllers)
{
	GK_ASSERT(cam && cullTree);

	const Ogre::Plane* planes = cam->getCamera()->getFrustumPlanes();

	btVector3 normals[6];
	btScalar offsets[6];

	for (int i = 0; i < 6; ++i)
	{
		normals[i].setValue(planes[i].normal.x, planes[i].normal.y, planes[i].normal.z);
		offsets[i] = planes[i].d;
	}


	m_tot = controllers.size();
	m_tvs = 0;

	if (!controllers.empty())
	{
		// Mark all invisible

		gkPhysicsControllers::Iterator iter = controllers.iterator();
		while (iter.hasMoreElements())
			iter.getNext()->_markDbvt(false);
	}

	btDbvt::collideKDOP(cullTree->m_sets[1].m_root, normals, offsets, 6, *this);
	btDbvt::collideKDOP(cullTree->m_sets[0].m_root, normals, offsets, 6, *this);


	if (gkEngine::getSingleton().getUserDefs().debugFps)
	{
		char buf[72];
		sprintf(buf, "%i, %i\n", m_tvs, m_tot);
		m_debug.setValue(gkString(buf));
	}
}
