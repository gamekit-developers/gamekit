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
#ifndef _OgreActionManager_h_
#define _OgreActionManager_h_



#include "OgrePrerequisites.h"
#include "OgreMathUtils.h"
#include "OgreUtilsAllocator.h"

namespace Ogre 
{
class Action;


/// Updates a collection of weight blended Action objects
/// Could be expanded to an entire NLA system 
class ActionManager : public AllocObject
{
protected:
	Action		*m_active, *m_blend;
	float		m_blendTime;
public:
	ActionManager();
	~ActionManager();

	OGRE_INLINE Action* getActive(void) {return m_active;}
	void setAction(Action *act);

	/// update all 
	void update(float delta);
};

}

#endif//_OgreActionManager_h_
