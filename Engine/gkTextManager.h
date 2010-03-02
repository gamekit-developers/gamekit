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
#ifndef _gkTextFileManager_h_
#define _gkTextFileManager_h_

#include "gkCommon.h"
#include "gkHashedString.h"
#include "gkTextFile.h"
#include "OgreSingleton.h"


// Internal text buffer manager
class gkTextManager : public Ogre::Singleton<gkTextManager>
{
public:

    typedef utHashTable<gkHashedString, gkTextFile*> TextFiles;

protected:
    TextFiles m_files;

public:
    gkTextManager();
    virtual ~gkTextManager();


    // Get internal file 
    gkTextFile* getFile(const gkString& name);


    // Create new text file 
    gkTextFile* create(const gkString& name, const gkTextFile::TextType &type = gkTextFile::TT_UNKNOWN);


    // Destroys named file
    void destroy(const gkString& name);

    // Destroys file pointer
    void destroy(gkTextFile *ob);

    // Destroys all internal files
    void destroyAll(void);

    // Test for file existance
    bool hasFile(const gkString& name);


    // Singleton access 
    static gkTextManager& getSingleton(void);
    static gkTextManager* getSingletonPtr(void);
};


#endif//_gkTextFileManager_h_
