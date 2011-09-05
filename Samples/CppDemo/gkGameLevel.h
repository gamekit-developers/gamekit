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
#ifndef _gkGameLevel_h_
#define _gkGameLevel_h_


#include "gkEngine.h"
#include "gkApplication.h"
#include "gkInput.h"
#include "gkInstancedManager.h"
#include "gkResourceManager.h"


class gkGameLevel : public gkEngine::Listener,
	public gkInstancedManager::InstancedListener,
	public gkResourceManager::ResourceListener
{
public:

	gkGameLevel();
	virtual ~gkGameLevel();


	void loadLevel(const gkLevel& level);

	gkScene* getLevel(void);

	gkJoystick* getJoystick(void) {return m_joy;}

protected:

	void notifyInstanceCreated(gkInstancedObject* inst);
	void notifyResourceCreated(gkResource* res);

	void loadPickup(void);

	gkScene* m_pickup;
	gkScene* m_killThemAll;
	gkLevel m_level;

	gkGamePlayer* m_player;
	utArray<gkGamePlayer*> m_enemies;

	gkKeyboard* m_keyboard;
	gkMouse* m_mouse;
	gkJoystick* m_joy;
	void tick(gkScalar delta);
};


#endif//_gkGameLevel_h_
