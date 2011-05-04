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
#include "gkCommon.h"

#include "OgreException.h"
#include "OgreTexture.h"
#include "OgreTextureManager.h"

#include "gkBlendInternalFile.h"
#include "gkBlendFile.h"
#include "gkBlendLoader.h"
#include "gkSceneManager.h"
#include "gkScene.h"
#include "gkGameObject.h"

#include "Converters/gkAnimationConverter.h"
#include "Converters/gkParticleConverter.h"

#include "gkBlenderDefines.h"
#include "gkBlenderSceneConverter.h"
#include "gkTextureLoader.h"
#include "gkPath.h"
#include "gkLogger.h"

#include "gkTextManager.h"
#include "gkTextFile.h"
#include "gkEngine.h"
#include "gkUserDefs.h"

#ifdef OGREKIT_OPENAL_SOUND
# include "Sound/gkSoundManager.h"
# include "Sound/gkSound.h"
#endif

#ifdef OGREKIT_COMPILE_OGRE_SCRIPTS
#include "gkFontManager.h"
#include "gkFont.h"
#endif

//using namespace Ogre;



gkBlendFile::gkBlendFile(const gkString& blendToLoad, const gkString& group)
	:	m_name(blendToLoad),
		m_group(group),
		m_animFps(24),
		m_activeScene(0),
		m_findScene(""),
		m_hasBFont(false),
		m_file(0)
{
}



gkBlendFile::~gkBlendFile()
{
	if (!m_loaders.empty())
	{
		ManualResourceLoaderList::Iterator it = m_loaders.iterator();
		while (it.hasMoreElements())
			delete it.getNext();
	}
}



bool gkBlendFile::parse(int opts, const gkString& scene)
{	
	m_file = new gkBlendInternalFile();
	if (!m_file->parse(m_name))
	{
		delete m_file;
		m_file = 0;
		return false;
	}

	doVersionTests();

	m_findScene = scene;

	if (opts & gkBlendLoader::LO_ONLY_ACTIVE_SCENE)
		loadActive();
	else
		createInstances();

	delete m_file;
	m_file = 0;
	return true;
}

void gkBlendFile::readCurSceneInfo(Blender::Scene* scene)
{
	if (!scene) return;

	m_animFps = scene->r.frs_sec / scene->r.frs_sec_base;

	gkUserDefs& defs = gkEngine::getSingleton().getUserDefs();
	defs.animFps = m_animFps;
	defs.rtss = (scene->gm.matmode == GAME_MAT_GLSL);
}


void gkBlendFile::loadActive(void)
{
	// Load / convert only the active scene.

	Blender::FileGlobal* fg = m_file->getFileGlobal();
	if (fg)
	{		
		if (!fg->curscene)
			fg->curscene = m_file->getFirstScene();

		Blender::Scene* sc = fg->curscene;

		readCurSceneInfo(sc);

		buildAllTextures();
		buildAllFonts();
		buildTextFiles();
		buildAllSounds();
		buildAllActions();
		buildAllParticles();

		// parse & build
		if (sc)
		{
			gkBlenderSceneConverter conv(this, sc);
			conv.convert();

			m_activeScene = (gkScene*)gkSceneManager::getSingleton().getByName(gkResourceName(GKB_IDNAME(sc), m_group));
			if (m_activeScene)
				m_scenes.push_back(m_activeScene);
		}
	}

}



void gkBlendFile::createInstances(void)
{
	GK_ASSERT(m_file);

	Blender::FileGlobal* fg = m_file->getFileGlobal();

	Blender::Scene* curscene = fg ? fg->curscene : 0;

	readCurSceneInfo(curscene);

	// Load / convert all
	buildAllTextures();
	buildAllFonts();
	buildTextFiles();
	buildAllSounds();
	buildAllActions();
	buildAllParticles();

	gkBlendListIterator iter = m_file->getSceneList();
	while (iter.hasMoreElements())
	{
		Blender::Scene* sc = (Blender::Scene*)iter.getNext();

		if (m_findScene.empty() || m_findScene == GKB_IDNAME(sc))
		{			
			gkBlenderSceneConverter conv(this, sc);
			conv.convert();

			gkScene* gks = (gkScene*)gkSceneManager::getSingleton().getByName(gkResourceName(GKB_IDNAME(sc), m_group));
			if (gks)
				m_scenes.push_back(gks);
		}
	}


	if (curscene)
	{
		// Grab the main scene
		m_activeScene = (gkScene*) gkSceneManager::getSingleton().getByName(gkResourceName(GKB_IDNAME(curscene), m_group));
	}

	if (m_activeScene == 0 && !m_scenes.empty())
		m_activeScene = m_scenes.front();
}





gkScene* gkBlendFile::getSceneByName(const gkString& name)
{

	Scenes::Iterator it = m_scenes.iterator();
	while (it.hasMoreElements())
	{
		gkScene* ob = it.getNext();
		if (ob->getName() == name)
			return ob;
	}
	return 0;
}


void gkBlendFile::buildTextFiles(void)
{
	// Create a list of all internal text blocks

	gkTextManager& txtMgr = gkTextManager::getSingleton();

	gkBlendListIterator iter = m_file->getTextList();
	while (iter.hasMoreElements())
	{
		Blender::Text* txt = (Blender::Text*)iter.getNext();
	
		Blender::TextLine* tl = (Blender::TextLine*)txt->lines.first;

		std::stringstream ss;
		while (tl)
		{
			tl->line[tl->len] = 0;

			ss << tl->line << '\n';
			tl = tl->next;
		}


		gkString str = ss.str();

		gkResourceName txtName(GKB_IDNAME(txt), m_group);

		if (!str.empty() && !txtMgr.exists(txtName))
		{
			gkTextFile* tf = (gkTextFile*)txtMgr.create(txtName);
			tf->setText(str);

			if (!m_hasBFont)
				m_hasBFont = tf->getType() == gkTextManager::TT_BFONT;
		}
	}

	txtMgr.parseScripts(m_group);
}



void gkBlendFile::buildAllTextures(void)
{
	gkBlendListIterator iter = m_file->getImageList();
	while (iter.hasMoreElements())	
	{
		Blender::Image* ima = (Blender::Image*)iter.getNext();
		// don't try & convert zero users
		if (ima->id.us <= 0)
			continue;

		gkString name(GKB_IDNAME(ima));

		Ogre::TexturePtr tex = Ogre::TextureManager::getSingleton().getByName(name, m_group);
		if (tex.isNull())
		{
			gkTextureLoader* loader = new gkTextureLoader(ima);
			tex = Ogre::TextureManager::getSingleton().create(GKB_IDNAME(ima), m_group, true, loader);

			if (!tex.isNull())
				m_loaders.push_back(loader);
			else
				delete loader;
		}
	}
}


void gkBlendFile::buildAllParticles(void)
{
#ifdef OGREKIT_USE_PARTICLE
	gkParticleConverter conv(m_group, m_animFps);

	gkBlendListIterator iter = m_file->getParticleList();
	while (iter.hasMoreElements())	
	{
		Blender::ParticleSettings* ps = (Blender::ParticleSettings*)iter.getNext();

		conv.convertParticle(ps);
	}
#endif
}

void gkBlendFile::buildAllSounds(void)
{
#ifdef OGREKIT_OPENAL_SOUND
	gkSoundManager* mgr = gkSoundManager::getSingletonPtr();

	if (!mgr->isValidContext())
		return;
	
	gkBlendListIterator iter = m_file->getSoundList();
	while (iter.hasMoreElements())	
	{
		Blender::bSound* sound = (Blender::bSound*)iter.getNext();

		// skip zero users
		if (sound->id.us <= 0)
			continue;

		gkPath pth(sound->name);
		bool isFile = pth.isFile();

		if (sound->packedfile || sound->newpackedfile || isFile)
		{
			Blender::PackedFile* pak = sound->packedfile ? sound->packedfile : sound->newpackedfile;
			if (((pak && pak->data) || isFile) && !mgr->exists(gkResourceName(GKB_IDNAME(sound), m_group)))
			{
				gkSound* sndObj = mgr->create<gkSound>(gkResourceName(GKB_IDNAME(sound), m_group));
				if (!sndObj)
					continue;

				if (isFile)
				{
					// Attempt to stream from file
					if (!sndObj->load(pth.getPath().c_str()))
					{
						mgr->destroy(sndObj);
						continue;
					}

					gkLogMessage("Sound: Loaded file " << pth.getPath() << " as" << GKB_IDNAME(sound) << ".");
				}
				else if (pak && pak->data)
				{
					// load from buffer
					if (!sndObj->load(pak->data, pak->size))
					{
						mgr->destroy(sndObj);
						continue;
					}

					gkLogMessage("Sound: Loaded buffer " << GKB_IDNAME(sound) << ".");
				}
				else
					GK_ASSERT(0);
			}
		}
	}

#endif
}

void gkBlendFile::buildAllFonts(void)
{
#ifdef OGREKIT_COMPILE_OGRE_SCRIPTS
	if (!m_hasBFont)
		return;
	
	gkFontManager& fmgr = gkFontManager::getSingleton();

	gkBlendListIterator iter = m_file->getVFontList();
	while (iter.hasMoreElements())	
	{
		Blender::VFont* vf = (Blender::VFont*)iter.getNext();
	
		if (vf->id.us <= 0 || !vf->packedfile)
			continue;

		Blender::PackedFile* pak = vf->packedfile;

		gkFont* fnt = (gkFont*)fmgr.create(gkResourceName(GKB_IDNAME(vf), m_group));
		fnt->setData(pak->data, pak->size);
	}
#endif
}


void gkBlendFile::buildAllActions(void)
{
	gkAnimationLoader anims(m_group);
		
    gkBlendListIterator iter = m_file->getActionList();
	anims.convertActions(iter, m_file->getVersion() <= 249, m_animFps);
}



void gkBlendFile::doVersionTests(void)
{	
	int version = m_file->getVersion();

	
	Blender::FileGlobal* fg = m_file->getFileGlobal() ;

	if (version <= 242 && fg && fg->curscene == 0)
	{
		fg->curscene = m_file->getFirstScene();
	}

	if (version <= 249)
	{
		gkBlendListIterator iter = m_file->getObjectList();
		while (iter.hasMoreElements())	
		{
			Blender::Object* ob = (Blender::Object*)iter.getNext();		

			if (ob->gameflag & OB_DYNAMIC)
				ob->body_type = ob->gameflag & OB_RIGID_BODY ? OB_BODY_TYPE_RIGID : OB_BODY_TYPE_DYNAMIC;
			else if (ob->gameflag & OB_RIGID_BODY)
				ob->body_type = OB_BODY_TYPE_RIGID;
			else
				ob->body_type = OB_BODY_TYPE_STATIC;
		}
	}

	if (version <= 250)
	{
		gkBlendListIterator iter = m_file->getObjectList();
		while (iter.hasMoreElements())	
		{
			Blender::Object* ob = (Blender::Object*)iter.getNext();	
		
			for (Blender::bConstraint* bc = (Blender::bConstraint*)ob->constraints.first; bc; bc = bc->next)
			{
				// convert rotation types to radians
				if (bc->type == CONSTRAINT_TYPE_ROTLIMIT)
				{
					Blender::bRotLimitConstraint* lr = (Blender::bRotLimitConstraint*)bc->data;
					lr->xmax *= gkRPD;
					lr->xmin *= gkRPD;
					lr->ymax *= gkRPD;
					lr->ymin *= gkRPD;
					lr->zmax *= gkRPD;
					lr->zmin *= gkRPD;
				}
			}

			ob = (Blender::Object*)ob->id.next;
		}
	}

	// BFont test
	{
		m_hasBFont = false;
	
		gkBlendListIterator iter = m_file->getTextList();
		while (iter.hasMoreElements())
		{
			Blender::Text* txt = (Blender::Text*)iter.getNext();
					
			if (gkString(txt->id.name).find(".bfont"))
			{
				m_hasBFont = true;
				break;
			}
		}
	}
}
