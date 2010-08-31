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

#ifndef VDVEHICLENODE_H
#define VDVEHICLENODE_H

#include "OgreKit.h"
#include "vdVehicle.h"

class vdVehicleNode : public gkLogicNode
{
public:
	enum
	{
		UPDATE,
		FRONT,
		REAR,
		LEFT,
		RIGHT,
		ZROT,
	};

	DECLARE_SOCKET_TYPE(UPDATE, bool);
	DECLARE_SOCKET_TYPE(FRONT, bool);
	DECLARE_SOCKET_TYPE(REAR, bool);
	DECLARE_SOCKET_TYPE(LEFT, bool);
	DECLARE_SOCKET_TYPE(RIGHT, bool);
	DECLARE_SOCKET_TYPE(ZROT, gkScalar);

	vdVehicleNode(gkLogicTree *parent, size_t id);
	virtual ~vdVehicleNode() {}

	bool evaluate(gkScalar tick);

	void setVehicle(vdVehicle *v)   {m_vehicle = v;}

private:
	vdVehicle *m_vehicle;
};

#endif // VDVEHICLENODE_H
