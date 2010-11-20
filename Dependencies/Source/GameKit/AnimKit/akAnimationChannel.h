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
#ifndef _akAnimationChannel_h_
#define _akAnimationChannel_h_


#include "akBezierSpline.h"
#include "akCommon.h"

#include "utString.h"


class akAnimationChannel
{
public:
	typedef utArray<akBezierSpline*> Splines;


protected:

	const utString       m_name;
	Splines              m_splines;
	akAnimation*    m_action;

public:

	akAnimationChannel(const utString& name, akAnimation* parent);
	virtual ~akAnimationChannel();


	void addSpline(akBezierSpline* spline);
	const akBezierSpline** getSplines(void) const;

	int getNumSplines(void) const;

	const utString& getName(void) const {return m_name;}


	///Evaluates the curve for the given time.
	///time is the actual frame, eg; [1-25]
	///delta is the time expressed in [0-1]
	///weight is the abount of blending from a previous evaluation
	///to the next evaluation. expressed in [0-1]
	///object is the game object used to apply evaluation result
	void evaluate(const akScalar& time, const akScalar& delta, const akScalar& weight, void* object) const;
	
protected:
	virtual void evaluateImpl(const akScalar& time, const akScalar& delta, const akScalar& weight, void* object) const = 0;
};


#endif//_akAnimationChannel_h_
