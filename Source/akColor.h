/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

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

#ifndef AKCOLOR_H
#define AKCOLOR_H


class akColor
{
public:
	enum BlendType
	{
		BT_MIXTURE,
		BT_MULTIPLY,
		BT_ADDITIVE,
		BT_SUBTRACT,
		BT_SCREEN,
		BT_DARKEN,
		BT_LIGHTEN,
		BT_COLOR
	};
	
	akColor(float red, float green, float blue, float alpha=1.0f)
		: r(red), g(green), b(blue), a(alpha)
	{
	}
	
	float r,g,b,a;
};

#endif // AKCOLOR_H
