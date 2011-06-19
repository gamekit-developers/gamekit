/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

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

#ifndef AKBLOADER_H
#define AKBLOADER_H

#include "utString.h"

#include "akMathUtils.h"

#include "Blender.h"

#define AKB_IDNAME(x) ((x) && (x)->id.name[0] != '0' ? (x)->id.name + 2 : "")

class akDemoBase;
class akMesh;
class akSkeleton;

namespace Blender
{
struct Object;
struct Mesh;
struct bArmature;
}

class akBLoader
{
public:
	akBLoader(akDemoBase* demo);
	
	void loadFile(const utString& filename);
	
private:
	akDemoBase* m_demo;

	void convertCameraObject(Blender::Object *bobj);
	void convertMeshObject(Blender::Object *bobj);
	void convertObjectMesh(Blender::Object *bobj);
	void convertMesh(Blender::Mesh* bme);
	void convertSkeleton(Blender::bArmature* bskel);
	void convertMeshSkinning(akMesh* mesh, Blender::Object* bobj, akSkeleton* skel);
	
	void removeMeshTempData(akMesh *mesh);
};

#endif // AKBLOADER_H
