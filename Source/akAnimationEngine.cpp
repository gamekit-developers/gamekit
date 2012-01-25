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

#include "akAnimationEngine.h"

#include "akMesh.h"
#include "akSkeleton.h"
#include "akAnimationClip.h"
#include "akAnimatedObject.h"

akAnimationEngine::akAnimationEngine()
{
}

akAnimationEngine::~akAnimationEngine()
{
	unsigned int i;
	
	for( i=0; i<m_animations.size(); i++)
	{
		delete m_animations[i];
	}
	
	for( i=0; i<m_meshes.size(); i++)
	{
		delete m_meshes[i];
	}
	
	for( i=0; i<m_skeletons.size(); i++)
	{
		delete m_skeletons[i];
	}
	
	for( i=0; i<m_animated.size(); i++)
	{
		delete m_animated[i];
	}
	
	m_animations.clear();
	m_meshes.clear();
	m_skeletons.clear();
	m_animated.clear();
}

void akAnimationEngine::addAnimationClip(const utHashedString& name, akAnimationClip* anim)
{
	m_animations.insert(name.hash(), anim);
}

akAnimationClip* akAnimationEngine::getAnimationClip(const utHashedString &name)
{
	UTsize pos = m_animations.find(name.hash());
	if(pos==UT_NPOS)
		return 0;
	return m_animations.at(pos);
}

void akAnimationEngine::addMesh(const utHashedString& name, akMesh *mesh)
{
	m_meshes.insert(name.hash(), mesh);
}

akMesh * akAnimationEngine::getMesh(const utHashedString &name)
{
	UTsize pos = m_meshes.find(name.hash());
	if(pos==UT_NPOS)
		return 0;
	return m_meshes.at(pos);
}

void akAnimationEngine::addSkeleton(const utHashedString &name, akSkeleton *skel)
{
	m_skeletons.insert(name.hash(), skel);
}

akSkeleton * akAnimationEngine::getSkeleton(const utHashedString &name)
{
	UTsize pos = m_skeletons.find(name.hash());
	if(pos==UT_NPOS)
		return 0;
	return m_skeletons.at(pos);
}

void akAnimationEngine::addAnimatedObject(const utHashedString &name, akAnimatedObject* obj)
{
	m_animated.insert(name.hash(), obj);
}

akAnimatedObject* akAnimationEngine::getAnimatedObject(const utHashedString &name)
{
	UTsize pos = m_animated.find(name.hash());
	if(pos==UT_NPOS)
		return 0;
	return m_animated.at(pos);
}

void akAnimationEngine::stepTime(akScalar deltaTimeInSeconds)
{
	unsigned int i;
	
	for( i=0; i<m_animated.size(); i++)
	{
		akAnimatedObject* object = m_animated.at(i);
		object->stepTime(deltaTimeInSeconds);
	}
}

