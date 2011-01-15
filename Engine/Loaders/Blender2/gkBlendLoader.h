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
#ifndef _gkBlendLoader_h_
#define _gkBlendLoader_h_

#include "gkLoaderCommon.h"
#include "utSingleton.h"


class gkBlendLoader : public utSingleton<gkBlendLoader>
{
public:
	typedef utArray<gkBlendFile*> FileList;


	enum LoadOptions
	{
		LO_ONLY_ACTIVE_SCENE		= 1 << 0,	// Load only the active scene found.
		LO_ALL_SCENES				= 1 << 1,	// Load all scenes.
		LO_IGNORE_CACHE_FILE		= 1 << 2,	// Load the blend file even if loaded.
		LO_CREATE_UNIQUE_GROUP		= 1 << 3,	// Create unique resource group.
		LO_CREATE_PRIVATE_GROUP		= 1 << 4	// Create private resource group, so invisible in the global pool.
	};


public:
	gkBlendLoader();
	~gkBlendLoader();

	gkBlendFile* loadFile(  const gkString& fname,
	                        int options = LO_ONLY_ACTIVE_SCENE,							
	                        const gkString& scene = "",
							const gkString& group = ""
	                     );

	gkBlendFile* loadFile(  const gkString& fname,
	                        const gkString& scene,
							const gkString& group = ""
	                     );


	gkBlendFile* getFileByName(const gkString& fname);


	GK_INLINE FileList&      getFiles(void)          {return m_files;}
	GK_INLINE gkBlendFile*   getActiveBlend(void)    {return m_activeFile;}

	void unloadAll(bool exceptActiveFile=false);
	void unloadGroup(const gkString& group);

	GK_INLINE void unloadFile(const gkString& fname) { unloadFile(getFileByName(fname)); }
	void unloadFile(gkBlendFile* blendFile);

	UT_DECLARE_SINGLETON(gkBlendLoader);

private:

	gkBlendFile* loadAndCatch(const gkString& fname,
	                          int options = LO_ONLY_ACTIVE_SCENE,
	                          const gkString& scene = "",
							  const gkString& group = ""
	                         );

	bool			hasResourceGroup(const gkString& group, gkBlendFile* exceptFile = NULL);

	gkBlendFile*    m_activeFile;
	FileList        m_files;
};




#endif//_gkBlendLoader_h_
