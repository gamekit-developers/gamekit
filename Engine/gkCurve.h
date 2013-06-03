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
#ifndef _gkCurveObject_h_
#define _gkCurveObject_h_

#include "gkGameObject.h"
#include "gkSerialize.h"


class gkCurve : public gkGameObject
{
public:
	gkCurve(gkInstancedManager* creator, const gkResourceName& name, const gkResourceHandle& handle);
	virtual ~gkCurve() {}


	GK_INLINE gkCurveProperties& getCurveProperties(void)           {return m_curveProps;}

	void updateProperties(void);

	int getPointCount();

	const gkVector3 getPoint(int nr);

	GK_INLINE bool isCyclic() { return m_curveProps.m_isCyclic; }

	void generateBezierPoints(gkScalar t);

	void showDebug();

private:
	gkGameObject* clone(const gkString& name);
	void makeBezier(gkScalar _t, utArray<gkVector3>& _first, utArray<gkVector3>& _second);	

	gkCurveProperties       m_curveProps;
	utArray<gkVector3>		m_bezierPoints;

	virtual void createInstanceImpl(void);
	virtual void destroyInstanceImpl(void);

};

#endif//_gkCurveObject_h_
