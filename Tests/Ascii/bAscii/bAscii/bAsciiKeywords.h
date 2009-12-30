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
#ifndef _bAsciiKeywords_h_
#define _bAsciiKeywords_h_

#include "bAsciiCommon.h"
#define BASCII_VERSION 1

// ----------------------------------------------------------------------------
struct bAsciiLocal
{
	const char*		name;
	unsigned int	hashname;
	bCode			op;
	bCode			ctor;
	bCode			arg;
	bCode			type;
	int				nrarg;
	int				maxarg;
	bAsciiLocal*	types;
};


// ----------------------------------------------------------------------------
extern bAsciiLocal*		bAscciFindGlobal(unsigned int code);
extern bAsciiLocal*		bAscciFindLocal(bAsciiLocal *st, unsigned int code);
extern void				bAscciBuildTables(void);

#define KW_GLOBAL			"Global"
#define KW_GLOBAL_CREATOR	"creator"
#define KW_GLOBAL_SCENE		"scene"


#define KW_SCENELIBRARY		"SceneLibrary"
#define KW_OBJECTLIBRARY	"ObjectLibrary"
#define KW_CAMERALIBRARY	"CameraLibrary"
#define KW_LIGHTLIBRARY		"LightLibrary"
#define KW_MESHLIBRARY		"MeshLibrary"

#define KW_SCENE			"Scene"
#define KW_SCENE_OBJECTS	"objects"

#define KW_OBJECT			"Object"
#define KW_OBJECT_DATA		"data"
#define KW_OBJECT_LOC		"location"
#define KW_OBJECT_ROT		"rotation"
#define KW_OBJECT_SIZE		"size"
#define KW_OBJECT_MATRIX	"matrix"
#define KW_OBJECT_PARENT	"parent"


#define KW_CAMERA			"Camera"
#define KW_CAMERA_CLIP		"clip"
#define KW_CAMERA_FOV		"fov"

#define KW_LIGHT			"Light"

#define KW_MESH				"Mesh"
#define KW_MESH_MATERIALS	"materials"
#define KW_MESH_VBUFFER		"VertexBuffer"
#define KW_MESH_VERTS		"vertices"
#define KW_MESH_NORMALS		"normals"

#define KW_MESH_IBUFFER		"IndexBuffer"
#define KW_MESH_INDICES		"indices"
#define KW_MESH_MATNRS		"materials"
#define KW_MESH_FLAGS		"flags"


#endif//_bAsciiKeywords_h_
