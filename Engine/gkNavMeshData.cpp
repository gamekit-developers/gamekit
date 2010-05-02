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
#include "gkEngine.h"
#include "gkScene.h"
#include "gkDynamicsWorld.h"

#include "btBulletDynamicsCommon.h"

#include "gkNavMeshData.h"
#include "gkGameObject.h"
#include "gkLogger.h"

#include "Recast.h"
#include "RecastLog.h"
#include "RecastTimer.h"
#include "DetourNavMeshBuilder.h"
#include "DetourNavMesh.h"
#include <memory>

typedef gkGameObject::NavMeshData NavMeshData;

gkNavMesh::~gkNavMesh() 
{ 
	delete data; 
}

//////////////////////////////////////////////////////////////////////////////

gkNavCreator::gkNavCreator(gkNavMeshData* data) 
: gkActiveObject("navMeshTask"), m_data(data),
m_cfg(new rcConfig)
{
	memset(m_cfg, 0, sizeof(rcConfig)); 

	const gkScalar CELL_SIZE = 0.17f;
	const gkScalar CELL_HEIGHT = 0.10f;
	const gkScalar AGENT_MAX_SLOPE = 45.0f;
	const gkScalar AGENT_HEIGHT = 0.6f;
	const gkScalar AGENT_MAX_CLIMB = 0.3f;
	const gkScalar AGENT_RADIUS = 0.3f;
	const gkScalar EDGE_MAX_LEN = 12.0f;
	const gkScalar EDGE_MAX_ERROR = 1.3f;
	const gkScalar REGION_MIN_SIZE = 50;
	const gkScalar REGION_MERGE_SIZE = 20;
	const gkScalar VERTS_PER_POLY = 6.0f;
	const gkScalar DETAIL_SAMPLE_DIST = 6.0f;
	const gkScalar DETAIL_SAMPLE_ERROR = 1.0f;

	m_cfg->cs = CELL_SIZE;
	m_cfg->ch = CELL_HEIGHT;
	m_cfg->walkableSlopeAngle = AGENT_MAX_SLOPE;
	m_cfg->walkableHeight = (int)ceilf(AGENT_HEIGHT / m_cfg->ch);
	m_cfg->walkableClimb = (int)ceilf(AGENT_MAX_CLIMB / m_cfg->ch);
	m_cfg->walkableRadius = (int)ceilf(AGENT_RADIUS / m_cfg->cs);
	m_cfg->maxEdgeLen = (int)(EDGE_MAX_LEN / m_cfg->cs);
	m_cfg->maxSimplificationError = EDGE_MAX_ERROR;
	m_cfg->minRegionSize = (int)rcSqr(REGION_MIN_SIZE);
	m_cfg->mergeRegionSize = (int)rcSqr(REGION_MERGE_SIZE);
	m_cfg->maxVertsPerPoly = (int)VERTS_PER_POLY;
	m_cfg->detailSampleDist = DETAIL_SAMPLE_DIST < 0.9f ? 0 : m_cfg->cs * DETAIL_SAMPLE_DIST;
	m_cfg->detailSampleMaxError = m_cfg->ch * DETAIL_SAMPLE_ERROR;
}

gkNavCreator::~gkNavCreator() 
{
	join();

	delete m_cfg;
}

void gkNavCreator::startJob()
{
	gkPtrRef<gkCall> pCall(new Call(*this));

	enqueue(pCall);

	resetButKeepLast();
}

void gkNavCreator::set(PNAVMESH navMesh)
{
	gkCriticalSection::Lock guard(m_cs);

	m_navMesh = navMesh;
}

PNAVMESH gkNavCreator::get() const
{
	PNAVMESH p;
	{
		gkCriticalSection::Lock guard(m_cs);

		p = m_navMesh;
	}

	return p;
}

void gkNavCreator::Call::run()
{
	// These are just sample areas to use consistent values across the samples.
	// The use should specify these base on his needs.
	enum SamplePolyAreas
	{
		SAMPLE_POLYAREA_GROUND,
		SAMPLE_POLYAREA_WATER,
		SAMPLE_POLYAREA_ROAD,
		SAMPLE_POLYAREA_DOOR,
		SAMPLE_POLYAREA_GRASS,
		SAMPLE_POLYAREA_JUMP,
	};

	enum SamplePolyFlags
	{
		SAMPLE_POLYFLAGS_WALK = 0x01,		// Ability to walk (ground, grass, road)
		SAMPLE_POLYFLAGS_SWIM = 0x02,		// Ability to swim (water).
		SAMPLE_POLYFLAGS_DOOR = 0x04,		// Ability to move through doors.
		SAMPLE_POLYFLAGS_JUMP = 0x08,		// Ability to jump.
		SAMPLE_POLYFLAGS_ALL = 0xffff		// All abilities.
	};

	typedef gkNavMeshData::Data Data;
	std::auto_ptr<Data > meshData(m_creator.m_data->cloneData());

	if(!meshData->getVertCount())
		return;

	rcConfig& cfg = *m_creator.m_cfg;

	gkScalar bmin[3], bmax[3];

	const gkScalar* verts = meshData->getVerts();
	int nverts = meshData->getVertCount();
	const int* tris = meshData->getTris();
	const gkScalar* trinorms = meshData->getNormals();
	int ntris = meshData->getTriCount();

	rcCalcBounds(verts, nverts, bmin, bmax);

	//
	// Step 1. Initialize build config.
	//


	// Set the area where the navigation will be build.
	// Here the bounds of the input mesh are used, but the
	// area could be specified by an user defined box, etc.
	vcopy(cfg.bmin, bmin);
	vcopy(cfg.bmax, bmax);
	rcCalcGridSize(cfg.bmin, cfg.bmax, cfg.cs, &cfg.width, &cfg.height);

	rcBuildTimes m_buildTimes; 
	// Reset build times gathering.
	memset(&m_buildTimes, 0, sizeof(m_buildTimes));
	rcSetBuildTimes(&m_buildTimes);

	// Start the build process.	
	rcTimeVal totStartTime = rcGetPerformanceTimer();
	
	//gkPrintf("Building navigation:");
	//gkPrintf(" - %d x %d cells", cfg.width, cfg.height);
	//gkPrintf(" - %.1fK verts, %.1fK tris", nverts/1000.0f, ntris/1000.0f);

	//
	// Step 2. Rasterize input polygon soup.
	//
	
	// Allocate voxel heighfield where we rasterize our input data to.
	rcHeightfield heightField;

	if (!rcCreateHeightfield(heightField, cfg.width, cfg.height, cfg.bmin, cfg.bmax, cfg.cs, cfg.ch))
	{
		gkPrintf("buildNavigation: Could not create solid heightfield.");
		return;
	}
	
	{
		// Allocate array that can hold triangle flags.
		// If you have multiple meshes you need to process, allocate
		// and array which can hold the max number of triangles you need to process.
		
		utArray<unsigned char> triflags;
		triflags.resize(ntris);
		
		// Find triangles which are walkable based on their slope and rasterize them.
		// If your input data is multiple meshes, you can transform them here, calculate
		// the flags for each of the meshes and rasterize them.
		memset(triflags.ptr(), 0, ntris*sizeof(unsigned char));
		rcMarkWalkableTriangles(cfg.walkableSlopeAngle, verts, nverts, tris, ntris, triflags.ptr());
		rcRasterizeTriangles(verts, nverts, tris, triflags.ptr(), ntris, heightField);
	}

	//
	// Step 3. Filter walkables surfaces.
	//
	
	// Once all geoemtry is rasterized, we do initial pass of filtering to
	// remove unwanted overhangs caused by the conservative rasterization
	// as well as filter spans where the character cannot possibly stand.
	rcFilterLedgeSpans(cfg.walkableHeight, cfg.walkableClimb, heightField);
	rcFilterWalkableLowHeightSpans(cfg.walkableHeight, heightField);

	//
	// Step 4. Partition walkable surface to simple regions.
	//

	// Compact the heightfield so that it is faster to handle from now on.
	// This will result more cache coherent data as well as the neighbours
	// between walkable cells will be calculated.
	rcCompactHeightfield chf;
	if (!rcBuildCompactHeightfield(cfg.walkableHeight, cfg.walkableClimb, RC_WALKABLE, heightField, chf))
	{
		gkPrintf("buildNavigation: Could not build compact data.");
		return;
	}
	
	// Erode the walkable area by agent radius.
	if (!rcErodeArea(RC_WALKABLE_AREA, cfg.walkableRadius, chf))
	{
		gkPrintf("buildNavigation: Could not erode.");
		return;
	}
	
	// (Optional) Mark areas.
	/*const ConvexVolume* vols = m_geom->getConvexVolumes();
	for (int i  = 0; i < m_geom->getConvexVolumeCount(); ++i)
		rcMarkConvexPolyArea(vols[i].verts, vols[i].nverts, vols[i].hmin, vols[i].hmax, (unsigned char)vols[i].area, chf);
	*/
	
	// Prepare for region partitioning, by calculating distance field along the walkable surface.
	if (!rcBuildDistanceField(chf))
	{
		gkPrintf("buildNavigation: Could not build distance field.");
		return;
	}

	// Partition the walkable surface into simple regions without holes.
	if (!rcBuildRegions(chf, cfg.borderSize, cfg.minRegionSize, cfg.mergeRegionSize))
	{
		gkPrintf("buildNavigation: Could not build regions.");
		return;
	}
	
	//
	// Step 5. Trace and simplify region contours.
	//
	
	// Create contours.
	rcContourSet cset;

	if (!rcBuildContours(chf, cfg.maxSimplificationError, cfg.maxEdgeLen, cset))
	{
		gkPrintf("buildNavigation: Could not create contours.");
		return;
	}
	
	//
	// Step 6. Build polygons mesh from contours.
	//
	
	// Build polygon navmesh from the contours.
	rcPolyMesh pmesh;
	if (!rcBuildPolyMesh(cset, cfg.maxVertsPerPoly, pmesh))
	{
		gkPrintf("buildNavigation: Could not triangulate contours.");
		return;
	}
	
	//
	// Step 7. Create detail mesh which allows to access approximate height on each polygon.
	//

	rcPolyMeshDetail dmesh;

	if (!rcBuildPolyMeshDetail(pmesh, chf, cfg.detailSampleDist, cfg.detailSampleMaxError, dmesh))
	{
		gkPrintf("buildNavigation: Could not build detail mesh.");
		return;
	}

	// At this point the navigation mesh data is ready, you can access it from pmesh.
	// See rcDebugDrawPolyMesh or dtCreateNavMeshData as examples how to access the data.
	
	//
	// Step 8. Create Detour data from Recast poly mesh.
	//
	
	// The GUI may allow more max points per polygon than Detour can handle.
	// Only build the detour navmesh if we do not exceed the limit.

	PNAVMESH navMesh;

	// Maximum number of vertices per navigation polygon.
	static const int DT_STAT_VERTS_PER_POLYGON = 6;

	// The GUI may allow more max points per polygon than Detour can handle.
	// Only build the detour navmesh if we do not exceed the limit.
	if (cfg.maxVertsPerPoly <= DT_VERTS_PER_POLYGON)
	{
		// Update poly flags from areas.
		for (int i = 0; i < pmesh.npolys; ++i)
		{
			if (pmesh.areas[i] == RC_WALKABLE_AREA)
				pmesh.areas[i] = SAMPLE_POLYAREA_GROUND;
				
			if (pmesh.areas[i] == SAMPLE_POLYAREA_GROUND ||
				pmesh.areas[i] == SAMPLE_POLYAREA_GRASS ||
				pmesh.areas[i] == SAMPLE_POLYAREA_ROAD)
			{
				pmesh.flags[i] = SAMPLE_POLYFLAGS_WALK;
			}
			else if (pmesh.areas[i] == SAMPLE_POLYAREA_WATER)
			{
				pmesh.flags[i] = SAMPLE_POLYFLAGS_SWIM;
			}
			else if (pmesh.areas[i] == SAMPLE_POLYAREA_DOOR)
			{
				pmesh.flags[i] = SAMPLE_POLYFLAGS_WALK | SAMPLE_POLYFLAGS_DOOR;
			}
		}

		dtNavMeshCreateParams params;
		memset(&params, 0, sizeof(params));
		params.verts = pmesh.verts;
		params.vertCount = pmesh.nverts;
		params.polys = pmesh.polys;
		params.polyAreas = pmesh.areas;
		params.polyFlags = pmesh.flags;
		params.polyCount = pmesh.npolys;
		params.nvp = pmesh.nvp;
		params.detailMeshes = dmesh.meshes;
		params.detailVerts = dmesh.verts;
		params.detailVertsCount = dmesh.nverts;
		params.detailTris = dmesh.tris;
		params.detailTriCount = dmesh.ntris;
/*		params.offMeshConVerts = m_geom->getOffMeshConnectionVerts();
		params.offMeshConRad = m_geom->getOffMeshConnectionRads();
		params.offMeshConDir = m_geom->getOffMeshConnectionDirs();
		params.offMeshConAreas = m_geom->getOffMeshConnectionAreas();
		params.offMeshConFlags = m_geom->getOffMeshConnectionFlags();
		params.offMeshConCount = m_geom->getOffMeshConnectionCount();
		*/		
		params.walkableHeight = cfg.walkableHeight*cfg.ch;
		params.walkableRadius = cfg.walkableRadius*cfg.cs;;
		params.walkableClimb = cfg.walkableClimb*cfg.ch;
		vcopy(params.bmin, pmesh.bmin);
		vcopy(params.bmax, pmesh.bmax);
		params.cs = cfg.cs;
		params.ch = cfg.ch;
		
		unsigned char* navData = 0;
		int navDataSize = 0;
		
		if (!dtCreateNavMeshData(&params, &navData, &navDataSize))
		{
			gkPrintf("Could not build Detour navmesh.");
			return;
		}


		navMesh = PNAVMESH(new gkNavMesh(new dtNavMesh));
		
		if (!navMesh->data->init(navData, navDataSize, DT_TILE_FREE_DATA, 2048))
		{
			delete [] navData;
			gkPrintf("Could not init Detour navmesh");
			return;
		}
	}

	rcTimeVal totEndTime = rcGetPerformanceTimer();

	// Show performance stats.
/*	{
		const float pc = 100.0f / rcGetDeltaTimeUsec(totStartTime, totEndTime);
		
		gkPrintf("Rasterize: %.1fms (%.1f%%)", m_buildTimes.rasterizeTriangles/1000.0f, m_buildTimes.rasterizeTriangles*pc);
		
		gkPrintf("Build Compact: %.1fms (%.1f%%)", m_buildTimes.buildCompact/1000.0f, m_buildTimes.buildCompact*pc);
		
		gkPrintf("Filter Border: %.1fms (%.1f%%)", m_buildTimes.filterBorder/1000.0f, m_buildTimes.filterBorder*pc);
		gkPrintf("Filter Walkable: %.1fms (%.1f%%)", m_buildTimes.filterWalkable/1000.0f, m_buildTimes.filterWalkable*pc);
		gkPrintf("Filter Reachable: %.1fms (%.1f%%)", m_buildTimes.filterMarkReachable/1000.0f, m_buildTimes.filterMarkReachable*pc);
		
		gkPrintf("Build Distancefield: %.1fms (%.1f%%)", m_buildTimes.buildDistanceField/1000.0f, m_buildTimes.buildDistanceField*pc);
		gkPrintf("  - distance: %.1fms (%.1f%%)", m_buildTimes.buildDistanceFieldDist/1000.0f, m_buildTimes.buildDistanceFieldDist*pc);
		gkPrintf("  - blur: %.1fms (%.1f%%)", m_buildTimes.buildDistanceFieldBlur/1000.0f, m_buildTimes.buildDistanceFieldBlur*pc);
		
		gkPrintf("Build Regions: %.1fms (%.1f%%)", m_buildTimes.buildRegions/1000.0f, m_buildTimes.buildRegions*pc);
		gkPrintf("  - watershed: %.1fms (%.1f%%)", m_buildTimes.buildRegionsReg/1000.0f, m_buildTimes.buildRegionsReg*pc);
		gkPrintf("    - expand: %.1fms (%.1f%%)", m_buildTimes.buildRegionsExp/1000.0f, m_buildTimes.buildRegionsExp*pc);
		gkPrintf("    - find catchment basins: %.1fms (%.1f%%)", m_buildTimes.buildRegionsFlood/1000.0f, m_buildTimes.buildRegionsFlood*pc);
		gkPrintf("  - filter: %.1fms (%.1f%%)", m_buildTimes.buildRegionsFilter/1000.0f, m_buildTimes.buildRegionsFilter*pc);
		
		gkPrintf("Build Contours: %.1fms (%.1f%%)", m_buildTimes.buildContours/1000.0f, m_buildTimes.buildContours*pc);
		gkPrintf("  - trace: %.1fms (%.1f%%)", m_buildTimes.buildContoursTrace/1000.0f, m_buildTimes.buildContoursTrace*pc);
		gkPrintf("  - simplify: %.1fms (%.1f%%)", m_buildTimes.buildContoursSimplify/1000.0f, m_buildTimes.buildContoursSimplify*pc);
		
		gkPrintf("Build Polymesh: %.1fms (%.1f%%)", m_buildTimes.buildPolymesh/1000.0f, m_buildTimes.buildPolymesh*pc);
		gkPrintf("Build Polymesh Detail: %.1fms (%.1f%%)", m_buildTimes.buildDetailMesh/1000.0f, m_buildTimes.buildDetailMesh*pc);
		
		gkPrintf("Polymesh: Verts:%d  Polys:%d", pmesh.nverts, pmesh.npolys);
	}
*/

	m_creator.set(navMesh);

	gkPrintf("Navigation mesh created: %.1fms", rcGetDeltaTimeUsec(totStartTime, totEndTime)/1000.0f);
}


//////////////////////////////////////////////////////////////////////////////////////


gkNavMeshData::gkNavMeshData(gkScene* scene) 
: m_object(0), m_scene(scene)//, m_debug(0), os(L"momo.obj")
{
	GK_ASSERT(m_scene);

	m_navCreator = new gkNavCreator(this);

	//m_debug = new gkPhysicsDebug(m_scene->getDynamicsWorld());
}

gkNavMeshData::~gkNavMeshData()
{
	delete m_navCreator;

	reset();
	//delete m_debug;
}

void gkNavMeshData::unload(gkGameObject* pObj)
{
	if(!isValid(pObj)) return;

	gkCriticalSection::Lock guard(m_cs);

	if(m_data.tris.empty()) return;

	GK_ASSERT(!pObj->getNavData().isEmpty());

	int indexBase = pObj->getNavData().triangleBaseIndex;
	size_t vertexBaseIndex = indexBase/2;
	size_t normalBaseIndex = indexBase/6;

	int nIndex = pObj->getNavData().nIndex;
	size_t nVertex = nIndex/2;
	size_t nNormal = nIndex/3;

	int indexValue = nIndex/2;

	{
		Data::TRIANGLES::iterator it = m_data.tris.erase(m_data.tris.begin() + indexBase, m_data.tris.begin() + indexBase + nIndex);
		m_data.verts.erase(m_data.verts.begin() + vertexBaseIndex, m_data.verts.begin() + vertexBaseIndex + nVertex);
		m_data.normals.erase(m_data.normals.begin() + normalBaseIndex, m_data.normals.begin() + normalBaseIndex + nNormal);

		while(it != m_data.tris.end())
		{
			if(*it > indexBase)
			{
				*it -= indexValue;
			}

			++it;
		}
	}

	pObj->setNavData(NavMeshData());

	gkGameObjectSet& objs = m_scene->getLoadedObjects();
	gkGameObjectSet::iterator it = objs.begin();
    while(it != objs.end())
	{
		gkGameObject* p = *it;
        p->recalcNavData(indexBase, nIndex);
		++it;
	}
}

void gkNavMeshData::refresh(gkGameObject* pObj)
{
	if(!isValid(pObj)) return;

	gkCriticalSection::Lock guard(m_cs);

	m_object = pObj;

	if(m_object && m_object->isLoaded())
	{
		AddCollisionObj();
	}

	m_navCreator->startJob();
}

void gkNavMeshData::reset()
{
	gkCriticalSection::Lock guard(m_cs);

	m_data.verts.clear();
	m_data.tris.clear();
	m_data.normals.clear();
}

void gkNavMeshData::processTriangle(btVector3* triangle,int partId, int triangleIndex)
{
	btVector3 v1 = m * triangle[0];
	btVector3 v2 = m * triangle[1];
	btVector3 v3 = m * triangle[2];

	gkVector3 vv1(v1.x(), v1.z(), v1.y());
	gkVector3 vv2(v2.x(), v2.z(), v2.y());
	gkVector3 vv3(v3.x(), v3.z(), v3.y());

	addTriangle(vv1, vv2, vv3, triangleIndex);

//	m_debug->drawLine(v1, v2, btVector3(1,1,0));
//	m_debug->drawLine(v2, v3, btVector3(1,1,0));
//	m_debug->drawLine(v3, v1, btVector3(1,1,0));
}

void gkNavMeshData::addTriangle(const gkVector3& v1, const gkVector3& v2, const gkVector3& v3, int triangleIndex)
{
	if(m_object->getNavData().isEmpty())
	{
		size_t n = m_data.verts.size();

		int a = n; 
		int b = n+1;
		int c = n+2;

		m_data.verts.push_back(v1);
		m_data.verts.push_back(v2);
		m_data.verts.push_back(v3);

		m_data.tris.push_back(a);
		m_data.tris.push_back(b);
		m_data.tris.push_back(c);

		gkVector3 normal((v3-v1).crossProduct(v2-v1)); 
		normal.normalise();

		m_data.normals.push_back(normal);

		m_data.tris.push_back(c);
		m_data.tris.push_back(b);
		m_data.tris.push_back(a);

		m_data.normals.push_back(-normal);
	/*
		os << "v " << v1.x << " " << v1.y << " " << v1.z << "\n";
		os << "v " << v2.x << " " << v2.y << " " << v2.z << "\n";
		os << "v " << v3.x << " " << v3.y << " " << v3.z << "\n";

		a++; b++; c++;
		os << "f " << a  << " " << b << " " << c << "\n";
		os << "f " << c  << " " << b << " " << a << "\n";
	*/
	}
	else
	{
		size_t tBaseIndex = m_object->getNavData().triangleBaseIndex + triangleIndex;
		size_t vBaseIndex = tBaseIndex/2;
		size_t nBaseIndex = tBaseIndex/6;

		gkVector3& ov1 = m_data.verts[vBaseIndex];
		gkVector3& ov2 = m_data.verts[vBaseIndex+1];
		gkVector3& ov3 = m_data.verts[vBaseIndex+2];
		
		ov1 = v1;
		ov2 = v2;
		ov3 = v3;

		gkVector3 normal((v3-v1).crossProduct(v2-v1)); 
		normal.normalise();

		m_data.normals[nBaseIndex] = normal;
		m_data.normals[nBaseIndex+1] = -normal;
	}
}

void gkNavMeshData::AddCollisionObj()
{
	size_t tIndex = m_data.tris.size();

	gkScene* pScene = m_object->getOwner();

	GK_ASSERT(pScene);

	btVector3 aabbMin,aabbMax;
	pScene->getDynamicsWorld()->getBulletWorld()->getBroadphase()->getBroadphaseAabb(aabbMin,aabbMax);
	
	aabbMin-=btVector3(BT_LARGE_FLOAT,BT_LARGE_FLOAT,BT_LARGE_FLOAT);
	aabbMax+=btVector3(BT_LARGE_FLOAT,BT_LARGE_FLOAT,BT_LARGE_FLOAT);

	btCollisionObject* colObj = m_object->getAttachedObject()->getCollisionObject();

	const btCollisionShape* shape = colObj->getCollisionShape();

	m = colObj->getWorldTransform();

	int shapetype=shape->getShapeType();

	switch(shapetype)
	{
		case SPHERE_SHAPE_PROXYTYPE:
		{
			break;
		}

		case BOX_SHAPE_PROXYTYPE:
		{
			const btBoxShape* boxShape = static_cast<const btBoxShape*>(shape);
			btVector3 halfExtent = boxShape->getHalfExtentsWithMargin();
			
			static int indices[36] = {
				0,1,2,
				3,2,1,
				4,0,6,
				6,0,2,
				5,1,4,
				4,1,0,
				7,3,1,
				7,1,5,
				5,4,7,
				7,4,6,
				7,2,3,
				7,6,2};

			static btVector3 vertices[8]={	btVector3(1,1,1),btVector3(-1,1,1),	btVector3(1,-1,1),	btVector3(-1,-1,1),	btVector3(1,1,-1),	btVector3(-1,1,-1),	btVector3(1,-1,-1),	btVector3(-1,-1,-1)};
			int si=36;
			for (int i=0;i<si;i+=3)
			{
				btVector3 v1 = m * (vertices[indices[i]]*halfExtent);
				btVector3 v2 = m * (vertices[indices[i+1]]*halfExtent);
				btVector3 v3 = m * (vertices[indices[i+2]]*halfExtent);

				gkVector3 vv1(v1.x(), v1.z(), v1.y());
				gkVector3 vv2(v2.x(), v2.z(), v2.y());
				gkVector3 vv3(v3.x(), v3.z(), v3.y());

				addTriangle(vv1, vv2, vv3, i/3);

				//m_debug->drawLine(v1, v2, btVector3(1,1,0));
				//m_debug->drawLine(v2, v3, btVector3(1,1,0));
				//m_debug->drawLine(v3, v1, btVector3(1,1,0));
			}

			break;
		}

		default:

			if (shape->isConcave() && !shape->isInfinite())
			{
				btConcaveShape* concaveMesh = (btConcaveShape*) shape;

				concaveMesh->processAllTriangles(this, aabbMin, aabbMax);
			}

			break;
	}

	if(m_object->getNavData().isEmpty())
	{
		size_t n = m_data.tris.size() - tIndex;

		GK_ASSERT(n >= 6); // minimum 2 triangles

		m_object->setNavData(NavMeshData(tIndex, n));
	}
}

bool gkNavMeshData::isValid(gkGameObject* pObj) const
{
	const int& physicsState = pObj->getProperties().physicsState;

	return physicsState == GK_RIGID_BODY || physicsState == GK_STATIC;
}


