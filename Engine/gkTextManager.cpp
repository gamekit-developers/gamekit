/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Charlie C.

    Contributor(s): vekoon.
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
#include "utScript.h"


#ifdef OGREKIT_COMPILE_OGRE_SCRIPTS
#include "OgreMaterialManager.h"
#include "OgreParticleSystemManager.h"
#include "OgreHighLevelGpuProgramManager.h"
#include "OgreFontManager.h"
#include "OgreOverlayManager.h"
#include "gkFontManager.h"
#include "External/Ogre/gkOgreBlendArchive.h"
#include "OgreCompositorManager.h"
#endif //OGREKIT_COMPILE_OGRE_SCRIPTS


#ifdef OGREKIT_USE_LUA
#include "Script/Lua/gkLuaManager.h"
#endif


struct TextToTypeItem
{
	const char* name;
	const int   type;
};

static TextToTypeItem TextItemMap[] =
{
	{".material",   gkTextManager::TT_MATERIAL},
	{".particle",   gkTextManager::TT_PARTICLE},
	{".compositor", gkTextManager::TT_COMPOSIT},
	{".overlay",    gkTextManager::TT_OVERLAY},
	{".fontdef",    gkTextManager::TT_FONT},
	{".cg",         gkTextManager::TT_CG},
	{".glsl",       gkTextManager::TT_GLSL},
	{".hlsl",       gkTextManager::TT_HLSL},
	{".lua",        gkTextManager::TT_LUA},
	{".xml",        gkTextManager::TT_XML},
	{".ntree",      gkTextManager::TT_NTREE},
	{".bfont",      gkTextManager::TT_BFONT},
	{0, -1}
};

int gkTextManager::registerType(const gkString &extension, const gkString &name, TextTypeManager *manager)
{
    UTsize idx = m_userTypes.find(extension);

    if (idx == UT_NPOS)
    {
        m_userTypeCount++;

        UserType ut;

        ut.id = ((int)TT_USER) + m_userTypeCount;
        ut.name = name;
        ut.manager = manager;

        m_userTypes.insert(extension, ut);

        return ut.id;
    }

    return 0;
}

bool gkTextManager::unregisterType(int id)
{
    gkTextManager::UserTypeMap::Iterator it = m_userTypes.iterator();

    while (it.hasMoreElements())
    {
        UserTypeMap::Iterator::Pair item = it.getNext();
        UserType ut = item.second;

        if (ut.id == id)
        {
            m_userTypes.remove(item.first);

            return true;
        }
    }

    return false;
}

gkTextManager::UserType gkTextManager::getUserType(int id)
{
    UserTypeMap::Iterator it = m_userTypes.iterator();

    while (it.hasMoreElements())
    {
        UserTypeMap::Iterator::Pair item = it.getNext();
        UserType ut = item.second;

        if (ut.id == id)
        {
            return ut;
        }
    }

    return gkTextManager::UserType();
}


gkTextManager::gkTextManager()
	:    gkResourceManager("TextManager", "TextFile")
	,    m_userTypeCount(0)
{
}

gkTextManager::~gkTextManager()
{
	destroyAll();
}


int gkTextManager::getTextType(const gkString& name)
{
    gkString ext = name;

	int i = 0;
	while (TextItemMap[i].name != 0)
	{
		if (name.find(TextItemMap[i].name) != name.npos)
			return TextItemMap[i].type;
		++i;
	}

    UserTypeMap::Iterator it = m_userTypes.iterator();

    while (it.hasMoreElements())
    {
        UserTypeMap::Iterator::Pair item = it.getNext();
        UserType ut = item.second;

        if (name.find(item.first.str()) != name.npos)
        {
            return ut.id;
        }
    }

	return TT_ANY;
}

void gkTextManager::getTextFiles(TextArray& dest, int textType)
{
	gkResourceManager::ResourceIterator iter = getResourceIterator();
	while (iter.hasMoreElements())
	{
		gkTextFile* tf = static_cast<gkTextFile*>(iter.getNext().second);

		if (textType < 0 || tf->getType() == textType)
			dest.push_back(tf);
	}
}



gkResource* gkTextManager::createImpl(const gkResourceName& name, const gkResourceHandle& handle)
{
	UTsize tt = getTextType(name.getName());

	return new gkTextFile(this, name, handle, tt);
}

void gkTextManager::parseScripts(const gkString& group)
{
	gkResourceManager::ResourceIterator iter = getResourceIterator();
	while (iter.hasMoreElements())
	{
		gkTextFile* tf = static_cast<gkTextFile*>(iter.getNext().second);

		if (!group.empty() && tf->getGroupName() != group) continue;

		const gkString& buf = tf->getText();
		const int type = tf->getType();


#ifdef OGREKIT_COMPILE_OGRE_SCRIPTS

		try
		{

			if (type == TT_MATERIAL)
			{
				Ogre::DataStreamPtr memStream(
				    OGRE_NEW Ogre::MemoryDataStream((void*)buf.c_str(), buf.size()));

				Ogre::MaterialManager::getSingleton().parseScript(memStream, group);

			}
			else if (type == TT_PARTICLE)
			{
				Ogre::DataStreamPtr memStream(
				    OGRE_NEW Ogre::MemoryDataStream((void*)buf.c_str(), buf.size()));


				Ogre::ParticleSystemManager::getSingleton().parseScript(memStream, group);
			}
			else if (type == TT_FONT)
			{
				// Note: font must be an external file (.ttf anyway (texture fonts are not tested) )
				Ogre::DataStreamPtr memStream(
				    OGRE_NEW Ogre::MemoryDataStream((void*)buf.c_str(), buf.size()));

				Ogre::FontManager::getSingleton().parseScript(memStream, group);
			}
			else if (type == TT_COMPOSIT)
			{
				Ogre::DataStreamPtr memStream(
				    OGRE_NEW Ogre::MemoryDataStream((void*)buf.c_str(), buf.size()));

				Ogre::CompositorManager::getSingleton().parseScript(memStream, group);
			}
		}
		catch (Ogre::Exception& e)
		{
			gkLogMessage("TextManager: " << e.getDescription());
			continue;
		}



		if (type == TT_BFONT)
		{
			utMemoryStream stream;
			stream.open(buf.c_str(), buf.size(), utStream::SM_READ);

			gkFontManager::getSingleton().parseScript(&stream);
		}
#endif
#ifdef OGREKIT_USE_LUA

		if (type == TT_LUA)
			gkLuaManager::getSingleton().createFromText(gkResourceName(tf->getResourceName().getName(), group), buf);
#endif

        if (type > TT_USER)
        {
            gkTextManager::UserType ut = getUserType(type);

            if (ut.id == type)
            {
                if (ut.manager)
                {
                    utMemoryStream stream;
                    stream.open(buf.c_str(), buf.size(), utStream::SM_READ);

                    utScript script;
                    script.parseBuffer(ut.name, gkString((const char*)stream.ptr()));

                    ut.manager->notifyScriptResourceLoaded(tf, &script);
                }
            }
        }
	}

#ifdef OGREKIT_COMPILE_OGRE_SCRIPTS

	// Overlays are a dependant script. (.material .font)

	try
	{

		TextArray overlays;
		getTextFiles(overlays, TT_OVERLAY);



		TextArray::Iterator it = overlays.iterator();
		while (it.hasMoreElements())
		{
			gkTextFile* tf = (gkTextFile*)it.getNext();

			const gkString& buf = tf->getText();
			const int type = tf->getType();

			Ogre::DataStreamPtr memStream(
			    OGRE_NEW Ogre::MemoryDataStream((void*)buf.c_str(), buf.size()));


			Ogre::OverlayManager::getSingleton().parseScript(memStream, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		}
	}
	catch (Ogre::Exception& e)
	{
		gkLogMessage("TextManager: " << e.getDescription());
	}

#endif

}


UT_IMPLEMENT_SINGLETON(gkTextManager);
