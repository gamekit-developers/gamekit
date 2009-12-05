/*
-------------------------------------------------------------------------------
	This file is part of the Ogre GameKit port.
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
#include "OgreException.h"
#include "OgreTexture.h"
#include "OgreTextureManager.h"
#include "gkBlendFile.h"
#include "gkBlendLoader.h"
#include "gkSceneObjectManager.h"
#include "gkSceneObject.h"

#include "Internal/gkSceneLoader.h"
#include "Internal/gkTextureLoader.h"
#include "gkPath.h"
#include "gkUtils.h"
#include "gkLogger.h"

#include "bBlenderFile.h"
#include "bMain.h"
#include "blender.h"

using namespace Ogre;


//-----------------------------------------------------------------------------
gkBlendFile::gkBlendFile(const String& file, const String& group) :
		mGroup(group), mFile(new bParse::bBlenderFile(file.c_str()))
{
}


//-----------------------------------------------------------------------------
gkBlendFile::~gkBlendFile()
{
	///@todo need to let the parent owner know
	gkManualResourceLoaderListIterator it(mLoaders);
	while (it.hasMoreElements())
		delete it.getNext();

	gkManualObjectLoaderListIterator oit(mObjects);
	while (oit.hasMoreElements())
		delete oit.getNext();

	delete mFile;
	mFile= 0;
}


//-----------------------------------------------------------------------------
void gkBlendFile::_registerImage(Blender::Image* ima)
{
	GK_ASSERT(ima);

	if (mImageLookup.find(ima) == GK_NPOS)
	{
		/// is it external ?
		gkPath p(ima->name);
		const String& base= p.base();

		if (gkUtils::isResource(base))
		{
			/// use it over manual
			TextureManager::getSingleton().create(base, mGroup);
			return;
		}

		gkTextureLoader *loader= new gkTextureLoader(this, ima);
		TexturePtr tex;
		try
		{
			tex= TextureManager::getSingleton().create(GKB_IDNAME(ima), mGroup, true, loader);
		}
		catch (...)
		{
			delete loader;
			return;
		}

		if (!tex.isNull())
		{
			mImages.push_back(tex.getPointer());
			mImageLookup.insert(ima, tex.getPointer());
		}

		_registerLoader(loader);
	}
}

//-----------------------------------------------------------------------------
void* gkBlendFile::_findPtr(void* ptr)
{
	return mFile->findLibPointer(ptr);
}

//-----------------------------------------------------------------------------
bool gkBlendFile::_parse(void)
{
	mFile->parse(false);
	if (!mFile->ok())
	{
		gkPrintf("Blend file loading failed.");
		return false;
	}

	bParse::bMain *mp= mFile->getMain();

	/// Real conversion happens when a registered scene is loaded.
	/// For all objects in the scene, scene->load() will call a loadResource
	/// method, which in turn will do the same for all objects,
	/// Objects can then be reloaded if they ever need to be unloaded (Needs testing)

	gkSceneObjectManager &sceneMgr= gkSceneObjectManager::getSingleton();
	bParse::bListBasePtr* lbp= mp->getScene();

#ifdef EXTRACT_ALL_SCENES
	for (size_t i=0; i<lbp->size(); i++)
	{
		Blender::Scene *sc= (Blender::Scene*)lbp->at(i);
#else
	{
		Blender::FileGlobal *glob = (Blender::FileGlobal*)mFile->getFileGlobal();
		Blender::Scene *sc= 0;
		if (glob)
			sc = (Blender::Scene *)glob->curscene;

		if (!sc)
			sc = (Blender::Scene*)lbp->at(0);

#endif
		if (sc != 0)
		{
			gkSceneObjectLoader *loader= new gkSceneObjectLoader(this, sc);
			gkSceneObjectPtr newscene= sceneMgr.create(GKB_IDNAME(sc), mGroup, true, loader);

			mLoaders.push_back(loader);
			mScenes.push_back(newscene.getPointer());
		}
	}
	return true;
}


//-----------------------------------------------------------------------------
gkSceneObject* gkBlendFile::findScene(const String& name)
{
	gkSceneIterator it(mScenes);
	while (it.hasMoreElements())
	{
		gkSceneObject* ob= it.getNext();
		if (ob->getName() == name)
			return ob;
	}
	return 0;
}
