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

	gkString getRuntimePath();

	gkString getHelpFilePath()								{ return getString("app.help.path"); }
	gkString getHelpTopicFile()								{ return getString("app.help.topic"); }
		
	gkString getBaseBlendFile()								{ return getString("app.file.base_blend"); }
	gkString getProjTemplateFile()							{ return getString("app.file.proj_template"); }

	//-- edit

	int getEditTabSize(int defValue=4)						{ return getInt("app.edit.tab_size", defValue); }
	int getEditIndentSize(int defValue=4)					{ return getInt("app.edit.indent_size", defValue); }
	gkString getEditFontName(const gkString& defValue="")	{ return getString("app.edit.font_name", defValue); }
	int	getEditFontSize(int defValue=0)						{ return getInt("app.edit.font_size", defValue); }
	int	getEditLineNumberMargin(int defValue=0)				{ return getInt("app.edit.linenumber_margin", defValue); }
	int	getEditBookmarkMargin(int defValue=0)				{ return getInt("app.edit.bookmark_margin", defValue); }
	int	getEditFolderMargin(int defValue=0)					{ return getInt("app.edit.folder_margin", defValue); }
	gkString getEditKeywords(const gkString& defValue="")	{ return getString("app.edit.keywords", defValue); }

	bool getEditViewLineNumber()							{ return getBool("app.edit_view.line_number"); }		
	bool getEditViewFolder()								{ return getBool("app.edit_view.folder"); }
	bool getEditViewBookmark()								{ return getBool("app.edit_view.bookmark"); }
	bool getEditViewLongLineMark()							{ return getBool("app.edit_view.long_line_mark"); }
	bool getEditViewEOL()									{ return getBool("app.edit_view.eol"); }
	bool getEditViewWhiteSpace()							{ return getBool("app.edit_view.white_space"); }
	bool getEditViewIndentGuide()							{ return getBool("app.edit_view.indent_guide"); }

	wxColour getEditDefaultFontColor()						{ return getColor("app.edit_font_color.default"); }
	wxColour getEditCommentFontColor()						{ return getColor("app.edit_font_color.comment"); }
	wxColour getEditCommentLineFontColor()					{ return getColor("app.edit_font_color.comment_line"); }
	wxColour getEditCommentDocFontColor()					{ return getColor("app.edit_font_color.comment_doc"); }
	wxColour getEditNumberFontColor()						{ return getColor("app.edit_font_color.number"); }
	wxColour getEditWordFontColor()							{ return getColor("app.edit_font_color.word"); }
	wxColour getEditStringFontColor()						{ return getColor("app.edit_font_color.string"); }
	wxColour getEditIdentifierFontColor()					{ return getColor("app.edit_font_color.identifier"); }
	wxColour getEditPreprocessorFontColor()					{ return getColor("app.edit_font_color.preprocessor"); }
	wxColour getEditOperatorFontColor()						{ return getColor("app.edit_font_color.operator"); }

	wxColour getEditDefaultFontBgColor()					{ return getColor("app.edit_font_bgcolor.default", *wxWHITE); }
	wxColour getEditCommentFontBgColor()					{ return getColor("app.edit_font_bgcolor.comment", *wxWHITE); }
	wxColour getEditCommentLineFontBgColor()				{ return getColor("app.edit_font_bgcolor.comment_line", *wxWHITE); }
	wxColour getEditCommentDocFontBgColor()					{ return getColor("app.edit_font_bgcolor.comment_doc", *wxWHITE); }
	wxColour getEditNumberFontBgColor()						{ return getColor("app.edit_font_bgcolor.number", *wxWHITE); }
	wxColour getEditWordFontBgColor()						{ return getColor("app.edit_font_bgcolor.word", *wxWHITE); }
	wxColour getEditStringFontBgColor()						{ return getColor("app.edit_font_bgcolor.string", *wxWHITE); }
	wxColour getEditIdentifierFontBgColor()					{ return getColor("app.edit_font_bgcolor.identifier", *wxWHITE); }
	wxColour getEditPreprocessorFontBgColor()				{ return getColor("app.edit_font_bgcolor.preprocessor", *wxWHITE); }
	wxColour getEditOperatorFontBgColor()					{ return getColor("app.edit_font_bgcolor.operator", *wxWHITE); }

	bool getEditDefaultFontBold()							{ return getBool("app.edit_font_bold.default"); }
	bool getEditCommentFontBold()							{ return getBool("app.edit_font_bold.comment"); }
	bool getEditCommentLineFontBold()						{ return getBool("app.edit_font_bold.comment_line"); }
	bool getEditCommentDocFontBold()						{ return getBool("app.edit_font_bold.comment_doc"); }
	bool getEditNumberFontBold()							{ return getBool("app.edit_font_bold.number"); }
	bool getEditWordFontBold()								{ return getBool("app.edit_font_bold.word"); }
	bool getEditStringFontBold()							{ return getBool("app.edit_font_bold.string"); }
	bool getEditIdentifierFontBold()						{ return getBool("app.edit_font_bold.identifier"); }
	bool getEditPreprocessorFontBold()						{ return getBool("app.edit_font_bold.preprocessor"); }
	bool getEditOperatorFontBold()							{ return getBool("app.edit_font_bold.operator"); }

	wxColour getColor(const gkString& name, const wxColour& defValue=*wxBLACK);

	UT_DECLARE_SINGLETON(luConfig)
};




#define LuConfig luConfig::getSingleton()

#endif //_luConfig_h_