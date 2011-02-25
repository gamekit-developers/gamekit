/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): Xavier T.
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


#include "akAnimationCurve.h"
#include "akCommon.h"

#include "utString.h"

/// Channel are used to organised (group) curves into animation clip.
/// For skeleton animation the channel name must corespond with the bone name.
class akAnimationChannel
{
public:
	enum Type
	{
		AC_TRANSFORM,
		AC_BONE,
		AC_MORPH,
		AC_META
	};

	typedef utArray<akAnimationCurve*> Splines;


protected:

	int                  m_type;
	const utString       m_name;
	Splines              m_splines;
	bool                 m_isEulerRotation;

public:

	akAnimationChannel(Type type, const utString& name);
	~akAnimationChannel();

	void addSpline(akAnimationCurve* spline);
	
	const akAnimationCurve** getSplines(void) const;

	int getNumSplines(void) const;
	
	void evaluate(akTransformState& transform, akScalar time, akScalar weight=1.0f, akScalar delta=0.5f) const;
	
	UT_INLINE int getType(void) const
	{
		return m_type;
	}
	
	UT_INLINE const utString& getName(void) const
	{
		return m_name;
	}

	UT_INLINE bool isEulerRotation(void) const
	{
		return m_isEulerRotation;
	}
	
	UT_INLINE void setEulerRotation(bool v)
	{
		m_isEulerRotation = v;
	}

};

#endif//_akAnimationChannel_h_
