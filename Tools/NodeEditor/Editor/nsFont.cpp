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
#include "nsFont.h"
#include <wx/image.h>
#include <wx/font.h>
#include <wx/dcclient.h>
#include <wx/dcmemory.h>



nsFont::nsFont(const wxFont& font, int size)
{
	// wxInitAllImageHandlers();
	m_imaData       = 0;
	m_offset        = 0;
	m_tex           = 0;
	m_width         = 2;
	m_height        = 2;
	m_baseHeight    = 2;
	m_res           = size;
	m_font          = font;
	doCreate();
}


nsFont::~nsFont()
{
	doDestroy();
}


nsChar nsFont::getChar( char ch ) const
{
	int pos = ( int )ch;

	nsChar r = {0.f, 0.f, 0.f, 0.f};

	if ( pos >= NS_CHAR_START && pos <= NS_CHAR_END && !m_chars.empty())
	{
		// offset form zero
		r = *m_chars[pos - NS_CHAR_START];
	}
	return r;
}


NSfloat nsFont::getPixelWidth(NSfloat size, const char* str, int len)
{
	NSfloat scale = size / (NSfloat)m_res;
	NSfloat xoffs = 0.f;
	NSfloat bh = getChar( '0' ).height;

	NSfloat ret = 0;
	for ( int i = 0; i < len; i++ )
	{
		char ch = str[i];
		nsChar fc = getChar( ch );

		if ( ch == ' ' )
			xoffs += fc.width * scale;
		else if ( ch == '\t' )
			xoffs += getChar( ' ' ).width * 4 * scale;
		else if ( ch == '\n' )
			continue;
		else
			xoffs += fc.width * scale;


		ret = NSMax( ret, xoffs );
	}
	return ret;
}


// Render font to single channel image
void nsFont::doCreate(void)
{
	const wxCoord padding = NS_CHAR_PADDING;

	wxPoint pt = findBestSize(m_res);
	m_width = pt.x; m_height = pt.y;


	wxMemoryDC dc;
	wxBitmap bitmap(m_width, m_height);
	dc.SelectObject(bitmap);
	m_font.SetPointSize(m_res);
	dc.SetBackground(*wxBLACK);
	dc.Clear();
	dc.SetFont(m_font);
	dc.SetTextForeground(*wxWHITE);

	wxCoord xpos = 0, ypos = 0;

	// allocate total characters
	m_chars.reserve(NS_TOTAL_CHAR);

	for (size_t i = NS_CHAR_START; i < NS_CHAR_END; i++)
	{
		wxString str((wxChar)i);
		nsChar* ch = new nsChar;

		wxCoord w, h;
		dc.GetTextExtent(str, &w, &h);
		ch->xco     = xpos;
		ch->yco     = ypos;
		ch->width   = w;
		ch->height  = h;

		m_chars.push_back(ch);

		m_baseHeight = wxMax(m_baseHeight, h);
		m_baseWidth  = wxMax(m_baseWidth, w);

		dc.DrawText(str, xpos, ypos);

		xpos += (w + padding);
		if ( (xpos + (w + padding)) > (m_width - m_res) )
		{
			xpos = 0;
			ypos += (h + padding);
		}
	}

	// get pixel data
	wxImage ima = bitmap.ConvertToImage();
	//ima.SaveFile(m_font.GetFaceName() + ".png");
	int size = m_width * m_height;

	m_imaData = new unsigned char[size];
	unsigned char* pixels = ima.GetData();

	// convert to alpha/luminosity
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			int in  = (y * m_width + x) * 3;
			int out = (y * m_width + x);

			// just take red component
			m_imaData[out] = pixels[in];
		}
	}

	dc.SelectObject(wxNullBitmap);
}



void nsFont::doDestroy(void)
{
	if (m_imaData)
		delete [] m_imaData;
	m_imaData = 0;

	CharacterVector::Pointer ptr = m_chars.ptr();
	UTsize nr = m_chars.size(), i = 0;

	while (i < nr)
		delete ptr[i++];

	m_chars.clear();
}



// lookup for the best fitting image size
wxPoint nsFont::findBestSize(int res)
{
	wxMemoryDC dc;

	m_font.SetPointSize(res);
	dc.SetFont(m_font);

	wxCoord w, h;
	wxCoord mw = 0.0, mh = 0.0;
	const wxCoord pad = NS_CHAR_PADDING;


	for (size_t i = NS_CHAR_START; i < NS_CHAR_END; i++)
	{
		wxString str((wxChar)i);
		dc.GetTextExtent(str, &w, &h);

		mw = wxMax(w, mw);
		mh = wxMax(h, mh);
	}

	wxCoord r = (mw + pad) * (mh + pad) * NS_TOTAL_CHAR;
	wxCoord t = (wxCoord)sqrtf((float)r);
	t += wxMax(mw, mh);
	wxCoord p = NSPow2(t);

	wxCoord wi = p, he = p;
	he = (wxCoord)(p * 0.5);
	return wxPoint(wi, he);
}
