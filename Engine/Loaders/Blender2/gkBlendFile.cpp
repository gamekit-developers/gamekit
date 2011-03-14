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

//#include "bBlenderFile.h"
//#include "bMain.h"
#include "fbtBlend.h"
#include "Blender.h"

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
#include "gkUtils.h"
#include "gkLogger.h"

#include "gkTextManager.h"
#include "gkTextFile.h"
#include "gkEngine.h"
#include "gkUserDefs.h"

#include "gkCommon.h"

#ifdef OGREKIT_OPENAL_SOUND
# include "Sound/gkSoundManager.h"
# include "Sound/gkSound.h"
#endif

#ifdef OGREKIT_COMPILE_OGRE_SCRIPTS
#include "gkFontManager.h"
#include "gkFont.h"
#endif

using namespace Ogre;



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
	m_file = new fbtBlend();
	int status = m_file->parse(m_name.c_str(), fbtFile::PM_COMPRESSED);
	if (status != fbtFile::FS_OK)
	{
		delete m_file;
		m_file = 0;
		gkLogMessage("BlendFile: File " << m_name << " loading failed. code: " << status);
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




void gkBlendFile::loadActive(void)
{
	// Load / convert only the active scene.

	Blender::FileGlobal* fg = m_file->m_fg;
	if (fg)
	{		
		if (!fg->curscene)
			fg->curscene = (Blender::Scene*)m_file->m_scene.first;

		if (fg->curscene)
			m_animFps = fg->curscene->r.frs_sec / fg->curscene->r.frs_sec_base;

		gkEngine::getSingleton().getUserDefs().animFps = m_animFps;


		buildAllTextures();
		buildAllFonts();
		buildTextFiles();
		buildAllSounds();
		buildAllActions();
		buildAllParticles();

		// parse & build
		Blender::Scene* sc = (Blender::Scene*)fg->curscene;
		if (sc != 0)
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

	Blender::FileGlobal* fg = m_file->m_fg;
	if (fg && fg->curscene)
		gkEngine::getSingleton().getUserDefs().animFps = fg->curscene->r.frs_sec / fg->curscene->r.frs_sec_base;

	m_animFps = gkEngine::getSingleton().getUserDefs().animFps;

	// Load / convert all
	buildAllTextures();
	buildAllFonts();
	buildTextFiles();
	buildAllSounds();
	buildAllActions();
	buildAllParticles();

	Blender::Scene* sc = (Blender::Scene*)m_file->m_scene.first;

	while (sc)
	{

		if (m_findScene.empty() || m_findScene == GKB_IDNAME(sc))
		{			
			gkBlenderSceneConverter conv(this, sc);
			conv.convert();

			gkScene* gks = (gkScene*)gkSceneManager::getSingleton().getByName(gkResourceName(GKB_IDNAME(sc), m_group));
			if (gks)
				m_scenes.push_back(gks);
		}

		sc = (Blender::Scene*)sc->id.next;
	}


	if (fg && fg->curscene)
	{
		// Grab the main scene
		m_activeScene = (gkScene*) gkSceneManager::getSingleton().getByName(gkResourceName(GKB_IDNAME(fg->curscene), m_group));
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

	Blender::Text* txt = (Blender::Text*)m_file->m_text.first;

	while (txt)
	{
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

		txt = (Blender::Text*)txt->id.next;
	}

	txtMgr.parseScripts(m_group);
}



void gkBlendFile::buildAllTextures(void)
{
	for (Blender::Image* ima = (Blender::Image*)m_file->m_image.first; ima != 0; ima = (Blender::Image*)ima->id.next)
	{
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
	gkParticleConverter conv(m_group, m_animFps);

	Blender::ParticleSettings* ps = (Blender::ParticleSettings*)m_file->m_particle.first;
	
	while (ps)
	{	
		conv.convertParticle(ps);

		ps = (Blender::ParticleSettings*)ps->id.next;
	}
}

void gkBlendFile::buildAllSounds(void)
{
#ifdef OGREKIT_OPENAL_SOUND
	gkSoundManager* mgr = gkSoundManager::getSingletonPtr();

	if (!mgr->isValidContext())
		return;
	
	for (Blender::bSound* sound = (Blender::bSound*)m_file->m_sound.first; sound != 0; sound = (Blender::bSound*)sound->id.next)
	{		
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

	for (Blender::VFont* vf = (Blender::VFont*)m_file->m_vfont.first; vf != 0; vf = (Blender::VFont*)vf->id.next)
	{
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
		
	anims.convertActions(&m_file->m_action, m_file->getVersion() <= 249, m_animFps);
}



void gkBlendFile::doVersionTests(void)
{	
	int version = m_file->getVersion();

	
	Blender::FileGlobal* fg = m_file->m_fg ;

	if (version <= 242 && fg && fg->curscene == 0)
	{
		fg->curscene = (Blender::Scene*)m_file->m_scene.first;
	}

	if (version <= 249)
	{
		Blender::Object* ob = (Blender::Object*)m_file->m_object.first;
		while (ob)
		{

			if (ob->gameflag & OB_DYNAMIC)
				ob->body_type = ob->gameflag & OB_RIGID_BODY ? OB_BODY_TYPE_RIGID : OB_BODY_TYPE_DYNAMIC;
			else if (ob->gameflag & OB_RIGID_BODY)
				ob->body_type = OB_BODY_TYPE_RIGID;
			else
				ob->body_type = OB_BODY_TYPE_STATIC;

			ob = (Blender::Object*)ob->id.next;
		}
	}

	if (version <= 250)
	{
		Blender::Object* ob = (Blender::Object*)m_file->m_object.first;
		while (ob)
		{

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
	
		Blender::Text* ob = (Blender::Text*)m_file->m_text.first;

		while (ob)
		{			
			if (gkString(ob->id.name).find(".bfont"))
			{
				m_hasBFont = true;
				break;
			}

			ob = (Blender::Text*)ob->id.next;
		}
	}
}
