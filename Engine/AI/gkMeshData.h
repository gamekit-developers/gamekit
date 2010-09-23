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
#ifndef _gkMeshData_h_
#define _gkMeshData_h_

#include "gkCommon.h"
#include "gkMathUtils.h"
#include "Thread/gkPtrRef.h"

struct gkMeshData : public gkReferences, gkNonCopyable
{
	typedef std::vector<gkVector3> VERTS;
	VERTS verts;

	typedef std::vector<int> TRIANGLES;
	TRIANGLES tris;

	typedef std::vector<gkVector3> NORMALS;

	NORMALS normals;

	GK_INLINE const float* getVerts() const { return &verts.at(0).x; }
	GK_INLINE const float* getNormals() const { return &normals.at(0).x; }
	GK_INLINE const int* getTris() const { return &tris.at(0); }
	GK_INLINE int getVertCount() const { return verts.size(); }
	GK_INLINE int getTriCount() const { return tris.size() / 3; }
	GK_INLINE void copy(const gkMeshData& obj) {verts = obj.verts; tris = obj.tris; normals = obj.normals; }
};

typedef gkPtrRef<gkMeshData> PMESHDATA;

#endif//_gkMeshData_h_
