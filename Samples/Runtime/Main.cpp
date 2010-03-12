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
#include "OgreKit.h"


class OgreKit : public gkCoreApplication, public gkWindowSystem::Listener
{
public:
    gkString    m_blend;
    gkScene    *m_scene;

public:
    OgreKit(const gkString &blend)
        :   m_blend(blend), m_scene(0)
    {
        m_prefs.winsize.x   = 800;
        m_prefs.winsize.y   = 600;
        m_prefs.wintitle    = gkString("OgreKit Demo (Press Escape to exit)[") + m_blend + gkString("]");
        m_prefs.blendermat  = true;
        m_prefs.verbose     = false;
        m_prefs.grabInput   = true;
        m_prefs.debugPhysics    = false;
        m_prefs.debugPhysicsAabb = false;

        gkPath path = "OgreKitStartup.cfg";
        // overide settings if found
        if (path.isFileInBundle()) m_prefs.load(path.getPath());

        m_prefs.userWindow  = false;
    }

    virtual ~OgreKit()
    {
    }

    bool load(void)
    {
        gkBlendFile *blend = m_engine->loadBlendFile(m_blend);
        if (!blend)
        {
            gkPrintf("File loading failed.\n");
            return false;
        }

        if (m_prefs.userWindow) m_engine->initializeWindow();

        gkSceneIterator scit = blend->getSceneIterator();
        if (!scit.hasMoreElements())
        {
            gkPrintf("No usable scenes found in blend.\n");
            return false;
        }

        m_scene = scit.peekNext();

        m_scene->load();

        gkLuaScript *script = gkLuaManager::getSingleton().getScript("OnLoad.lua");

        // load user application
        if (script) 
            script->execute();

        // add input hooks
        gkWindowSystem::getSingleton().addListener(this);
        return true;
    }

    void keyReleased(const gkKeyboard &key, const gkScanCode &sc)
    {
        if (sc == KC_ESCKEY) m_engine->requestExit();
    }
};


int main(int argc, char **argv)
{
    TestMemory;
    char *fname = "momo_ogre.blend";

#if GK_PLATFORM != GK_PLATFORM_APPLE
    if (argc > 1)
        fname = argv[argc-1];
#endif
    
    OgreKit okit(gkUtils::getFile(fname));
    okit.run();
    return 0;
}
