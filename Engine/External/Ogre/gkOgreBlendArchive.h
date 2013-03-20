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
#ifndef _gkOgreBlendArchive_h_
#define _gkOgreBlendArchive_h_

#include "utSingleton.h"
#include "OgreArchiveFactory.h"
#include "OgreArchiveManager.h"

class gkBlendArchive : public Ogre::Archive 
{
public:
	gkBlendArchive(const Ogre::String& name, const Ogre::String& archType);
	~gkBlendArchive();

	bool isCaseSensitive(void) const { return true; }

	void load();
	void unload();

	Ogre::DataStreamPtr open(const Ogre::String& filename, bool readOnly=true) const;

	Ogre::StringVectorPtr list(bool recursive = true, bool dirs = false);

	Ogre::FileInfoListPtr listFileInfo(bool recursive = true, bool dirs = false);

	Ogre::StringVectorPtr find(const Ogre::String& pattern, bool recursive = true, bool dirs = false);

	time_t  getModifiedTime (const Ogre::String &filename) { return 0; }
#if OGRE_VERSION >= 0x10800
	Ogre::FileInfoListPtr findFileInfo(const Ogre::String& pattern, bool recursive = true, bool dirs = false) const;
#else
	Ogre::FileInfoListPtr findFileInfo(const Ogre::String& pattern, bool recursive = true, bool dirs = false);
#endif

	bool exists(const Ogre::String& filename);
};


class gkBlendArchiveFactory : public Ogre::ArchiveFactory
{
public:
	static const Ogre::String ARCHIVE_TYPE;

	virtual ~gkBlendArchiveFactory();
	GK_INLINE const Ogre::String& getType(void) const { return ARCHIVE_TYPE; }
#if OGRE_VERSION_MAJOR==1 && OGRE_VERSION_MINOR>=9
	Ogre::Archive* createInstance(const Ogre::String& name, bool readOnly=true);
#else
	Ogre::Archive* createInstance(const Ogre::String& name);
#endif
	void destroyInstance(Ogre::Archive* arch);

	void addArchiveFactory(void);
};


#endif//_gkOgreBlendArchive_h_
