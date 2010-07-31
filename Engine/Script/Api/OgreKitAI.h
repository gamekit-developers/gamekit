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
#ifndef _OgreKitAI_h_
#define _OgreKitAI_h_

#include "OgreKitCommon.h"
#include "OgreKitMath.h"
#include "OgreKitUtils.h"
#include "Script/Lua/gkLuaUtils.h"


namespace OgreKit
{

// WhenEvent
class WhenEvent
{
public:
	~WhenEvent();
	void when(Self self, Function when);

	OGRE_KIT_WRAP_CLASS_COPY_CTOR(WhenEvent, gkFSM::Event, m_event);
};


class FSM
{
private:
	typedef utHashTable<utIntHashKey, class FSM_UpdateEvent *> EVENT;

	gkFSM	*m_fsm;
	int		m_curState;
	EVENT	m_events;

public:

	FSM();
	~FSM();


	void  update();
	void  setState(int state);
	int   getState();



	void  addStartTrigger(int state, Self self, Function trigger);
	void  addEndTrigger(int state, Self self, Function trigger);


	void  addEvent(int state, Self self, Function update);

	Pointer<WhenEvent> addTransition(int from, int to);
	Pointer<WhenEvent> addTransition(int from, int to, unsigned long ms);
	Pointer<WhenEvent> addTransition(int from, int to, unsigned long ms, Self self, Function trigger);

};


}


#endif//_OgreKitAI_h_
