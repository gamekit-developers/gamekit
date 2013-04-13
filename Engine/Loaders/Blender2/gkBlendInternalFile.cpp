/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 harkon.kr.

    Contributor(s): Thomas Trocha(dertom)
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
#include "gkBlendInternalFile.h"
#include "gkLogger.h"
#include "utStreams.h"

#if OGREKIT_USE_BPARSE == 0
#include "fbtTypes.h"
#endif

gkBlendListIterator::gkBlendListIterator(List* list)
	:	m_list(list),
#if OGREKIT_USE_BPARSE
		m_index(0)
#else
		m_index(list ? list->first : 0)
#endif
{
}

bool gkBlendListIterator::hasMoreElements() const
{
	if (m_list == 0) return false;

#if OGREKIT_USE_BPARSE
	return m_index < m_list->size();
#else
	return m_index != 0;
#endif
}


gkBlendListIterator::ListItem* gkBlendListIterator::getNext(void)	 
{ 
#if OGREKIT_USE_BPARSE
	return m_list->at(m_index++);
#else
	ListItem* item = m_index;
	m_index = m_index->next;
	return item;
#endif
}

//--

gkBlendInternalFile::gkBlendInternalFile()
	:	m_file(0)
{
}

gkBlendInternalFile::~gkBlendInternalFile()
{
	delete m_file;
	m_file = 0;
}

bool gkBlendInternalFile::parse(const gkString& fname)
{
	if (fname.empty()) 
	{
		gkLogMessage("BlendFile: File " << fname << " loading failed. File name is empty.");
		return false;
	}

#if OGREKIT_USE_BPARSE

	utMemoryStream fs;
	fs.open(fname.c_str(), utStream::SM_READ);

	if (!fs.isOpen())
	{
		gkLogMessage("BlendFile: File " << fname << " loading failed. No such file.");
		return false;
	}

	// Write contents and inflate.
	utMemoryStream buffer(utStream::SM_WRITE);
	fs.inflate(buffer);

	m_file = new bParse::bBlenderFile((char*)buffer.ptr(), buffer.size());
	m_file->parse(false);

	if (!m_file->ok())
	{
		gkLogMessage("BlendFile: File " << fname << " loading failed. Data error.");
		return false;
	}

#else

	m_file = new fbtBlend();
	int status = m_file->parse(fname.c_str(), fbtFile::PM_COMPRESSED);
	if (status != fbtFile::FS_OK)
	{
		delete m_file;
		m_file = 0;
		gkLogMessage("BlendFile: File " << fname << " loading failed. code: " << status);
		return false;
	}

#endif

	return true;
}


bool gkBlendInternalFile::parse(const void* mem, int size)
{
#if OGREKIT_USE_BPARSE
	gkLogMessage("BlendFile: MemoryBlend not supported in bparse!");
	return false;
#else
	m_file = new fbtBlend();

	// first check to use uncompressed version
	int status = m_file->parse(mem,size, fbtFile::PM_UNCOMPRESSED,true);
#ifndef OGREKIT_DISABLE_ZIP
	// if this fails with invalid-headerstring try to uncompress the blend
	if (status == fbtFile::FS_INV_HEADER_STR) {
		status = m_file->parse(mem,size, fbtFile::PM_COMPRESSED);
	}
#endif

	if (status != fbtFile::FS_OK)
	{
		delete m_file;
		m_file = 0;
		gkLogMessage("BlendFile: MemoryBlend loading failed. code: " << status);
		return false;
	}

	return true;
#endif
}

Blender::FileGlobal* gkBlendInternalFile::getFileGlobal()
{
	GK_ASSERT(m_file);
	
#if OGREKIT_USE_BPARSE
	return (Blender::FileGlobal*)m_file->getFileGlobal();
#else
	return m_file->m_fg;
#endif
}

int gkBlendInternalFile::getVersion()
{
	GK_ASSERT(m_file);

#if OGREKIT_USE_BPARSE
	return m_file->getMain()->getVersion();
#else
	return m_file->getVersion();
#endif
}

Blender::Scene* gkBlendInternalFile::getFirstScene()
{
	GK_ASSERT(m_file);
	
	gkBlendListIterator iter = getSceneList();

	return iter.hasMoreElements() ? (Blender::Scene*)iter.getNext() : 0;
}


#if OGREKIT_USE_BPARSE

	#define IMPLEMENT_GET_ITER_LIST(FNAME, BNAME, FBTNAME) \
		gkBlendListIterator gkBlendInternalFile::FNAME() \
		{ \
			GK_ASSERT(m_file); \
			gkBlendListIterator iter(m_file->getMain()->BNAME()); \
			return iter; \
		}

#else

	#define IMPLEMENT_GET_ITER_LIST(FNAME, BNAME, FBTNAME) \
		gkBlendListIterator gkBlendInternalFile::FNAME() \
		{ \
			GK_ASSERT(m_file); \
			gkBlendListIterator iter(&m_file->FBTNAME); \
			return iter; \
		}

#endif


IMPLEMENT_GET_ITER_LIST(getSceneList,		getScene,		m_scene)
IMPLEMENT_GET_ITER_LIST(getTextList,		getText,		m_text)
IMPLEMENT_GET_ITER_LIST(getSoundList,		getSound,		m_sound)
IMPLEMENT_GET_ITER_LIST(getActionList,		getAction,		m_action)
IMPLEMENT_GET_ITER_LIST(getObjectList,		getObject,		m_object)
IMPLEMENT_GET_ITER_LIST(getParticleList,	getParticle,	m_particle)
IMPLEMENT_GET_ITER_LIST(getVFontList,		getVfont,		m_vfont)
IMPLEMENT_GET_ITER_LIST(getMeshList,		getMesh,		m_mesh)
IMPLEMENT_GET_ITER_LIST(getArmatureList,	getArmature,	m_armature)
IMPLEMENT_GET_ITER_LIST(getGroupList,		getGroup,		m_group)
IMPLEMENT_GET_ITER_LIST(getScriptList,		getScript,		m_script)
IMPLEMENT_GET_ITER_LIST(getCameraList,		getCamera,		m_camera)
IMPLEMENT_GET_ITER_LIST(getWorldList,		getWorld,		m_world)
IMPLEMENT_GET_ITER_LIST(getMatList,			getMat,			m_mat)
IMPLEMENT_GET_ITER_LIST(getImageList,		getImage,		m_image)
IMPLEMENT_GET_ITER_LIST(getLampList,		getLamp,		m_lamp)
IMPLEMENT_GET_ITER_LIST(getLattList,		getLatt,		m_latt)
IMPLEMENT_GET_ITER_LIST(getIpoList,			getIpo,			m_ipo)
IMPLEMENT_GET_ITER_LIST(getKeyList,			getKey,			m_key)
IMPLEMENT_GET_ITER_LIST(getCurveList,		getCurve,		m_curve)
IMPLEMENT_GET_ITER_LIST(getNodeTreeList,	getNodetree,	m_nodetree)
IMPLEMENT_GET_ITER_LIST(getLibraryList,		getLibrary,		m_library)
IMPLEMENT_GET_ITER_LIST(getMBallList,		getMball,		m_mball)





