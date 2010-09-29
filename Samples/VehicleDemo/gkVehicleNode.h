/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

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

#ifndef GKVEHICLENODE_H
#define GKVEHICLENODE_H

#include "OgreKit.h"
#include "gkVehicle.h"

class gkVehicleNode : public gkLogicNode
{
public:
	enum
	{
		UPDATE,
		FRONT,
		REAR,
		LEFT,
		RIGHT,
		STEER_TIME,
		HAND_BRAKE,
		GEAR_UP,
		GEAR_DOWN,
		ZROT,
		KMH,
		GEAR,
		RPM,
	};

	DECLARE_SOCKET_TYPE(UPDATE, bool);
	DECLARE_SOCKET_TYPE(FRONT, bool);
	DECLARE_SOCKET_TYPE(REAR, bool);
	DECLARE_SOCKET_TYPE(LEFT, bool);
	DECLARE_SOCKET_TYPE(RIGHT, bool);
	DECLARE_SOCKET_TYPE(STEER_TIME, gkScalar);
	DECLARE_SOCKET_TYPE(HAND_BRAKE, bool);
	DECLARE_SOCKET_TYPE(GEAR_UP, bool);
	DECLARE_SOCKET_TYPE(GEAR_DOWN, bool);
	DECLARE_SOCKET_TYPE(ZROT, gkScalar);
	DECLARE_SOCKET_TYPE(KMH, int);
	DECLARE_SOCKET_TYPE(GEAR, int);
	DECLARE_SOCKET_TYPE(RPM, int);

	gkVehicleNode(gkLogicTree* parent, size_t id);
	virtual ~gkVehicleNode() {}

	bool evaluate(gkScalar tick);

	void setVehicle(gkVehicle* v)   {m_vehicle = v;}

private:
	gkVehicle* m_vehicle;
	gkScalar   m_steer;
};

#endif // GKVEHICLENODE_H
