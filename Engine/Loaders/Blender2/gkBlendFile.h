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
#ifndef _gkBlendFile_h_
#define _gkBlendFile_h_

#include "gkLoaderCommon.h"
#include "gkObject.h"
#include "OgreResourceGroupManager.h"

typedef utList<gkScene*>                gkSceneList;
typedef utListIterator<gkSceneList>     gkSceneIterator;
typedef utList<Ogre::Texture*>          gkImageList;
typedef utListIterator<gkImageList>     gkImageIterator;


// Loaded blend file
class gkBlendFile : public utListClass<gkBlendFile>::Link
{
public:
    typedef utList<Ogre::ManualResourceLoader*> ManualResourceLoaderList;


    // Image lookup for sharing textures
    typedef utHashTable<utPointerHashKey, Ogre::Texture*> ImageTextureHashMap;

public:
    gkBlendFile(const gkString& group = GK_DEF_GROUP);
    ~gkBlendFile();
    bool parse(bParse::bBlenderFile *bfp);


    void    registerLoader(Ogre::ManualResourceLoader *loader)  {m_loaders.push_back(loader);}

    gkScene *findScene(const gkString& name);

    GK_INLINE gkSceneIterator getSceneIterator(void) {return gkSceneIterator(m_scenes);}
    GK_INLINE gkImageIterator getImageIterator(void) {return gkImageIterator(m_images); }

    GK_INLINE const gkString&   getGroup(void)          {return m_group;}
    bParse::bBlenderFile        *getInternalFile(void)  {GK_ASSERT(m_file); return m_file;}


protected:

    void doVersionTests(void);

    void buildTextFiles(void);    
	void buildAllTextures();
	void buildAllSounds();

    ManualResourceLoaderList  m_loaders;   // Ogre Loaders
    const gkString            m_group;     // Ogre Ogre::Resource Group
    bParse::bBlenderFile*     m_file;      // bParse File Pointer
    gkSceneList               m_scenes;    // All Scenes
    gkImageList               m_images;    // All Textures
    ImageTextureHashMap       m_imageLookup;
};


#endif//_gkBlendFile_h_
