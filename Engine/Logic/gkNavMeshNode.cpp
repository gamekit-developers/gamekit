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
#include "OgreRoot.h"
#include "OgreEntity.h"
#include "OgreMesh.h"
#include "OgreSubMesh.h"
#include "OgreSubEntity.h"

#include "gkNavMeshNode.h"
#include "gkLogger.h"
#include "gkEngine.h"
#include "gkScene.h"
#include "gkGameObject.h"
#include "gkRigidBody.h"
#include "gkEntity.h"
#include "gkUtils.h"
#include "gkPhysicsDebug.h"
#include "gkDynamicsWorld.h"
#include "gkNavMeshData.h"

#include "Recast.h"
#include "RecastLog.h"
#include "RecastTimer.h"
#include "DetourNavMeshBuilder.h"
#include "DetourNavMesh.h"

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btShapeHull.h"

#include "LinearMath/btTransform.h"
#include "BulletCollision/CollisionShapes/btTriangleCallback.h"

struct ConfigData : public rcConfig
{
	gkNavMeshData* meshData;

	ConfigData()
	{
		memset(this, 0, sizeof(*this));
	}

	~ConfigData()
	{
		delete meshData;
	}
};

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

gkNavMeshNode::gkNavMeshNode(gkLogicTree *parent, size_t id) 
: gkLogicNode(parent, id),
m_task("navMeshTask"),
m_navMesh(0),
m_cfg(new ConfigData)
{
	ADD_ISOCK(UPDATE, false);

	ADD_ISOCK(CELL_SIZE, 0.17f);
	ADD_ISOCK(CELL_HEIGHT, 0.10f);
	ADD_ISOCK(AGENT_MAX_SLOPE, 45.0f);
	ADD_ISOCK(AGENT_HEIGHT, 0.6f);
	ADD_ISOCK(AGENT_MAX_CLIMB, 0.3f);
	ADD_ISOCK(AGENT_RADIUS, 0.3f);
	ADD_ISOCK(EDGE_MAX_LEN, 12.0f);
	ADD_ISOCK(EDGE_MAX_ERROR, 1.3f);
	ADD_ISOCK(REGION_MIN_SIZE, 50);
	ADD_ISOCK(REGION_MERGE_SIZE, 20);
	ADD_ISOCK(VERTS_PER_POLY, 6.0f);
	ADD_ISOCK(DETAIL_SAMPLE_DIST, 6.0f);
	ADD_ISOCK(DETAIL_SAMPLE_ERROR, 1.0f);

	ADD_OSOCK(OUTPUT, 0);
}

gkNavMeshNode::~gkNavMeshNode()
{
	m_task.wait();

	delete m_navMesh;

	delete m_cfg;
}

bool gkNavMeshNode::evaluate(gkScalar tick)
{
	dtNavMesh* navMesh = 0;

	{
		gkCriticalSection::Lock guard(m_cs);

		navMesh = m_navMesh;
	}
	
	if(GET_SOCKET_VALUE(UPDATE))
	{
		if(!m_cfg->meshData)
		{
			update(tick);
		}
	}
	else if(m_navMesh)
	{
		if(!m_cfg->meshData)
		{
			SET_SOCKET_VALUE(OUTPUT, m_navMesh);
		}
		else
		{
			delete m_cfg->meshData;

			m_cfg->meshData = 0;
		}
	}

	return false;
}

void gkNavMeshNode::update(gkScalar tick)
{
	SET_SOCKET_VALUE(OUTPUT, 0);

	delete m_navMesh;
	m_navMesh = 0;

	ConfigData& cfg = *m_cfg;

	cfg.cs = GET_SOCKET_VALUE(CELL_SIZE);
	cfg.ch = GET_SOCKET_VALUE(CELL_HEIGHT);
	cfg.walkableSlopeAngle = GET_SOCKET_VALUE(AGENT_MAX_SLOPE);
	cfg.walkableHeight = (int)ceilf(GET_SOCKET_VALUE(AGENT_HEIGHT) / cfg.ch);
	cfg.walkableClimb = (int)ceilf(GET_SOCKET_VALUE(AGENT_MAX_CLIMB) / cfg.ch);
	cfg.walkableRadius = (int)ceilf(GET_SOCKET_VALUE(AGENT_RADIUS) / cfg.cs);
	cfg.maxEdgeLen = (int)(GET_SOCKET_VALUE(EDGE_MAX_LEN) / cfg.cs);
	cfg.maxSimplificationError = GET_SOCKET_VALUE(EDGE_MAX_ERROR);
	cfg.minRegionSize = (int)rcSqr(GET_SOCKET_VALUE(REGION_MIN_SIZE));
	cfg.mergeRegionSize = (int)rcSqr(GET_SOCKET_VALUE(REGION_MERGE_SIZE));
	cfg.maxVertsPerPoly = (int)GET_SOCKET_VALUE(VERTS_PER_POLY);
	cfg.detailSampleDist = GET_SOCKET_VALUE(DETAIL_SAMPLE_DIST) < 0.9f ? 0 : cfg.cs * GET_SOCKET_VALUE(DETAIL_SAMPLE_DIST);
	cfg.detailSampleMaxError = cfg.ch * GET_SOCKET_VALUE(DETAIL_SAMPLE_ERROR);

	gkScene* pScene = gkEngine::getSingleton().getActiveScene();

	GK_ASSERT(pScene);

	cfg.meshData = pScene->getMeshData()->cloneData();

	if(cfg.meshData->getVertCount())
		m_task.start(this);
	else
	{
		delete cfg.meshData;
		cfg.meshData = 0;
	}
}

void gkNavMeshNode::run()
{
	ConfigData& cfg = *m_cfg;

	gkScalar bmin[3], bmax[3];

	const gkScalar* verts = cfg.meshData->getVerts();
	int nverts = cfg.meshData->getVertCount();
	const int* tris = cfg.meshData->getTris();
	const gkScalar* trinorms = cfg.meshData->getNormals();
	int ntris = cfg.meshData->getTriCount();

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

	dtNavMesh* navMesh = 0;

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
*/		params.walkableHeight = GET_SOCKET_VALUE(AGENT_HEIGHT);
		params.walkableRadius = GET_SOCKET_VALUE(AGENT_RADIUS);
		params.walkableClimb = GET_SOCKET_VALUE(AGENT_MAX_CLIMB);
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


		navMesh = new dtNavMesh;
		
		if (!navMesh->init(navData, navDataSize, DT_TILE_FREE_DATA, 2048))
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

	gkPrintf("Navigation mesh created: %.1fms", rcGetDeltaTimeUsec(totStartTime, totEndTime)/1000.0f);

	gkCriticalSection::Lock guard(m_cs);

	GK_ASSERT(!m_navMesh);

	m_navMesh = navMesh;
}




