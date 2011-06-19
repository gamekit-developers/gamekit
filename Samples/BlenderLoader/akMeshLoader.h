/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): Xavier T.
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
#ifndef AKMESHLOADER_H
#define AKMESHLOADER_H

#include "akMesh.h"

#define AK_UV_MAX 8

namespace Blender
{
struct Object;
struct Mesh;
struct MDeformVert;
struct Material;
struct Image;
struct MCol;
struct MTex;
struct MVert;
struct MFace;
}

class gkBlendFile;

class akSubMeshPair;
class akDemoBase;

class akMeshLoader
{
public:

	akMeshLoader(akDemoBase* demo, akMesh* gmesh, Blender::Object* bobject, Blender::Mesh* bmesh);
	~akMeshLoader();

	void convert(bool sortByMat, bool openglVertexColor);

private:

	friend class akSubMeshPair;
	
	struct PackedFace
	{
		Blender::MVert*          verts;
		akScalar               uvLayers[8][4][2];
		int                     totlay;
		unsigned int*            colors;
		unsigned int*            index;
	};

	struct TempVert
	{
		akVector3       co;                 // vertex coordinates
		akVector3       no;                 // normals
		unsigned int    vcol;               // vertex color
		akScalar       uv[AK_UV_MAX][2];    // texture coordinates < GK_UV_MAX
	};
	
	struct TempFace
	{
		TempVert v0, v1, v2;
		unsigned int i0, i1, i2;
	};

	void addTriangle(akSubMeshPair* sub,
								unsigned int fi,
								const TempVert& v0,
								unsigned int i0,
								const TempVert& v1,
								unsigned int i1,
								const TempVert& v2,
								unsigned int i2);

	void convertIndexedTriangle( TempFace* dest,
	                             unsigned int v0,
	                             unsigned int v1,
	                             unsigned int v2,
	                             PackedFace& face
	                           );

	akVector3 calcNormal(akVector3& v0, akVector3& v1, akVector3& v2);
	void calcNormal(TempFace* tri);
	akVector3 calcMorphNormal(const Blender::MFace& bface, float *pos);

	void convertVertexGroups(akSubMeshPair* subpair);
	void convertMorphTargets(akSubMeshPair* subpair);
	
	void convertMaterial(Blender::Material* bma, akSubMeshPair* subpair);
	void convertTextureFace(akSubMeshPair* subpair);

	int findTextureLayer(Blender::MTex* te);


	unsigned int packColour(const Blender::MCol& col, bool opengl);

	akDemoBase* m_demo;
	akMesh*          m_gmesh;
	Blender::Mesh*   m_bmesh;
	Blender::Object* m_bobj;
};

#endif//AKMESHLOADER_H
