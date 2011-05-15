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
#ifndef _gkGamePlayer_h_
#define _gkGamePlayer_h_


#include "gkEngine.h"
#include "gkApplication.h"
#include "Animation/gkAnimation.h"
#include "gkFSM.h"


class gkRadarSensor;
class akAnimationSequence;



class gkGamePlayer : public gkFSM
{
public:

	struct Data
	{
		gkGameObject* m_physics;
		gkGameObject* m_xRot, *m_zRot;
		gkCamera*     m_camera;
		gkEntity*     m_entity;
		gkSkeleton*   m_skeleton;

		void setup(gkGamePlayer* player)
		{
			GK_ASSERT(player);

			m_physics = player->m_physics;
			m_xRot = player->m_xRot;
			m_zRot = player->m_zRot;
			m_camera = player->m_camera;
			m_entity = player->m_entity;
			m_skeleton = player->m_skeleton;
		}
	};

	friend struct Data;
public:

	gkGamePlayer(gkGameLevel* levelData, bool forceKeyMouseInput=false);
	~gkGamePlayer();


	void load(gkBlendFile* playerData);


	void update(gkScalar delta);

	void notifyState(int state);

	Data& getData(void) {return m_playerData;}

	gkGamePlayer* clone();

	void setPosition(const gkVector3& pos);

protected:
	void loadAnimations();
	void loadConstraints();
	void loadHuds();

	void setInitialText(void);

	bool wantsToWalk(void);
	bool wantsToRun(void);
	bool wantsToStop(void);
	bool wantsToJump(void);
	bool wantsToWhip(void);
	bool wantsToWhipAndKickDone(void);
	bool wantsToKickAndWhipDone(void);

	bool wantsComboAttack(void);

	bool isOnLedge(void);
	bool isDoneWhip(void);
	bool isDoneKick(void);
	bool isDoneKickAndNotWhip(void);
	bool isDoneWhipAndNotKick(void);

	bool isLandingDone(void);
	bool isOnGroundAndNotFromRun(void);
	bool isOnGroundAndFromRun(void);

	bool isDoneComboAttack(void);


	// starting triggers
	void idleStart(int from, int to);
	void jumpStart(int from, int to);
	void landStart(int from, int to);
	void comboStart(int from, int to);
	void kickStart(int from, int to);


	// ending triggers
	void idleEnd(int from, int to);
	void walkEnd(int from, int to);
	void runEnd(int from, int to);
	void jumpEnd(int from, int to);
	void landEnd(int from, int to);
	void whipEnd(int from, int to);
	void kickEnd(int from, int to);
	void runAttack0End(int from, int to);
	void runAttack1End(int from, int to);


	void whipState(void);
	void moveState(void);
	void idleState(void);
	void jumpState(void);
	void landState(void);
	void comboState(void);
	void kickState(void);

	void applyComboThrust(gkScalar fac = 5.f);
	void applyJump(void);
	bool groundTest(void);

	bool m_textInit;

	int m_jumpFrom;

	gkGameLevel*  m_levelData;


	Data          m_playerData;

	gkGameObject* m_physics;
	gkGameObject* m_xRot, *m_zRot;
	gkCamera*     m_camera;
	gkEntity*     m_entity;
	gkSkeleton*   m_skeleton;

	gkAnimations m_animations;
	gkAnimationSequence* m_comboAttack;

	int m_idleSwitch;


	gkHUD* m_momoData, *m_cameraData;
	gkHUDElement* m_currentState, *m_cameraState;


	gkGameController *m_input;
};


#endif//_gkGamePlayer_h_
