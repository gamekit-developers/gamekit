/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): Thomas Trocha.
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

/*

 This class converts a mesh from blender-file version 2.63+
 Since 'bmesh' was added to trunk, the data-representation changed from faces to polys.
 This converter is called by gkBlenderMeshConverter's convert-method as soon as it
 realizes that there is no face-data.

 */

#ifndef _gkMeshConverter26_h_
#define _gkMeshConverter26_h_

#include "gkMeshConverter.h"
#include "gkSerialize.h"
#include "gkMesh.h"



class gkBlendFile;


class gkBlenderMeshConverter26
{
public:

	gkBlenderMeshConverter26(gkMesh* gmesh, Blender::Object* bobject, Blender::Mesh* bmesh);
	~gkBlenderMeshConverter26();

	bool convert(void);

private:

	typedef utArray<int>                                AssignmentIndexList;
	typedef utArray<gkDeformVertex>                     AssignmentList;
	typedef utHashTable<utIntHashKey, AssignmentList>   AssignmentListMap;

	void convertBoneAssignments(int dgi, AssignmentListMap& dest);
	void assignBoneAssignments(gkSubMesh* me, AssignmentListMap& dest);
	void findWeight(int i, gkSubMesh* me, AssignmentListMap& dest);


	struct PackedFace
	{
		Blender::MDeformVert*    dverts;
		Blender::MVert*          verts;
		gkVector2               uvLayers[8][4];
		int                     totlay;
		unsigned int*            colors;
		unsigned int*            index;
	};


	struct TempFace
	{
		gkVertex v0, v1, v2;
		unsigned int i0, i1, i2;
	};


	void convertIndexedTriangle( TempFace* dest,
	                             unsigned int v0,
	                             unsigned int v1,
	                             unsigned int v2,
	                             PackedFace& face
	                           );

	void calcNormal(TempFace* tri);

	void convertMaterial(Blender::Material* bma, gkMaterialProperties& gma, class gkMeshHashKey26& hk);
	void convertTextureFace(gkMaterialProperties& gma, class gkMeshHashKey26& hk, Blender::Image** imas);

	int findTextureLayer(Blender::MTex* te);


	unsigned int packColour(const Blender::MLoopCol& col, bool opengl);


	gkMesh*          m_gmesh;
	Blender::Mesh*   m_bmesh;
	Blender::Object* m_bobj;
};




#endif//_gkMeshConverter26_h_
