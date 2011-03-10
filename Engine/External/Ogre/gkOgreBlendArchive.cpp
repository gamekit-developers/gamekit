/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 harkon.kr

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

using namespace Ogre;

gkBlendArchive::gkBlendArchive(const String& name, const String& archType )
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

DataStreamPtr gkBlendArchive::open(const String& filename, bool readOnly) const
{
	//TODO: support other type, if need
	gkTextFile* tf = gkTextManager::getSingletonPtr() ? 
		(gkTextFile*)gkTextManager::getSingleton().getByName(filename) : 0;

	if (!tf) 
	{
		return DataStreamPtr();
	}

	const gkString& buf = tf->getText();
	const int type = tf->getType();

	Ogre::DataStreamPtr memStream(
		    OGRE_NEW Ogre::MemoryDataStream((void*)buf.c_str(), buf.size()));

	return memStream;
}

StringVectorPtr gkBlendArchive::list(bool recursive, bool dirs)
{
	// directory change requires locking due to saved returns
	return StringVectorPtr(new StringVector());
}

FileInfoListPtr gkBlendArchive::listFileInfo(bool recursive, bool dirs)
{
	return FileInfoListPtr(new FileInfoList());
}

StringVectorPtr gkBlendArchive::find(const String& pattern, bool recursive, bool dirs)
{
	return StringVectorPtr(new StringVector());
}

FileInfoListPtr gkBlendArchive::findFileInfo(const String& pattern, bool recursive, bool dirs) const
{
	return FileInfoListPtr(new FileInfoList());
}

bool gkBlendArchive::exists(const String& filename)
{
	return gkTextManager::getSingleton().exists(filename);
}

//--

const String gkBlendArchiveFactory::ARCHIVE_TYPE = "BLEND";

gkBlendArchiveFactory::~gkBlendArchiveFactory() 
{
}

Archive *gkBlendArchiveFactory::createInstance(const String& name ) 
{
	return new gkBlendArchive(name, ARCHIVE_TYPE);
}

void gkBlendArchiveFactory::destroyInstance(Archive* arch) 
{ 
	delete arch; 
}

void gkBlendArchiveFactory::addArchiveFactory(void)
{
	Ogre::ArchiveManager::getSingleton().addArchiveFactory(this);
}