/*
Maya Character Exporter
Copyright (c) 2009 Jun Murakawa

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. 
   If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include <assert.h>
#include "PolyList.h"

bool VertIndex::isEqual(VertIndex &other)
{
	return pos == other.pos && normal == other.normal && uv == other.uv;
}

PolyList::PolyList()
{
}

PolyList::PolyList(int polyCount)
{
	list.resize( polyCount );
}

PolyList::~PolyList(void)
{
}

void PolyList::triangulate()
{
	// Calculate number of trinangle
	int triCount = 0;
	for (unsigned int i = 0; i < list.size(); i++)
	{
		int numTris = list[i].numVerts - 2;
		triCount += numTris;
	}

	// Create new list of triangles by copying indices from old Poly list
	vector<Poly> newList(triCount);
	triCount = 0;
	for (unsigned int i = 0; i < list.size(); i++)
	{
		int numTris = list[i].numVerts - 2;
		for (int j = 0; j < numTris; j++) // For each triangle in this Poly
		{
			newList[triCount].numVerts = 3;
			newList[triCount].indices[0] = list[i].indices[0];
			newList[triCount].indices[1] = list[i].indices[j + 1];
			newList[triCount].indices[2] = list[i].indices[j + 2];
			newList[triCount].matID = list[i].matID; 
			triCount++;
		}
	}
	list = newList;
}

void PolyList::extract( SMatGrp* matGrp )
{
	matGrp->triCount = (uint)list.size();
	if ( matGrp->triCount > 0 )
	{
		matGrp->triIndices = (uint*)calloc(matGrp->triCount*3, sizeof(uint));//new unsigned int[ matGrp->triCount * 3 ];
		unsigned int count = 0;
		for (unsigned int i = 0; i < list.size(); i++)
		{
			Poly *poly = &list[i];
			assert( poly->numVerts == 3 );
			for (int j = 0; j < 3; j++)
			{
				int index = poly->indices[j];
				matGrp->triIndices[ count++ ] = index;
			}
		}
		assert( count == matGrp->triCount * 3 );
	}
}

MaterialGroup::MaterialGroup(const PolyList *polyList)
{
	for ( unsigned int i = 0; i < polyList->list.size(); i++ )
	{
		const Poly *poly = &polyList->list[i];
		int matID = poly->matID;
		// is this material new?
		vector<int>::iterator it = find( MatIDs.begin(), MatIDs.end(), matID );
		if ( MatIDs.end() == it )
		{
			// Yes, so create a new PolyList and add this poly there
			MatIDs.push_back( matID );
			PolyList *newList = new PolyList();
			newList->list.push_back( *poly );
			PolyListList.push_back( newList );
		}
		else
		{
			// No, add this poly to existing PolyList
			unsigned int listID = (unsigned int)(it - MatIDs.begin());
			PolyList *list = PolyListList[listID];
			list->list.push_back( *poly );
		}
	}
}

MaterialGroup::~MaterialGroup()
{
	for ( vector< PolyList* >::iterator it = PolyListList.begin(); it < PolyListList.end(); it++ )
	{
		PolyList *list = *it;
		delete list;
	}
}

void MaterialGroup::extract( SMesh* mesh )
{
	mesh->numMats = (uint)MatIDs.size();
	if ( mesh->numMats > 0 )
	{
		mesh->matGrps = (SMatGrp*)calloc( mesh->numMats, sizeof(SMatGrp) );//new SMatGrp[ mesh->numMats ];
		for (size_t i = 0; i < mesh->numMats; i++)
		{
			SMatGrp* matGrp = &mesh->matGrps[i];
			matGrp->materialID = MatIDs[i];
			PolyList *list = PolyListList[i];
			list->extract( matGrp );
		}
	}
}
