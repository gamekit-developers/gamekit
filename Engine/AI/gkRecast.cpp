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
#include "btBulletDynamicsCommon.h"
#include "gkRecast.h"
#include "gkMeshData.h"
#include "gkEngine.h"
#include "gkScene.h"
#include "gkLogger.h"
#include "gkActiveObject.h"
#include "gkGameObject.h"

#include "Recast.h"
#include "RecastLog.h"
#include "RecastTimer.h"
#include "DetourNavMeshBuilder.h"
#include "DetourNavMesh.h"
#include <memory>

gkDetourNavMesh::~gkDetourNavMesh()
{
	if (m_p) delete m_p;
}

PDT_NAV_MESH gkRecast::createNavMesh(PMESHDATA meshData, const Config& config)
{
	if (!meshData.get())
		return PDT_NAV_MESH(0);

	rcConfig cfg;

	cfg.cs = config.CELL_SIZE;
	cfg.ch = config.CELL_HEIGHT;

	GK_ASSERT(cfg.ch && "cfg.ch cannot be zero");
	GK_ASSERT(cfg.ch && "cfg.ch cannot be zero");

	cfg.walkableSlopeAngle = config.AGENT_MAX_SLOPE;
	cfg.walkableHeight = (int)ceilf(config.AGENT_HEIGHT / cfg.ch);
	cfg.walkableClimb = (int)ceilf(config.AGENT_MAX_CLIMB / cfg.ch);
	cfg.walkableRadius = (int)ceilf(config.AGENT_RADIUS / cfg.cs);
	cfg.maxEdgeLen = (int)(config.EDGE_MAX_LEN / cfg.cs);
	cfg.maxSimplificationError = config.EDGE_MAX_ERROR;
	cfg.minRegionSize = (int)rcSqr(config.REGION_MIN_SIZE);
	cfg.mergeRegionSize = (int)rcSqr(config.REGION_MERGE_SIZE);
	cfg.maxVertsPerPoly = gkMin(config.VERTS_PER_POLY, DT_VERTS_PER_POLYGON);
	cfg.tileSize = config.TILE_SIZE;
	cfg.borderSize = cfg.walkableRadius + 4; // Reserve enough padding.
	cfg.detailSampleDist = config.DETAIL_SAMPLE_DIST < 0.9f ? 0 : cfg.cs * config.DETAIL_SAMPLE_DIST;
	cfg.detailSampleMaxError = cfg.ch * config.DETAIL_SAMPLE_ERROR;

	if (!meshData->getVertCount())
		return PDT_NAV_MESH(0);

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
	rcVcopy(cfg.bmin, bmin);
	rcVcopy(cfg.bmax, bmax);
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
		return PDT_NAV_MESH(0);
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
		memset(triflags.ptr(), 0, ntris * sizeof(unsigned char));
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
		return PDT_NAV_MESH(0);
	}

	// Erode the walkable area by agent radius.
	if (!rcErodeArea(RC_WALKABLE_AREA, cfg.walkableRadius, chf))
	{
		gkPrintf("buildNavigation: Could not erode.");
		return PDT_NAV_MESH(0);
	}

	//
	// Mark areas from objects
	//

	gkScene* scene = gkEngine::getSingleton().getActiveScene();
	gkGameObjectSet& objects = scene->getInstancedObjects();
	gkGameObjectSet::Iterator it = objects.iterator();
	while (it.hasMoreElements())
	{
		gkGameObject* obj = it.getNext();

		if (!obj->getNavData().isEmpty())
		{
			size_t tBaseIndex = obj->getNavData().triangleBaseIndex;
			size_t vBaseIndex = tBaseIndex / 2;

			const float* v = verts + vBaseIndex;
			const int nVerts = obj->getNavData().nIndex / 3;

			const gkGameObjectProperties& prop = obj->getProperties();

			rcMarkConvexPolyArea(v, nVerts, obj->getNavData().hmin, obj->getNavData().hmax, prop.m_findPathFlag, chf);
		}
	}

	// Prepare for region partitioning, by calculating distance field along the walkable surface.
	if (!rcBuildDistanceField(chf))
	{
		gkPrintf("buildNavigation: Could not build distance field.");
		return PDT_NAV_MESH(0);
	}

	// Partition the walkable surface into simple regions without holes.
	if (!rcBuildRegions(chf, cfg.borderSize, cfg.minRegionSize, cfg.mergeRegionSize))
	{
		gkPrintf("buildNavigation: Could not build regions.");
		return PDT_NAV_MESH(0);
	}


	//
	// Step 5. Trace and simplify region contours.
	//

	// Create contours.
	rcContourSet cset;

	if (!rcBuildContours(chf, cfg.maxSimplificationError, cfg.maxEdgeLen, cset))
	{
		gkPrintf("buildNavigation: Could not create contours.");
		return PDT_NAV_MESH(0);
	}


	//
	// Step 6. Build polygons mesh from contours.
	//

	// Build polygon navmesh from the contours.
	rcPolyMesh pmesh;
	if (!rcBuildPolyMesh(cset, cfg.maxVertsPerPoly, pmesh))
	{
		gkPrintf("buildNavigation: Could not triangulate contours.");
		return PDT_NAV_MESH(0);
	}


	//
	// Step 7. Create detail mesh which allows to access approximate height on each polygon.
	//

	rcPolyMeshDetail dmesh;

	if (!rcBuildPolyMeshDetail(pmesh, chf, cfg.detailSampleDist, cfg.detailSampleMaxError, dmesh))
	{
		gkPrintf("buildNavigation: Could not build detail mesh.");
		return PDT_NAV_MESH(0);
	}

	// At this point the navigation mesh data is ready, you can access it from pmesh.
	// See rcDebugDrawPolyMesh or dtCreateNavMeshData as examples how to access the data.

	//
	// Step 8. Create Detour data from Recast poly mesh.
	//

	PDT_NAV_MESH navMesh;

	// Update poly flags from areas.
	for (int i = 0; i < pmesh.npolys; ++i)
		pmesh.flags[i] = 0xFFFF & pmesh.areas[i];

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
	/*        params.offMeshConVerts = m_geom->getOffMeshConnectionVerts();
	    params.offMeshConRad = m_geom->getOffMeshConnectionRads();
	    params.offMeshConDir = m_geom->getOffMeshConnectionDirs();
	    params.offMeshConAreas = m_geom->getOffMeshConnectionAreas();
	    params.offMeshConFlags = m_geom->getOffMeshConnectionFlags();
	    params.offMeshConCount = m_geom->getOffMeshConnectionCount();
	    */
	params.walkableHeight = cfg.walkableHeight * cfg.ch;
	params.walkableRadius = cfg.walkableRadius * cfg.cs;;
	params.walkableClimb = cfg.walkableClimb * cfg.ch;
	rcVcopy(params.bmin, pmesh.bmin);
	rcVcopy(params.bmax, pmesh.bmax);
	params.cs = cfg.cs;
	params.ch = cfg.ch;

	unsigned char* navData = 0;
	int navDataSize = 0;

	if (!dtCreateNavMeshData(&params, &navData, &navDataSize))
	{
		gkPrintf("Could not build Detour navmesh.");
		return PDT_NAV_MESH(0);
	}

	navMesh = PDT_NAV_MESH(new gkDetourNavMesh(new dtNavMesh));

	if (!navMesh->m_p->init(navData, navDataSize, DT_TILE_FREE_DATA, 2048))
	{
		delete [] navData;
		gkPrintf("Could not init Detour navmesh");
		return PDT_NAV_MESH(0);
	}

	rcTimeVal totEndTime = rcGetPerformanceTimer();

	gkPrintf("Navigation mesh created: %.1fms", rcGetDeltaTimeUsec(totStartTime, totEndTime) / 1000.0f);

	return navMesh;
}

bool gkRecast::findPath(PDT_NAV_MESH navMesh, const gkVector3& from, const gkVector3& to, const gkVector3& polyPickExt, int maxPathPolys, PATH_POINTS& path, unsigned short includeFlags, unsigned short excludeFlags)
{
	GK_ASSERT(!(includeFlags & excludeFlags) && "includeFlags with excludeFlags cannot overlap");

	if (navMesh.get() && navMesh->m_p)
	{
		gkVector3 startPos(from);
		gkVector3 endPos(to);

		std::swap(startPos.y, startPos.z);
		std::swap(endPos.y, endPos.z);

		dtQueryFilter filter;
		filter.includeFlags = includeFlags;
		filter.excludeFlags = excludeFlags;

		dtPolyRef startRef = navMesh->m_p->findNearestPoly(startPos.ptr(), polyPickExt.ptr(), &filter, 0);

		dtPolyRef endRef = navMesh->m_p->findNearestPoly(endPos.ptr(), polyPickExt.ptr(), &filter, 0);

		if (startRef && endRef)
		{
			utArray<dtPolyRef> polys;
			polys.resize(maxPathPolys);

			int npolys = navMesh->m_p->findPath(startRef, endRef, startPos.ptr(), endPos.ptr(), &filter, polys.ptr(), maxPathPolys);

			if (npolys > 1)
			{
				path.clear();

				utArray<gkScalar> straightPath;
				straightPath.resize(maxPathPolys * 3);

				int nstraightPath = navMesh->m_p->findStraightPath(startPos.ptr(), endPos.ptr(), polys.ptr(), npolys, straightPath.ptr(), 0, 0, maxPathPolys);

				std::swap(startPos.y, startPos.z);
				std::swap(endPos.y, endPos.z);

				gkVector3 point;

				for (int i = 0; i < nstraightPath * 3; i += 3)
				{
					point.x = straightPath[i];
					point.y = straightPath[i+2];
					point.z = straightPath[i+1];

					path.push_back(point);
				}

				return true;
			}
		}
	}

	return false;
}
