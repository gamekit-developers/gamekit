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

#ifndef AKANIMATIONPLAYERSET_H
#define AKANIMATIONPLAYERSET_H

#include "akAnimationClip.h"
#include "akAnimationPlayer.h"


/// A container for animation players
/// Permit to advance all enabled players at once, synchronise them, ...
/// Once all the time postions and weigths are set, it can evaluate all
/// the enabled animations it contain's. The blending is additive so make sure
/// the pose you pass evaluate() is set to 0 (identity).
class akAnimationPlayerSet
{
public:
	typedef utArray<akAnimationPlayer*> AnimationPlayers;

private:
	AnimationPlayers m_players;
	
public:
	akAnimationPlayerSet();
	~akAnimationPlayerSet();
	
	akAnimationPlayer* addNewAnimationPlayer(akAnimationClip *anim);
	
	void addAnimationPlayer(akAnimationPlayer *player);
	
	void removeAnimationPlayer(akAnimationPlayer *player);
	
	akAnimationPlayer* getAnimationPlayer(UTsize i);
	
	AnimationPlayers::ConstPointer getAnimationPlayers();
	
	void stepTime(akScalar seconds);
	
	void evaluate(akSkeletonPose* pose) const;
	void evaluate(akTransformState* pose) const;
	
	UT_INLINE int getNumAnimationPlayers() const
	{
		return m_players.size();
	}
};

#endif // AKANIMATIONPLAYERSET_H
