/*
-------------------------------------------------------------------------------
--                                                                           --
--  This file is part of GameKit.                                            --
--  http://gamekit.googlecode.com/                                           --
--                                                                           --
--                                                                           --
--  Released under the zlib license.                                         --
--  http://www.gzip.org/zlib/zlib_license.html                               --
--                                                                           --
--                                                                           --
-- Runtime test using the V8 JavaScript Engine                               -- 
-------------------------------------------------------------------------------
*/
#include "OgreKitApplication.h"
#include "Script/V8/gkV8Manager.h"


// ----------------------------------------------------------------------------
OgreKitApplication::OgreKitApplication(const gkString &blend)
    :   m_blend(blend)
{
}

// ----------------------------------------------------------------------------
OgreKitApplication::~OgreKitApplication()
{
    if (!m_update.IsEmpty())
        m_update.Dispose();
    if (!m_load.IsEmpty())
        m_load.Dispose();
}


// ----------------------------------------------------------------------------
bool OgreKitApplication::load(void)
{
    gkBlendFile *blend = m_engine->loadBlendFile(gkUtils::getFile(m_blend));
    if (!blend)
    {
        gkPrintf("File loading failed.\n");
        return false;
    }

    gkSceneIterator scit = blend->getSceneIterator();
    if (!scit.hasMoreElements())
    {
        gkPrintf("No usable scenes found in blend.\n");
        return false;
    }

    m_scene = scit.peekNext();

    m_scene->load();


    // call load function
    if (!m_load.IsEmpty())
        m_load->Call(m_load, 0, NULL);

    // add input hooks
    gkWindowSystem::getSingleton().addListener(this);
    return true;
}


// ----------------------------------------------------------------------------
void OgreKitApplication::loadBlend(const char *name)
{
    m_blend = name;
}


// ----------------------------------------------------------------------------
void OgreKitApplication::addListener(CallbackCode code, CallbackObject fp)
{
    if (code == ON_TICK)
        m_update = fp;
    if (code == ON_LOAD)
        m_load = fp;
}


// ----------------------------------------------------------------------------
void OgreKitApplication::tick(gkScalar rate)
{
    if (!m_update.IsEmpty())
    {
        v8::Handle<v8::Value> argv[1];
        argv[0] = v8::Number::New(rate);
        m_update->Call(m_update, 1, argv);
    }
}

// ----------------------------------------------------------------------------
void OgreKitApplication::keyReleased(const gkKeyboard &key, const gkScanCode &sc)
{
    if (sc == KC_ESCKEY) 
        m_engine->requestExit();
}

// ----------------------------------------------------------------------------
void OgreKitApplication::start(void)
{
    m_prefs.winsize.x   = gkMax(m_defs.winx, 640);
    m_prefs.winsize.y   = gkMax(m_defs.winy, 480);
    m_prefs.fullscreen  = m_defs.fullscreen;
    m_prefs.verbose     = m_defs.verbose;
    m_prefs.wintitle    = m_defs.wintitle;
    m_prefs.grabInput   = m_defs.grab;
    run();
}
