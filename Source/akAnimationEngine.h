/*
-------------------------------------------------------------------------------
	This file is part of OgreKit.
	http://gamekit.googlecode.com/
	
	Copyright (c) 2006-2010 Xavier T.
	
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

#ifndef AKANIMATIONENGINE_H
#define AKANIMATIONENGINE_H

#include "utTypes.h"
#include "utString.h"

#include "akCommon.h"
#include "akMathUtils.h"

class akAnimationEngine
{
public:
	typedef utHashTable<utIntHashKey, akAnimationClip*> AnimationClips;
	typedef utHashTable<utIntHashKey, akSkeleton*> Skeletons;
	typedef utHashTable<utIntHashKey, akMesh*> Meshes;
	typedef utHashTable<utIntHashKey, akAnimatedObject*> AnimatedObjects;

protected:
	AnimationClips  m_animationClips;
	Skeletons       m_skeletons;
	Meshes          m_meshes;
	AnimatedObjects m_animatedObjects;

public:
	akAnimationEngine();
	virtual ~akAnimationEngine();
	
	void              addAnimationClip(const utHashedString& name, akAnimationClip* anim);
	akAnimationClip*  getAnimationClip(const utHashedString& name);
	void              addMesh(const utHashedString& name, akMesh* mesh);
	akMesh*           getMesh(const utHashedString& name);
	void              addSkeleton(const utHashedString& name, akSkeleton* skel);
	akSkeleton*       getSkeleton(const utHashedString& name);
	void              addAnimatedObject(const utHashedString& name, akAnimatedObject* obj);
	akAnimatedObject* getAnimatedObject(const utHashedString& name);
	
	// Nothing there yet. Is this really an engine?
	//void start(void);
	//void stop(void);

	void stepTime(akScalar deltaTimeInSeconds);
	
	UT_INLINE AnimationClips::ConstPointer getAnimations(void)
	{
		return m_animationClips.ptr();
	}
	
	UT_INLINE UTsize getNumAnimations(void)
	{
		return m_animationClips.size();
	}
	
	UT_INLINE Meshes::ConstPointer getMeshes(void)
	{
		return m_meshes.ptr();
	}
	
	UT_INLINE UTsize getNumMeshes(void)
	{
		return m_meshes.size();
	}
	
	UT_INLINE Skeletons::ConstPointer getSkeletons(void)
	{
		return m_skeletons.ptr();
	}
	
	UT_INLINE UTsize getNumSkeletons(void)
	{
		return m_skeletons.size();
	}
	
	UT_INLINE AnimatedObjects::ConstPointer getAnimatedObjects(void)
	{
		return m_animatedObjects.ptr();
	}
	
	UT_INLINE UTsize getNumAnimatedObjects(void)
	{
		return m_animatedObjects.size();
	}
	
};

#endif // AKANIMATIONENGINE_H
