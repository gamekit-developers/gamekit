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
#ifndef _OgreManualSkeleton_h_
#define _OgreManualSkeleton_h_

#include "OgrePrerequisites.h"
#include "OgreList.h"
#include "OgreHashTable.h"
#include "OgreHashedString.h"
#include "OgreUtilsAllocator.h"

namespace Blender { class bAction; }


namespace Ogre 
{

class Action;
class ManualBone;


/// Manually animated Ogre skeleton.
/// Animation states are derrived from Action classes 
class ManualSkeleton : public AllocObject
{
public:
	typedef HashTable<HashedString, Action*> Actions;
	typedef HashTable<HashedString, ManualBone*> Bones;

protected:

	Entity*		m_animatedMesh;
	Actions		m_actions;
	Bones		m_bones;

	void		modifyBones(void);

public:

	ManualSkeleton(Entity *entity);
	~ManualSkeleton();

	ManualBone* getBone(const String& name);
	Action*		createAction(const String& name);
	Action*		getAction(const String& name);


	OGRE_INLINE bool hasBone(const String& name)	{ return m_bones.find(name) != OGRE_NPOS; }
	OGRE_INLINE bool hasAction(const String& name)	{ return m_actions.find(name) != OGRE_NPOS; }


	/// Set action information from blender action
	Action* createAction(Blender::bAction *bact);
};


}

#endif//_OgreManualSkeleton_h_
