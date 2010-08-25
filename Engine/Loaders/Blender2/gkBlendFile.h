/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): Nestor Silveira.
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
#ifndef _gkBlendFile_h_
#define _gkBlendFile_h_

#include "gkLoaderCommon.h"
#include "gkObject.h"
#include "OgreResourceGroupManager.h"


// Loaded blend file
class gkBlendFile
{
public:

	// Manual loaders.
	typedef utArray<Ogre::ManualResourceLoader *> ManualResourceLoaderList;

	// Image lookup for sharing textures
	typedef utHashTable<utPointerHashKey, Ogre::Texture *> ImageTextureHashMap;


	// Complete list of all scenes contained in the file.
	typedef utArray<gkScene *> Scenes;

public:


	gkBlendFile(const gkString &blendToLoad, const gkString &group = GK_DEF_GROUP);
	~gkBlendFile();




	bool parse(int opts);


	// Scene access 

	gkScene *getSceneByName(const gkString &name);

	GK_INLINE gkScene *getMainScene(void) {return m_activeScene;}
	GK_INLINE Scenes  &getScenes(void)    {return m_scenes;}



	// Internal registration.
	void _registerLoader(Ogre::ManualResourceLoader *loader)  {m_loaders.push_back(loader);}


	// Internal bParse access.
	bParse::bBlenderFile *_getInternalFile(void) {GK_ASSERT(m_file); return m_file;}




    // Access to the original group name for placing created resources in the same group.
	GK_INLINE const gkString &getGroup(void) {return m_group;}


	// File path name.
	GK_INLINE const gkString &getName(void) {return m_name;}


protected:

	void doVersionTests(void);
	void buildTextFiles(void);
	void buildAllTextures();
	void buildAllSounds();


	void loadActive(void);
	void loadAll(void);

	ManualResourceLoaderList  m_loaders;     // Ogre Loaders
	const gkString            m_group;       // Ogre Ogre::Resource Group
	const gkString            m_name;        // Current file path.
 	bParse::bBlenderFile     *m_file;        // bParse File Pointer
	Scenes                    m_scenes;      // All Scenes
	gkScene                  *m_activeScene; // Main scene found during parse.
	ImageTextureHashMap       m_imageLookup;
};


#endif//_gkBlendFile_h_
