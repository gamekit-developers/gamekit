/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): none yet.
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
#include "gkBlendLoader.h"
#include "gkBlendFile.h"
#include "gkLogger.h"
#include "gkEngine.h"
#include "gkUserDefs.h"
#include "gkUtils.h"
#include "bBlenderFile.h"
#include "Blender.h"



gkBlendLoader::gkBlendLoader()
	:    m_activeFile(0)
{
}



gkBlendLoader::~gkBlendLoader()
{
	unloadAll(false);
}

void gkBlendLoader::clearResourceGroup(const gkString& inResourceGroup)
{
	if (Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(inResourceGroup))
		Ogre::ResourceGroupManager::getSingleton().clearResourceGroup(inResourceGroup);
}

void gkBlendLoader::unloadAll(bool exceptActiveFile)
{
	FileList::Iterator it = m_files.iterator();
	while (it.hasMoreElements())
	{
		FileList::ValueType file = it.getNext();
		if (m_activeFile != file)			
			delete file;
	}

	m_files.clear();
	
	if (m_activeFile)
	{
		if (exceptActiveFile)
		{	
			m_files.push_back(m_activeFile);	
		}
		else
		{
			delete m_activeFile; m_activeFile = NULL;		
		}
	}
}

gkBlendFile* gkBlendLoader::getFileByName(const gkString& fname)
{
	FileList::Iterator it = m_files.iterator();
	while (it.hasMoreElements())
	{
		gkBlendFile* fp = it.getNext();

		if (fp->getFilePath() == fname)
			return fp;
	}

	return 0;
}



gkBlendFile* gkBlendLoader::loadAndCatch(const gkString& fname, int options, const gkString& inResourceGroup, const gkString& scene)
{
	m_activeFile = getFileByName(fname);
	if (m_activeFile != 0)
		return m_activeFile;

	bParse::bLog::detail = gkEngine::getSingleton().getUserDefs().verbose ? 1 : 0;

	m_activeFile = new gkBlendFile(fname, inResourceGroup);

	if (m_activeFile->parse(options, scene))
	{
		m_files.push_back(m_activeFile);
		return m_activeFile;
	}
	else
	{
		delete m_activeFile;
		m_activeFile = m_files.empty() ? 0 : m_files.back();
	}

	return 0;
}


gkBlendFile* gkBlendLoader::loadFile(const gkString& fname, const gkString& scene, const gkString& inResourceGroup)
{
	return loadFile(fname, LO_ALL_SCENES, inResourceGroup, scene);
}


gkBlendFile* gkBlendLoader::loadFile(const gkString& fname, int options, const gkString& inResourceGroup, const gkString& scene)
{
	bool resetLoad = false;
	try
	{
		if (!Ogre::ResourceGroupManager::getSingleton().resourceGroupExists(inResourceGroup))
			Ogre::ResourceGroupManager::getSingleton().createResourceGroup(inResourceGroup);

		return loadAndCatch(fname, options, inResourceGroup);
	}
	catch (Ogre::Exception& e)
	{
		gkLogMessage("BlendLoader: Ogre exception: " << e.getDescription());
		resetLoad = true;
	}
	catch (...)
	{
		gkLogMessage("BlendLoader: Unknown exception");
		resetLoad = true;
	}

	if (resetLoad)
	{
		if (m_activeFile)
		{
			delete m_activeFile;
			m_activeFile = m_files.empty() ? 0 : m_files.back();
		}
	}

	return 0;
}

UT_IMPLEMENT_SINGLETON(gkBlendLoader);
