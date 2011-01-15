/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 harkon.kr

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

#include "StdAfx.h"
#include "okUtils.h"
#include "gkFontManager.h"

#ifdef __WXGTK__
#include "gdk/gdk.h"
#include "gdk/gdkx.h"
#include "gtk/gtk.h"
#endif

gkString getNativeWinHandleFromWxWin(wxWindow* win)
{
	gkString handle;
	if (!win) return handle;
#ifdef WIN32
	handle = Ogre::StringConverter::toString((size_t)((HWND)win->GetHandle()));
#elif defined(__WXGTK__)
	// copied from wxOgreMVC
	// handle format: display:screen:window of types ulong:uint:ulong
	GtkWidget* widget = win->GetHandle();
	gtk_widget_realize( widget );   // Mandatory. Otherwise, a segfault happens.
	Ogre::StringUtil::StrStreamType handleStream;
	Display* display = GDK_WINDOW_XDISPLAY( widget->window );
	Window wid = GDK_WINDOW_XWINDOW( widget->window );   // Window is a typedef for XID, which is a typedef for unsigned int
	// Get the right display (DisplayString() returns ":display.screen")
	gkString displayStr = DisplayString( display );
	displayStr = displayStr.substr( 1, ( displayStr.find( ".", 0 ) - 1 ) );
	// Put all together
	handleStream << displayStr << ':' << DefaultScreen( display ) << ':' << wid;
	handle = handleStream.str();
#else
#error Not supported yet!
#endif
	return handle;
}

gkString getBaseFileName(const gkString& fileName)
{
	wxFileName fn(fileName);
	return WX2GK(fn.GetName());
}

void dumpGkInfo()
{
	gkPrintf("scene: %d object: %d objinst: %d group: %d sound: %d lua: %d skel: %d mesh: %d anim: %d text: %d blend: %d font: %d window: %d resgroup: %d", 
		gkSceneManager::getSingleton().getResourceCount(),
		gkGameObjectManager::getSingleton().getResourceCount(),
		gkGameObjectManager::getSingleton().getInstances().size(),
		gkGroupManager::getSingleton().getResourceCount(),
		gkSoundManager::getSingleton().getResourceCount(),
		gkLuaManager::getSingleton().getResourceCount(),
		gkSkeletonManager::getSingleton().getResourceCount(),
		gkMeshManager::getSingleton().getResourceCount(),
		gkAnimationManager::getSingleton().getResourceCount(),
		gkTextManager::getSingleton().getResourceCount(),
		gkBlendLoader::getSingleton().getFiles().size(),
		gkFontManager::getSingleton().getResourceCount(),
		gkWindowSystem::getSingleton().getWindowCount(),
		gkResourceGroupManager::getSingleton().getResourceGroupList().size()
	);

	
	gkResourceManager::ResourceIterator it = gkSceneManager::getSingleton().getResourceIterator();
	while (it.hasMoreElements())
	{
		gkScene* scene = static_cast<gkScene*>(it.getNext().second);
		gkPrintf("scene name: %s group: %s objects: %d instances: %d light: %d camera: %d",
			scene->getName().c_str(),
			scene->getResourceName().getFullName().c_str(),
			scene->getObjects().size(),
			scene->getInstancedObjects().size(),
			scene->getLights().size(),
			scene->getCameras().size()
		);
	}
}