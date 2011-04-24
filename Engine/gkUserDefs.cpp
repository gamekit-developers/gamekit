/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): silveira.nestor.
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

#include "gkUserDefs.h"
#include "gkLogger.h"
#include "gkPath.h"
#include "gkUtils.h"
#include "gkPath.h"
#include "gkWindowSystem.h"
#include "gkViewport.h"

#include "OgreException.h"
#include "OgreConfigFile.h"
#include "OgreStringConverter.h"

gkUserDefs::gkUserDefs()
	:
#ifdef OGREKIT_BUILD_MOBILE
	viewportOrientation("landscaperight"),

#ifdef OGRE_BUILD_RENDERSYSTEM_GLES2
	rendersystem(OGRE_RS_GLES2),
#else
    rendersystem(OGRE_RS_GLES), 
#endif

#else
	rendersystem(OGRE_RS_GL),
	viewportOrientation(""),
#endif
	sceneManager(-1),
	log("OgreKit.log"),
	verbose(true),
	winsize(800, 600),
	wintitle("Ogre GameKit Demo"),
	fullscreen(false),
	framingType(gkViewport::FRAMING_EXTEND),
	resources(""),
	blendermat(true),
	matblending(false),
	grabInput(true),
	debugFps(false),
	debugPhysics(false),
	debugPhysicsAabb(false),
	enableshadows(true),
	buildStaticGeometry(false),
	useBulletDbvt(true),
	showDebugProps(false),
	debugSounds(false),
	fsaa(false),
	fsaaSamples(4),
	disableSound(false),
	shadowtechnique("stencilmodulative"),
	colourshadow(0.8f, 0.8f, 0.8f),
	fardistanceshadow(0),
	defaultMipMap(5),
	extWinhandle(""),
	animFps(24.f),
	shaderCachePath(""),
	rtss(false),
	hasFixedCapability(true)
{
}



void gkUserDefs::load(const gkString& fname)
{
	gkString startup;

	gkPath pth(fname);
	if (pth.isFile())
		startup = pth.getPath();

	if (startup.empty())
		return;


	try
	{
		// try and initialize prefs

		Ogre::ConfigFile fp;
		fp.load(startup);

		Ogre::ConfigFile::SectionIterator cit = fp.getSectionIterator();
		while (cit.hasMoreElements())
		{
			Ogre::ConfigFile::SettingsMultiMap* ptr = cit.getNext();
			for (Ogre::ConfigFile::SettingsMultiMap::iterator dit = ptr->begin(); dit != ptr->end(); ++dit)
			{
				gkString key = dit->first;
				gkString val = dit->second;

				// not case sensitive
				Ogre::StringUtil::toLowerCase(key);
				parseString(key, val);
			}
		}
	}
	catch (Ogre::Exception& e)
	{
		gkLogMessage("Failed to initialize resource file!\n" << e.getDescription());
	}
}

bool gkUserDefs::isD3DRenderSystem(OgreRenderSystem rs)
{
	return rs == OGRE_RS_D3D9 || rs == OGRE_RS_D3D10 || rs == OGRE_RS_D3D11;
}

OgreRenderSystem gkUserDefs::getOgreRenderSystem(const gkString& val)
{
	OgreRenderSystem rendersystem = OGRE_RS_GL;

	if (val.find("d3d9") != val.npos)
		rendersystem = OGRE_RS_D3D9;
	else if (val.find("d3d10") != val.npos)
		rendersystem = OGRE_RS_D3D10;
	else if (val.find("d3d11") != val.npos)
		rendersystem = OGRE_RS_D3D11;

	return rendersystem;
}

int gkUserDefs::getViewportFramingType(const gkString& val)
{
	int framingType = gkViewport::FRAMING_EXTEND;

	if (val.find("crop") != val.npos)
		framingType = gkViewport::FRAMING_CROP;
	else if (val.find("letterbox") != val.npos)
		framingType = gkViewport::FRAMING_LETTERBOX;

	return framingType;
}

void gkUserDefs::parseString(const gkString& key, const gkString& val)
{
#define KeyEq(b) (key == b)

	if (KeyEq("rendersystem"))
	{
		rendersystem = getOgreRenderSystem(val);
		return;
	}
	if (KeyEq("viewportorientation"))
	{
		viewportOrientation = val;
		return;
	}
	if (KeyEq("log"))
	{
		log = val;
		return;
	}
	if (KeyEq("debugfps"))
	{
		debugFps = Ogre::StringConverter::parseBool(val);
		return;
	}
	if (KeyEq("debugphysics"))
	{
		debugPhysics = Ogre::StringConverter::parseBool(val);
		return;
	}
	if (KeyEq("debugphysicsaabb"))
	{
		debugPhysicsAabb = Ogre::StringConverter::parseBool(val);
		return;
	}
	if (KeyEq("grabinput"))
	{
		grabInput = Ogre::StringConverter::parseBool(val);
		return;
	}
	if (KeyEq("verbose"))
	{
		verbose = Ogre::StringConverter::parseBool(val);
		return;
	}
	if (KeyEq("winsize"))
	{
		winsize = Ogre::StringConverter::parseVector2(val);
		return;
	}
	if (KeyEq("wintitle"))
	{
		wintitle = val;
		return;
	}
	if (KeyEq("fullscreen"))
	{
		fullscreen = Ogre::StringConverter::parseBool(val);
		return;
	}
	if (KeyEq("framingtype"))
	{
		framingType = getViewportFramingType(val);
		return;
	}
	if (KeyEq("resources"))
	{
		gkPath p(val);
		if (p.isFile())
			resources = val;

		return;
	}
	if (KeyEq("blendermat"))
	{
		blendermat = Ogre::StringConverter::parseBool(val);
		return;
	}
	if (KeyEq("matblending"))
	{
		matblending = Ogre::StringConverter::parseBool(val);
		return;
	}	
	if (KeyEq("buildinstances"))
	{
		buildStaticGeometry = Ogre::StringConverter::parseBool(val);
		return;
	}
	if (KeyEq("frustumculling"))
	{
		useBulletDbvt = Ogre::StringConverter::parseBool(val);
		return;
	}
	if (KeyEq("showdebugprops"))
	{
		showDebugProps = Ogre::StringConverter::parseBool(val);
		return;
	}
	if (KeyEq("debugsounds"))
	{
		debugSounds = Ogre::StringConverter::parseBool(val);
		return;
	}
	if (KeyEq("disablesound"))
	{
		disableSound = Ogre::StringConverter::parseBool(val);
		return;
	}
	if (KeyEq("enableshadows"))
	{
		enableshadows = Ogre::StringConverter::parseBool(val);
		return;
	}
	if (KeyEq("shadowtechnique"))
	{
		shadowtechnique = val;
		return;
	}
	if (KeyEq("colourshadow"))
	{
		colourshadow = Ogre::StringConverter::parseColourValue(val);
		return;
	}
	if (KeyEq("fardistanceshadow"))
	{
		fardistanceshadow = Ogre::StringConverter::parseReal(val);
		return;
	}
	if (KeyEq("fsaa"))
	{
		fsaa = Ogre::StringConverter::parseBool(val);
		return;
	}
	if (KeyEq("rtss"))
	{
		rtss = Ogre::StringConverter::parseBool(val);
		return;
	}
	if (KeyEq("fsaasamples"))
	{
		fsaaSamples = gkClamp<int>(Ogre::StringConverter::parseInt(val), 0, 16);
		return;
	}
	if (KeyEq("defaultmipmap"))
	{
		defaultMipMap = gkMax<int>(0, Ogre::StringConverter::parseInt(val));
		return;
	}
	if (KeyEq("shaderCachePath"))
	{
		shaderCachePath = val;
		return;
	}

#undef KeyEq
}
