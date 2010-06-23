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
#ifndef _gkCharacterNode_h_
#define _gkCharacterNode_h_

#include "gkStateMachineNode.h"

class gkGameObject;
class gkEntity;
class gkPhysicsDebug;
class gkSteeringObject;

class gkCharacterNode : public gkStateMachineNode
{
public:

	static const int NULL_STATE = -1;

	typedef int STATE;

    class Listener
    {
    public:
        virtual ~Listener() {}
		virtual STATE updateAI(gkCharacterNode* obj, gkScalar tick) = 0;
    };

	enum 
	{
		//Inputs
		ANIM_BLEND_FRAMES = MAX_SOCKETS,

		ENABLE_ROTATION,
		ROTATION_VALUE,

		//Outputs
		ANIM_HAS_REACHED_END,
		ANIM_NOT_HAS_REACHED_END,
		ANIM_TIME_POSITION,
		POSITION,
		ROTATION,
		AI_STATE,
		FALLING,
		NOT_FALLING
	};

	DECLARE_SOCKET_TYPE(ANIM_BLEND_FRAMES, gkScalar);
	DECLARE_SOCKET_TYPE(ENABLE_ROTATION, bool);
	DECLARE_SOCKET_TYPE(ROTATION_VALUE, gkQuaternion);

	DECLARE_SOCKET_TYPE(ANIM_HAS_REACHED_END, bool);
	DECLARE_SOCKET_TYPE(ANIM_NOT_HAS_REACHED_END, bool);
	DECLARE_SOCKET_TYPE(ANIM_TIME_POSITION, gkScalar);
	DECLARE_SOCKET_TYPE(POSITION, gkVector3);
	DECLARE_SOCKET_TYPE(ROTATION, gkQuaternion);
	DECLARE_SOCKET_TYPE(AI_STATE, STATE);
	DECLARE_SOCKET_TYPE(FALLING, bool);
	DECLARE_SOCKET_TYPE(NOT_FALLING, bool);

    gkCharacterNode(gkLogicTree *parent, size_t id);
	~gkCharacterNode();

	void initialize();
	bool evaluate(gkScalar tick);
	void update(gkScalar tick);
	
	struct StateData
	{
		STATE m_state;
		bool m_loop;
		gkString m_animName;
		gkScalar m_velocity;
		bool m_allow_rotation;

		StateData() : m_state(-1), m_loop(false), m_velocity(0) {}
		StateData(STATE state, const gkString& animName, bool loop, gkScalar velocity, bool allow_rotation = true)
			: m_state(state), m_animName(animName), m_loop(loop), m_velocity(velocity), m_allow_rotation(allow_rotation) {}
	};

	typedef std::map<STATE, StateData> MAP;

	void setMapping(const MAP& map);
	void setObj(gkGameObject* obj) {m_obj = obj;}

	GK_INLINE void setListener(Listener *listener) { m_listener = listener; }
	GK_INLINE void setForward(const gkVector3& forward) { m_forward = forward; }
	
private:
	void update_state(gkScalar tick);
	void update_animation(STATE oldState);
	void notifyState(int state);
	StateData* getStateData(int state);
	
private:

	MAP m_map;

	gkGameObject* m_obj;
	gkEntity* m_ent;

	StateData* m_currentStateData;

	gkScene* m_scene;

	Listener* m_listener;

	gkVector3 m_forward;
};


#endif//_gkCharacterNode_h_
