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
#include "gkResourceGroupManager.h"
//#include "bBlenderFile.h"
#include "Blender.h"


gkBlendLoader::gkBlendLoader()
	:   m_activeFile(0)
{
}



gkBlendLoader::~gkBlendLoader()
{
	UTsize i;
	for (i = 0; i < m_files.size(); i++)
		delete m_files[i];
	m_files.clear();
	m_activeFile = 0;
}

bool gkBlendLoader::hasResourceGroup(const gkString& group, gkBlendFile* exceptFile)
{
	UTsize i;
	for (i = 0; i < m_files.size(); i++)
		if (m_files[i] != exceptFile && m_files[i]->getResourceGroup() == group)
			return true;

	return false;
}



void gkBlendLoader::unloadAll(bool exceptActiveFile)
{
	UTsize i;
	for (i = 0; i < m_files.size(); i++)
	{
		if (m_files[i] != m_activeFile)
		{
			gkString group = m_files[i]->getResourceGroup();
			delete m_files[i]; m_files[i] = 0;
			gkResourceGroupManager::getSingleton().destroyResourceGroup(group);
		}
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
			gkString group = m_activeFile->getResourceGroup();
			delete m_activeFile; m_activeFile = 0;
			gkResourceGroupManager::getSingleton().destroyResourceGroup(group);
		}
	}
}

void gkBlendLoader::unloadGroup(const gkString& group)
{
	FileList tmp;
	UTsize i;
	for (i = 0; i < m_files.size(); i++)
	{
		if (m_files[i]->getResourceGroup() == group)
		{
			gkString resgroup = m_files[i]->getResourceGroup();
			delete m_files[i]; m_files[i] = 0;
		}
		else
			tmp.push_back(m_files[i]);
	}

	gkResourceGroupManager::getSingleton().destroyResourceGroup(group);

	m_files = tmp;
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

void gkBlendLoader::unloadFile(gkBlendFile* blendFile)
{
	if (!blendFile) return;

	gkString group = blendFile->getResourceGroup();	

	m_files.erase(m_files.find(blendFile));
	if (m_activeFile == blendFile)
		m_activeFile = NULL;
	
	delete blendFile;

	if (!hasResourceGroup(group))
		gkResourceGroupManager::getSingleton().destroyResourceGroup(group);
}

gkBlendFile* gkBlendLoader::loadAndCatch(const gkString& fname, int options, const gkString& scene, const gkString& group)
{
	if ((options & LO_IGNORE_CACHE_FILE) != 0)
	{
		m_activeFile = getFileByName(fname);
		if (m_activeFile != 0)
			return m_activeFile;
	}

	bool useUniqueGroup = false;
	gkString groupName = group;
	if (groupName.empty() && (options & LO_CREATE_UNIQUE_GROUP) != 0) 
	{
		groupName = gkUtils::getUniqueName("BLEND");
		useUniqueGroup = true;
	}

	bool inGlolbalPool = (options & LO_CREATE_PRIVATE_GROUP) == 0;

	gkResourceGroupManager::getSingleton().createResourceGroup(groupName, inGlolbalPool);

	//bParse::bLog::detail = gkEngine::getSingleton().getUserDefs().verbose ? 1 : 0;

	m_activeFile = new gkBlendFile(fname, groupName);

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


gkBlendFile* gkBlendLoader::loadFile(const gkString& fname, const gkString& scene, const gkString& group)
{
	return loadFile(fname, LO_ALL_SCENES, scene, group);
}


gkBlendFile* gkBlendLoader::loadFile(const gkString& fname, int options, const gkString& scene, const gkString& group)
{
	bool resetLoad = false;
	try
	{
		return loadAndCatch(fname, options, scene, group);
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
