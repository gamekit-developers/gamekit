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
#include "akAnimation.h"
#include "akAnimationChannel.h"


akAnimationChannel::akAnimationChannel(const utString& name, akAnimation* parent)
	:    m_name(name), m_action(parent)
{
}




akAnimationChannel::~akAnimationChannel()
{
	akBezierSpline** splines = m_splines.ptr();
	int len = getNumSplines(), i = 0;
	while (i < len)
		delete splines[i++];
}



void akAnimationChannel::addSpline(akBezierSpline* spline)
{
	if (m_splines.empty())
		m_splines.reserve(16);
	m_splines.push_back(spline);
}




const akBezierSpline** akAnimationChannel::getSplines(void) const
{
	return (const akBezierSpline**)m_splines.ptr();
}



int akAnimationChannel::getNumSplines(void) const
{
	return (int)m_splines.size();
}



void akAnimationChannel::evaluate(const akScalar& time, const akScalar& delta, const akScalar& weight, void* object) const
{
	evaluateImpl(time, delta, weight, object);
}
