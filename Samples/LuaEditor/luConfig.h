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

	wxColour getDefaultFontColor()		{ return getColor("app.edit_font_color.default"); }
	wxColour getCommentFontColor()		{ return getColor("app.edit_font_color.comment"); }
	wxColour getCommentLineFontColor()	{ return getColor("app.edit_font_color.comment_line"); }
	wxColour getCommentDocFontColor()	{ return getColor("app.edit_font_color.comment_doc"); }
	wxColour getNumberFontColor()		{ return getColor("app.edit_font_color.number"); }
	wxColour getWordFontColor()			{ return getColor("app.edit_font_color.word"); }
	wxColour getStringFontColor()		{ return getColor("app.edit_font_color.string"); }
	wxColour getIdentifierFontColor()	{ return getColor("app.edit_font_color.identifier"); }
	wxColour getPreprocessorFontColor()	{ return getColor("app.edit_font_color.preprocessor"); }
	wxColour getOperatorFontColor()		{ return getColor("app.edit_font_color.operator"); }

	wxColour getDefaultFontBgColor()		{ return getColor("app.edit_font_bgcolor.default", *wxWHITE); }
	wxColour getCommentFontBgColor()		{ return getColor("app.edit_font_bgcolor.comment", *wxWHITE); }
	wxColour getCommentLineFontBgColor()	{ return getColor("app.edit_font_bgcolor.comment_line", *wxWHITE); }
	wxColour getCommentDocFontBgColor()		{ return getColor("app.edit_font_bgcolor.comment_doc", *wxWHITE); }
	wxColour getNumberFontBgColor()			{ return getColor("app.edit_font_bgcolor.number", *wxWHITE); }
	wxColour getWordFontBgColor()			{ return getColor("app.edit_font_bgcolor.word", *wxWHITE); }
	wxColour getStringFontBgColor()			{ return getColor("app.edit_font_bgcolor.string", *wxWHITE); }
	wxColour getIdentifierFontBgColor()		{ return getColor("app.edit_font_bgcolor.identifier", *wxWHITE); }
	wxColour getPreprocessorFontBgColor()	{ return getColor("app.edit_font_bgcolor.preprocessor", *wxWHITE); }
	wxColour getOperatorFontBgColor()		{ return getColor("app.edit_font_bgcolor.operator", *wxWHITE); }

	bool getDefaultFontBold()			{ return getBool("app.edit_font_bold.default"); }
	bool getCommentFontBold()			{ return getBool("app.edit_font_bold.comment"); }
	bool getCommentLineFontBold()		{ return getBool("app.edit_font_bold.comment_line"); }
	bool getCommentDocFontBold()		{ return getBool("app.edit_font_bold.comment_doc"); }
	bool getNumberFontBold()			{ return getBool("app.edit_font_bold.number"); }
	bool getWordFontBold()				{ return getBool("app.edit_font_bold.word"); }
	bool getStringFontBold()			{ return getBool("app.edit_font_bold.string"); }
	bool getIdentifierFontBold()		{ return getBool("app.edit_font_bold.identifier"); }
	bool getPreprocessorFontBold()		{ return getBool("app.edit_font_bold.preprocessor"); }
	bool getOperatorFontBold()			{ return getBool("app.edit_font_bold.operator"); }

	wxColour getColor(const gkString& name, const wxColour& defValue=*wxBLACK);

	UT_DECLARE_SINGLETON(luConfig)
};




#define LuConfig luConfig::getSingleton()

#endif //_luConfig_h_