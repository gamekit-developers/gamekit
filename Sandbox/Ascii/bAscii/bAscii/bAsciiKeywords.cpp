/*
-------------------------------------------------------------------------------
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#include "bAsciiKeywords.h"
#include "bAsciiOpCodes.h"

#define Null {0, UT_NPOS, OP_UNKNOWN, OP_UNKNOWN,OP_UNKNOWN, -1, 0}

// ----------------------------------------------------------------------------
// Global Tables
// ----------------------------------------------------------------------------

static bAsciiLocal GlobalType[] = {

    {KW_GLOBAL_CREATOR, UT_NPOS, OP_GLOBAL_CREATOR, OP_UNKNOWN, OP_STRING, OP_TYPE_STRING, 1, 1, 0},
    {KW_GLOBAL_SCENE,   UT_NPOS, OP_GLOBAL_SCENE,   OP_UNKNOWN, OP_STRING, OP_TYPE_STRING, 1, 1, 0},
    Null
};

// ----------------------------------------------------------------------------
// Scene Tables
// ----------------------------------------------------------------------------

static bAsciiLocal SceneType[] = {

    {KW_SCENE_OBJECTS, UT_NPOS, OP_SCENE_OBJECTS, OP_UNKNOWN, OP_STRING, OP_TYPE_STRING, -1, -1, 0},
    Null
};

static bAsciiLocal SceneLibrary[] = {

    {KW_SCENE, UT_NPOS, OP_BLK_SC, OP_STRING, OP_UNKNOWN, OP_TYPE_STRUCT, -1, -1, SceneType},
    Null
};

// ----------------------------------------------------------------------------
// Object Tables
// ----------------------------------------------------------------------------


static bAsciiLocal ObjectType[] = {

    {KW_OBJECT_DATA,   UT_NPOS, OP_OBJECT_LIB,    OP_UNKNOWN, OP_STRING, OP_TYPE_STRING,    1,  1,  0},
    {KW_OBJECT_LOC,    UT_NPOS, OP_OBJECT_LOC,    OP_UNKNOWN, OP_NUMBER, OP_TYPE_FLOAT3,    3,  3,  0},
    {KW_OBJECT_ROT,    UT_NPOS, OP_OBJECT_ROT,    OP_UNKNOWN, OP_NUMBER, OP_TYPE_FLOAT3,    3,  3,  0},
    {KW_OBJECT_SIZE,   UT_NPOS, OP_OBJECT_SIZE,   OP_UNKNOWN, OP_NUMBER, OP_TYPE_FLOAT3,    3,  3,  0},
    {KW_OBJECT_MATRIX, UT_NPOS, OP_OBJECT_MATRIX, OP_UNKNOWN, OP_NUMBER, OP_TYPE_FLOAT16,   16, 16, 0},
    {KW_OBJECT_PARENT, UT_NPOS, OP_OBJECT_PARENT, OP_UNKNOWN, OP_STRING, OP_TYPE_STRING,    1,  1,  0},
    Null
};

static bAsciiLocal ObjectLibrary[] = {

    {KW_OBJECT, UT_NPOS, OP_BLK_OB, OP_STRING, OP_UNKNOWN, OP_TYPE_STRUCT, -1, -1, ObjectType},
    Null
};



// ----------------------------------------------------------------------------
// Camera Tables
// ----------------------------------------------------------------------------

static bAsciiLocal CameraType[] = {

    {KW_CAMERA_CLIP, UT_NPOS, OP_CAMERA_CLIP, OP_UNKNOWN, OP_NUMBER, OP_TYPE_FLOAT2, 2, 2, 0},
    {KW_CAMERA_FOV,  UT_NPOS, OP_CAMERA_FOV,  OP_UNKNOWN, OP_NUMBER, OP_TYPE_FLOAT,  1, 1, 0},
    Null
};

static bAsciiLocal CameraLibrary[] = {

    {KW_CAMERA, UT_NPOS, OP_BLK_CA, OP_STRING, OP_UNKNOWN, OP_TYPE_STRUCT, -1, -1, CameraType},
    Null
};



// ----------------------------------------------------------------------------
// Light Tables
// ----------------------------------------------------------------------------

static bAsciiLocal LightType[] = {

    Null
};

static bAsciiLocal LightLibrary[] = {

    {KW_LIGHT, UT_NPOS, OP_BLK_CA, OP_STRING, OP_UNKNOWN, OP_TYPE_STRUCT, -1, -1, LightType},
    Null
};



// ----------------------------------------------------------------------------
// Mesh Tables
// ----------------------------------------------------------------------------

static bAsciiLocal MeshVertexBufferType[] = {

    {KW_MESH_VERTS,     UT_NPOS, OP_MESH_VERTEX_VERTS,   OP_UNKNOWN, OP_NUMBER, OP_TYPE_FLOAT_ARRAY, 3, -1, 0},
    {KW_MESH_NORMALS,   UT_NPOS, OP_MESH_VERTEX_NORMALS, OP_UNKNOWN, OP_NUMBER, OP_TYPE_FLOAT_ARRAY, 3, -1, 0},
    Null
};

static bAsciiLocal MeshIndexBufferType[] = {

    {KW_MESH_INDICES,   UT_NPOS, OP_MESH_INDEX_INDICES,     OP_UNKNOWN, OP_NUMBER, OP_TYPE_INT_ARRAY,   4, -1, 0},
    {KW_MESH_MATNRS,    UT_NPOS, OP_MESH_INDEX_MATERIALS,   OP_UNKNOWN, OP_NUMBER, OP_TYPE_INT_ARRAY,   1, -1, 0},
    {KW_MESH_FLAGS,     UT_NPOS, OP_MESH_INDEX_FLAGS,       OP_UNKNOWN, OP_NUMBER, OP_TYPE_INT_ARRAY,   1, -1, 0},
    Null
};

static bAsciiLocal MeshType[] = {

    {KW_MESH_VBUFFER,   UT_NPOS, OP_MESH_VERTEX_BUFFER, OP_NUMBER, OP_UNKNOWN, OP_TYPE_STRUCT,          -1, -1, MeshVertexBufferType},
    {KW_MESH_IBUFFER,   UT_NPOS, OP_MESH_INDEX_BUFFER,  OP_NUMBER, OP_UNKNOWN, OP_TYPE_STRUCT,          -1, -1, MeshIndexBufferType},
    {KW_MESH_MATERIALS, UT_NPOS, OP_MESH_MATERIALS,     OP_UNKNOWN, OP_STRING, OP_TYPE_STRING_ARRAY,    -1, 16, 0},
    Null
};

static bAsciiLocal MeshLibrary[] = {

    {KW_MESH, UT_NPOS, OP_BLK_ME, OP_STRING, OP_UNKNOWN, OP_TYPE_STRUCT, -1, -1, MeshType},
    Null
};


// ----------------------------------------------------------------------------
static bAsciiLocal bAsciiGlobals[] = 
{
    {KW_GLOBAL,         UT_NPOS, OP_LIB_GB, OP_UNKNOWN, OP_UNKNOWN, OP_TYPE_STRUCT, -1, -1, GlobalType},
    {KW_SCENELIBRARY,   UT_NPOS, OP_LIB_SC, OP_UNKNOWN, OP_UNKNOWN, OP_TYPE_STRUCT, -1, -1, SceneLibrary},
    {KW_OBJECTLIBRARY,  UT_NPOS, OP_LIB_OB, OP_UNKNOWN, OP_UNKNOWN, OP_TYPE_STRUCT, -1, -1, ObjectLibrary},
    {KW_CAMERALIBRARY,  UT_NPOS, OP_LIB_CA, OP_UNKNOWN, OP_UNKNOWN, OP_TYPE_STRUCT, -1, -1, CameraLibrary},
    {KW_LIGHTLIBRARY,   UT_NPOS, OP_LIB_LA, OP_UNKNOWN, OP_UNKNOWN, OP_TYPE_STRUCT, -1, -1, LightLibrary},
    {KW_MESHLIBRARY,    UT_NPOS, OP_LIB_ME, OP_UNKNOWN, OP_UNKNOWN, OP_TYPE_STRUCT, -1, -1, MeshLibrary},
    Null
};

// ----------------------------------------------------------------------------
bAsciiLocal* bAscciFindGlobal(unsigned int code)
{
	bAsciiLocal *type = bAsciiGlobals;

	while (type && type->name)
	{
		if (type->hashname == code)
			return type;
		++type;
	}
	return 0;
}

// ----------------------------------------------------------------------------
bAsciiLocal* bAscciFindLocal(bAsciiLocal *st, unsigned int code)
{
	if (st && st->types)
	{
		bAsciiLocal *l = st->types;

		while (l && l->name)
		{
			if (l->hashname == code)
				return l;
			++l;
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
bAsciiLocal* bAscciGetGlobal(bCode code)
{
	bAsciiLocal *type = bAsciiGlobals;

	while (type && type->name)
	{
		if (type->op == code)
			return type;
		++type;
	}
	return 0;
}

// ----------------------------------------------------------------------------
bAsciiLocal* bAscciGetLocal(bAsciiLocal *parent, bCode code)
{
	if (parent && parent->types)
	{
		bAsciiLocal *l = parent->types;

		while (l && l->name)
		{
			if (l->op == code)
				return l;
			++l;
		}
	}
	return 0;
}

// ----------------------------------------------------------------------------
const char* bAsciiKeyword(bAsciiLocal *parent, bCode code)
{
	if (parent)
	{
		bAsciiLocal *l = parent;

		while (l && l->name)
		{
			if (l->op == code)
				return l->name;
			++l;
		}
	}
	return "Unknwon";
}

// ----------------------------------------------------------------------------
static void bAsciiBuildSubTypes(bAsciiLocal *st)
{
	if (!st) return;

	while (st && st->name)
	{
		if (st->hashname == UT_NPOS)
			st->hashname = (unsigned int)::utHash(st->name);

		if (st->types)
			bAsciiBuildSubTypes(st->types);
		++st;
	}
}


// ----------------------------------------------------------------------------
void bAscciBuildTables(void)
{
	bAsciiLocal *type = bAsciiGlobals;

	while (type && type->name)
	{
		if (type->hashname == UT_NPOS)
			type->hashname = (unsigned int)::utHash(type->name);

		if (type->types)
			bAsciiBuildSubTypes(type->types);

		++type;
	}
}
