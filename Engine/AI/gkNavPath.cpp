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
#include "Ogre.h"
#include "gkNavPath.h"
#include "gkNavMeshData.h"
#include "Recast.h"
#include "RecastLog.h"
#include "RecastTimer.h"
#include "DetourNavMeshBuilder.h"
#include "DetourNavMesh.h"
#include "gkScene.h"
#include "gkDynamicsWorld.h"
#include "gkPhysicsDebug.h"

gkNavPath::gkNavPath() 
{
}

gkNavPath::~gkNavPath() 
{
}

bool gkNavPath::create(const gkNavMeshData& data, const gkVector3& from, const gkVector3& to, const gkVector3& polyPickExt, int maxPathPolys)
{
	PNAVMESH navMesh = data.getNavigationMesh();
	
	if(navMesh.get())
	{
		m_path.clear();

		gkVector3 startPos(from);
		gkVector3 endPos(to);

		std::swap(startPos.y, startPos.z);
		std::swap(endPos.y, endPos.z);

		dtQueryFilter filter;

		dtPolyRef startRef = navMesh->data->findNearestPoly(startPos.ptr(), polyPickExt.ptr(), &filter, 0);

		dtPolyRef endRef = navMesh->data->findNearestPoly(endPos.ptr(), polyPickExt.ptr(), &filter, 0);

		if(startRef && endRef)
		{
			utArray<dtPolyRef> polys;
			polys.resize(maxPathPolys);

			int npolys = navMesh->data->findPath(startRef, endRef, startPos.ptr(), endPos.ptr(), &filter, polys.ptr(), maxPathPolys);

			if(npolys)
			{
				utArray<gkScalar> straightPath;
				straightPath.resize(maxPathPolys*3);

				int nstraightPath = navMesh->data->findStraightPath(startPos.ptr(), endPos.ptr(), polys.ptr(), npolys, straightPath.ptr(), 0, 0, maxPathPolys);

				std::swap(startPos.y, startPos.z);
				std::swap(endPos.y, endPos.z);
				
				gkVector3 point;

				for(int i=0; i<nstraightPath*3; i+=3)
				{
					point.x = straightPath[i];
					point.y = straightPath[i+2];
					point.z = straightPath[i+1];

					m_path.push_back(point);
				}

				m_path.push_back(endPos);

				return true;
			}
		}
	}

	return false;
}

gkScalar gkNavPath::getDirection(const gkVector3& current_pos, gkScalar foundThreshold, gkVector3& dir)
{
	gkScalar d = 0;
	
	if(!m_path.empty())
	{
		const gkVector3& targetPos = m_path.back();

		const gkVector3& pos = m_path.front();

		dir = pos - current_pos;
		
		d = dir.length();

		if(d < foundThreshold || current_pos.distance(targetPos) < foundThreshold)
		{
			m_path.pop_front();
			
			//return d;//getDirection(current_pos, foundThreshold, dir);
		}

		return d;
	}

	return d;
}

void gkNavPath::showPath(gkScene* scene, const gkVector3& from)
{
	gkPhysicsDebug* debug = scene->getDynamicsWorld()->getDebug();

	if(debug)
	{
		unsigned int n = m_path.size();

		if(n)
		{
			static const btVector3 RED_COLOR(1,0,0);

			gkVector3 oldPoint = from;

			for(unsigned int i=0; i<n; i++)
			{
				gkVector3 point = m_path.at(i);

				debug->drawLine(
					btVector3(oldPoint.x, oldPoint.y, oldPoint.z), 
					btVector3(point.x, point.y, point.z), 
					RED_COLOR
				);

				oldPoint = m_path.at(i);
			}
		}
	}
}


