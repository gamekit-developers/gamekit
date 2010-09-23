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
#ifndef _nsFont_h_
#define _nsFont_h_

#include "nsCommon.h"
#include "nsMath.h"
#include <wx/font.h>


typedef struct nsTextExtents
{
	NSfloat width, height;
} nsTextExtents;

typedef struct nsChar
{
	NSfloat xco, yco;
	NSfloat width, height;
} nsChar;


#define NS_CHAR_START   32
#define NS_CHAR_END     127
#define NS_TOTAL_CHAR   (NS_CHAR_END - NS_CHAR_START)
#define NS_CHAR_PADDING  5




// WX -> GL font converter
class nsFont
{
public:
	typedef utArray<nsChar*> CharacterVector;


public:
	nsFont(const wxFont& font, int size);
	~nsFont();

	nsChar      getChar( char ch ) const;
	NSfloat     getPixelWidth(NSfloat size, const char* str, int len);

private:
	friend class nsRenderSystemPrivate;

	void        doCreate(void);
	void        doDestroy(void);
	wxPoint     findBestSize(int res);


	unsigned char*   m_imaData;
	unsigned int    m_tex;
	int             m_offset, m_maxChar;
	float           m_width, m_height;
	float           m_baseHeight;
	float           m_baseWidth;
	wxCoord         m_res;
	wxFont          m_font;
	CharacterVector m_chars;
};

#endif//_nsFont_h_
