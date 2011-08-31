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

#include <Rocket/Core.h>

#define READ_FONT_FROM_FILE 0

#if READ_FONT_FROM_FILE == 0
#include "fonts/uifonts.inl"
#endif

void installRocketFonts()
{
#if READ_FONT_FROM_FILE
		// Load the fonts from the path to the sample directory.
		Rocket::Core::FontDatabase::LoadFontFace("fonts/Delicious_Roman.otf");
		Rocket::Core::FontDatabase::LoadFontFace("fonts/Delicious_Bold.otf");
		Rocket::Core::FontDatabase::LoadFontFace("fonts/Delicious_Italic.otf");
		Rocket::Core::FontDatabase::LoadFontFace("fonts/Delicious_BoldItalic.otf");
#else
		const Rocket::Core::Font::Style snormal = Rocket::Core::Font::STYLE_NORMAL;
		const Rocket::Core::Font::Style sitalic = Rocket::Core::Font::STYLE_ITALIC;
		const Rocket::Core::Font::Weight wbold = Rocket::Core::Font::WEIGHT_BOLD;
		const Rocket::Core::Font::Weight wnormal = Rocket::Core::Font::WEIGHT_NORMAL;
		const char* family = "Delicious";
	
		Rocket::Core::FontDatabase::LoadFontFace(DELICIOUS_ROMAN, DELICIOUS_ROMAN_SIZE, family, snormal, wnormal);
		Rocket::Core::FontDatabase::LoadFontFace(DELICIOUS_BOLD, DELICIOUS_BOLD_SIZE, family, snormal, wbold);
		Rocket::Core::FontDatabase::LoadFontFace(DELICIOUS_ITALIC, DELICIOUS_ITALIC_SIZE, family, sitalic, wnormal);
		Rocket::Core::FontDatabase::LoadFontFace(DELICIOUS_BOLDITALIC, DELICIOUS_BOLDITALIC_SIZE, family, sitalic, wbold);
#endif
}