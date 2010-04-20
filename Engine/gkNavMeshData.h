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

#include "gkCommon.h"
#include "gkSerialize.h"
#include "LinearMath/btTransform.h"
#include "BulletCollision/CollisionShapes/btTriangleCallback.h"

#include <iostream>
#include <fstream>

class gkGameObject;
class gkPhysicsDebug;

class gkNavMeshData : public btTriangleCallback
{
public:
	gkNavMeshData();

	~gkNavMeshData();

	bool refresh(gkGameObject* pObj);

	gkNavMeshData* cloneData()
	{
		gkNavMeshData* p = new gkNavMeshData;

		p->m_verts = m_verts;
		p->m_tris = m_tris;
		p->m_normals = m_normals;

		return p;
	}

	inline const float* getVerts() const { return &m_verts.at(0).x; }
	inline const float* getNormals() const { return &m_normals.at(0).x; }
	inline const int* getTris() const { return &m_tris.at(0); }
	inline int getVertCount() const { return m_verts.size(); }
	inline int getTriCount() const { return m_tris.size()/3; }

private:

	void processTriangle(btVector3* triangle,int partId, int triangleIndex);
	
	void addTriangle(const gkVector3& v1, const gkVector3& v2, const gkVector3& v3, int triangleIndex);

	void AddCollisionObj();

private:
	
	gkGameObject* m_object;

	typedef std::vector<gkVector3> VERTS;
	VERTS m_verts;

	typedef std::vector<int> TRIANGLES;
	TRIANGLES m_tris;

	typedef std::vector<gkVector3> NORMALS;

	NORMALS m_normals;

	//gkPhysicsDebug* m_debug;

	btTransform m;

	//std::ofstream os;
};


#endif//_gkNavMeshData_h_
