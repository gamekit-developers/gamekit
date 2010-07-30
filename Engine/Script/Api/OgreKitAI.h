/*
-------------------------------------------------------------------------------
--                                                                           --
--  This file is part of GameKit.                                            --
--  http://gamekit.googlecode.com/                                           --
--                                                                           --
--                                                                           --
--  Released under the zlib license.                                         --
--  http://www.gzip.org/zlib/zlib_license.html                               --
--                                                                           --
--                                                                           --
-------------------------------------------------------------------------------
-- OgreKit AI Bindings
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
    typedef utHashTable<utIntHashKey, class FSM_UpdateEvent*> EVENT;

    gkFSM       *m_fsm;
    int         m_curState;
    EVENT       m_events;

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
