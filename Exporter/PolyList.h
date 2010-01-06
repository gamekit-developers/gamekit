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

#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

#include "../SharedStructures.h"

//////////////////////////////////////////////////////////////////////

// My index structures
struct VertIndex {
	int pos;	// Position Index
	int normal; // Normal Index
	int uv;		// Texture coordinates Index

	// True if this index is equal to other one
	bool isEqual(VertIndex &other);

};

//////////////////////////////////////////////////////////////////////

// My polygon structure
struct Poly {
	static const int MAX_POLY_VERTS = 20;
	int numVerts;
	int indices[MAX_POLY_VERTS];
	int matID;							// Material ID
};

//////////////////////////////////////////////////////////////////////

// My polygon list class
class PolyList {

public:
	vector<Poly> list;

	PolyList();
	PolyList(int polyCount);

	~PolyList();

	void triangulate();

	void PolyList::extract( SMatGrp* matGrp );
};

//////////////////////////////////////////////////////////////////////

class MaterialGroup 
{
public:
	vector< int >		MatIDs;
	vector< PolyList* > PolyListList;

	// create a material group from a poly list
	MaterialGroup( const PolyList *polyList );
	~MaterialGroup();

	void extract( SMesh* smesh );
};



