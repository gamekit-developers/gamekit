/*
-------------------------------------------------------------------------------
	This file is part of OgreKit.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#include "OgreAction.h"
#include "OgreActionChannel.h"
#include "OgreBone.h"
#include "OgreManualBone.h"


using namespace Ogre;



ActionChannel::ActionChannel(Action *parent, ManualBone* bone) : 
	m_bone(bone), m_action(parent)
{
	OGRE_ASSERT(bone);
}


ActionChannel::~ActionChannel()
{
	BezierSpline **splines = m_splines.ptr();
	int len = getNumSplines(), i=0;
	while (i < len)
		delete splines[i++];

}


void ActionChannel::addSpline(BezierSpline* spline)
{
	if (m_splines.empty()) 
		m_splines.reserve(16);
	m_splines.push_back(spline);
}


const BezierSpline** ActionChannel::getSplines(void)
{
	return (const BezierSpline**)m_splines.ptr();
}


int ActionChannel::getNumSplines(void)
{
	return (int)m_splines.size();
}


void ActionChannel::evaluate(float time, float delta, float weight)
{
	const BezierSpline **splines = getSplines();
	int len = getNumSplines(), i=0, nvrt;

	/// clear previous channel
	TransformState channel;
	channel.setIdentity();

	while (i < len)
	{
		const BezierSpline *spline = splines[i++];

		nvrt = spline->getNumVerts();
		const BezierVertex* verts = spline->getVerts();

		float eval = 0.f;
		if (nvrt > 0)
			eval = spline->interpolate(delta, time);

		switch (spline->getCode())
		{
		case SC_LOC_X: { channel.loc.x = eval; break; }
		case SC_LOC_Y: { channel.loc.y = eval; break; }
		case SC_LOC_Z: { channel.loc.z = eval; break; }
		case SC_SCL_X: { channel.scl.x = eval; break; }
		case SC_SCL_Y: { channel.scl.y = eval; break; }
		case SC_SCL_Z: { channel.scl.z = eval; break; }
		case SC_ROT_X: { channel.rot.x = eval; break; }
		case SC_ROT_Y: { channel.rot.y = eval; break; }
		case SC_ROT_Z: { channel.rot.z = eval; break; }
		case SC_ROT_W: { channel.rot.w = eval; break; }
		}
	}


	// prevent divide by zero
	if (OgreFuzzy(channel.rot.Norm()))
		channel.rot = Ogre::Quaternion::IDENTITY;
	else
		channel.rot.normalise();

	OGRE_ASSERT(!channel.loc.isNaN());
	OGRE_ASSERT(!channel.rot.isNaN());
	OGRE_ASSERT(!channel.scl.isNaN());

	const TransformState &bind = m_bone->m_bind;
	TransformState &pose = m_bone->m_pose;

	// save previous pose
	TransformState blendmat = m_bone->m_pose;

	// combine relitave to binding position 
	pose.loc = bind.loc + bind.rot * channel.loc;
	pose.rot = bind.rot * channel.rot;
	pose.scl = bind.scl * channel.scl;

	if (weight < 1.0)
	{
		// blend poses
		pose.loc = MathUtils::interp(blendmat.loc, pose.loc, weight);
		pose.rot = MathUtils::interp(blendmat.rot, pose.rot, weight);
		pose.rot.normalise();
		pose.scl = MathUtils::interp(blendmat.scl, pose.scl, weight);
	}


	Ogre::Bone *bone = m_bone->m_bone;
	bone->setPosition(pose.loc);
	bone->setOrientation(pose.rot);
	bone->setScale(pose.scl);
}
