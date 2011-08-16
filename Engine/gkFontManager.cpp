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
#include "gkCommon.h"

#ifdef OGREKIT_COMPILE_OGRE_SCRIPTS


#include "gkFontManager.h"
#include "gkFont.h"
#include "gkLogger.h"
#include "OgreFontManager.h"
#include "utScript.h"
#include "gkVariable.h"
#include "gkMathUtils.h"



gkFontManager::gkFontManager()
	:    gkResourceManager("FontManager", "Font")
{
}


gkFontManager::~gkFontManager()
{
	destroyAll();
}


gkResource* gkFontManager::createImpl(const gkResourceName& name, const gkResourceHandle& handle)
{
	return new gkFont(this, name, handle);
}


void gkFontManager::parseScript(utMemoryStream* buffer)
{
	utScript script;
	script.parseBuffer("FontManager:parseScript", gkString((const char*)buffer->ptr()));


	utScriptTree* treePtr = script.getTreePtr();

	if (treePtr->getNodes().empty())
		return;


	utScriptTree::Nodes::Iterator niter = treePtr->getNodes().iterator();
	while (niter.hasMoreElements())
	{
		utScriptNode* tree = niter.getNext();

		if (!tree->hasAttribute("source"))
		{
			gkLogMessage("FontManager: Missing font script attribute 'source'");
			continue;
		}
		const gkString src = tree->getAttribute("source")->getValue();

		if (!exists(src))
		{
			gkLogMessage("FontManager: Missing internal font " << src);
			continue;
		}

		const gkString name = tree->getType();

		Ogre::FontPtr fp = Ogre::FontManager::getSingleton().getByName(name);
		if (!fp.isNull())
		{
			// use it
			return;
		}


		gkFont* gkf = (gkFont*)getByName(src);


		int size = 0, res = UT_NPOS;

		if (tree->hasAttribute("size"))
			gkFromString(tree->getAttribute("size")->getValue(), size);

		if (tree->hasAttribute("resolution"))
			gkFromString(tree->getAttribute("resolution")->getValue(), res);

		try
		{

			gkFont::Loader* fl = gkf->addLoader(size > 0 && size != UT_NPOS ? size : 12, res != UT_NPOS ? res : 55);
			fp = Ogre::FontManager::getSingleton().create(name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true, fl);
			fl->setFont(fp.getPointer());
		}
		catch (Ogre::Exception& e)
		{
			gkLogMessage("FontManager::parseScript: " << e.getDescription());
		}
	}
}


UT_IMPLEMENT_SINGLETON(gkFontManager);

#endif
