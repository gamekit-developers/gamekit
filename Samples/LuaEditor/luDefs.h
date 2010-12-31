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


#ifndef _LuaDefs_h_
#define _LuaDefs_h_

#define WIN_SIZE_X			640
#define WIN_SIZE_Y			480
#define LOG_BOX_HEIGHT		140

#define STATUS_FPS			4
#define APP_TITLE			"OgreKit LuaEditor"
#define HELP_TITLE			"OgreKit Lua Help"
#define LU_CONFIG_FILE		"Config/luconfig.xml"


#define FILE_EXT_BLEND		"blend"
#define FILE_EXT_LUA		"lua"
#define FILE_EXT_PROJ		"okproj"


#define PANEL_TITLE_HEIGHT	24

enum MENU_ID
{
	ID_STATUS_TIMER	= wxID_HIGHEST + 100,	
	ID_WAKEUP_TIMER,
	ID_LOG_CLEAR,
	ID_LOG_SAVE,
	ID_TIMER_PAUSE,
	ID_SHOW_HELP,
	ID_OPEN_PROJ_FILE,
	ID_OPEN_BLEND_FILE,
	ID_OPEN_LUA_FILE,
	ID_NEW_PROJ_FILE,
	ID_NEW_LUA_FILE,
	ID_NEW_BLEND_FILE,
	ID_PROJ_ACTIVE_FILE,
	ID_PROJ_STARTUP_FILE,
	ID_PROJ_REMOVE_FILE,
	ID_SAVE_LUA_FILE,

	ID_EXPORT_DISTRIBUTION,

	ID_RUN_RUNTIME,
	ID_RUN_BLENDER,
	ID_SHOW_PROJ_WIN,
	ID_SHOW_LOG_WIN,
	ID_SHOW_INSP_WIN,
	ID_SHOW_PROPS_WIN,

	ID_TOOL_ANIM_MANAGER,
	ID_TOOL_STATE_MANAGER,
	
	ID_CAMERA_POINTS,
	ID_CAMERA_WIREFRAME,
	ID_CAMERA_SOLID,
	ID_CAMERA_POS_RESET,
	ID_CAMERA_POS_TOP,
	ID_CAMERA_POS_FRONT,
	ID_CAMERA_POS_RIGHT,

	ID_SHOW_PHYSICS_DEBUG,
	ID_SHOW_BOUNDING_BOX,
	ID_SHOW_AXIS,
	ID_WIN_PAGE_PREV,
    ID_WIN_PAGE_NEXT,
    
	//-- code edit
	ID_CODE_INDENTINC,
    ID_CODE_INDENTRED,
    ID_CODE_FINDNEXT,
    ID_CODE_REPLACE,
    ID_CODE_REPLACENEXT,
    ID_CODE_BRACEMATCH,
    ID_CODE_GOTO,
    ID_CODE_PAGEACTIVE,
    ID_CODE_DISPLAYEOL,
    ID_CODE_INDENTGUIDE,
    ID_CODE_LINENUMBER,
    ID_CODE_LONGLINEON,
    ID_CODE_WHITESPACE,
    ID_CODE_FOLDTOGGLE,
    ID_CODE_OVERTYPE,
    ID_CODE_READONLY,
    ID_CODE_WRAPMODEON,
    ID_CODE_CHANGECASE,
    ID_CODE_CHANGELOWER,
    ID_CODE_CHANGEUPPER,
    ID_CODE_HILIGHTLANG,
    ID_CODE_HILIGHTFIRST,
    ID_CODE_HILIGHTLAST = ID_CODE_HILIGHTFIRST + 99,
    ID_CODE_CONVERTEOL,
    ID_CODE_CONVERTCR,
    ID_CODE_CONVERTCRLF,
    ID_CODE_CONVERTLF,
    ID_CODE_USECHARSET,
    ID_CODE_CHARSETANSI,
    ID_CODE_CHARSETMAC,
    ID_CODE_SELECTLINE,	
	ID_CODE_BOOKMARK_TOGGLE,
	ID_CODE_BOOKMARK_CELAR,
	ID_CODE_BOOKMARK_NEXT,
	ID_CODE_BOOKMARK_PREV
};

enum WIN_ID
{
	ID_MAIN_FRAME = wxID_HIGHEST + 1000,
	ID_LOG_BOX,
	ID_LOG_RUNTIME_EDIT,
	ID_CONSOLE_EDIT,
	ID_PROJ_PANEL,
	ID_PROJ_TREE,
	ID_PROJ_LIST,
	ID_PROJ_INSP_WIN,
	ID_PROJ_OBJ_TYPE_CHICE,
	ID_PROJ_OBJ_SEARCH_TEXT,
	ID_PROPS_PANEL,
	ID_PROPS_PROJ,
	ID_BOOK_MAIN,
	ID_BOOK_OUTPUT,
	ID_BOOK_PROP,
	ID_FIND_BUTTON,
	ID_REPLACE_BUTTON,
	ID_REPLACE_ALL_BUTTON,
	ID_NEW_PROJ_WIZARD,
	ID_WIZARD_TEMPL_LIST,

	//-- anim dlg
	ID_ANIM_PROPS,
	ID_ANIM_OBJ_ANIM_LIST,
	ID_ANIM_SCENE_ANIM_LIST,
	ID_ANIM_ADD_BUTTON,
	ID_ANIM_DEL_BUTTON,
	ID_ANIM_PLAY_BUTTON,
	ID_ANIM_CODE_BUTTON,
	ID_ANIM_CREATE_BUTTON,
	ID_ANIM_TIME_SLIDER,
	ID_ANIM_LENGTH_EDIT,
	ID_ANIM_CURPOS_EDIT,
	ID_ANIM_INCPOS_BUTTON,
	ID_ANIM_DECPOS_BUTTON,
	ID_ANIM_POSSTEP_EDIT,
	ID_ANIM_REFRESH_BUTTON,
	ID_ANIM_ENTITY_NAME_EDIT
};

enum ITEM
{
	ITEM_NONE = 0,
	ITEM_BLEND,
	ITEM_LUA,
	ITEM_PROJ,
	ITEM_SCENE,
	ITEM_NULL,
	ITEM_CAMERA,
	ITEM_LIGHT,
	ITEM_ENTITY,
	ITEM_OBJECT,
	ITEM_SKELETON
};

ITEM getLuFileType(const wxString& fileName);
const char* getLuObjectTypeName(gkGameObjectTypes type);

#endif //_LuaDefs_h_