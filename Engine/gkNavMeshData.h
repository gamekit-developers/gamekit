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
#ifndef _gkNavMeshData_h_
#define _gkNavMeshData_h_

#include "LinearMath/btTransform.h"
#include "BulletCollision/CollisionShapes/btTriangleCallback.h"

#include "gkCommon.h"
#include "gkSerialize.h"
#include "gkCriticalSection.h"
#include "gkActiveObject.h"
#include "gkPtrRef.h"

#include <iostream>
#include <fstream>

class gkGameObject;
class gkPhysicsDebug;
class gkScene;
class dtNavMesh;
struct rcConfig;

struct gkNavMesh : public gkReferences
{
	dtNavMesh* data;

	gkNavMesh(dtNavMesh* p) : data(p) {}
	~gkNavMesh();
};

typedef gkPtrRef<gkNavMesh> PNAVMESH;

class gkNavMeshData;
class gkNavCreator : gkActiveObject 
{
public:

	gkNavCreator(gkNavMeshData* data);
	~gkNavCreator();

	void startJob();

	void set(PNAVMESH navMesh);
	PNAVMESH get() const;

private:

	class Call : public gkCall
	{
	public:

		Call(gkNavCreator& creator) : m_creator(creator) {}
		~Call(){}
		void run();

	private:
		gkNavCreator& m_creator;
	};

	mutable gkCriticalSection m_cs;

	gkNavMeshData* m_data;

	PNAVMESH m_navMesh;

	rcConfig* m_cfg;

	friend class Call;
};

class gkNavMeshData : public btTriangleCallback
{
public:
	gkNavMeshData(gkScene* scene);

	~gkNavMeshData();

	void unload(gkGameObject* pObj);
	void refresh(gkGameObject* pObj);
	void reset();

	PNAVMESH getNavigationMesh() const { return m_navCreator->get(); }

	struct Data
	{
		typedef std::vector<gkVector3> VERTS;
		VERTS verts;

		typedef std::vector<int> TRIANGLES;
		TRIANGLES tris;

		typedef std::vector<gkVector3> NORMALS;

		NORMALS normals;

		inline const float* getVerts() const { return &verts.at(0).x; }
		inline const float* getNormals() const { return &normals.at(0).x; }
		inline const int* getTris() const { return &tris.at(0); }
		inline int getVertCount() const { return verts.size(); }
		inline int getTriCount() const { return tris.size()/3; }
	};

	Data* cloneData()
	{
		gkCriticalSection::Lock guard(m_cs);

		Data* p = new Data;

		*p = m_data;

		return p;
	}

	void startJob();

private:

	void processTriangle(btVector3* triangle,int partId, int triangleIndex);
	
	void addTriangle(const gkVector3& v1, const gkVector3& v2, const gkVector3& v3, int triangleIndex);

	void AddCollisionObj();

	bool isValid(gkGameObject* pObj) const;

private:
	
	gkNavCreator* m_navCreator;

	gkCriticalSection m_cs;

	gkGameObject* m_object;

	Data m_data;

	//gkPhysicsDebug* m_debug;

	btTransform m;

	gkScene* m_scene;

	//std::ofstream os;
};


#endif//_gkNavMeshData_h_
