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
#ifndef _gkStaticNavMeshNode_h_
#define _gkStaticNavMeshNode_h_

#include "gkLogicNode.h"

struct rcHeightfield;
struct rcCompactHeightfield;
struct rcContourSet;
struct rcPolyMesh;
struct rcPolyMeshDetail;
class dtNavMesh;
class gkPhysicsDebug;
class gkRigidBody;
class btVector3;

#include "LinearMath/btTransform.h"
#include "BulletCollision/CollisionShapes/btTriangleCallback.h"

#include <iostream>
#include <fstream>

class gkNavMeshLoader : public btTriangleCallback
{
public:
	gkNavMeshLoader();
	~gkNavMeshLoader();
	
	bool load(gkGameObject* pObj);

	inline const float* getVerts() const { return &m_verts.ptr()->x; }
	inline const float* getNormals() const { return &m_normals.ptr()->x; }
	inline const int* getTris() const { return m_tris.ptr(); }
	inline int getVertCount() const { return m_verts.size(); }
	inline int getTriCount() const { return m_tris.size()/3; }

	virtual void processTriangle(btVector3* triangle,int partId, int triangleIndex);

private:
	
	void addTriangle(const gkVector3& v1, const gkVector3& v2, const gkVector3& v3);
	void AddCollisionObj(gkRigidBody* pBody);

private:
	
	typedef utArray<gkVector3> VERTS;
	VERTS m_verts;

	typedef utArray<int> TRIANGLES;
	TRIANGLES m_tris;

	typedef utArray<gkVector3> NORMALS;

	NORMALS m_normals;

	gkPhysicsDebug* m_debug;

	btTransform m;

	//std::ofstream os;
};


class gkStaticNavMeshNode : public gkLogicNode
{
public:

	enum
	{
		UPDATE,
		CREATE_NAV_MESH,
		FIND_PATH,

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

		MAX_PATH_POLYS,

		OBJ,
		END_X_POS,
		END_Y_POS,
		POLY_PICK_EXT,

		PATH_FOUND,
	};

	DECLARE_SOCKET_TYPE(UPDATE, bool);
	DECLARE_SOCKET_TYPE(CREATE_NAV_MESH, bool);
	DECLARE_SOCKET_TYPE(FIND_PATH, bool);
	
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

	DECLARE_SOCKET_TYPE(MAX_PATH_POLYS, int);

	DECLARE_SOCKET_TYPE(OBJ, gkGameObject*);
	DECLARE_SOCKET_TYPE(END_X_POS, gkScalar);
	DECLARE_SOCKET_TYPE(END_Y_POS, gkScalar);
	DECLARE_SOCKET_TYPE(POLY_PICK_EXT, gkVector3);

	DECLARE_SOCKET_TYPE(PATH_FOUND, bool);
	

    gkStaticNavMeshNode(gkLogicTree *parent, size_t id);

	~gkStaticNavMeshNode();

	bool evaluate(gkScalar tick);
	void update(gkScalar tick);

private:

	void ClearData();
	bool Create();
	void FindPath();

private:

	gkNavMeshLoader* m_meshLoader;

	const gkScalar* m_verts;
	int m_nverts;
	const int* m_tris;
	const gkScalar* m_trinorms;
	int m_ntris;
	gkScalar m_bmin[3], m_bmax[3];

	bool m_keepInterResults;

	unsigned char* m_triflags;
	rcHeightfield* m_solid;
	rcCompactHeightfield* m_chf;
	rcContourSet* m_cset;
	rcPolyMesh* m_pmesh;
	rcPolyMeshDetail* m_dmesh;
	dtNavMesh* m_navMesh;

	int m_npolys;
	typedef unsigned int dtPolyRef;
	dtPolyRef* m_polys;
	gkScalar* m_straightPath;
	int m_nstraightPath;

	gkPhysicsDebug* m_debug;
};

#endif//_gkStaticNavMeshNode_h_
