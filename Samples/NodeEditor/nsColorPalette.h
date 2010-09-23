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
#ifndef _nsColorPalette_h_
#define _nsColorPalette_h_

#include "nsMath.h"
#include "nsNodeEnums.h"



class nsColorPalette
{
private:
	mutable unsigned int    m_image;
	NScolor                 m_color[NS_PAL_MAX];

public:
	nsColorPalette() : m_image(0)
	{
		m_color[NS_PAL_DARK]           = 0x2b2b2bFF;
		m_color[NS_PAL_MIDDLE]         = 0x808080FF;
		m_color[NS_PAL_LIGHT]          = 0x808080FF;
		m_color[NS_PAL_SHADOW]         = 0x0000004B;
		m_color[NS_PAL_HIGHLIGHT]      = 0xFFFFFFFF;
		m_color[NS_PAL_TEXT]           = 0xFFFFFFC0;
		m_color[NS_PAL_TEXT_HEADER]    = 0x000000FF;
	}

	nsColorPalette(const nsColorPalette& oth) : m_image(oth.m_image)
	{
		for (int i = NS_PAL_DARK; i < NS_PAL_MAX; ++i)
			m_color[i] = oth.m_color[i];
	}

	nsColorPalette(const NScolor* oth) : m_image(0)
	{
		for (int i = NS_PAL_DARK; i < NS_PAL_MAX; ++i)
			m_color[i] = oth[i];
	}


	void setColorColor(int i, const NScolor& val)
	{
		if (i >= 0 && i < NS_PAL_MAX)
			m_color[i] = val;
	}

	const NScolor& getIndexedColor(int i) const
	{
		if (i >= 0 && i < NS_PAL_MAX)
			return m_color[i];

		static const NScolor Blank(1, 1, 1, 1);
		return Blank;
	}

	UT_INLINE unsigned int&  getImage(void)         const   {return m_image;}
	UT_INLINE bool          isImageValid(void)      const   {return m_image != 0;}
	UT_INLINE const NScolor& getDark(void)          const   {return m_color[NS_PAL_DARK];}
	UT_INLINE const NScolor& getMiddle(void)        const   {return m_color[NS_PAL_MIDDLE];}
	UT_INLINE const NScolor& getLight(void)         const   {return m_color[NS_PAL_LIGHT];}
	UT_INLINE const NScolor& getShadow(void)        const   {return m_color[NS_PAL_SHADOW];}
	UT_INLINE const NScolor& getHighlight(void)     const   {return m_color[NS_PAL_HIGHLIGHT];}
	UT_INLINE const NScolor& getText(void)          const   {return m_color[NS_PAL_TEXT];}
	UT_INLINE const NScolor& getTextHeader(void)    const   {return m_color[NS_PAL_TEXT_HEADER];}
};



// Color coding is done by group / socket type
enum nsSystemPaletteEnum
{
	// groups
	NS_SPE_ANIMATION,
	NS_SPE_INPUT,
	NS_SPE_OUTPUT,
	NS_SPE_CONVERTER,


	// sockets
	NS_SPE_BOOL,
	NS_SPE_INT,
	NS_SPE_FLOAT,
	NS_SPE_STRING,
	NS_SPE_VEC2,
	NS_SPE_VEC3,
	NS_SPE_VEC4,
	NS_SPE_QUAT,
	NS_SPE_OBJECT,
};


class nsSystemPalette
{
public:
	static const nsColorPalette& getPalette(int in);
};


#endif//_nsColorPalette_h_
