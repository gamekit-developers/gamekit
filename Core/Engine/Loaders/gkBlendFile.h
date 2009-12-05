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
#ifndef _gkBlendFile_h_
#define _gkBlendFile_h_

#include "gkLoaderCommon.h"
#include "OgreResourceGroupManager.h"




//-----------------------------------------------------------------------------
class gkBlendFile
{
public:
	gkBlendFile(const Ogre::String& file, const Ogre::String& group= Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	~gkBlendFile();
	bool _parse(void);
	void _registerLoader(Ogre::ManualResourceLoader *);
	void _registerLoader(gkManualObjectLoader *);
	void _registerImage(Blender::Image* ima);
	void* _findPtr(void* ptr);

	gkSceneObject*	findScene(const Ogre::String& name);
	gkSceneIterator   getSceneIterator(void);
	gkImageIterator   getImageIterator(void);


	const Ogre::String& getGroup(void);
	bParse::bBlenderFile* getInternalFile(void);

protected:

	gkManualResourceLoaderList	mLoaders;   // Ogre Loaders
	gkManualObjectLoaderList	  mObjects;   // Game Object Loaders
	const Ogre::String				mGroup;	 // Ogre Ogre::Resource Group
	bParse::bBlenderFile*	   mFile;	  // bParse File Pointer
	gkSceneList				   mScenes;	// All Scenes
	gkImageList				   mImages;	// All Textures
	gkImageTextureHashMap		 mImageLookup;
};


//-----------------------------------------------------------------------------
GK_INLINE gkSceneIterator gkBlendFile::getSceneIterator(void)
{
	return gkSceneIterator(mScenes);
}

//-----------------------------------------------------------------------------
GK_INLINE gkImageIterator gkBlendFile::getImageIterator(void)
{
	return gkImageIterator(mImages);
}


//-----------------------------------------------------------------------------
GK_INLINE const Ogre::String& gkBlendFile::getGroup(void)
{
	return mGroup;
}

//-----------------------------------------------------------------------------
GK_INLINE void gkBlendFile::_registerLoader(Ogre::ManualResourceLoader *loader)
{
	mLoaders.push_back(loader);
}

//-----------------------------------------------------------------------------
GK_INLINE void gkBlendFile::_registerLoader(gkManualObjectLoader *loader)
{
	mObjects.push_back(loader);
}

//-----------------------------------------------------------------------------
GK_INLINE bParse::bBlenderFile* gkBlendFile::getInternalFile(void)
{
	GK_ASSERT(mFile);
	return mFile;
}




#endif//_gkBlendFile_h_
