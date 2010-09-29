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
	Ogre::String displayStr = DisplayString( display );
	displayStr = displayStr.substr( 1, ( displayStr.find( ".", 0 ) - 1 ) );
	// Put all together
	handleStream << displayStr << ':' << DefaultScreen( display ) << ':' << wid;
	handle = handleStream.str();
#else
#error Not supported yet!
#endif
	return handle;
}
