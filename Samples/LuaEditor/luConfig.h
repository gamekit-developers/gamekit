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

#ifndef _luConfig_h_
#define _luConfig_h_

#include "Lib/liXmlConfig.h"

class luConfig : public xml::xmlConfig, public utSingleton<luConfig>
{
public:

#ifdef WIN32
	gkString getRuntimePath() { return getString("app.runtime.win32_path"); }
#else
	gkString getRuntimePath() { return getString("app.runtime.linux_path"); }
#endif

	gkString getHelpFilePath()  { return getString("app.help.path"); }
	gkString getHelpTopicFile() { return getString("app.help.topic"); }

	gkString getEditFontName(const gkString& defValue="")	{ return getString("app.edit.font_name", defValue); }
	int	getEditFontSize(int defValue=0)						{ return getInt("app.edit.font_size", defValue); }
	int	getEditLineNumberMargin(int defValue=0)				{ return getInt("app.edit.linenumber_margin", defValue); }
	int	getEditBookmarkMargin(int defValue=0)				{ return getInt("app.edit.bookmark_margin", defValue); }
	int	getEditFolderMargin(int defValue=0)					{ return getInt("app.edit.folder_margin", defValue); }
	gkString getEditKeywords(const gkString& defValue="")	{ return getString("app.edit.keywords", defValue); }
	
	gkString getBaseBlendFile()		{ return getString("app.file.base_blend"); }
	gkString getProjTemplateFile()	{ return getString("app.file.proj_template"); }

	UT_DECLARE_SINGLETON(luConfig)
};




#define LuConfig luConfig::getSingleton()

#endif //_luConfig_h_