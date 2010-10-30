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
#include "luDefs.h"

ITEM getLuFileType(const wxString& fileName)
{
	wxString ext = wxFileName(fileName).GetExt().Lower();

	if (ext == FILE_EXT_BLEND)
		return ITEM_BLEND;
	else if (ext == FILE_EXT_LUA)
		return ITEM_LUA;
	else if (ext == FILE_EXT_PROJ)
		return ITEM_PROJ;

	return ITEM_NONE;
}

const char* getLuObjectTypeName(gkGameObjectTypes type)
{
	switch (type)
	{
	case GK_OB_NULL: return "Null";
	case GK_CAMERA: return "Camera";
	case GK_LIGHT: return "Light";
	case GK_ENTITY: return "Entity";
	case GK_OBJECT: return "Object";
	case GK_SKELETON: return "Skeleton";
	default:
		break;
	}

	return "Unknown";
}

