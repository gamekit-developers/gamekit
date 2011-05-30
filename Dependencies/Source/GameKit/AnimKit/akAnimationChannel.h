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
		AC_TRANSFORM =0,
		AC_BONE,
		AC_MORPH,
		AC_META
	};

	typedef utArray<akAnimationCurve*> Curves;


protected:

	UTuint32             m_type;
	const utHashedString m_name;
	Curves               m_curves;
	bool                 m_isEulerRotation;

public:

	akAnimationChannel(UTuint32 type, const utHashedString& name);
	~akAnimationChannel();

	void addCurve(akAnimationCurve* spline);
	
	const akAnimationCurve** getCurves(void) const;

	int getNumCurves(void) const;
	
	void evaluate(akPose& pose, akScalar time, akScalar weight=1.0f, akScalar delta=0.5f) const;
	void evaluate(akTransformState& transform, akScalar time, akScalar weight=1.0f, akScalar delta=0.5f) const;
	
	UT_INLINE UTuint32 getType(void) const
	{
		return m_type;
	}
	
	UT_INLINE const utHashedString& getName(void) const
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
