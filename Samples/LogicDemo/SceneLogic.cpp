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

#include "SceneLogic.h"
#include "MomoLogic.h"
#include "RatLogic.h"
#include "Common.h"
#include "DetourNavMesh.h"

namespace object
{
gkString PLAYER = "Player";
gkString CAMERA = "View";
}

namespace material
{
gkString ARROW_CURSOR = "ArrowCursor";
}

SceneLogic::SceneLogic(gkScene* pScene)
	: m_scene(pScene),
	  m_tree(0),
	  m_momo(0),
	  m_camera(pScene->getMainCamera()),
	  m_navMeshData(0),
	  m_activeObject("SceneLogic")
{
	GK_ASSERT(m_scene);
	m_tree = gkNodeManager::getSingleton().createLogicTree(m_scene->getGroupName());

	CreateInput();

	CreateMomo();
	CreateRats();

	CreateExit();
	CreateCursor();
	CreatePick();
	CreateDebug();

	m_tree->solveOrder();

	m_camera->attachLogic(m_tree);

	m_navMeshData = PNAVMESHDATA(new gkNavMeshData(pScene));

	pScene->setNavMeshData(m_navMeshData);

	m_navMeshData->createInstances();
}

SceneLogic::~SceneLogic()
{
	m_activeObject.join();
}

void SceneLogic::CreateMomo()
{
	m_momo = PMOMO(new MomoLogic(m_scene->getObject(object::PLAYER), this));
}

void SceneLogic::CreateRats()
{
	const gkGameObjectSet& objs = m_scene->getInstancedObjects();

	gkGameObjectSet::ConstIterator it = objs.iterator();

	while (it.hasMoreElements())
	{
		gkGameObject* ob = it.getNext();

		if (ob->getProperties().isDynamic())
		{
			if (ob->getName().find("Rat") != gkString::npos)
			{
				m_rats.push_back(PRAT(new RatLogic(ob, this, m_momo)));
			}
		}

	}
}

void SceneLogic::CreateInput()
{
	m_ctrlKeyNode = m_tree->createNode<gkKeyNode>();
	m_ctrlKeyNode->setKey(KC_TABKEY);

	m_shiftKeyNode = m_tree->createNode<gkKeyNode>();
	m_shiftKeyNode->setKey(KC_LEFTSHIFTKEY);

	m_wKeyNode = m_tree->createNode<gkKeyNode>();
	m_wKeyNode->setKey(KC_WKEY);

	m_sKeyNode = m_tree->createNode<gkKeyNode>();
	m_sKeyNode->setKey(KC_SKEY);

	m_cKeyNode = m_tree->createNode<gkKeyNode>();
	m_cKeyNode->setKey(KC_CKEY);

	m_spcKeyNode = m_tree->createNode<gkKeyNode>();
	m_spcKeyNode->setKey(KC_SPACEKEY);

	m_mouseNode = m_tree->createNode<gkMouseNode>();

	m_leftMouseNode = m_tree->createNode<gkMouseButtonNode>();

	m_rightMouseNode = m_tree->createNode<gkMouseButtonNode>();
	m_rightMouseNode->setButton(gkMouse::Right);
}


void SceneLogic::CreateExit()
{
	gkExitNode* exit = m_tree->createNode<gkExitNode>();

	gkKeyNode* key = m_tree->createNode<gkKeyNode>();

	key->setKey(KC_ESCKEY);

	exit->getEXIT()->link(key->getPRESS());
}

void SceneLogic::CreateCursor()
{
	gkCursorNode* cursor = m_tree->createNode<gkCursorNode>();

	cursor->getENABLE()->link(m_ctrlKeyNode->getIS_DOWN());
	cursor->getUPDATE()->link(m_mouseNode->getMOTION());
	cursor->getX()->link(m_mouseNode->getABS_X());
	cursor->getY()->link(m_mouseNode->getABS_Y());
	cursor->getMATERIAL_NAME()->setValue(material::ARROW_CURSOR);
}

void SceneLogic::CreatePick()
{
	gkPickNode* pick = m_tree->createNode<gkPickNode>();

	pick->getUPDATE()->link(m_ctrlKeyNode->getIS_DOWN());
	pick->getCREATE_PICK()->link(m_rightMouseNode->getPRESS());
	pick->getRELEASE_PICK()->link(m_rightMouseNode->getRELEASE());
	pick->getX()->link(m_mouseNode->getABS_X());
	pick->getY()->link(m_mouseNode->getABS_Y());
}

void SceneLogic::CreateDebug()
{
	gkShowPhysicsNode* showPhysics = m_tree->createNode<gkShowPhysicsNode>();
	showPhysics->getENABLE()->link(m_cKeyNode->getIS_DOWN());
}

void SceneLogic::refreshNavigationMesh()
{
	tryToUpdateNavMeshForSteeringObjs();

	gkRecast::Config config;

	m_scene->asyncTryToCreateNavigationMesh(m_activeObject, config, m_navMesh);
}

void SceneLogic::tryToUpdateNavMeshForSteeringObjs()
{
	if (m_navMesh.hasResult())
	{
		PDT_NAV_MESH navMesh = m_navMesh.getResult();

		m_momo->m_steeringObject->setNavMesh(navMesh);

		RATQUEUE::iterator it = m_rats.begin();

		while (it != m_rats.end())
		{
			PRAT rat = *it;

			rat->m_steeringFollowing->setNavMesh(navMesh);

			++it;
		}
	}
}

void SceneLogic::tick(gkScalar rate)
{
	tryToUpdateNavMeshForSteeringObjs();
}
