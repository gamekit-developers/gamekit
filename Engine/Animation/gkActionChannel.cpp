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
#include "gkAction.h"
#include "gkActionChannel.h"



gkActionChannel::gkActionChannel(gkAction *parent, gkBone *bone) 
	:	gkAnimationChannel(parent), m_bone(bone)
{
	GK_ASSERT(bone);
}


gkActionChannel::~gkActionChannel()
{
}


void gkActionChannel::evaluate(gkScalar time, gkScalar delta, gkScalar weight)
{
	const gkBezierSpline **splines = getSplines();
	int len = getNumSplines(), i = 0, nvrt;

	// clear previous channel
	gkTransformState channel;
	channel.setIdentity();

	while (i < len)
	{
		const gkBezierSpline *spline = splines[i++];

		nvrt = spline->getNumVerts();
		const gkBezierVertex *verts = spline->getVerts();

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
	if (gkFuzzy(channel.rot.Norm()))
		channel.rot = Ogre::Quaternion::IDENTITY;
	else
		channel.rot.normalise();

	GK_ASSERT(!channel.loc.isNaN());
	GK_ASSERT(!channel.rot.isNaN());
	GK_ASSERT(!channel.scl.isNaN());

	// Apply specifics 
	m_bone->applyChannelTransform(channel, weight);
}
