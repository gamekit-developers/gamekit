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

#ifndef _gkBlendInternalFile_h_
#define _gkBlendInternalFile_h_

#include "gkCommon.h"

#if OGREKIT_USE_BPARSE
#include "bBlenderFile.h"
#include "bMain.h"
#else
#include "fbtBlend.h"
#endif

#include "Blender.h"


		 
class gkBlendListIterator
{
public:
#if OGREKIT_USE_BPARSE
	typedef bParse::bListBasePtr List;
	typedef int ListIndex;
	typedef bParse::bStructHandle ListItem;
#else
	typedef fbtList List;
	typedef fbtList::Link* ListIndex;
	typedef fbtList::Link ListItem;
#endif

protected:
	List* m_list;
	ListIndex m_index;

public:
	gkBlendListIterator(List* list);

	bool hasMoreElements() const;
	ListItem* getNext(void);
};


class gkBlendInternalFile
{
#if OGREKIT_USE_BPARSE
	bParse::bBlenderFile*		m_file;		// bParse File Pointer
#else
	fbtBlend*					m_file;
#endif

public:
	gkBlendInternalFile();
	~gkBlendInternalFile();

	bool parse(const gkString& fname);

	bool parse(const void* mem, int size);

	Blender::FileGlobal* getFileGlobal();
	Blender::Scene* getFirstScene();

	int getVersion();

	gkBlendListIterator getSceneList();
	gkBlendListIterator getTextList();

	gkBlendListIterator getObjectList();
	gkBlendListIterator getMeshList();	
	gkBlendListIterator getLampList();
	gkBlendListIterator getCameraList();

	gkBlendListIterator getMatList();
	gkBlendListIterator getTexList();
	gkBlendListIterator getImageList();

	gkBlendListIterator getIpoList();
	gkBlendListIterator getKeyList();
	gkBlendListIterator getWorldList();
	
	gkBlendListIterator getScriptList();
	gkBlendListIterator getVFontList();
	gkBlendListIterator getSoundList();
	gkBlendListIterator getGroupList();
	gkBlendListIterator getArmatureList();
	gkBlendListIterator getActionList();		
	gkBlendListIterator getParticleList();

	gkBlendListIterator getLattList();
	gkBlendListIterator getCurveList();
	gkBlendListIterator getLibraryList();
	gkBlendListIterator getNodeTreeList();
	gkBlendListIterator getMBallList();

};

#endif//_gkBlendInternalFile_h_
