/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 harkon.kr

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
#include "gkCommon.h"
#include "gkTextManager.h"
#include "gkTextFile.h"
#include "gkLogger.h"
#include "External/Ogre/gkOgreBlendArchive.h"

//using namespace Ogre;

gkBlendArchive::gkBlendArchive(const Ogre::String& name, const Ogre::String& archType )
	: Archive(name, archType)
{
}

gkBlendArchive::~gkBlendArchive()
{
	unload();
}

void gkBlendArchive::load()
{
}

void gkBlendArchive::unload()
{
}

Ogre::DataStreamPtr gkBlendArchive::open(const Ogre::String& filename, bool readOnly) const
{
	//TODO: support other type, if need
	gkTextFile* tf = gkTextManager::getSingletonPtr() ? 
		(gkTextFile*)gkTextManager::getSingleton().getByName(filename) : 0;

	if (!tf) 
	{
		return Ogre::DataStreamPtr();
	}

	const gkString& buf = tf->getText();
	const int type = tf->getType();

	Ogre::DataStreamPtr memStream(
		    OGRE_NEW Ogre::MemoryDataStream((void*)buf.c_str(), buf.size()));

	return memStream;
}

Ogre::StringVectorPtr gkBlendArchive::list(bool recursive, bool dirs)
{
	// directory change requires locking due to saved returns
	return Ogre::StringVectorPtr(new Ogre::StringVector());
}

Ogre::FileInfoListPtr gkBlendArchive::listFileInfo(bool recursive, bool dirs)
{
	return Ogre::FileInfoListPtr(new Ogre::FileInfoList());
}

Ogre::StringVectorPtr gkBlendArchive::find(const Ogre::String& pattern, bool recursive, bool dirs)
{
	return Ogre::StringVectorPtr(new Ogre::StringVector());
}

#if OGRE_VERSION >= 0x10800
Ogre::FileInfoListPtr gkBlendArchive::findFileInfo(const Ogre::String& pattern, bool recursive, bool dirs) const
#else
Ogre::FileInfoListPtr gkBlendArchive::findFileInfo(const Ogre::String& pattern, bool recursive, bool dirs)
#endif
{
	return Ogre::FileInfoListPtr(new Ogre::FileInfoList());
}


bool gkBlendArchive::exists(const Ogre::String& filename)
{
	return gkTextManager::getSingleton().exists(filename);
}

//--

const Ogre::String gkBlendArchiveFactory::ARCHIVE_TYPE = "BLEND";

gkBlendArchiveFactory::~gkBlendArchiveFactory() 
{
}

#if OGRE_VERSION_MAJOR==1 && OGRE_VERSION_MINOR>=9
Ogre::Archive *gkBlendArchiveFactory::createInstance(const Ogre::String& name, bool readOnly )
#else
Ogre::Archive *gkBlendArchiveFactory::createInstance(const Ogre::String& name )
#endif
{
	return new gkBlendArchive(name, ARCHIVE_TYPE);
}

void gkBlendArchiveFactory::destroyInstance(Ogre::Archive* arch) 
{ 
	delete arch; 
}

void gkBlendArchiveFactory::addArchiveFactory(void)
{
	Ogre::ArchiveManager::getSingleton().addArchiveFactory(this);
}
