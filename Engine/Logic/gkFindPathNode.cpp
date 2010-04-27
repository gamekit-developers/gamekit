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
#include "gkLogger.h"
#include "gkEngine.h"
#include "gkDynamicsWorld.h"
#include "gkPhysicsDebug.h"
#include "gkScene.h"
#include "gkFindPathNode.h"
#include "Recast.h"
#include "RecastLog.h"
#include "RecastTimer.h"
#include "DetourNavMeshBuilder.h"
#include "DetourNavMesh.h"

gkFindPathNode::gkFindPathNode(gkLogicTree *parent, size_t id) 
: gkLogicNode(parent, id),
m_navMesh(0)
{
	ADD_ISOCK(UPDATE, false);
	ADD_ISOCK(MAX_PATH_POLYS, 256);
	ADD_ISOCK(START_POS, gkVector3::ZERO);
	ADD_ISOCK(END_POS, gkVector3::ZERO);
	ADD_ISOCK(POLY_PICK_EXT, gkVector3(2, 4, 2));
	ADD_ISOCK(SHOW_PATH_OFFSET, gkVector3::ZERO);
	ADD_ISOCK(REDO_PATH_IF_FOLLOWING, false);
	ADD_OSOCK(PATH, 0);
	ADD_OSOCK(PATH_FOUND, false);
}

gkFindPathNode::~gkFindPathNode()
{
}

bool gkFindPathNode::evaluate(gkScalar tick)
{
	gkScene* pScene = gkEngine::getSingleton().getActiveScene();

	m_navMesh = pScene->getNavigationMesh();

	return (GET_SOCKET_VALUE(UPDATE) || m_path.retry) && m_navMesh.get() && m_navMesh->data;
}

void gkFindPathNode::update(gkScalar tick)
{
	findPath();

	showPath();
}

void gkFindPathNode::findPath()
{
	if(!GET_SOCKET_VALUE(REDO_PATH_IF_FOLLOWING) && m_path.following) return;

	SET_SOCKET_VALUE(PATH_FOUND, false);
	SET_SOCKET_VALUE(PATH, 0);
	m_path.path.clear();

	gkVector3 startPos = GET_SOCKET_VALUE(START_POS);

	gkVector3 endPos = GET_SOCKET_VALUE(END_POS);
	
	std::swap(startPos.y, startPos.z);
	std::swap(endPos.y, endPos.z);

	rcTimeVal totStartTime = rcGetPerformanceTimer();

	gkVector3 polyPickExt = GET_SOCKET_VALUE(POLY_PICK_EXT);

	dtQueryFilter filter;

	dtPolyRef startRef = m_navMesh->data->findNearestPoly(startPos.ptr(), polyPickExt.ptr(), &filter, 0);

	dtPolyRef endRef = m_navMesh->data->findNearestPoly(endPos.ptr(), polyPickExt.ptr(), &filter, 0);

	if(startRef && endRef)
	{
		int MAX_POLYS = GET_SOCKET_VALUE(MAX_PATH_POLYS);

		utArray<dtPolyRef> polys;
		polys.resize(MAX_POLYS);

		int npolys = m_navMesh->data->findPath(startRef, endRef, startPos.ptr(), endPos.ptr(), &filter, polys.ptr(), MAX_POLYS);

		if(npolys)
		{
			utArray<gkScalar> straightPath;
			straightPath.resize(MAX_POLYS*3);

			int nstraightPath = m_navMesh->data->findStraightPath(startPos.ptr(), endPos.ptr(), polys.ptr(), npolys, straightPath.ptr(), 0, 0, MAX_POLYS);

			rcTimeVal totEndTime = rcGetPerformanceTimer();

			//gkPrintf("FINDPATH TOTAL: %.1fms", rcGetDeltaTimeUsec(totStartTime, totEndTime)/1000.0f);

			std::swap(startPos.y, startPos.z);
			std::swap(endPos.y, endPos.z);
			
			m_path.path.push_back(startPos);

			gkVector3 point;

			for(int i=0; i<nstraightPath*3; i+=3)
			{
				point.x = straightPath[i];
				point.y = straightPath[i+2];
				point.z = straightPath[i+1];

				m_path.path.push_back(point);
			}

			m_path.path.push_back(endPos);

			SET_SOCKET_VALUE(PATH_FOUND, true);
			SET_SOCKET_VALUE(PATH, &m_path);
		}
	}
}

void gkFindPathNode::showPath()
{
	gkScene* pScene = gkEngine::getSingleton().getActiveScene();
	
	gkPhysicsDebug *debug = pScene->getDynamicsWorld()->getDebug();
	
	if(debug)
	{
		PathData* pathData = GET_SOCKET_VALUE(PATH);

		unsigned int n = pathData ? pathData->path.size() : 0;

		if(n)
		{
			static const btVector3 RED_COLOR(1,0,0);

			gkVector3 offset = GET_SOCKET_VALUE(SHOW_PATH_OFFSET);

			gkVector3 oldPoint = pathData->path.at(0) + offset;

			for(unsigned int i=1; i<n; i++)
			{
				gkVector3 point = pathData->path.at(i) + offset;

				debug->drawLine(
					btVector3(oldPoint.x, oldPoint.y, oldPoint.z), 
					btVector3(point.x, point.y, point.z), 
					RED_COLOR
				);

				oldPoint = pathData->path.at(i) + offset;
			}
		}
	}
}

