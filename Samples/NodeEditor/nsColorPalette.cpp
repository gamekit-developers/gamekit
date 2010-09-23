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
#include "nsColorPalette.h"


static const NScolor BLACKOPAQUE                    = 0x000000FF;
static const NScolor BLACKALPHA                     = 0x0000004B;
static const NScolor BLACKALPHA2                    = 0x00000080;
static const NScolor WHITEOPAQUE                    = 0xFFFFFFFF;
static const NScolor WHITEALPHA                     = 0xFFFFFFC0;
static const NScolor SELECTED                       = 0xEBE0714B;
static const NScolor MIDDLE_GREY                    = 0x797979FF;
static const NScolor RED[NS_PAL_MAX]                = {0xA83232FF, MIDDLE_GREY, 0xEB7171FF, BLACKALPHA, WHITEOPAQUE, WHITEALPHA, BLACKOPAQUE};
static const NScolor YELLOW[NS_PAL_MAX]             = {0xA89E32FF, MIDDLE_GREY, 0xEBE071FF, BLACKALPHA, WHITEOPAQUE, WHITEALPHA, BLACKOPAQUE};
static const NScolor BLUE[NS_PAL_MAX]               = {0x335EA8FF, MIDDLE_GREY, 0x70B1EBFF, BLACKALPHA, WHITEOPAQUE, WHITEALPHA, BLACKALPHA2};
static const NScolor ORANGE[NS_PAL_MAX]             = {0xA85A32FF, MIDDLE_GREY, 0xEBAD71FF, BLACKALPHA, WHITEOPAQUE, WHITEALPHA, BLACKOPAQUE};
static const NScolor GREEN[NS_PAL_MAX]              = {0x91A832FF, MIDDLE_GREY, 0xD2EB71FF, BLACKALPHA, WHITEOPAQUE, WHITEALPHA, BLACKOPAQUE};
static const NScolor PURPLE[NS_PAL_MAX]             = {0x5832A8FF, MIDDLE_GREY, 0x9771EBFF, BLACKALPHA, WHITEOPAQUE, WHITEALPHA, BLACKOPAQUE};
static const NScolor ORANGE_RED[NS_PAL_MAX]         = {0xA84232FF, MIDDLE_GREY, 0xEB8171FF, BLACKALPHA, WHITEOPAQUE, WHITEALPHA, BLACKOPAQUE};
static const NScolor YELLOW_ORANGE[NS_PAL_MAX]      = {0xA87132FF, MIDDLE_GREY, 0xEBB271FF, BLACKALPHA, WHITEOPAQUE, WHITEALPHA, BLACKOPAQUE};
static const NScolor YELLOW_GREEN[NS_PAL_MAX]       = {0x6DA832FF, MIDDLE_GREY, 0xAEEB71FF, BLACKALPHA, WHITEOPAQUE, WHITEALPHA, BLACKOPAQUE};
static const NScolor BLUE_GREEN[NS_PAL_MAX]         = {0x3295A8FF, MIDDLE_GREY, 0x71D6EBFF, BLACKALPHA, WHITEOPAQUE, WHITEALPHA, BLACKOPAQUE};
static const NScolor BLUE_PURPLE[NS_PAL_MAX]        = {0x3C32A8FF, MIDDLE_GREY, 0x7B71EBFF, BLACKALPHA, WHITEOPAQUE, WHITEALPHA, BLACKOPAQUE};
static const NScolor RED_PURPLE[NS_PAL_MAX]         = {0xA832A8FF, MIDDLE_GREY, 0xEB71EBFF, BLACKALPHA, WHITEOPAQUE, WHITEALPHA, BLACKOPAQUE};
static const NScolor GREY[NS_PAL_MAX]               = {0x292929FF, MIDDLE_GREY, 0x808080FF, BLACKALPHA, WHITEOPAQUE, WHITEALPHA, BLACKOPAQUE};
static const NScolor GREY2[NS_PAL_MAX]              = {0x2B2B2BFF, MIDDLE_GREY, 0xC0C0C0FF, BLACKALPHA, WHITEOPAQUE, WHITEALPHA, BLACKOPAQUE};


static nsColorPalette NS_SPE_ANIMATION_PAL(RED);
static nsColorPalette NS_SPE_INPUT_PAL(YELLOW_GREEN);
static nsColorPalette NS_SPE_OUTPUT_PAL(YELLOW_ORANGE);
static nsColorPalette NS_SPE_CONVERTER_PAL(RED_PURPLE);



static nsColorPalette NS_SPE_BOOL_PAL(ORANGE);
static nsColorPalette NS_SPE_INT_PAL(BLUE_GREEN);
static nsColorPalette NS_SPE_FLOAT_PAL(BLUE);
static nsColorPalette NS_SPE_STRING_PAL(RED);
static nsColorPalette NS_SPE_DEF_PAL(GREY);
static nsColorPalette NS_SPE_OBJECT_PAL(GREY2);



const nsColorPalette& nsSystemPalette::getPalette(int in)
{
#define PAL_CASE(x) case x: return x##_PAL;
	switch (in)
	{
		PAL_CASE(NS_SPE_ANIMATION);
		PAL_CASE(NS_SPE_INPUT);
		PAL_CASE(NS_SPE_OUTPUT);
		PAL_CASE(NS_SPE_CONVERTER);
		PAL_CASE(NS_SPE_BOOL);
		PAL_CASE(NS_SPE_INT);
		PAL_CASE(NS_SPE_FLOAT);
		PAL_CASE(NS_SPE_STRING);
		PAL_CASE(NS_SPE_OBJECT)
	}
	return NS_SPE_DEF_PAL;
}
