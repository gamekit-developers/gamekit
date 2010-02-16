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
#include "OgreException.h"
#include "OgreConfigFile.h"
#include "OgreStringConverter.h"

#include "gkUserDefs.h"
#include "gkLogger.h"
#include "gkPath.h"
#include "gkUtils.h"
#include "gkPath.h"


using namespace Ogre;




gkUserDefs::gkUserDefs() :
        plugins(StringUtil::BLANK),
        rendersystem(OGRE_RS_GL),
        log("OgreKit.log"),
        verbose(true),
        winsize(800, 600),
        wintitle("Ogre GameKit Demo"),
        fullscreen(false),
        resources(""),
        animspeed(25),
        startframe(1),
        tickrate(45),
        blendermat(false),
        autoWindow(true),
        grabInput(true),
        multipassmat(false),
        gl_vert_profile("arbvp1"),
        gl_frag_profile("arbfp1"),
        dx_vert_profile("vs_1_1"),
        dx_frag_profile("ps_2_x")
{
}


void gkUserDefs::load(const gkString &fname)
{
    gkString startup;

    gkPath pth(fname);
    if (pth.isFile())
        startup = pth.getPath();

    if (startup.empty())
        return;


    try
    {
        // try and load prefs

        ConfigFile fp;
        fp.load(startup);

        ConfigFile::SectionIterator cit = fp.getSectionIterator();
        while (cit.hasMoreElements())
        {
            ConfigFile::SettingsMultiMap *ptr = cit.getNext();
            for (ConfigFile::SettingsMultiMap::iterator dit = ptr->begin(); dit != ptr->end(); ++dit)
            {
                gkString key = dit->first;
                gkString val = dit->second;

                // not case sensitive
                StringUtil::toLowerCase(key);


                if (key == "plugins")
                {
                    gkPath pth = val;
                    if (!pth.isDir())
                        plugins = val;
                }
                else if (key == "log")
                    log = val;
                else if (key == "autowindow")
                    autoWindow = StringConverter::parseBool(val);
                else if (key == "verbose")
                    verbose = StringConverter::parseBool(val);
                else if (key == "winsize")
                    winsize = StringConverter::parseVector2(val);
                else if (key == "wintitle")
                    wintitle = val;
                else if (key == "fullscreen")
                    fullscreen = StringConverter::parseBool(val);
                else if (key == "resources")
                {
                    gkPath p(val);
                    if (p.isFile())
                        resources = val;
                }
                else if (key == "animspeed")
                {
                    animspeed = StringConverter::parseReal(val);
                    animspeed = gkMaxf(animspeed, 1.0);
                }
                else if (key == "startframe")
                {
                    startframe = StringConverter::parseReal(val);
                    startframe = gkMaxf(startframe, 1.0);
                }
                else if (key == "tickrate")
                {
                    tickrate = StringConverter::parseReal(val);
                    tickrate = gkMaxf(tickrate, 25);
                }
                else if (key == "blendermat")
                    blendermat = StringConverter::parseBool(val);
            }
        }
    }
    catch (Exception &e)
    {
        gkLogMessage("Failed to load resource file!\n" << e.getDescription());
    }
}
