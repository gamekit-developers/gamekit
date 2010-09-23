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

#ifndef _SceneLogic_h_
#define _SceneLogic_h_

#include "OgreKit.h"

class MomoLogic;
class RatLogic;
class dtNavMesh;

class SceneLogic
{
public:
	SceneLogic(gkScene* pScene);
	~SceneLogic();

	void refreshNavigationMesh();
	void tryToUpdateNavMeshForSteeringObjs();
	void tick(gkScalar rate);

private:

	void CreateInput();
	void CreateExit();
	void CreateCursor();
	void CreatePick();
	void CreateCursorCameraArcBall();
	void CreateDebug();
	void CreateMomo();
	void CreateRats();
	void CreateCamera();

public:

	gkScene* m_scene;

	gkLogicTree* m_tree;

	gkKeyNode* m_ctrlKeyNode;

	gkKeyNode* m_shiftKeyNode;

	gkKeyNode* m_wKeyNode;

	gkKeyNode* m_sKeyNode;

	gkKeyNode* m_cKeyNode;

	gkKeyNode* m_spcKeyNode;

	gkMouseNode* m_mouseNode;

	gkMouseButtonNode* m_leftMouseNode;

	gkMouseButtonNode* m_rightMouseNode;

	gkGameObject* m_camera;

	PNAVMESHDATA m_navMeshData;

	gkActiveObject m_activeObject;

	gkScene::ASYNC_DT_RESULT m_navMesh;

private:

	typedef gkPtrRef<MomoLogic> PMOMO;
	typedef gkPtrRef<RatLogic> PRAT;

	PMOMO m_momo;

	typedef std::deque<PRAT> RATQUEUE;
	RATQUEUE m_rats;
};

#endif//_SceneLogic_h_
