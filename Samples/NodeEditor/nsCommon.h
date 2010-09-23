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
#ifndef _nsCommon_h_
#define _nsCommon_h_

#include "utTypes.h"
#include "utCommon.h"
#include "nsString.h"

// all forwards
class nsMainWindow;
class nsNodeCanvas;
class nsSolutionBrowser;
class nsGlHiddenContext;
class nsRenderSystem;
class nsRenderSystemPrivate;
class nsClipboard;
class nsSocketProperty;
class nsNodePropertyPage;
class nsPropertyManager;

// data types
class nsNodeManager;
class nsNodeTree;
class nsNode;
class nsSocket;
class nsNodeDef;
class nsSocketDef;

class wxAuiManager;
class wxMenu;
class wxMenuItem;
class wxPGProperty;


typedef utList<nsNode*>                nsNodes;
typedef utListIterator<nsNodes>         nsNodeIterator;
typedef utListReverseIterator<nsNodes>  nsReverseNodeIterator;
typedef utList<nsNodeTree*>            nsTrees;
typedef utListIterator<nsTrees>         nsTreeIterator;

typedef utListClass<nsNodeDef>              nsNodeListClass;
typedef utListIterator<nsNodeListClass>     nsNodeDefIterator;
typedef utListClass<nsSocket>               nsSocketClass;
typedef utList<nsSocket*>                   nsSocketList;
typedef utListClass<nsSocketDef>            nsSocketListClass;
typedef utListIterator<nsSocketListClass>   nsSocketDefIterator;

enum nsWindowId
{
	NS_WID_MAINWINDOW,
	NS_WID_SOLUTION,
	NS_WID_SOLUTION_DATA,
	NS_WID_WORKSPACE,
	NS_WID_WORKSPACE_DATA,
	NS_WID_PROPERTY,
	NS_WID_PROPERTY_DATA,
	NS_WID_PROPERTY_TREE,
	NS_WID_PROPERTY_NODE,
	NS_WID_CANVAS,
};



enum nsEventID
{
	// File
	NSID_NEW,
	NSID_OPEN,
	NSID_SAVE,
	NSID_SAVEAS,
	NSID_QUIT,


	// Edit
	NS_ID_UNDO,
	NS_ID_REDO,
	NS_ID_CUT,
	NS_ID_COPY,
	NS_ID_PASTE,
	NS_ID_DUPLICATE,
	NS_ID_DELETE,
	NS_ID_SELECT_ALL,

	// View
	NS_ID_SOLUTION,
	NS_ID_PROPERTIES,
	NS_ID_VARIABLES,
	NS_ID_FULLSCREEN,

	// Project
	NS_ID_NEW_TREE,

	// Node
	NSID_NODE_MENU_START,
	// dynamic
	NSID_NODE_MENU_END = NSID_NODE_MENU_START + 9999,
};

enum nsNodeState
{
	NDST_INACTIVE,
	NDST_ACTIVE,
};


#define nsDefaultSize wxSize(200, 200)

#define nsHeaderBorderSize  0
#define nsContentBorderSize 0
#define nsBorderNone        wxBORDER_NONE
#define nsBorderDefault     wxBORDER_THEME



// string conversion
#define wxFromAscii(x)      wxString(x.c_str(), x.size())
#define wxToAscii(x)        (const char*)x.mb_str()
#define wxToSTDString(x)    nsString(wxToAscii(x))


#endif//_nsCommon_h_
