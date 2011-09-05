/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

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
#include "gkGamePlayer.h"
#include "Controllers/gkDefaultController.h"
#include "Controllers/gkJoystickController.h"
#include "gkGameLevel.h"
#include "gkCollisionCameraConstraint.h"
#include "OgreKit.h"
#include "Graphics/gkHUDManager.h"
#include "Graphics/gkHUD.h"
#include "Graphics/gkHUDElement.h"

#include "Animation/gkAnimationManager.h"


gkGamePlayer::gkGamePlayer(gkGameLevel* levelData, bool forceKeyMouseInput)
	:    m_levelData(levelData),
	     m_physics(0),
	     m_xRot(0),
	     m_zRot(0),
	     m_camera(0),
	     m_entity(0),
	     m_skeleton(0),
	     m_idleSwitch(0),
	     m_jumpFrom(-1),
	     m_textInit(false),
	     m_momoData(0),
	     m_currentState(0),
	     m_cameraData(0),
	     m_cameraState(0),
	     m_comboAttack(0)
{
	if (!forceKeyMouseInput && gkWindowSystem::getSingleton().getJoystick(0))
		m_input = new gkJoystickController(this);
	else
		m_input = new gkDefaultController(this);
}


gkGamePlayer::~gkGamePlayer()
{
	delete m_input;
}

void _setTransform(gkGameObject* gobj, gkGameObject* pobj)
{
	gkMatrix4 omat, pmat;

	gobj->getProperties().m_transform.toMatrix(omat);
	pobj->getProperties().m_transform.toMatrix(pmat);

	omat = pmat.inverse() * omat;

	gkTransformState st;
	gkMathUtils::extractTransform(omat, st.loc, st.rot, st.scl);


	// apply
	gobj->setTransform(st);
}

gkGamePlayer* gkGamePlayer::clone()
{
	gkGamePlayer* player = new gkGamePlayer(m_levelData, true);
	gkScene* dest = m_levelData->getLevel();

	player->m_physics = dest->cloneObject(m_physics, 0, true);
	player->m_skeleton = (gkSkeleton*)dest->cloneObject(m_skeleton, 0, true);
	player->m_entity = (gkEntity*)dest->cloneObject(m_entity, 0, false);
	player->m_entity->setSkeleton(player->m_skeleton);
	player->m_camera = 0;
	player->m_zRot = 0;	
	player->m_xRot = 0;	

	player->m_entity->createInstance();

	player->m_skeleton->addChild(player->m_entity);
	player->m_physics->addChild(player->m_skeleton);

#if 0
	_setTransform(player->m_entity, player->m_skeleton);
	_setTransform(player->m_skeleton, player->m_physics);
#else	
	player->m_skeleton->setTransform(m_skeleton->getTransform());
	player->m_entity->setTransform(m_entity->getTransform());	
	player->m_physics->setTransform(m_physics->getTransform());
#endif

	

	player->m_playerData.setup(player);
	
	player->loadConstraints();
	player->loadAnimations();

	//player->m_physics->rotate(gkEuler(0,0,180)); //?

	return player;
}

void gkGamePlayer::loadAnimations()
{
	// save animation states
	m_skeleton->addAnimation("Momo_Idle1");
	m_skeleton->addAnimation("Momo_IdleNasty");
	m_skeleton->addAnimation("Momo_Walk");
	m_skeleton->addAnimation("Momo_Run");
	m_skeleton->addAnimation("Momo_Jump");
	m_skeleton->addAnimation("Momo_TailWhip");
	m_skeleton->addAnimation("Momo_Kick");
	m_skeleton->addAnimation("Momo_WallFlip");
	
	m_animations[GK_ANIM_IDLE]        = m_skeleton->getAnimationPlayer("Momo_Idle1");
	m_animations[GK_ANIM_IDLE_NASTY]  = m_skeleton->getAnimationPlayer("Momo_IdleNasty");
	m_animations[GK_ANIM_WALK]        = m_skeleton->getAnimationPlayer("Momo_Walk");
	m_animations[GK_ANIM_RUN]         = m_skeleton->getAnimationPlayer("Momo_Run");
	m_animations[GK_ANIM_JUMP]        = m_skeleton->getAnimationPlayer("Momo_Jump");
	m_animations[GK_ANIM_WHIP]        = m_skeleton->getAnimationPlayer("Momo_TailWhip");
	m_animations[GK_ANIM_KICK]        = m_skeleton->getAnimationPlayer("Momo_Kick");
	m_animations[GK_ANIM_WALL_FLIP]   = m_skeleton->getAnimationPlayer("Momo_WallFlip");

	//m_animations[GK_ANIM_WALL_FLIP]->setMode(AK_ACT_END | AK_ACT_INVERSE);
	m_animations[GK_ANIM_IDLE_CURRENT] = m_animations[GK_ANIM_IDLE];

	//m_animations[GK_ANIM_JUMP]->setMode(AK_ACT_END);
	
	m_animations[GK_ANIM_WHIP]->setSpeedFactor(gkAppData::gkAnimationAttack);
	m_animations[GK_ANIM_KICK]->setSpeedFactor(gkAppData::gkAnimationAttack);

	//gkString groupName = playerData->getResourceGroup();
	
	// COMBO: Kick + Run + Whip
	m_comboAttack = gkAnimationManager::getSingleton().createAnimationSequence(gkUtils::getUniqueName("Momo_ComboAttack"));
	m_comboAttack->addItem("Momo_Kick",      0.f,       17.f/25.f, 0.f,      3.f/25.f);
	m_comboAttack->addItem("Momo_Run",       15.f/25.f, 32.f/25.f, 2.f/25.f, 4.f/25.f);
	m_comboAttack->addItem("Momo_TailWhip",  25.f/25.f, 45.f/25.f, 7.f/25.f, 0.f/25.f);

	m_animations[GK_ANIM_COMBO_ATTACK] = m_skeleton->addAnimation(m_comboAttack, "Momo_ComboAttack");
	m_animations[GK_ANIM_COMBO_ATTACK]->setMode(AK_ACT_END); //not LOOP



	setState(GK_PLAY_IDLE);


	FSM_TRANSITION_WHEN(this, GK_PLAY_IDLE, GK_PLAY_WALK, wantsToWalk);
	FSM_TRANSITION_WHEN(this, GK_PLAY_IDLE, GK_PLAY_RUN, wantsToRun);
	FSM_TRANSITION_WHEN(this, GK_PLAY_IDLE, GK_PLAY_JUMP, wantsToJump);
	FSM_TRANSITION_WHEN(this, GK_PLAY_IDLE, GK_PLAY_ATTACK_0, wantsToWhip);
	FSM_TRANSITION_WHEN(this, GK_PLAY_IDLE, GK_PLAY_ATTACK_COMBO, wantsComboAttack);


	FSM_TRANSITION_WHEN(this, GK_PLAY_ATTACK_0, GK_PLAY_ATTACK_1, wantsToKickAndWhipDone);
	FSM_TRANSITION_WHEN(this, GK_PLAY_ATTACK_0, GK_PLAY_JUMP, wantsToJump);
	FSM_TRANSITION_WHEN_DELAY(this, GK_PLAY_ATTACK_0, GK_PLAY_IDLE, 100, isDoneWhipAndNotKick);


	FSM_TRANSITION_WHEN(this, GK_PLAY_ATTACK_1, GK_PLAY_ATTACK_0, wantsToWhipAndKickDone);
	FSM_TRANSITION_WHEN(this, GK_PLAY_ATTACK_1, GK_PLAY_JUMP, wantsToJump);
	FSM_TRANSITION_WHEN_DELAY(this, GK_PLAY_ATTACK_1, GK_PLAY_IDLE, 100, isDoneKickAndNotWhip);


	FSM_TRANSITION_WHEN(this, GK_PLAY_ATTACK_COMBO, GK_PLAY_IDLE, isDoneComboAttack);
	FSM_TRANSITION_WHEN(this, GK_PLAY_ATTACK_COMBO, GK_PLAY_JUMP, wantsToJump);



	FSM_TRANSITION_WHEN(this, GK_PLAY_WALK, GK_PLAY_RUN, wantsToRun);
	FSM_TRANSITION_WHEN(this, GK_PLAY_WALK, GK_PLAY_JUMP, wantsToJump);
	FSM_TRANSITION_WHEN(this, GK_PLAY_WALK, GK_PLAY_IDLE, wantsToStop);
	FSM_TRANSITION_WHEN(this, GK_PLAY_WALK, GK_PLAY_ATTACK_0, wantsToWhip);
	FSM_TRANSITION_WHEN(this, GK_PLAY_WALK, GK_PLAY_ATTACK_COMBO, wantsComboAttack);


	FSM_TRANSITION_WHEN(this, GK_PLAY_RUN, GK_PLAY_IDLE, wantsToStop);
	FSM_TRANSITION_WHEN(this, GK_PLAY_RUN, GK_PLAY_JUMP, wantsToJump);
	FSM_TRANSITION_WHEN(this, GK_PLAY_RUN, GK_PLAY_WALK, wantsToWalk);
	FSM_TRANSITION_WHEN(this, GK_PLAY_RUN, GK_PLAY_RUN_ATTACK_0, wantsToWhip);
	FSM_TRANSITION_WHEN(this, GK_PLAY_RUN, GK_PLAY_ATTACK_COMBO, wantsComboAttack);


	FSM_TRANSITION_WHEN(this, GK_PLAY_RUN_ATTACK_0, GK_PLAY_RUN_ATTACK_1, wantsToKickAndWhipDone);
	FSM_TRANSITION_WHEN(this, GK_PLAY_RUN_ATTACK_0, GK_PLAY_JUMP, wantsToJump);
	FSM_TRANSITION_WHEN_DELAY(this, GK_PLAY_RUN_ATTACK_0, GK_PLAY_RUN, 100, isDoneWhip);


	FSM_TRANSITION_WHEN(this, GK_PLAY_RUN_ATTACK_1, GK_PLAY_RUN_ATTACK_0, wantsToWhipAndKickDone);
	FSM_TRANSITION_WHEN(this, GK_PLAY_RUN_ATTACK_1, GK_PLAY_JUMP, wantsToJump);
	FSM_TRANSITION_WHEN_DELAY(this, GK_PLAY_RUN_ATTACK_1, GK_PLAY_RUN, 100, isDoneKick);



	FSM_TRANSITION_WHEN(this, GK_PLAY_JUMP, GK_PLAY_RUN, isOnGroundAndFromRun);
	FSM_TRANSITION_WHEN(this, GK_PLAY_JUMP, GK_PLAY_LAND, isOnGroundAndNotFromRun);


	FSM_TRANSITION_WHEN(this, GK_PLAY_LAND, GK_PLAY_IDLE, isLandingDone);


	FSM_START_TRIG(this, GK_PLAY_IDLE, idleStart);
	FSM_START_TRIG(this, GK_PLAY_JUMP, jumpStart);
	FSM_START_TRIG(this, GK_PLAY_LAND, landStart);
	FSM_START_TRIG(this, GK_PLAY_ATTACK_COMBO, comboStart);
	FSM_START_TRIG(this, GK_PLAY_ATTACK_1, kickStart);


	FSM_END_TRIG(this, GK_PLAY_RUN, runEnd);
	FSM_END_TRIG(this, GK_PLAY_WALK, walkEnd);
	FSM_END_TRIG(this, GK_PLAY_IDLE, idleEnd);
	FSM_END_TRIG(this, GK_PLAY_LAND, landEnd);

	FSM_END_TRIG(this, GK_PLAY_ATTACK_1, kickEnd);
	FSM_END_TRIG(this, GK_PLAY_ATTACK_0, whipEnd);
	FSM_END_TRIG(this, GK_PLAY_RUN_ATTACK_0, runAttack0End);
	FSM_END_TRIG(this, GK_PLAY_RUN_ATTACK_1, runAttack1End);
}

void gkGamePlayer::loadConstraints()
{
	gkScene* dest = m_levelData->getLevel();

	gkPhysicsProperties& props = m_physics->getProperties().m_physics;
	props.m_mode |= GK_CONTACT;

	// add constraint clamping
	if (m_xRot)
	{
		gkLimitRotConstraint* lr = new gkLimitRotConstraint();
		lr->setLimitX(gkVector2(-90, 5));
		dest->addConstraint(m_xRot, lr);
	}

	if (m_physics)
	{
		gkLimitLocConstraint* ll = new gkLimitLocConstraint();
		ll->setMinX(-30.f);
		ll->setMaxX(30.f);
		ll->setMinY(-30.f);
		ll->setMaxY(30.f);
		ll->setMinZ(0.f);
		ll->setMaxZ(30.f);
		dest->addConstraint(m_physics, ll);
	}

	if (m_camera)
	{
		gkCollisionCameraConstraint* col = new gkCollisionCameraConstraint();
		col->setTarget(m_physics);
		col->setLength(.95f);
		col->setForwardOffs(.125f);
		col->setDownOffs(.125f);	
		dest->addConstraint(m_camera, col);
		m_camera->setMainCamera(true);
	}
}

void gkGamePlayer::loadHuds()
{
	m_momoData = gkHUDManager::getSingleton().getOrCreate("m_momoData");
	if (m_momoData)
	{
		m_currentState = m_momoData->getChild("m_momoData/m_currentState");
		m_momoData->show();
	}
	
	m_cameraData = gkHUDManager::getSingleton().getOrCreate("Camera_StateOverlay");
	if (m_cameraData)
	{
		m_cameraState = m_cameraData->getChild("Info");
		m_cameraData->show();
	}
}

void gkGamePlayer::load(gkBlendFile* playerData)
{
	gkScene* dest = m_levelData->getLevel();
	gkScene* tscene = playerData->getMainScene();


	GK_ASSERT(
	    tscene->hasObject(GK_RESOURCE_PLAYER_SKEL) &&
	    tscene->hasObject(GK_RESOURCE_PLAYER_MESH) &&
	    tscene->hasObject(GK_RESOURCE_PLAYER_VIEW) &&
	    tscene->hasObject(GK_RESOURCE_PLAYER_ZROT) &&
	    tscene->hasObject(GK_RESOURCE_PLAYER_XROT) &&
	    tscene->hasObject(GK_RESOURCE_PLAYER_PHYS)
	);


	m_skeleton = tscene->getObject(GK_RESOURCE_PLAYER_SKEL)->getSkeleton();
	m_entity   = tscene->getObject(GK_RESOURCE_PLAYER_MESH)->getEntity();
	m_camera   = tscene->getObject(GK_RESOURCE_PLAYER_VIEW)->getCamera();
	m_zRot     = tscene->getObject(GK_RESOURCE_PLAYER_ZROT);
	m_xRot     = tscene->getObject(GK_RESOURCE_PLAYER_XROT);
	m_physics  = tscene->getObject(GK_RESOURCE_PLAYER_PHYS);

	m_playerData.setup(this);
	
	dest->addObject(m_skeleton);
	dest->addObject(m_entity);
	dest->addObject(m_camera);
	dest->addObject(m_zRot);
	dest->addObject(m_xRot);
	dest->addObject(m_physics);

	loadConstraints();
	loadAnimations();
	loadHuds();
}

void gkGamePlayer::setPosition(const gkVector3& pos)
{
	GK_ASSERT(m_physics);
	m_physics->setPosition(pos);
}

void gkGamePlayer::setInitialText(void)
{

	if (m_currentState)
	{
		//if (m_levelData->getJoystick())
		//	m_currentState->setValue("Found game controller!");
		//else
		//	m_currentState->setValue("No game controller found...");
	}
}

void gkGamePlayer::notifyState(int state)
{
	if (m_currentState)
	{
		switch (state)
		{
		case GK_PLAY_IDLE:
			{
				if (m_animations[GK_ANIM_IDLE_CURRENT] == m_animations[GK_ANIM_IDLE_NASTY])
					m_currentState->setValue("IdleNasty");
				else
					m_currentState->setValue("Idle");
			}
			break;
		case GK_PLAY_WALK:
			m_currentState->setValue("Walking");
			break;
		case GK_PLAY_RUN:
			m_currentState->setValue("Running");
			break;
		case GK_PLAY_JUMP:
			m_currentState->setValue("Jumping");
			break;
		case GK_PLAY_ATTACK_0:
			m_currentState->setValue("Tail Whip");
			break;
		case GK_PLAY_ATTACK_1:
			m_currentState->setValue("Thrust Kick");
			break;
		case GK_PLAY_RUN_ATTACK_0:
			m_currentState->setValue("Run Tail Whip");
			break;
		case GK_PLAY_RUN_ATTACK_1:
			m_currentState->setValue("Run Thrust Kick");
			break;
		case GK_PLAY_LEDGE:
			m_currentState->setValue("Ledge Hang");
			break;
		case GK_PLAY_LAND:
			m_currentState->setValue("Landing");
			break;
		case GK_PLAY_ATTACK_COMBO:
			m_currentState->setValue("Kick, Run, Whip");
			break;
		}
	}
}



bool gkGamePlayer::isOnGroundAndNotFromRun(void)
{
	return !wantsToRun() && groundTest();
}


bool gkGamePlayer::isOnGroundAndFromRun(void)
{
	return !wantsToStop() && groundTest();
}

bool gkGamePlayer::wantsToRun(void)
{
	return m_input->hasInput(gkGameController::IC_RUN);
}


bool gkGamePlayer::wantsToWalk(void)
{
	return m_input->hasInput(gkGameController::IC_WALK);
}

bool gkGamePlayer::wantsToStop(void)
{
	return m_input->hasInput(gkGameController::IC_STOP);
}


bool gkGamePlayer::wantsToWhip(void)
{
	return m_input->hasInput(gkGameController::IC_BUTTON_0);
}

bool gkGamePlayer::wantsToJump(void)
{
	return m_input->hasInput(gkGameController::IC_BUTTON_1);
}

bool gkGamePlayer::wantsComboAttack(void)
{
	return m_input->hasInput(gkGameController::IC_BUTTON_2);
}


bool gkGamePlayer::wantsToWhipAndKickDone(void)
{
	return wantsToWhip() && m_animations[GK_ANIM_KICK]->getTimePosition() >= m_animations[GK_ANIM_KICK]->getLength() - 7.f/25.f;
}

bool gkGamePlayer::wantsToKickAndWhipDone(void)
{
	return wantsToWhip() && m_animations[GK_ANIM_WHIP]->getTimePosition() >= m_animations[GK_ANIM_WHIP]->getLength() - 7.f/25.f;
}


bool gkGamePlayer::isDoneComboAttack(void)
{
	return m_animations[GK_ANIM_COMBO_ATTACK]->isDone();
}


bool gkGamePlayer::isDoneKickAndNotWhip(void)
{
	return isDoneKick() && !wantsToWhip();
}


bool gkGamePlayer::isDoneWhipAndNotKick(void)
{
	return isDoneWhip() && !wantsToWhip();
}


bool gkGamePlayer::isLandingDone(void)
{
	return m_animations[GK_ANIM_JUMP]->isDone();
}

bool gkGamePlayer::isDoneWhip(void)
{
	return m_animations[GK_ANIM_WHIP]->isDone();
}

bool gkGamePlayer::isDoneKick(void)
{
	return m_animations[GK_ANIM_KICK]->isDone();
}

bool gkGamePlayer::isOnLedge(void)
{
	return false;
}

void gkGamePlayer::idleStart(int from, int to)
{
	m_animations[GK_ANIM_IDLE_CURRENT]->setTimePosition(0.f);
}


void gkGamePlayer::jumpStart(int from, int to)
{
	m_animations[GK_ANIM_JUMP]->reset();
	m_animations[GK_ANIM_JUMP]->setSpeedFactor(1.0f);
	applyJump();

}

void gkGamePlayer::comboStart(int from, int to)
{
	m_animations[GK_ANIM_COMBO_ATTACK]->reset();
	applyComboThrust(3.5556f);
}


void gkGamePlayer::landStart(int from, int to)
{
	/// Move jump toward the end
	m_animations[GK_ANIM_JUMP]->setTimePosition(20.f/25.f);
	m_animations[GK_ANIM_JUMP]->setSpeedFactor(gkAppData::gkAnimationFast);
}

void gkGamePlayer::kickStart(int from, int to)
{
	applyComboThrust(2.f);
}



void gkGamePlayer::idleEnd(int from, int to)
{
	if ((m_idleSwitch++) > 3)
	{
		m_idleSwitch = 0;
		m_animations[GK_ANIM_IDLE_CURRENT] = m_animations[GK_ANIM_IDLE_NASTY];
	}
	else
		m_animations[GK_ANIM_IDLE_CURRENT] = m_animations[GK_ANIM_IDLE];



	switch (to)
	{
	case GK_PLAY_JUMP:
		{
			m_jumpFrom = from;
			applyJump();
		} break;
	case GK_PLAY_IDLE:
		{
			// do skid stop
		}
	default:
		break;
	}
}



void gkGamePlayer::walkEnd(int from, int to)
{
	switch (to)
	{
	case GK_PLAY_JUMP:
		{
			m_jumpFrom = from;
			applyJump();
		} break;
	case GK_PLAY_IDLE:
		{
			// do skid stop
		}
	default:
		break;
	}
}

void gkGamePlayer::runEnd(int from, int to)
{
	switch (to)
	{
	case GK_PLAY_JUMP:
		{
			m_jumpFrom = from;
			applyJump();
		} break;
	case GK_PLAY_RUN_ATTACK_0:
	case GK_PLAY_RUN_ATTACK_1:
		applyComboThrust();
		break;

	case GK_PLAY_IDLE:
		{
			// do skid stop
		}
	default:
		break;
	}
}


void gkGamePlayer::jumpEnd(int from, int to)
{
	switch (to)
	{
	case GK_PLAY_RUN:
		{
			m_jumpFrom = -1;

			m_animations[GK_ANIM_JUMP]->reset();

			// hit the ground running
			m_animations[GK_ANIM_RUN]->setTimePosition(9.f);
		} break;
	case GK_PLAY_IDLE:
		{
			// do skid stop
		}
	default:
		break;
	}
}


void gkGamePlayer::runAttack0End(int from, int to)
{
	switch (to)
	{
	case GK_PLAY_RUN_ATTACK_1:
		applyComboThrust();
		break;
	default:
		break;
	}

}

void gkGamePlayer::runAttack1End(int from, int to)
{
	switch (to)
	{
	case GK_PLAY_RUN_ATTACK_0:
		applyComboThrust();
		break;
	default:
		break;
	}

}

void gkGamePlayer::landEnd(int from, int to)
{
	m_animations[GK_ANIM_JUMP]->reset();
	m_animations[GK_ANIM_JUMP]->setSpeedFactor(1.0f);
}



void gkGamePlayer::whipEnd(int from, int to)
{
	m_animations[GK_ANIM_WHIP]->reset();
}

void gkGamePlayer::kickEnd(int from, int to)
{
	m_animations[GK_ANIM_KICK]->reset();
}




bool gkGamePlayer::groundTest(void)
{	
	gkScene* scene = m_physics->getOwner();
	gkDynamicsWorld* dyn = scene->getDynamicsWorld();

	btDynamicsWorld* btw = dyn->getBulletWorld();

	const gkScalar range = gkAppData::gkPlayerHeadZ;
	const gkScalar angle = range * gkMath::Tan(22.5f);


	const gkVector3 vec    = m_physics->getPosition();
	const gkQuaternion ori = m_physics->getOrientation();
	const gkVector3 dir    = gkVector3(0, 0, -gkAppData::gkPlayerHeadZ / 2.f);

	btTransform btt;
	btt.setIdentity();
	btt.setOrigin(btVector3(vec.x + dir.x, vec.y + dir.y, vec.z + dir.z));

	gkAllContactResultCallback exec;

	btConeShapeZ btcs(angle, range);
	btCollisionObject btco;
	btco.setCollisionShape(&btcs);
	btco.setWorldTransform(btt);

	btw->contactTest( &btco, exec );

	if (btw->getDebugDrawer())
		btw->debugDrawObject(btt, &btcs, btVector3(0.f, 1.f, 0.f));


	if (!exec.hasHit())
		return false;

	if (!exec.m_contactObjects.empty())
		exec.m_contactObjects.erase(m_physics->getCollisionObject());

	if (!exec.m_contactObjects.empty())
	{
		utArray<const btCollisionObject*>::ConstIterator cit = exec.m_contactObjects.iterator();
		while (cit.hasMoreElements())
		{
			gkGameObject* ob = gkPhysicsController::castObject(cit.getNext());

			if (gkPhysicsController::sensorTest(ob, "Floor", "", true) ||
			        gkPhysicsController::sensorTest(ob, "Crate", "", true))
			{
				return true;
			}
		}
	}
	return false;
}


void gkGamePlayer::applyJump(void)
{
	gkVector3 vec = m_physics->getLinearVelocity();
	vec.z = gkAppData::gkPlayerImpulseZ;

	if (m_jumpFrom == GK_PLAY_RUN)
		vec.y *= 1.1111f;

	m_physics->setLinearVelocity(vec);
}



void gkGamePlayer::whipState(void)
{
	m_entity->playAnimation(m_animations[GK_ANIM_WHIP], gkAppData::gkGlobalActionBlend);
}

void gkGamePlayer::kickState(void)
{
	m_entity->playAnimation(m_animations[GK_ANIM_KICK], gkAppData::gkGlobalActionBlend);
}


void gkGamePlayer::applyComboThrust(gkScalar fac)
{

	gkVector3 m = m_entity->getOrientation() * gkVector3(0, fac, m_physics->getLinearVelocity().z);
	m_physics->setLinearVelocity(m);
}


void gkGamePlayer::comboState(void)
{
	static gkScalar prev = -1;
	gkScalar cur = m_animations[GK_ANIM_COMBO_ATTACK]->getTimePosition();

	if( prev < cur && prev <15.f/25.f && cur >15.f/25.f)
		applyComboThrust();
		
	prev = cur;
	
	m_entity->playAnimation(m_animations[GK_ANIM_COMBO_ATTACK], gkAppData::gkGlobalActionBlend);
}

void gkGamePlayer::moveState(void)
{
	m_input->movePlayer();

	m_entity->playAnimation(m_animations[this->getState() == GK_PLAY_WALK ? GK_ANIM_WALK : GK_ANIM_RUN], gkAppData::gkGlobalActionBlend);
}




void gkGamePlayer::idleState(void)
{
	m_entity->playAnimation(m_animations[GK_ANIM_IDLE_CURRENT], gkAppData::gkGlobalActionBlend);
}


void gkGamePlayer::landState(void)
{
	m_entity->playAnimation(m_animations[GK_ANIM_JUMP], 0.f);
}



void gkGamePlayer::jumpState(void)
{
	if (m_animations[GK_ANIM_JUMP]->getTimePosition() > 17.f/25.f)
	{
		m_animations[GK_ANIM_JUMP]->setTimePosition(17.f/25.f);
		m_animations[GK_ANIM_JUMP]->setSpeedFactor(0);
	}

	m_entity->playAnimation(m_animations[GK_ANIM_JUMP], gkAppData::gkGlobalActionBlend);
}


void gkGamePlayer::update(gkScalar delta)
{
	if (!m_textInit)
	{
		m_textInit = true;
		setInitialText();
	}

	m_input->updateInputState();
	if (m_camera)
		m_input->moveCamera();

	gkFSM::update();

	switch (getState())
	{
	case GK_PLAY_IDLE:
		idleState();
		break;
	case GK_PLAY_WALK:
	case GK_PLAY_RUN:
		moveState();
		break;
	case GK_PLAY_JUMP:
		jumpState();
		break;
	case GK_PLAY_LAND:
		landState();
		break;
	case GK_PLAY_ATTACK_COMBO:
		comboState();
		break;
	case GK_PLAY_RUN_ATTACK_0:
	case GK_PLAY_ATTACK_0:
		whipState();
		break;
	case GK_PLAY_RUN_ATTACK_1:
	case GK_PLAY_ATTACK_1:
		kickState();
		break;
	}
}
