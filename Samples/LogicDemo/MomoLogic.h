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

#ifndef _MomoLogic_h_
#define _MomoLogic_h_

#include "OgreKit.h"
#include "ACalls.h"

class SceneLogic;

class MomoLogic : public gkReferences
{
public:
	MomoLogic(gkGameObject* obj, SceneLogic* scene);

	~MomoLogic();

	gkCharacterNode::STATE updateAI(gkScalar tick);

	gkScalar getGravity();
	bool hasImpactGround();


private:

	void CreateKick();
	void CreateGrab();
	void CreateDustTrail();
	void CreateImpactGroundFX();
	void CreateStateMachine();
	void CreateCamera();
	void handleLand2IdleTranstion(int fromState, int toState);
	void StartLand(int from, int to);
	void EndLand(int from, int to);

public:

	gkCharacterNode* m_characterNode;
	gkRayTestNode* m_kickTestNode;
	gkGameObject* m_obj;

private:

	SceneLogic* m_scene;
	gkLogicTree* m_tree;

	gkGrabNode* m_momoGrab;
	gkCameraNode* m_cameraNode;

	gkSteeringPathFollowing* m_steeringObject;
	gkScreenRayTestNode* m_screenTargetNode;

	gkIfNode<bool, CMP_AND>* m_ifSelectNode;

	bool m_following;
	bool m_hasImpactGround;

	friend class SceneLogic;
};

#endif//_MomoLogic_h_
