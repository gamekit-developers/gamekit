/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Nestor Silveira.

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

#ifndef _RatLogic_h_
#define _RatLogic_h_

#include "OgreKit.h"

class SceneLogic;
class MomoLogic;

class RatLogic : public gkReferences, public gkCharacterNode::Listener
{
	typedef gkPtrRef<MomoLogic> PMOMO;

public:
	RatLogic(gkGameObject* obj, SceneLogic* scene, PMOMO momo);
	~RatLogic();
	gkCharacterNode* getCharacterNode() const {return m_characterNode; }
	gkCharacterNode::STATE updateAI(gkCharacterNode* obj, gkScalar tick);
private:

	void getBehaviour();

private:
	gkGameObject* m_obj;
	SceneLogic* m_scene;
	gkLogicTree* m_tree;
	PMOMO m_momo;
	gkCharacterNode* m_characterNode;
	gkSteeringObject* m_steeringObject;
	gkSteeringCapture* m_steeringCapture;
	gkSteeringPathFollowing* m_steeringFollowing;
};

#endif//_RatLogic_h_
