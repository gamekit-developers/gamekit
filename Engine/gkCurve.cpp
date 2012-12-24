/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2012 Thomas Trocha

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
#include "gkCurve.h"
#include "OgreLight.h"
#include "OgreSceneManager.h"
#include "OgreSceneNode.h"
#include "gkScene.h"



gkCurve::gkCurve(gkInstancedManager* creator, const gkResourceName& name, const gkResourceHandle& handle)
	:    gkGameObject(creator, name, handle, GK_CURVE),
	     m_curveProps()
{
}




void gkCurve::updateProperties(void)
{
	// TODO
}




void gkCurve::createInstanceImpl(void)
{
	gkGameObject::createInstanceImpl();

	GK_ASSERT(!m_light);

// TODO
}



void gkCurve::destroyInstanceImpl(void)
{
	GK_ASSERT(m_light);


	gkGameObject::destroyInstanceImpl();
}



gkGameObject* gkCurve::clone(const gkString& name)
{
	gkCurve* cl = new gkCurve(getInstanceCreator(), name, -1);
	cl->m_curveProps = m_curveProps;

	gkGameObject::cloneImpl(cl);
	return cl;
}

const gkVector3 gkCurve::getPoint(int nr) {
	if (nr < m_curveProps.m_points.size()) {
		return getWorldTransform() * m_curveProps.m_points.at(nr);
	}
	else
	{
		// TODO: better error handling
		return gkVector3::ZERO;
	}
}
