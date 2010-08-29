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
#ifndef _gkActionChannel_h_
#define _gkActionChannel_h_

#include "gkBezierSpline.h"
#include "gkTransformState.h"
#include "gkSkeleton.h"


class gkAction;


enum gkActionChannelCode
{
	SC_LOC_X,
	SC_LOC_Y,
	SC_LOC_Z,
	SC_SCL_X,
	SC_SCL_Y,
	SC_SCL_Z,
	SC_ROT_X,
	SC_ROT_Y,
	SC_ROT_Z,
	SC_ROT_W,
};


///An action channel holds splines for each manually controlled bone.
///Each spline may consist of one spline per gkActionChannelCode
class gkActionChannel
{
public:
	typedef utArray<gkBezierSpline *> Splines;


protected:
	gkBone             *m_bone;
	Splines             m_splines;
	gkAction           *m_action;

public:

	gkActionChannel(gkAction *parent, gkBone *bone);
	~gkActionChannel();

	GK_INLINE gkTransformState &getPoseTransfom(void)
	{ GK_ASSERT(m_bone); return m_bone->getPose(); }

	GK_INLINE gkMatrix4 getPoseMatrix(void)
	{ GK_ASSERT(m_bone); return m_bone->getPose().toMatrix(); }

	GK_INLINE gkBone *getBone(void)
	{ GK_ASSERT(m_bone); return m_bone; }

	void addSpline(gkBezierSpline *spline);

	const gkBezierSpline **getSplines(void);

	int getNumSplines(void);


	///Evaluates the curve for the given time. 
	///time is the actual frame, eg; [1-25]
	///delta is the time expressed in [0-1]
	///weight is the abount of blending from a previous evaluation 
	///to the next evaluation. expressed in [0-1]
	void evaluate(gkScalar time, gkScalar delta, gkScalar weight);
};


#endif//_gkActionChannel_h_
