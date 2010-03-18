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
#ifndef _gkStateNode_h_
#define _gkStateNode_h_

#include "gkLogicNode.h"
#include "LinearMath/btQuickprof.h"

class gkStateNode : public gkLogicNode
{
public:

	enum
	{
		UPDATE,
		CURRENT_STATE,
		CURRENT_NAME,
		MAX_SOCKETS
	};

    gkStateNode(gkLogicTree *parent, size_t id);

	~gkStateNode();

	bool evaluate(gkScalar tick);
	void update(gkScalar tick);

    GK_INLINE gkLogicSocket* getUpdate() {return &m_sockets[UPDATE];}
	GK_INLINE gkLogicSocket* getCurrentState() {return &m_sockets[CURRENT_STATE];}
	GK_INLINE gkLogicSocket* getCurrentName() {return &m_sockets[CURRENT_NAME];}

	gkLogicSocket* addTransition(int from, int to, unsigned long ms = 0);

	void addTranslation(int state, const gkString& name);

private:

	gkLogicSocket m_sockets[MAX_SOCKETS];

	typedef utPointerHashKey EVENT;
	typedef utIntHashKey STATE;

	struct Data
	{
		unsigned long m_ms;
		int m_state;

		Data() : m_ms(0), m_state(0) {}

		Data(unsigned long ms, int state) 
			: m_ms(ms), m_state(state)
		{
		}
	};

	typedef utHashTable<EVENT, Data > REACTION;
	typedef utHashTableIterator<REACTION> REACTION_ITERATOR;

	typedef utHashTable<STATE, REACTION> TRANSITIONS;

	TRANSITIONS m_transitions;

	typedef utHashTable<STATE, gkString> TRANSLATION;

	TRANSLATION m_translation;

	btClock m_timer;

	int m_currentState;

	typedef utArray<gkLogicSocket*> EVENTS;

	EVENTS m_events;
};


#endif//_gkStateNode_h_
