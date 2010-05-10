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

class SceneLogic;

class MomoLogic : public gkReferences
{
public:
	MomoLogic(const gkString& name, SceneLogic* scene);
	~MomoLogic();

private:

	void CreateNodes();
	void CreatePlayer();
	void CreatePathfinding();
	void CreateKick();
	void CreateGrab();
	void CreateMove();
	void CreateDustTrail();
	void CreateLoadUnload();
	void CreateAnimation();
	void CreateStateMachine();
	void CreateCamera();

public:

	gkObjNode* m_playerNode;
	gkRayTestNode* m_kickTestNode;

private:

	gkString m_name;
	SceneLogic* m_scene;
	gkLogicTree* m_tree;
	gkAnimationNode* m_animNode;
	gkFindPathNode* m_pathFindingNode;
	gkStateMachineNode* m_stateMachineNode;
	gkFollowPathNode* m_followPathNode;
	gkGrabNode* m_momoGrab;
	gkCameraNode* m_cameraNode;
};

#endif//_MomoLogic_h_
