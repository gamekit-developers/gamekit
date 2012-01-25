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

#include "akAnimationPlayerSet.h"

akAnimationPlayerSet::akAnimationPlayerSet()
{
}

akAnimationPlayerSet::~akAnimationPlayerSet()
{
	for(int i=0; i<m_players.size(); i++)
		delete m_players[i];
	m_players.clear();
}

akAnimationPlayer * akAnimationPlayerSet::addNewAnimationPlayer(akAnimationClip *clip)
{
	akAnimationPlayer *player = new akAnimationPlayer(clip);
	addAnimationPlayer(player);
	return player;
	
}

void akAnimationPlayerSet::addAnimationPlayer(akAnimationPlayer *player)
{
	if(m_players.find(player) == UT_NPOS)
		m_players.push_back(player);
}

void akAnimationPlayerSet::removeAnimationPlayer(akAnimationPlayer *player)
{
	UTsize i;
	if(i = m_players.find(player) != UT_NPOS)
		m_players.erase(i);
}

akAnimationPlayer * akAnimationPlayerSet::getAnimationPlayer(UTsize i)
{
	if( i!=UT_NPOS && i<m_players.size() )
		return m_players.at(i);
	return 0;
}

akAnimationPlayerSet::AnimationPlayers::ConstPointer akAnimationPlayerSet::getAnimationPlayers()
{
	return m_players.ptr();
}

void akAnimationPlayerSet::stepTime(akScalar deltaTimeInSeconds)
{
	akAnimationPlayer* const* ptr = m_players.ptr();
	int len = getNumAnimationPlayers();
	
	for(int i=0; i<len; i++)
	{
		ptr[i]->stepTime(deltaTimeInSeconds);
	}
}

void akAnimationPlayerSet::evaluate(akPose *pose) const
{
	akAnimationPlayer* const* ptr = m_players.ptr();
	int len = getNumAnimationPlayers();
	
	for(int i=0; i<len; i++)
	{
		ptr[i]->evaluate(pose);
	}
}

void akAnimationPlayerSet::evaluate(akSkeletonPose *pose) const
{
	akAnimationPlayer* const* ptr = m_players.ptr();
	int len = getNumAnimationPlayers();
	
	for(int i=0; i<len; i++)
	{
		ptr[i]->evaluate(pose);
	}
}

void akAnimationPlayerSet::evaluate(akTransformState *pose) const
{
	akAnimationPlayer* const* ptr = m_players.ptr();
	int len = getNumAnimationPlayers();
	
	for(int i=0; i<len; i++)
	{
		ptr[i]->evaluate(pose);
	}
}
