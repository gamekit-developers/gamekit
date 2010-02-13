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
#ifndef _OgreManualBone_h_
#define _OgreManualBone_h_

#include "OgrePrerequisites.h"
#include "OgreMathUtils.h"
#include "OgreUtilsAllocator.h"
#include "OgreTransformState.h"

namespace Ogre 
{

/// Manually controlled bone.
class ManualBone : public AllocObject
{
protected:

public:

	ManualBone(Ogre::Bone *bone);
	~ManualBone();

	/// The main Ogre bone.
	Ogre::Bone *m_bone;


	/// Parent manual bone 
	ManualBone *m_parent;

	/// Ogre calculations are reltave to the rest position.
	const TransformState m_bind;

	/// The current pose matrix, calculated in ActionChannel::evaluate
	TransformState m_pose;

};


}


#endif//_OgreManualBone_h_
