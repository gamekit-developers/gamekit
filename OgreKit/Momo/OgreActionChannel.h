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
#ifndef _OgreActionChannel_h_
#define _OgreActionChannel_h_

#include "OgrePrerequisites.h"
#include "OgreBezierSpline.h"
#include "OgreArray.h"
#include "OgreUtilsAllocator.h"
#include "OgreTransformState.h"
#include "OgreManualBone.h"


namespace Ogre 
{
class Action;


enum ActionChannelCode
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


/// Channel holding splines for each manually controlled bone.
class ActionChannel : public AllocObject
{
public:
	typedef Array<BezierSpline*> Splines;


protected:
	ManualBone*			m_bone;
	Splines				m_splines;
	Action*				m_action;


public:

	ActionChannel(Action *parent, ManualBone* bone);
	~ActionChannel();

	/// Gain access to the pose transform
	OGRE_INLINE TransformState& getPoseTransfom(void) 
	{ OGRE_ASSERT(m_bone); return m_bone->m_pose; }

	/// Gain access to the pose matrix
	OGRE_INLINE Matrix4 getPoseMatrix(void) 
	{ OGRE_ASSERT(m_bone); return m_bone->m_pose.toMatrix(); }

	/// add spline for this channel
	void addSpline(BezierSpline* spline);

	/// Gain access to all splines
	const BezierSpline** getSplines(void);

	/// Total number of splines 
	int getNumSplines(void);

	/// evaluate curve for the given time. 
	/// Sets the transform matrix 
	void evaluate(float time, float delta, float weight);
};

}


#endif//_OgreActionChannel_h_
