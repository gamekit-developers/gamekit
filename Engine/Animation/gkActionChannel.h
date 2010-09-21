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

#include "Animation/gkAnimationChannel.h"
#include "gkBone.h"


///An action channel holds splines for each manually controlled bone.
///Each spline may consist of one spline per gkActionChannelCode
class gkActionChannel : public gkAnimationChannel
{
protected:
	gkBone* m_bone;

public:

	gkActionChannel(gkAction* parent, gkBone* bone);
	virtual ~gkActionChannel();

	GK_INLINE gkTransformState& getPoseTransfom(void)      { GK_ASSERT(m_bone); return m_bone->getPose(); }
	GK_INLINE gkMatrix4 getPoseMatrix(void)                { GK_ASSERT(m_bone); return m_bone->getPose().toMatrix(); }
	GK_INLINE gkBone* getBone(void)                        { GK_ASSERT(m_bone); return m_bone; }


	void evaluate(gkScalar time, gkScalar delta, gkScalar weight);
};


#endif//_gkActionChannel_h_
