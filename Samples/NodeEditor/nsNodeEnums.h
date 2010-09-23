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
#ifndef _nsNodeEnums_h_
#define _nsNodeEnums_h_


enum nsColorPaletteEnum
{
	NS_PAL_DARK = 0,
	NS_PAL_MIDDLE,
	NS_PAL_LIGHT,
	NS_PAL_SHADOW,
	NS_PAL_HIGHLIGHT,
	NS_PAL_TEXT,
	NS_PAL_TEXT_HEADER,
	NS_PAL_MAX
};


enum nsDirection
{
	NS_SOCK_NULL,
	NS_SOCK_IN,
	NS_SOCK_OUT,
};


enum nsNodeTypes
{
	NT_NULL,
	NT_SKELETON,

	NT_MOUSE_MOTION,
	NT_MOUSE_BUTTON,

	NT_OBJECT_MOTION,

	NT_BTI,
	NT_BTF,

	NT_ITB,
	NT_ITF,

	NT_FTB,
	NT_FTI,

	NT_MAX
};


enum nsSocketAlignment
{
	NS_SAL_TOP = 0,
	NS_SAL_BOTTOM,
};

enum nsGroupTypes
{
	NT_GROUP_ANIMATION,
	NT_GROUP_USER_INPUT,
	NT_GROUP_OUTPUT,
	NT_GROUP_CONVERTER,
	NT_GROUP_MAX,
};

static const float nsNodeHeaderSize       = 24.f;
static const float nsNodeEditSize         = 20.f;
static const float nsNodeEditOffset       = 2.f;
static const float nsDefaultSocketSize    = 14.f;
static const float nsDefaultSocketOffset  = (nsDefaultSocketSize + 5.f);

#define NS_MAKE_NODE_DEF_WIDTH(size) ((nsNodeHeaderSize * .5f) * size)

// default min / max values
#define NS_LINEAR_MAX    100000
#define NS_LINEAR_MIN   -(NS_LINEAR_MAX)
#define NS_ROT_MAX      360.0
#define NS_ROT_MIM      -(NS_ROT_MAX)
#define NS_STRING_MAX   32.0
#define NS_STRING_MIN   0.0



#endif//_nsNodeEnums_h_
