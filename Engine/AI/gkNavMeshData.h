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
#include "gkMeshData.h"
#include "Thread/gkCriticalSection.h"
#include "Thread/gkPtrRef.h"

class gkGameObject;
class gkScene;
class btCollisionObject;
class gkNavMeshData;

typedef gkPtrRef<gkNavMeshData> PNAVMESHDATA;

class gkNavMeshData : public btTriangleCallback, public gkReferences
{
public:
	gkNavMeshData(gkScene* scene);

	~gkNavMeshData();

	void destroyInstance(gkGameObject* pObj);
	void updateOrCreate(gkGameObject* pObj);
	void destroyInstances();
	void createInstances();

	gkMeshData* cloneData() const
	{
		gkCriticalSection::Lock guard(m_cs);

		gkMeshData* p = new gkMeshData;

		p->copy(m_data);

		return p;
	}

	GK_INLINE bool hasChanged() const { return m_hasChanged; }
	GK_INLINE void resetHasChanged() { m_hasChanged = false; }

private:

	void processTriangle(btVector3* triangle, int partId, int triangleIndex);

	void addTriangle(const gkVector3& v1, const gkVector3& v2, const gkVector3& v3, int triangleIndex);

	void AddCollisionObj();

	bool isValid(gkGameObject* pObj);

private:

	mutable gkCriticalSection m_cs;

	gkGameObject* m_object;

	gkMeshData m_data;

	btTransform m;

	gkScene* m_scene;

	bool m_hasChanged;
};


#endif//_gkNavMeshData_h_
