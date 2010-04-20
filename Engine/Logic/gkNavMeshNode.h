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
#ifndef _gkNavMeshNode_h_
#define _gkNavMeshNode_h_

#include "gkLogicNode.h"
#include "gkThread.h"

class dtNavMesh;
struct ConfigData;

class gkNavMeshNode : public gkLogicNode, public gkAbstractTaskHandler
{
public:

	enum
	{
		UPDATE,

		CELL_SIZE,
		CELL_HEIGHT,
		AGENT_MAX_SLOPE,
		AGENT_HEIGHT,
		AGENT_MAX_CLIMB,
		AGENT_RADIUS,
		EDGE_MAX_LEN,
		EDGE_MAX_ERROR,
		REGION_MIN_SIZE,
		REGION_MERGE_SIZE, 
		VERTS_PER_POLY,
		DETAIL_SAMPLE_DIST,
		DETAIL_SAMPLE_ERROR,
		OUTPUT
	};

	DECLARE_SOCKET_TYPE(UPDATE, bool);
	
	DECLARE_SOCKET_TYPE(CELL_SIZE, gkScalar);
	DECLARE_SOCKET_TYPE(CELL_HEIGHT, gkScalar);
	DECLARE_SOCKET_TYPE(AGENT_MAX_SLOPE, gkScalar);
	DECLARE_SOCKET_TYPE(AGENT_HEIGHT, gkScalar);
	DECLARE_SOCKET_TYPE(AGENT_MAX_CLIMB, gkScalar);
	DECLARE_SOCKET_TYPE(AGENT_RADIUS, gkScalar);
	DECLARE_SOCKET_TYPE(EDGE_MAX_LEN, gkScalar);
	DECLARE_SOCKET_TYPE(EDGE_MAX_ERROR, gkScalar);
	DECLARE_SOCKET_TYPE(REGION_MIN_SIZE, gkScalar);
	DECLARE_SOCKET_TYPE(REGION_MERGE_SIZE, gkScalar);
	DECLARE_SOCKET_TYPE(VERTS_PER_POLY, gkScalar);
	DECLARE_SOCKET_TYPE(DETAIL_SAMPLE_DIST, gkScalar);
	DECLARE_SOCKET_TYPE(DETAIL_SAMPLE_ERROR, gkScalar);

	DECLARE_SOCKET_TYPE(OUTPUT, dtNavMesh*);

    gkNavMeshNode(gkLogicTree *parent, size_t id);

	~gkNavMeshNode();

	bool evaluate(gkScalar tick);
	void update(gkScalar tick);

private:

	void run();

private:

	gkThreadInterface m_task;

	gkCriticalSection m_cs;

	dtNavMesh* m_navMesh;

	ConfigData* m_cfg;
};

#endif//_gkNavMeshNode_h_
