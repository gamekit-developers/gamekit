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
#ifndef _gkArcBallNode_h_
#define _gkArcBallNode_h_

#include "gkLogicNode.h"

class gkGameObject;
class gkRigidBody;

class gkArcBallNode : public gkLogicNode
{
public:

	enum 
	{
		UPDATE,
		CENTER_OBJ,
		CENTER_POSITION,
		RELX,
		RELY,
		RELZ,
		TARGET,
		MIN_PITCH,
		MAX_PITCH,
		MIN_ROLL,
		MAX_ROLL,
		KEEP_DISTANCE,
		CURRENT_ROLL,
		CURRENT_PITCH,
		MIN_Z,
		MAX_Z
	};

	DECLARE_SOCKET_TYPE(UPDATE, bool);
	DECLARE_SOCKET_TYPE(CENTER_OBJ, gkGameObject*);
	DECLARE_SOCKET_TYPE(CENTER_POSITION, gkVector3);
	DECLARE_SOCKET_TYPE(RELX, gkScalar);
	DECLARE_SOCKET_TYPE(RELY, gkScalar);
	DECLARE_SOCKET_TYPE(RELZ, gkScalar);
	DECLARE_SOCKET_TYPE(TARGET, gkGameObject*);
	DECLARE_SOCKET_TYPE(MIN_PITCH, gkScalar);
	DECLARE_SOCKET_TYPE(MAX_PITCH, gkScalar);
	DECLARE_SOCKET_TYPE(MIN_ROLL, gkScalar);
	DECLARE_SOCKET_TYPE(MAX_ROLL, gkScalar);
	DECLARE_SOCKET_TYPE(KEEP_DISTANCE, bool);
	DECLARE_SOCKET_TYPE(CURRENT_ROLL, gkQuaternion);
	DECLARE_SOCKET_TYPE(CURRENT_PITCH, gkQuaternion);
	DECLARE_SOCKET_TYPE(MIN_Z, gkScalar);
	DECLARE_SOCKET_TYPE(MAX_Z, gkScalar);

	gkArcBallNode(gkLogicTree *parent, size_t id);

	~gkArcBallNode();

	bool evaluate(gkScalar tick);
	void update(gkScalar tick);

	GK_INLINE gkLogicSocket<bool>* getUpdate() {return GET_SOCKET(UPDATE);}

    GK_INLINE gkLogicSocket<gkGameObject*>* getCenterObj() {return GET_SOCKET(CENTER_OBJ);}
	GK_INLINE gkLogicSocket<gkVector3>* getCenterPosition() {return GET_SOCKET(CENTER_POSITION);}

    GK_INLINE gkLogicSocket<gkScalar>* getRelX() {return GET_SOCKET(RELX);}
    GK_INLINE gkLogicSocket<gkScalar>* getRelY() {return GET_SOCKET(RELY);}
    GK_INLINE gkLogicSocket<gkScalar>* getRelZ() {return GET_SOCKET(RELZ);}

	GK_INLINE gkLogicSocket<gkScalar>* getMinPitch() {return GET_SOCKET(MIN_PITCH);}
	GK_INLINE gkLogicSocket<gkScalar>* getMaxPitch() {return GET_SOCKET(MAX_PITCH);}

	GK_INLINE gkLogicSocket<gkScalar>* getMinRoll() {return GET_SOCKET(MIN_ROLL);}
	GK_INLINE gkLogicSocket<gkScalar>* getMaxRoll() {return GET_SOCKET(MAX_ROLL);}

	GK_INLINE gkLogicSocket<bool>* getKeepDistance() {return GET_SOCKET(KEEP_DISTANCE);}

	GK_INLINE gkLogicSocket<gkQuaternion>* getRoll() {return GET_SOCKET(CURRENT_ROLL);}
	GK_INLINE gkLogicSocket<gkQuaternion>* getPitch() {return GET_SOCKET(CURRENT_PITCH);}

	GK_INLINE gkLogicSocket<gkScalar>* getMinZ() {return GET_SOCKET(MIN_Z);}
	GK_INLINE gkLogicSocket<gkScalar>* getMaxZ() {return GET_SOCKET(MAX_Z);}

	GK_INLINE gkLogicSocket<gkGameObject*>* getTarget() {return GET_SOCKET(TARGET);}

private:

	gkVector3 m_currentPosition;

	gkVector3 m_center;
	gkVector3 m_oldCenter;

	gkGameObject* m_target;
	gkGameObject* m_centerObj;

	gkQuaternion m_rollNode;
	gkQuaternion m_pitchNode;
};

#endif//_gkArcBallNode_h_