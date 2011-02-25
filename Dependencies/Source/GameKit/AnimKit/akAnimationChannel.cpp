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

#include "akAnimationChannel.h"
#include "akTransformState.h"
#include "akSkeleton.h"
#include "akSkeletonPose.h"
#include "akEuler.h"
#include "akPoseBlender.h"


akAnimationChannel::akAnimationChannel(Type type, const utString& name)
	:    m_name(name), m_type(type), m_isEulerRotation(false)
{
}

akAnimationChannel::~akAnimationChannel()
{
	akAnimationCurve** splines = m_splines.ptr();
	int len = getNumSplines(), i = 0;
	while (i < len)
		delete splines[i++];
}



void akAnimationChannel::addSpline(akAnimationCurve* spline)
{
	if (m_splines.empty())
		m_splines.reserve(16);
	m_splines.push_back(spline);
}




const akAnimationCurve** akAnimationChannel::getSplines(void) const
{
	return (const akAnimationCurve**)m_splines.ptr();
}



int akAnimationChannel::getNumSplines(void) const
{
	return (int)m_splines.size();
}



void akAnimationChannel::evaluate(akTransformState& transform, akScalar time, akScalar weight, akScalar delta) const
{
	const akAnimationCurve** splines = getSplines();
	int len = getNumSplines();

	akTransformState channel;
	channel.rot = Quat::identity();
	channel.loc = Vector3(0.f, 0.f, 0.f);
	channel.scale = Vector3(1.f, 1.f, 1.f);

	akEuler euler(0,0,0);
	
	for (int i=0; i<len; i++)
	{
		float eval = splines[i]->evaluate(time, delta);

		switch (splines[i]->getCode())
		{
		case akAnimationCurve::SC_LOC_X: { channel.loc.setX(eval); break; }
		case akAnimationCurve::SC_LOC_Y: { channel.loc.setY(eval); break; }
		case akAnimationCurve::SC_LOC_Z: { channel.loc.setZ(eval); break; }
		case akAnimationCurve::SC_SCL_X: { channel.scale.setX(eval); break; }
		case akAnimationCurve::SC_SCL_Y: { channel.scale.setY(eval); break; }
		case akAnimationCurve::SC_SCL_Z: { channel.scale.setZ(eval); break; }
		case akAnimationCurve::SC_ROT_QUAT_X: { channel.rot.setX(eval); break; }
		case akAnimationCurve::SC_ROT_QUAT_Y: { channel.rot.setY(eval); break; }
		case akAnimationCurve::SC_ROT_QUAT_Z: { channel.rot.setZ(eval); break; }
		case akAnimationCurve::SC_ROT_QUAT_W: { channel.rot.setW(eval); break; }
		case akAnimationCurve::SC_ROT_EULER_X: { euler.x = eval; break; }
		case akAnimationCurve::SC_ROT_EULER_Y: { euler.y = eval; break; }
		case akAnimationCurve::SC_ROT_EULER_Z: { euler.z = eval; break; }
		}
	}

	if(m_isEulerRotation)
		channel.rot = euler.toQuaternion();
	else
	{
		if ( akFuzzy(norm(channel.rot)) )
			channel.rot = Quat::identity();
		else
			normalize(channel.rot);
	}
	
	UT_ASSERT(!channel.loc.isNaN());
	UT_ASSERT(!channel.rot.isNaN());
	UT_ASSERT(!channel.scl.isNaN());
	
	akPoseBlender::blendJoint(akPoseBlender::PB_BM_ADD, akPoseBlender::PB_RM_LERP,
								weight, transform, channel, transform );
}

