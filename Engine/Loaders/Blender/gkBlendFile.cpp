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
#include "OgreException.h"
#include "OgreTexture.h"
#include "OgreTextureManager.h"
#include "OgreMaterialManager.h"
#include "OgreParticleSystemManager.h"

#include "bBlenderFile.h"
#include "bMain.h"
#include "blender.h"


#include "gkBlendFile.h"
#include "gkBlendLoader.h"
#include "gkSceneManager.h"
#include "gkScene.h"
#include "gkGameObject.h"

#include "gkSceneLoader.h"
#include "gkTextureLoader.h"
#include "gkPath.h"
#include "gkUtils.h"
#include "gkLogger.h"

#include "gkTextManager.h"
#include "gkTextFile.h"
#include "gkEngine.h"
#include "gkUserDefs.h"
#include "gkLuaManager.h"

using namespace Ogre;



gkBlendFile::gkBlendFile(const gkString &file, const gkString &group)
	:       m_group(group),
	        m_file(new bParse::bBlenderFile(file.c_str())),
	        m_owner(true)
{
}



gkBlendFile::gkBlendFile(bParse::bBlenderFile *file, const gkString &group)
	:       m_group(group),
	        m_file(file),
	        m_owner(file != 0)
{
}



gkBlendFile::~gkBlendFile()
{
	if (!m_loaders.empty())
	{
		utListIterator<ManualResourceLoaderList> it(m_loaders);
		while (it.hasMoreElements())
			delete it.getNext();
	}

	if (m_owner)
	{
		delete m_file;
		m_file = 0;
	}
}


void gkBlendFile::_registerImage(Blender::Image *ima)
{
	GK_ASSERT(ima);

	if (m_imageLookup.find(ima) == GK_NPOS)
	{
		// is it external ?
		gkPath p(ima->name);
		const gkString &base = p.base();

		if (gkUtils::isResource(base))
		{
			// use it over manual
			TextureManager::getSingleton().create(base, m_group);
			return;
		}

		gkTextureLoader *loader = new gkTextureLoader(this, ima);
		TexturePtr tex;
		try
		{
			tex = TextureManager::getSingleton().create(GKB_IDNAME(ima), m_group, true, loader);
		}
		catch (...)
		{
			delete loader;
			return;
		}

		if (!tex.isNull())
		{
			m_images.push_back(tex.getPointer());
			m_imageLookup.insert(ima, tex.getPointer());
		}

		_registerLoader(loader);
	}
}


void *gkBlendFile::_findPtr(void *ptr)
{
	return m_file->findLibPointer(ptr);
}


bool gkBlendFile::_parseFile(void)
{

	m_file->parse(false);
	if (!m_file->ok())
	{
		gkPrintf("Blend file loading failed.");
		return false;
	}

	return true;
}

#define G_FILE_GAME_MAT			 (1 << 12)				/* deprecated */
#define G_FILE_SHOW_PHYSICS		 (1 << 14)				/* deprecated */


bool gkBlendFile::_parse(void)
{
	if (!_parseFile())
		return false;


	bParse::bMain *mp = m_file->getMain();

	// Real conversion happens when a registered scene is loaded.
	// For all objects in the scene, scene->load() will call a loadResource
	// method, which in turn will do the same for all objects,
	// Objects can then be reloaded if they ever need to be unloaded (Needs testing)

	gkSceneManager &sceneMgr = gkSceneManager::getSingleton();
	bParse::bListBasePtr *lbp = mp->getScene();
	gkUserDefs &defs = gkEngine::getSingleton().getUserDefs();

#ifdef EXTRACT_ALL_SCENES
	for (size_t i = 0; i < lbp->size(); i++)
	{
		Blender::Scene *sc = (Blender::Scene *)lbp->at(i);
#else
	{
		Blender::FileGlobal *glob = (Blender::FileGlobal *)m_file->getFileGlobal();
		Blender::Scene *sc = 0;
		if (glob)
		{
			if (glob->fileflags & G_FILE_GAME_MAT)
				defs.blendermat = true;
			if (glob->fileflags & G_FILE_SHOW_PHYSICS)
				defs.debugPhysics = true;

			sc = (Blender::Scene *)glob->curscene;
		}

		if (!sc)
			sc = (Blender::Scene *)lbp->at(0);


#endif

		if (sc != 0)
		{
			gkScene *newscene = sceneMgr.create(GKB_IDNAME(sc),
			                                    new gkSceneObjectLoader(this, sc));
			m_scenes.push_back(newscene);

			if (defs.userWindow)
			{
				defs.winsize.x = (gkScalar)sc->r.xplay;
				defs.winsize.y = (gkScalar)sc->r.yplay;
				defs.fullscreen = sc->r.fullscreen != 0;
			}
		}
	}

	buildAllTextures();

	buildTextFiles();

	buildAllSounds();

	return true;
}

gkScene *gkBlendFile::findScene(const gkString &name)
{
	gkSceneIterator it(m_scenes);
	while (it.hasMoreElements())
	{
		gkScene *ob = it.getNext();
		if (ob->getName() == name)
			return ob;
	}
	return 0;
}


// Create a list of all internal text blocks
void gkBlendFile::buildTextFiles(void)
{
	bParse::bMain *mp = m_file->getMain();
	bParse::bListBasePtr *text = mp->getText();


	gkTextManager &txtMgr = gkTextManager::getSingleton();

	for (int i=0; i<text->size(); ++i)
	{
		Blender::Text *txt = (Blender::Text *)text->at(i);
		Blender::TextLine *tl = (Blender::TextLine *)txt->lines.first;

		std::stringstream ss;
		while (tl)
		{
			tl->line[tl->len] = 0;

			ss << tl->line << '\n';
			tl = tl->next;
		}

		// could be optimized
		gkString str = ss.str();
		if (!str.empty() && !txtMgr.hasFile(GKB_IDNAME(txt)))
		{
			gkTextFile *tf = txtMgr.create(GKB_IDNAME(txt));
			tf->setText(str);

			if(tf->getName().find(".material") != gkString::npos)
			{
				DataStreamPtr memStream(
				    OGRE_NEW MemoryDataStream((void *)str.c_str(), str.size()));

				Ogre::MaterialManager::getSingleton().parseScript(memStream, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			}
			else if(tf->getName().find(".particle") != gkString::npos)
			{
				DataStreamPtr memStream(
				    OGRE_NEW MemoryDataStream((void *)str.c_str(), str.size()));

				Ogre::ParticleSystemManager::getSingleton().parseScript(memStream, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			}
			else if(tf->getName().find(".lua") != gkString::npos)
				gkLuaManager::getSingleton().create(GKB_IDNAME(txt), str);
		}
	}
}

void gkBlendFile::buildAllTextures()
{
	bParse::bMain *mp = m_file->getMain();

	bParse::bListBasePtr *imaList = mp->getImage();

	for (int i=0; i<imaList->size(); ++i)
	{
		Blender::Image *ima = (Blender::Image *)imaList->at(i);

		_registerImage(ima);
	}
}

void gkBlendFile::buildAllSounds()
{
	bParse::bMain *mp = m_file->getMain();

	bParse::bListBasePtr *soundList = mp->getSound();

	for (int i=0; i<soundList->size(); ++i)
	{
		Blender::bSound *sound = (Blender::bSound *)soundList->at(i);
	}
}
