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
#include "AI/gkNavMeshData.h"
#include "AI/gkNavPath.h"

class gkGameObject;
class gkEntity;
class gkPhysicsDebug;

class gkCharacterNode : public gkStateMachineNode
{
public:

	enum 
	{
		//Inputs
		ANIM_BLEND_FRAMES = MAX_SOCKETS,
		ENABLE_GOTO,
		GOTO_POSITION,
		REDO_PATH,

		ENABLE_ROTATION,
		ROTATION_VALUE,

		//Outputs
		ANIM_HAS_REACHED_END,
		ANIM_NOT_HAS_REACHED_END,
		ANIM_TIME_POSITION,
		POSITION,
		ROTATION,
		AI_WANTED_STATE
	};

	DECLARE_SOCKET_TYPE(ANIM_BLEND_FRAMES, gkScalar);
	DECLARE_SOCKET_TYPE(ENABLE_GOTO, bool);
	DECLARE_SOCKET_TYPE(GOTO_POSITION, gkVector3);
	DECLARE_SOCKET_TYPE(REDO_PATH, bool);
	DECLARE_SOCKET_TYPE(ENABLE_ROTATION, bool);
	DECLARE_SOCKET_TYPE(ROTATION_VALUE, gkQuaternion);

	DECLARE_SOCKET_TYPE(ANIM_HAS_REACHED_END, bool);
	DECLARE_SOCKET_TYPE(ANIM_NOT_HAS_REACHED_END, bool);
	DECLARE_SOCKET_TYPE(ANIM_TIME_POSITION, gkScalar);
	DECLARE_SOCKET_TYPE(POSITION, gkVector3);
	DECLARE_SOCKET_TYPE(ROTATION, gkQuaternion);
	DECLARE_SOCKET_TYPE(AI_WANTED_STATE, int);
	

    gkCharacterNode(gkLogicTree *parent, size_t id);
	~gkCharacterNode();

	void initialize();
	bool evaluate(gkScalar tick);
	void update(gkScalar tick);
	
	typedef int STATE;

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

		bool operator < (const StateData& obj) const
		{
			return m_velocity < obj.m_velocity;
		}
	};

	typedef std::map<STATE, StateData> MAP;

	void setMapping(const MAP& map);
	void setObj(gkGameObject* obj) {m_obj = obj;}
	void setPolyPickExt(const gkVector3& polyPickExt) {m_polyPickExt = polyPickExt;}
	void setMaxPathPolys(int maxPathPolys) {m_maxPathPolys = maxPathPolys;}
	
private:
	void update_state(gkScalar tick);
	void update_animation(STATE oldState);
	void notifyState(int state);
	StateData* getStateData(int state);
	gkScalar getVelocityForDistance(gkScalar d, gkScalar tick, STATE& state) const;

private:

	MAP m_map;

	typedef std::vector<StateData> STATES_DATA;

	STATES_DATA m_statesData;

	gkGameObject* m_obj;
	gkEntity* m_ent;

	gkVector3 m_dir;
	gkVector3 m_up;

	gkNavPath m_navPath;

	gkVector3 m_polyPickExt;
	int m_maxPathPolys;

	StateData* m_currentStateData;

	gkScene* m_scene;

	bool m_createdNavMesh;
};


#endif//_gkCharacterNode_h_
