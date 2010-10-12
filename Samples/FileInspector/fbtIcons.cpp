/*
-------------------------------------------------------------------------------
    This file is part of FBT (File Binary Tables).
    http://gamekit.googlecode.com/

    Copyright (c) 2010 Charlie C & Erwin Coumans.

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
#include <wx/imaglist.h>
#include <wx/bitmap.h>
#include "fbtIcons.h"

#include "Resource/system-search.xpm"
#include "Resource/text-x-generic.xpm"
#include "Resource/package-x-generic.xpm"
#include "Resource/folder-open.xpm"
#include "Resource/folder.xpm"
#include "Resource/document-new.xpm"
#include "Resource/applications-other.xpm"
#include "Resource/chunk.xpm"


struct fbtIcon
{
	int id;
	int w, h;
	wxBitmap bitmap;
};



wxImageList* fbtMakeProjectImageList(void)
{
	fbtIcon fbt_project_icons[] =
	{
		{ICO_PRJ_SYSTEM,        16, 16, wxBitmap(system_search_xpm)},
		{ICO_PRJ_TEXT,          16, 16, wxBitmap(text_x_generic_xpm)},
		{ICO_PRJ_TEXT_NEW,      16, 16, wxBitmap(document_new_xpm)},
		{ICO_PRJ_PACKAGE,       16, 16, wxBitmap(package_x_generic_xpm)},
		{ICO_PRJ_FOLDER_CLOSE,  16, 16, wxBitmap(folder_xpm)},
		{ICO_PRJ_FOLDER_OPEN,   16, 16, wxBitmap(folder_open_xpm)},
		{ -1, -1}
	};
	
	wxImageList* il = new wxImageList(16, 16);

	int i = 0;
	while (fbt_project_icons[i].id != -1)
	{
		il->Add(fbt_project_icons[i].bitmap);
		++i;
	}

	return il;
}

wxImageList* fbtMakeInspectorImageList(void)
{
	fbtIcon fbt_inspector_icons[] =
	{
		{ICO_INS_SYSTEM,    16, 16, wxBitmap(package_x_generic_xpm)},
		{ICO_INS_CHUNK,     16, 16, wxBitmap(chunk_xpm)},
		{ICO_INS_FBT,       16, 16, wxBitmap(applications_other_xpm)},
		{ -1, -1}
	};
	
	wxImageList* il = new wxImageList(16, 16);

	int i = 0;
	while (fbt_inspector_icons[i].id != -1)
	{
		il->Add(fbt_inspector_icons[i].bitmap);
		++i;
	}

	return il;
}
