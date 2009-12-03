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
#ifndef _gkBlendLoader_h_
#define _gkBlendLoader_h_

#include "gkLoaderCommon.h"
#include "OgreSingleton.h"
#include "OgreResourceGroupManager.h"




// ----------------------------------------------------------------------------
// TODO: Make this an ogre resource loader / script parser
//	   scan resource diretories for the .blend, store the filename
//	   then call blendfile_resource->load() to extract info from the .blend
class gkBlendLoader : public Ogre::Singleton<gkBlendLoader>
{
public:
	gkBlendLoader();
	~gkBlendLoader();

	static gkBlendLoader& getSingleton(void);
	static gkBlendLoader* getSingletonPtr(void);

	gkBlendFile* loadFile(const Ogre::String& dblend, const Ogre::String& inResourceGroup= Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
private:

	gkBlendFileList mOpenFiles;
};




#endif//_gkBlendLoader_h_
