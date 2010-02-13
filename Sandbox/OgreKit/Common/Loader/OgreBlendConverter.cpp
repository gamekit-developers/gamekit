/*
-------------------------------------------------------------------------------
	This file is part of OgreKit.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
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
#include "OgreLoaderUtils.h"
#include "OgreBlenderDefines.h"
#include "OgreSkeleton.h"
#include "OgreSkeletonManager.h"
#include "OgreMesh.h"
#include "OgreMeshManager.h"
#include "OgreTexture.h"
#include "OgreTextureManager.h"
#include "OgreMeshLoader.h"
#include "OgreSkeletonLoader.h"
#include "OgreTextureLoader.h"
#include "OgreBlendConverter.h"
#include "OgreUtils.h"


using namespace Ogre;



BlendConverter::BlendConverter(bParse::bBlenderFile* file, const String& group) :
	m_file(file), m_group(group)
{
}



BlendConverter::~BlendConverter()
{
    ListIterator<ManualLoaderList> it(m_loaders);
    while (it.hasMoreElements())
        delete it.getNext();
}


Mesh *BlendConverter::createManualMesh(Blender::Object *ob)
{
	if (ob->type != OB_MESH) return 0;

	MeshLoader *loader = new MeshLoader(this, ob);
	MeshPtr me = MeshManager::getSingleton().create(OGRE_IDNAME(ob), m_group, true, loader);
	m_loaders.push_back(loader);

	return me.getPointer();
}


Skeleton *BlendConverter::createManualSkeleton(Blender::Object *ob)
{
	if (ob->type != OB_ARMATURE) return 0;

	SkeletonLoader *loader = new SkeletonLoader(this, ob);
	SkeletonPtr sk = SkeletonManager::getSingleton().create(OGRE_IDNAME(ob), m_group, true, loader);
	m_loaders.push_back(loader);

	return sk.getPointer();
}



void BlendConverter::registerImage(Blender::Image* ima)
{
	OGRE_ASSERT(ima);

	if (m_images.find(ima) == OGRE_NPOS)
	{
		/// is it external ?
		String base = Utils::findResourceBase(ima->name);

		if (!base.empty() && Utils::isResource(base))
		{
			/// use it over manual
			TextureManager::getSingleton().create(base, m_group);
			return;
		}

		TextureLoader *loader= new TextureLoader(this, ima);
		TexturePtr tex;
		try {
			tex= TextureManager::getSingleton().create(OGRE_IDNAME(ima), m_group, true, loader);
		}
		catch (...)
		{
			delete loader;
			return;
		}

		if (!tex.isNull())
			m_images.insert(ima, tex.getPointer());

		m_loaders.push_back(loader);
	}
}
