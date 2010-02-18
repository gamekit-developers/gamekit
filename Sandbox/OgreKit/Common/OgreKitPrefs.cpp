/*
-------------------------------------------------------------------------------
	This file is part of the Ogre GameKit port.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#include "OgreKitPrefs.h"
#include "OgreException.h"
#include "OgreConfigFile.h"
#include "OgreStringConverter.h"
#include "OgreMathUtils.h"


using namespace Ogre;


// ----------------------------------------------------------------------------
OgreKitPrefs::OgreKitPrefs() :
		rendersystem(OGRE_RS_GL),
		w(800),
		h(600),
		fullscreen(false),
		resources(""),
		aa_level(0)
{
}

// ----------------------------------------------------------------------------
void OgreKitPrefs::load(const String &fname)
{
	try
	{
		ConfigFile fp;
		fp.load(fname);

		ConfigFile::SectionIterator cit = fp.getSectionIterator();
		while (cit.hasMoreElements())
		{
			ConfigFile::SettingsMultiMap *ptr = cit.getNext();
			for (ConfigFile::SettingsMultiMap::iterator dit = ptr->begin(); dit != ptr->end(); ++dit)
			{
				String key = dit->first;
				String val = dit->second;

				/// not case sensitive
				StringUtil::toLowerCase(key);


				if (key == "winsize")
				{
					Vector2 size = StringConverter::parseVector2(val);
					w = (unsigned int)size.x;
					h = (unsigned int)size.y;
				}
				else if (key == "aa_level")
				{
					aa_level = StringConverter::parseInt(val);
					if (aa_level < 0)
						aa_level = 0;
					if (aa_level > 6)
						aa_level = 6;
				}
				else if (key == "fullscreen")
					fullscreen = StringConverter::parseBool(val);
				else if (key == "resources")
					resources = val;
			}
		}
	}
	catch (Ogre::Exception &)
	{
		// ignore
	}
}
