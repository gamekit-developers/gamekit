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

#include "gkStaticNavMeshNode.h"
#include "gkLogger.h"
#include "gkEngine.h"
#include "gkScene.h"
#include "gkGameObject.h"
#include "gkRigidBody.h"
#include "gkEntity.h"
#include "gkUtils.h"
#include "gkPhysicsDebug.h"
#include "gkDynamicsWorld.h"

#include "Recast.h"
#include "RecastLog.h"
#include "RecastTimer.h"
#include "DetourNavMeshBuilder.h"
#include "DetourNavMesh.h"

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btShapeHull.h"

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

gkStaticNavMeshNode::gkStaticNavMeshNode(gkLogicTree *parent, size_t id) 
: gkLogicNode(parent, id),
m_meshLoader(0),
m_verts(0),
m_nverts(0),
m_tris(0),
m_trinorms(0),
m_ntris(0),
m_keepInterResults(false),
m_triflags(0),
m_solid(0),
m_chf(0),
m_cset(0),
m_pmesh(0),
m_dmesh(0),
m_navMesh(0),
m_npolys(0),
m_polys(0),
m_straightPath(0),
m_nstraightPath(0),
m_debug(0)
{
	ADD_ISOCK(UPDATE, false);
	ADD_ISOCK(CREATE_NAV_MESH, false);
	ADD_ISOCK(FIND_PATH, false);

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

	ADD_ISOCK(MAX_PATH_POLYS, 256);
	
	ADD_ISOCK(OBJ, 0);
	ADD_ISOCK(END_X_POS, 0);
	ADD_ISOCK(END_Y_POS, 0);
	ADD_ISOCK(POLY_PICK_EXT, gkVector3(2, 4, 2));
	

	ADD_OSOCK(PATH_FOUND, false);
}

gkStaticNavMeshNode::~gkStaticNavMeshNode()
{
	ClearData();
}

bool gkStaticNavMeshNode::evaluate(gkScalar tick)
{
	return GET_SOCKET_VALUE(UPDATE) && GET_SOCKET_VALUE(OBJ);
}

void gkStaticNavMeshNode::ClearData()
{
	delete [] m_triflags;
	m_triflags = 0;

	delete m_solid;
	m_solid = 0;

	delete m_chf;
	m_chf = 0;

	delete m_cset;
	m_cset = 0;

	delete m_pmesh;
	m_pmesh = 0;

	delete m_dmesh;
	m_dmesh = 0;

	delete m_navMesh;
	m_navMesh = 0;

	delete m_meshLoader;
	m_meshLoader = 0;

	m_verts = 0;
	m_trinorms = 0;
	m_tris = 0;

	delete[] m_polys;
	m_polys = 0;
	delete[] m_straightPath;
	m_straightPath = 0;

	delete m_debug;
	m_debug = 0;
}

void gkStaticNavMeshNode::update(gkScalar tick)
{
	if(GET_SOCKET_VALUE(CREATE_NAV_MESH))
	{
		Create();
	}
	else if(GET_SOCKET_VALUE(FIND_PATH))
	{
		FindPath();
	}
}

bool gkStaticNavMeshNode::Create()
{
	// Step 0. Create mesh data

	ClearData();

	gkScene* pScene = gkEngine::getSingleton().getActiveScene();

	GK_ASSERT(pScene);

	if(!m_debug)
	{
		m_debug = new gkPhysicsDebug(pScene->getDynamicsWorld());
	}

	GK_ASSERT(!m_meshLoader);

	m_meshLoader = new gkNavMeshLoader;

	gkGameObjectList& objs = pScene->getLoadedObjects();

	gkGameObjectListIterator objIt(objs);
	
	while(objIt.hasMoreElements())
	{
		gkGameObject* pObj = objIt.getNext();

		if(pObj != GET_SOCKET_VALUE(OBJ))
		{
			m_meshLoader->load(pObj);
		}
	}

	if(!m_meshLoader->getVertCount())
		return false;

	m_verts = m_meshLoader->getVerts();
	m_nverts = m_meshLoader->getVertCount();
	m_tris = m_meshLoader->getTris();
	m_trinorms = m_meshLoader->getNormals();
	m_ntris = m_meshLoader->getTriCount();

	rcCalcBounds(m_verts, m_nverts, m_bmin, m_bmax);

	//
	// Step 1. Initialize build config.
	//

	rcConfig m_cfg;	
	
	// Init build configuration from GUI
	memset(&m_cfg, 0, sizeof(m_cfg));
	m_cfg.cs = GET_SOCKET_VALUE(CELL_SIZE);
	m_cfg.ch = GET_SOCKET_VALUE(CELL_HEIGHT);
	m_cfg.walkableSlopeAngle = GET_SOCKET_VALUE(AGENT_MAX_SLOPE);
	m_cfg.walkableHeight = (int)ceilf(GET_SOCKET_VALUE(AGENT_HEIGHT) / m_cfg.ch);
	m_cfg.walkableClimb = (int)ceilf(GET_SOCKET_VALUE(AGENT_MAX_CLIMB) / m_cfg.ch);
	m_cfg.walkableRadius = (int)ceilf(GET_SOCKET_VALUE(AGENT_RADIUS) / m_cfg.cs);
	m_cfg.maxEdgeLen = (int)(GET_SOCKET_VALUE(EDGE_MAX_LEN) / m_cfg.cs);
	m_cfg.maxSimplificationError = GET_SOCKET_VALUE(EDGE_MAX_ERROR);
	m_cfg.minRegionSize = (int)rcSqr(GET_SOCKET_VALUE(REGION_MIN_SIZE));
	m_cfg.mergeRegionSize = (int)rcSqr(GET_SOCKET_VALUE(REGION_MERGE_SIZE));
	m_cfg.maxVertsPerPoly = (int)GET_SOCKET_VALUE(VERTS_PER_POLY);
	m_cfg.detailSampleDist = GET_SOCKET_VALUE(DETAIL_SAMPLE_DIST) < 0.9f ? 0 : m_cfg.cs * GET_SOCKET_VALUE(DETAIL_SAMPLE_DIST);
	m_cfg.detailSampleMaxError = m_cfg.ch * GET_SOCKET_VALUE(DETAIL_SAMPLE_ERROR);

	// Set the area where the navigation will be build.
	// Here the bounds of the input mesh are used, but the
	// area could be specified by an user defined box, etc.
	vcopy(m_cfg.bmin, m_bmin);
	vcopy(m_cfg.bmax, m_bmax);
	rcCalcGridSize(m_cfg.bmin, m_cfg.bmax, m_cfg.cs, &m_cfg.width, &m_cfg.height);

	rcBuildTimes m_buildTimes; 
	// Reset build times gathering.
	memset(&m_buildTimes, 0, sizeof(m_buildTimes));
	rcSetBuildTimes(&m_buildTimes);

	// Start the build process.	
	rcTimeVal totStartTime = rcGetPerformanceTimer();
	
	gkPrintf("Building navigation:");
	gkPrintf(" - %d x %d cells", m_cfg.width, m_cfg.height);
	gkPrintf(" - %.1fK verts, %.1fK tris", m_nverts/1000.0f, m_ntris/1000.0f);

	//
	// Step 2. Rasterize input polygon soup.
	//
	
	// Allocate voxel heighfield where we rasterize our input data to.
	GK_ASSERT(!m_solid);
	m_solid = new rcHeightfield;
	if (!m_solid)
	{
		gkPrintf("buildNavigation: Out of memory 'solid'.");
		return false;
	}
	if (!rcCreateHeightfield(*m_solid, m_cfg.width, m_cfg.height, m_cfg.bmin, m_cfg.bmax, m_cfg.cs, m_cfg.ch))
	{
		gkPrintf("buildNavigation: Could not create solid heightfield.");
		return false;
	}
	
	// Allocate array that can hold triangle flags.
	// If you have multiple meshes you need to process, allocate
	// and array which can hold the max number of triangles you need to process.
	GK_ASSERT(!m_triflags);
	m_triflags = new unsigned char[m_ntris];
	if (!m_triflags)
	{
		gkPrintf("buildNavigation: Out of memory 'triangleFlags' (%d).", m_ntris);
		return false;
	}
	
	// Find triangles which are walkable based on their slope and rasterize them.
	// If your input data is multiple meshes, you can transform them here, calculate
	// the flags for each of the meshes and rasterize them.
	memset(m_triflags, 0, m_ntris*sizeof(unsigned char));
	rcMarkWalkableTriangles(m_cfg.walkableSlopeAngle, m_verts, m_nverts, m_tris, m_ntris, m_triflags);
	rcRasterizeTriangles(m_verts, m_nverts, m_tris, m_triflags, m_ntris, *m_solid);

	if (!m_keepInterResults)
	{
		delete [] m_triflags;
		m_triflags = 0;
	}

	//
	// Step 3. Filter walkables surfaces.
	//
	
	// Once all geoemtry is rasterized, we do initial pass of filtering to
	// remove unwanted overhangs caused by the conservative rasterization
	// as well as filter spans where the character cannot possibly stand.
	rcFilterLedgeSpans(m_cfg.walkableHeight, m_cfg.walkableClimb, *m_solid);
	rcFilterWalkableLowHeightSpans(m_cfg.walkableHeight, *m_solid);

	//
	// Step 4. Partition walkable surface to simple regions.
	//

	// Compact the heightfield so that it is faster to handle from now on.
	// This will result more cache coherent data as well as the neighbours
	// between walkable cells will be calculated.
	GK_ASSERT(!m_chf);
	m_chf = new rcCompactHeightfield;
	if (!m_chf)
	{
		gkPrintf("buildNavigation: Out of memory 'chf'.");
		return false;
	}
	if (!rcBuildCompactHeightfield(m_cfg.walkableHeight, m_cfg.walkableClimb, RC_WALKABLE, *m_solid, *m_chf))
	{
		gkPrintf("buildNavigation: Could not build compact data.");
		return false;
	}
	
	if (!m_keepInterResults)
	{
		delete m_solid;
		m_solid = 0;
	}

	// Erode the walkable area by agent radius.
	if (!rcErodeArea(RC_WALKABLE_AREA, m_cfg.walkableRadius, *m_chf))
	{
		gkPrintf("buildNavigation: Could not erode.");
		return false;
	}
	
	// (Optional) Mark areas.
	/*const ConvexVolume* vols = m_geom->getConvexVolumes();
	for (int i  = 0; i < m_geom->getConvexVolumeCount(); ++i)
		rcMarkConvexPolyArea(vols[i].verts, vols[i].nverts, vols[i].hmin, vols[i].hmax, (unsigned char)vols[i].area, *m_chf);
	*/
	
	// Prepare for region partitioning, by calculating distance field along the walkable surface.
	if (!rcBuildDistanceField(*m_chf))
	{
		gkPrintf("buildNavigation: Could not build distance field.");
		return false;
	}

	// Partition the walkable surface into simple regions without holes.
	if (!rcBuildRegions(*m_chf, m_cfg.borderSize, m_cfg.minRegionSize, m_cfg.mergeRegionSize))
	{
		gkPrintf("buildNavigation: Could not build regions.");
	}
	
	//
	// Step 5. Trace and simplify region contours.
	//
	
	// Create contours.
	GK_ASSERT(!m_cset);
	m_cset = new rcContourSet;
	if (!m_cset)
	{
		gkPrintf("buildNavigation: Out of memory 'cset'.");
		return false;
	}
	if (!rcBuildContours(*m_chf, m_cfg.maxSimplificationError, m_cfg.maxEdgeLen, *m_cset))
	{
		gkPrintf("buildNavigation: Could not create contours.");
		return false;
	}
	
	//
	// Step 6. Build polygons mesh from contours.
	//
	
	// Build polygon navmesh from the contours.
	GK_ASSERT(!m_pmesh);
	m_pmesh = new rcPolyMesh;
	if (!m_pmesh)
	{
		gkPrintf("buildNavigation: Out of memory 'pmesh'.");
		return false;
	}
	if (!rcBuildPolyMesh(*m_cset, m_cfg.maxVertsPerPoly, *m_pmesh))
	{
		gkPrintf("buildNavigation: Could not triangulate contours.");
		return false;
	}
	
	//
	// Step 7. Create detail mesh which allows to access approximate height on each polygon.
	//

	GK_ASSERT(!m_dmesh);
	m_dmesh = new rcPolyMeshDetail;
	if (!m_dmesh)
	{
		gkPrintf("buildNavigation: Out of memory 'pmdtl'.");
		return false;
	}

	if (!rcBuildPolyMeshDetail(*m_pmesh, *m_chf, m_cfg.detailSampleDist, m_cfg.detailSampleMaxError, *m_dmesh))
	{
		gkPrintf("buildNavigation: Could not build detail mesh.");
	}

	if (!m_keepInterResults)
	{
		delete m_chf;
		m_chf = 0;
		delete m_cset;
		m_cset = 0;
	}

	// At this point the navigation mesh data is ready, you can access it from m_pmesh.
	// See rcDebugDrawPolyMesh or dtCreateNavMeshData as examples how to access the data.
	
	//
	// Step 8. Create Detour data from Recast poly mesh.
	//
	
	// The GUI may allow more max points per polygon than Detour can handle.
	// Only build the detour navmesh if we do not exceed the limit.

	// Maximum number of vertices per navigation polygon.
	static const int DT_STAT_VERTS_PER_POLYGON = 6;

	// The GUI may allow more max points per polygon than Detour can handle.
	// Only build the detour navmesh if we do not exceed the limit.
	if (m_cfg.maxVertsPerPoly <= DT_VERTS_PER_POLYGON)
	{
		unsigned char* navData = 0;
		int navDataSize = 0;

		// Update poly flags from areas.
		for (int i = 0; i < m_pmesh->npolys; ++i)
		{
			if (m_pmesh->areas[i] == RC_WALKABLE_AREA)
				m_pmesh->areas[i] = SAMPLE_POLYAREA_GROUND;
				
			if (m_pmesh->areas[i] == SAMPLE_POLYAREA_GROUND ||
				m_pmesh->areas[i] == SAMPLE_POLYAREA_GRASS ||
				m_pmesh->areas[i] == SAMPLE_POLYAREA_ROAD)
			{
				m_pmesh->flags[i] = SAMPLE_POLYFLAGS_WALK;
			}
			else if (m_pmesh->areas[i] == SAMPLE_POLYAREA_WATER)
			{
				m_pmesh->flags[i] = SAMPLE_POLYFLAGS_SWIM;
			}
			else if (m_pmesh->areas[i] == SAMPLE_POLYAREA_DOOR)
			{
				m_pmesh->flags[i] = SAMPLE_POLYFLAGS_WALK | SAMPLE_POLYFLAGS_DOOR;
			}
		}


		dtNavMeshCreateParams params;
		memset(&params, 0, sizeof(params));
		params.verts = m_pmesh->verts;
		params.vertCount = m_pmesh->nverts;
		params.polys = m_pmesh->polys;
		params.polyAreas = m_pmesh->areas;
		params.polyFlags = m_pmesh->flags;
		params.polyCount = m_pmesh->npolys;
		params.nvp = m_pmesh->nvp;
		params.detailMeshes = m_dmesh->meshes;
		params.detailVerts = m_dmesh->verts;
		params.detailVertsCount = m_dmesh->nverts;
		params.detailTris = m_dmesh->tris;
		params.detailTriCount = m_dmesh->ntris;
/*		params.offMeshConVerts = m_geom->getOffMeshConnectionVerts();
		params.offMeshConRad = m_geom->getOffMeshConnectionRads();
		params.offMeshConDir = m_geom->getOffMeshConnectionDirs();
		params.offMeshConAreas = m_geom->getOffMeshConnectionAreas();
		params.offMeshConFlags = m_geom->getOffMeshConnectionFlags();
		params.offMeshConCount = m_geom->getOffMeshConnectionCount();
*/		params.walkableHeight = GET_SOCKET_VALUE(AGENT_HEIGHT);
		params.walkableRadius = GET_SOCKET_VALUE(AGENT_RADIUS);
		params.walkableClimb = GET_SOCKET_VALUE(AGENT_MAX_CLIMB);
		vcopy(params.bmin, m_pmesh->bmin);
		vcopy(params.bmax, m_pmesh->bmax);
		params.cs = m_cfg.cs;
		params.ch = m_cfg.ch;

		
		if (!dtCreateNavMeshData(&params, &navData, &navDataSize))
		{
			gkPrintf("Could not build Detour navmesh.");
			return false;
		}
		
		GK_ASSERT(!m_navMesh);
		m_navMesh = new dtNavMesh;
		if (!m_navMesh)
		{
			delete [] navData;
			gkPrintf("Could not create Detour navmesh");
			return false;
		}
		
		if (!m_navMesh->init(navData, navDataSize, DT_TILE_FREE_DATA, 2048))
		{
			delete [] navData;
			gkPrintf("Could not init Detour navmesh");
			return false;
		}
	}
	
	rcTimeVal totEndTime = rcGetPerformanceTimer();

	// Show performance stats.
	{
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
		
		gkPrintf("Polymesh: Verts:%d  Polys:%d", m_pmesh->nverts, m_pmesh->npolys);
		
		gkPrintf("TOTAL: %.1fms", rcGetDeltaTimeUsec(totStartTime, totEndTime)/1000.0f);
	}

	return true;
}

void gkStaticNavMeshNode::FindPath()
{
	SET_SOCKET_VALUE(PATH_FOUND, false);

	if (!m_navMesh)
	{
		Create();
	}

	if (m_navMesh)
	{
		gkVector3 startPos = GET_SOCKET_VALUE(OBJ)->getPosition();

		Ogre::Ray ray = gkUtils::CreateCameraRay(GET_SOCKET_VALUE(END_X_POS), GET_SOCKET_VALUE(END_Y_POS));

		gkVector3 endPos;
		
		if(!gkUtils::PickBody(ray, endPos))
			return;

		std::swap(startPos.y, startPos.z);
		std::swap(endPos.y, endPos.z);

		rcTimeVal totStartTime = rcGetPerformanceTimer();

		gkVector3 polyPickExt = GET_SOCKET_VALUE(POLY_PICK_EXT);

		dtQueryFilter filter;

		dtPolyRef startRef = m_navMesh->findNearestPoly(startPos.ptr(), polyPickExt.ptr(), &filter, 0);

		dtPolyRef endRef = m_navMesh->findNearestPoly(endPos.ptr(), polyPickExt.ptr(), &filter, 0);
	
		if (startRef && endRef)
		{
			if(m_polys)
			{
				delete m_polys;
				m_polys = 0;
			}

			int MAX_POLYS = GET_SOCKET_VALUE(MAX_PATH_POLYS);

			m_polys = new dtPolyRef[MAX_POLYS];

			m_npolys = m_navMesh->findPath(startRef, endRef, startPos.ptr(), endPos.ptr(), &filter, m_polys, MAX_POLYS);

			if (m_npolys)
			{
				if(m_straightPath)
				{
					delete m_straightPath;
					m_straightPath = 0;
				}

				m_straightPath = new gkScalar[MAX_POLYS*3];

				m_nstraightPath = m_navMesh->findStraightPath(startPos.ptr(), endPos.ptr(), m_polys, m_npolys, m_straightPath, 0, 0, MAX_POLYS);

				rcTimeVal totEndTime = rcGetPerformanceTimer();

				gkPrintf("FINDPATH TOTAL: %.1fms", rcGetDeltaTimeUsec(totStartTime, totEndTime)/1000.0f);

				std::swap(startPos.y, startPos.z);
				std::swap(endPos.y, endPos.z);

				gkVector3 oldPoint = startPos;
				gkVector3 point;

				const gkScalar Z_OFFSET = 0.3f;

				static const btVector3 RED_COLOR(1,0,0);

				for(int i=0; i<m_nstraightPath*3; i+=3)
				{
					point.x = m_straightPath[i];
					point.y = m_straightPath[i+2];
					point.z = m_straightPath[i+1];

					m_debug->drawLine(
						btVector3(oldPoint.x, oldPoint.y, oldPoint.z + Z_OFFSET), 
						btVector3(point.x, point.y, point.z + Z_OFFSET), 
						RED_COLOR
					);

					oldPoint = point;
				}

				m_debug->drawLine(
					btVector3(oldPoint.x, oldPoint.y, oldPoint.z + Z_OFFSET), 
					btVector3(endPos.x, endPos.y, endPos.z + Z_OFFSET), 
					RED_COLOR
				);

				m_debug->flush();

				SET_SOCKET_VALUE(PATH_FOUND, true);
			}
		}
	}
}

gkNavMeshLoader::gkNavMeshLoader() 
: m_debug(0)//, os(L"momo.obj")
{
	//gkScene* pScene = gkEngine::getSingleton().getActiveScene();

	//GK_ASSERT(pScene);

	//m_debug = new gkPhysicsDebug(pScene->getDynamicsWorld());
}

gkNavMeshLoader::~gkNavMeshLoader()
{
	//delete m_debug;
}

void gkNavMeshLoader::addTriangle(const gkVector3& v1, const gkVector3& v2, const gkVector3& v3)
{
	size_t n = m_verts.size();

	int a = n; 
	int b = n+1;
	int c = n+2;

	m_verts.push_back(v1);
	m_verts.push_back(v2);
	m_verts.push_back(v3);

	m_tris.push_back(a);
	m_tris.push_back(b);
	m_tris.push_back(c);

	gkVector3 normal((v3-v1).crossProduct(v2-v1)); 
	normal.normalise();

	m_normals.push_back(normal);

	m_tris.push_back(c);
	m_tris.push_back(b);
	m_tris.push_back(a);

	m_normals.push_back(-normal);
/*
	os << "v " << v1.x << " " << v1.y << " " << v1.z << "\n";
	os << "v " << v2.x << " " << v2.y << " " << v2.z << "\n";
	os << "v " << v3.x << " " << v3.y << " " << v3.z << "\n";

	a++; b++; c++;
	os << "f " << a  << " " << b << " " << c << "\n";
	os << "f " << c  << " " << b << " " << a << "\n";
*/
}

bool gkNavMeshLoader::load(gkGameObject* pObj)
{
	gkRigidBody* pBody = pObj->getAttachedBody();

	if(pBody)
	{
		AddCollisionObj(pBody);
	}

	return true;
}

void gkNavMeshLoader::processTriangle(btVector3* triangle,int partId, int triangleIndex)
{
	btVector3 v1 = m * triangle[0];
	btVector3 v2 = m * triangle[1];
	btVector3 v3 = m * triangle[2];

	gkVector3 vv1(v1.x(), v1.z(), v1.y());
	gkVector3 vv2(v2.x(), v2.z(), v2.y());
	gkVector3 vv3(v3.x(), v3.z(), v3.y());

	addTriangle(vv1, vv2, vv3);

//	m_debug->drawLine(v1, v2, btVector3(1,1,0));
//	m_debug->drawLine(v2, v3, btVector3(1,1,0));
//	m_debug->drawLine(v3, v1, btVector3(1,1,0));
}

void gkNavMeshLoader::AddCollisionObj(gkRigidBody* pBody)
{
	gkScene* pScene = gkEngine::getSingleton().getActiveScene();

	GK_ASSERT(pScene);

	btVector3 aabbMin,aabbMax;
	pScene->getDynamicsWorld()->getBulletWorld()->getBroadphase()->getBroadphaseAabb(aabbMin,aabbMax);
	
	aabbMin-=btVector3(BT_LARGE_FLOAT,BT_LARGE_FLOAT,BT_LARGE_FLOAT);
	aabbMax+=btVector3(BT_LARGE_FLOAT,BT_LARGE_FLOAT,BT_LARGE_FLOAT);

	btRigidBody* body = pBody->getBody();

	btCollisionObject* colObj = body;

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

				addTriangle(vv1, vv2, vv3);

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
}



