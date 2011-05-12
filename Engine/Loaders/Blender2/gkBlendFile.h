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
#include "gkInstancedObject.h"
#include "OgreResourceGroupManager.h"

//class fbtBlend;
class gkBlendInternalFile;

class gkBlendFile
{
public:
	typedef utArray<Ogre::ManualResourceLoader*> ManualResourceLoaderList;

	// Image lookup for sharing textures
	typedef utHashTable<utPointerHashKey, Ogre::Texture*> ImageTextureHashMap;

	typedef utArray<gkScene*> Scenes;

public:
	gkBlendFile(const gkString& blendToLoad, const gkString& group);
	~gkBlendFile();

	bool parse(int opts, const gkString& scene = "");

	gkScene* getSceneByName(const gkString& name);

	GK_INLINE gkScene* getMainScene(void) {return m_activeScene;}
	GK_INLINE Scenes&  getScenes(void)    {return m_scenes;}


	///Internal manual loader registration (used mainly for Blender image to Ogre Texture ).
	void _registerLoader(Ogre::ManualResourceLoader* loader)  {m_loaders.push_back(loader);}

	gkBlendInternalFile* _getInternalFile(void) {GK_ASSERT(m_file); return m_file;}

	///Access to the original group name. Used for placing created resources in the same group.
	GK_INLINE const gkString& getResourceGroup(void) {return m_group;}

	GK_INLINE const gkString& getFilePath(void)  {return m_name;}

protected:

	void doVersionTests(void);
	void buildTextFiles(void);
	void buildAllTextures(void);
	void buildAllSounds(void);
	void buildAllFonts(void);
	void buildAllActions(void);
	void buildAllParticles(void);


	void loadActive(void);
	void createInstances(void);

	void readCurSceneInfo(Blender::Scene* scene);

	ManualResourceLoaderList	m_loaders;			// Ogre Loaders
	const gkString				m_name;				// Current file path.
	const gkString				m_group;			// resource group
	float						m_animFps;				
	gkBlendInternalFile*		m_file;
	Scenes						m_scenes;			// All Scenes
	gkScene*					m_activeScene;		// Main scene found during parse.
	ImageTextureHashMap			m_imageLookup;
	gkString					m_findScene;
	bool						m_hasBFont;
};


#endif//_gkBlendFile_h_
