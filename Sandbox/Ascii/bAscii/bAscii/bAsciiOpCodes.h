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
#ifndef _bAsciiOpCodes_h_
#define _bAsciiOpCodes_h_

#include "bAsciiCommon.h"
#include "bAsciiKeywords.h"

// ----------------------------------------------------------------------------
// Compiler codes

enum bAsciiCompileCode
{
	OP_END = 0,
	OP_SECTION,
	OP_SEPERATOR,
	OP_ASSIGNMENT,
	OP_TERM,
	OP_BRACKET_OPEN,
	OP_BRACKET_CLOSE,
	OP_STRING,
	OP_ALPHA,
	OP_NUMBER,
};

// ----------------------------------------------------------------------------
// Library data

enum bAsciiLibraries
{
	OP_LIB_GB = 0,
	OP_LIB_SC, 
	OP_LIB_OB, 
	OP_LIB_CA, 
	OP_LIB_LA, 
	OP_LIB_ME, 
	OP_LIB_AR, 
	OP_LIB_MA, 
	OP_LIB_IM, 
	OP_LIB_TX,
	OP_LIB_MAX,
};

// ----------------------------------------------------------------------------
// Library data types

enum bAsciiBlockCode
{
	OP_BLK_SC = 0,
	OP_BLK_OB, 
	OP_BLK_CA, 
	OP_BLK_LA, 
	OP_BLK_ME, 
	OP_BLK_AR, 
	OP_BLK_MA, 
	OP_BLK_IM, 
	OP_BLK_TE, 
	OP_BLK_TX, 
};


// ----------------------------------------------------------------------------
// Argument types

enum bAsciiTypeCode
{
	OP_TYPE_NONE = 0,
	OP_TYPE_MIXED,
	OP_TYPE_STRING,
	OP_TYPE_BOOL,
	OP_TYPE_BOOL2,
	OP_TYPE_BOOL3,
	OP_TYPE_INT,
	OP_TYPE_INT2,
	OP_TYPE_INT3,
	OP_TYPE_FLOAT,
	OP_TYPE_FLOAT2,
	OP_TYPE_FLOAT3,
	OP_TYPE_FLOAT4,
	OP_TYPE_FLOAT9,
	OP_TYPE_FLOAT16,
	OP_TYPE_STRUCT,
	OP_TYPE_ARRAY_BEGIN,
	OP_TYPE_STRING_ARRAY,
	OP_TYPE_BOOL_ARRAY,
	OP_TYPE_INT_ARRAY,
	OP_TYPE_FLOAT_ARRAY,
	OP_TYPE_ARRAY_END,
	OP_TYPE_UNKNOWN,
};

// ----------------------------------------------------------------------------
// Global

enum bAsciiGlobalOp
{
	OP_GLOBAL_CREATOR=0,
	OP_GLOBAL_VERSION,
	OP_GLOBAL_SUBVERSION,
	OP_GLOBAL_SCENE,
};

// ----------------------------------------------------------------------------
// Scene

enum bAsciiSceneOp
{
	OP_SCENE_OBJECTS=0,
};



// ----------------------------------------------------------------------------
// Object

enum bAsciiObjectOp
{
	OP_OBJECT_LIB=0,
	OP_OBJECT_LOC,
	OP_OBJECT_ROT,
	OP_OBJECT_SIZE,
	OP_OBJECT_MATRIX,
	OP_OBJECT_PARENT,
};


// ----------------------------------------------------------------------------
// Camera

enum bAsciiCameraOp
{
	OP_CAMERA_CLIP=0,
	OP_CAMERA_FOV,
};



// ----------------------------------------------------------------------------
// Light

enum bAsciiLightOp
{
	OP_LIGHT_TODO=0,
};




// ----------------------------------------------------------------------------
// MESH

enum bAsciiMeshOp
{
	OP_MESH_VERTEX_BUFFER=0,
	OP_MESH_VERTEX_VERTS,
	OP_MESH_VERTEX_NORMALS,
	OP_MESH_INDEX_BUFFER,
	OP_MESH_INDEX_INDICES,
	OP_MESH_INDEX_MATERIALS,
	OP_MESH_INDEX_FLAGS,
	OP_MESH_MATERIALS,
};


#endif//_bAsciiOpCodes_h_
