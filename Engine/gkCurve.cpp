/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Thomas Trocha

    Contributor(s): oneminute.
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
#include "gkDebugger.h"



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

// TODO
}



void gkCurve::destroyInstanceImpl(void)
{

	gkGameObject::destroyInstanceImpl();
}



gkGameObject* gkCurve::clone(const gkString& name)
{
	gkCurve* cl = new gkCurve(getInstanceCreator(), name, -1);
	cl->m_curveProps = m_curveProps;

	gkGameObject::cloneImpl(cl);
	return cl;
}

const gkVector3 gkCurve::getPoint(int nr)
{
	switch (m_curveProps.m_type)
	{
	case gkCurveProperties::CU_Points:
		{
			if (nr < (int)m_curveProps.m_points.size()) {
				return getWorldTransform() * m_curveProps.m_points.at(nr);
			}
			else
			{
				// TODO: better error handling
				return gkVector3::ZERO;
			}
		}
	case gkCurveProperties::CU_Bezier:
		{
			if (nr < (int)m_bezierPoints.size()) {
				return getWorldTransform() * m_bezierPoints.at(nr);
			}
			else
			{
				// TODO: better error handling
				return gkVector3::ZERO;
			}
		}
	}
}

int gkCurve::getPointCount()
{
	switch (m_curveProps.m_type)
	{
	case gkCurveProperties::CU_Points:
		return m_curveProps.m_points.size();
	case gkCurveProperties::CU_Bezier:
		return m_bezierPoints.size();
	default:
		return 0;
	}
}

void gkCurve::generateBezierPoints(gkScalar t)
{
	m_bezierPoints.clear();

	UTsize triCount = m_curveProps.m_BezTriples.size();

	for (UTsize i = 0; i < triCount - 1; i++)
	{
		utArray<gkVector3>& bezFirst = m_curveProps.m_BezTriples.at(i);
		utArray<gkVector3>& bezSecond = m_curveProps.m_BezTriples.at(i + 1);

		makeBezier(t, bezFirst, bezSecond);
	}

	if (m_curveProps.m_isCyclic)
	{
		utArray<gkVector3>& bezFirst = m_curveProps.m_BezTriples.at(triCount - 1);
		utArray<gkVector3>& bezSecond = m_curveProps.m_BezTriples.at(0);

		makeBezier(t, bezFirst, bezSecond);
	}
}

void gkCurve::makeBezier(gkScalar t, utArray< gkVector3 >& first, utArray< gkVector3 >& second)
{
	gkVector3& pt0 = first.at(1);
	gkVector3& pt1 = first.at(2);
	gkVector3& pt2 = second.at(0);
	gkVector3& pt3 = second.at(1);

	for (gkScalar tt = 0.f; tt < 1.f; tt += t)
	{
		gkScalar yt = 1 - tt;
		gkScalar t1 = yt * yt;
		gkScalar t2 = 3 * yt * tt;
		gkVector3 pt = pt0 * t1 * yt + pt1 * t2 * yt + pt2 * t2 * tt + pt3 * tt * tt * tt;
		m_bezierPoints.push_back(pt);
	}
}

void gkCurve::showDebug()
{
	if (m_scene)
	{
		gkDebugger* debug = m_scene->getDebugger();

		if (!debug)
		{
			return;
		}

		unsigned int n = getPointCount();

		if (n)
		{
			static const gkVector3 RED_COLOR(0.8f, 0.5f, 0);

			gkVector3 oldPoint = getPoint(0);

			for (unsigned int i = 1; i < n; i++)
			{
				gkVector3 point = getPoint(i);

				debug->drawLine(
					oldPoint,
					point,
					RED_COLOR
					);

				oldPoint = point;
			}
		}
	}
}
