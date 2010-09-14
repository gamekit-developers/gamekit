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
#include <wx/app.h>
#include <wx/image.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include <wx/fontdlg.h>
#include <wx/statbox.h>
#include <wx/button.h>
#include <wx/filedlg.h>
#include <wx/font.h>
#include <wx/dialog.h>
#include <wx/dcmemory.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/font.h>
#include <wx/gauge.h>
#include <wx/spinctrl.h>
#include <wx/checkbox.h>

#include <vector>

#define CHARBEG         32
#define CHAREND         127
#define TOT (CHAREND - CHARBEG)


#define ID_FONT     10002
#define ID_SFONT    10003

#define SIZEX 255
#define SIZEY 72
#define PAD   20
#define PADS  10

static int pow2( int n )
{
	--n;
	n |= n >> 16;
	n |= n >> 8;
	n |= n >> 4;
	n |= n >> 2;
	n |= n >> 1;
	++n;
	return n;
}

static bool SAVE_IMA = false;
static wxString NAME = wxT( "" );




typedef struct wxTextExtents
{
	float width, height;
} wxTextExtents;

typedef struct wxCharacter
{
	float xco, yco;
	float width, height;
} wxCharacter;


class wxGLFont
{
public:

	typedef std::vector<wxCharacter*> CharacterVector;

public:
	wxGLFont( const wxFont& font, int size );
	~wxGLFont();

	void Save( const wxString& out );


private:
	void Create();
	void Destory();


	unsigned char* m_imaData;

	wxPoint FindSize( int res );

	int m_offset, m_maxChar;
	float m_width, m_height;
	float m_baseHeight;
	float m_baseWidth;
	wxCoord m_res;
	wxFont m_font;
	CharacterVector m_chars;
};


wxGLFont::wxGLFont( const wxFont& font, int size )
{
	m_imaData = 0;
	m_offset = 0;

	m_width = 2;
	m_height = 2;
	m_baseHeight = 2;
	m_res = size;

	m_font = font;
	Create();
}


wxGLFont::~wxGLFont()
{
	Destory();
}

void wxGLFont::Destory()
{
	if ( m_imaData )
		delete [] m_imaData;
	m_imaData = 0;

	for ( wxGLFont::CharacterVector::iterator it = m_chars.begin();
	        it != m_chars.end(); ++it )
		delete ( *it );

	m_chars.clear();
}

wxPoint wxGLFont::FindSize( int res )
{
	wxMemoryDC dc;

	m_font.SetPointSize( res );
	dc.SetFont( m_font );

	wxCoord w, h;
	wxCoord mw = 0.0, mh = 0.0;
	wxCoord pad = 5;

	for ( size_t i = CHARBEG; i < CHAREND; i++ )
	{
		wxString str( ( wxChar )i );


		dc.GetTextExtent( str, &w, &h );

		mw = wxMax( w, mw );
		mh = wxMax( h, mh );
	}

	wxCoord r = ( mw + pad ) * ( mh + pad ) * TOT;
	wxCoord t = ( wxCoord )sqrtf( ( float )r );
	t += wxMax( mw, mh );

	wxCoord p = pow2( t );

	wxCoord wi = p, he = p;

	if ( p* p * 0.5 >=  r )
		he = p * 0.5;

	return wxPoint( wi, he );
}


void wxGLFont::Create()
{
	Destory();

	wxCoord padding = 5;

	wxPoint pt = FindSize( m_res );
	m_width = pt.x; m_height = pt.y;

	wxMemoryDC dc;
	wxBitmap bitmap( m_width, m_height );
	dc.SelectObject( bitmap );
	m_font.SetPointSize( m_res );
	dc.SetBackground( wxColour( 0, 0, 0, 0 ) );
	dc.Clear();
	dc.SetFont( m_font );
	dc.SetTextForeground( wxColour( 0xFFFFFF ) );


	wxCoord xpos = 0, ypos = 0;

	for ( size_t i = CHARBEG; i < CHAREND; i++ )
	{
		wxString str( ( wxChar )i );

		wxCharacter* ch = new wxCharacter;

		ch->xco = ch->yco = 0;
		ch->width = ch->height = 0;
		m_chars.push_back( ch );


		wxCoord w, h;
		dc.GetTextExtent( str, &w, &h );
		ch->xco = xpos;
		ch->yco = ypos;
		ch->width = w;
		ch->height = h;

		m_baseHeight = wxMax( m_baseHeight, h );
		m_baseWidth  = wxMax( m_baseWidth, w );

		/* next char */
		dc.DrawText( str, xpos, ypos );

		xpos += ( w + padding );

		if ( ( xpos + ( w + padding ) ) > ( m_width - m_res ) )
		{
			xpos = 0;
			ypos += ( h + padding );
		}
	}

	wxImage ima = bitmap.ConvertToImage();

	if ( SAVE_IMA )
	{
		wxString tname = NAME;
		tname.Replace( wxT( ".font" ), wxT( "" ) );
		tname = wxString::Format( wxT( "%s%i.png" ), tname, m_res );

		ima.SaveFile( tname );
	}

	int size = m_width * m_height;

	m_imaData = new unsigned char[size];
	unsigned char* pixels = ima.GetData();

	/* convert to alpha */
	for ( int y = 0; y < m_height; y++ )
	{
		for ( int x = 0; x < m_width; x++ )
		{
			int in  = ( y * m_width + x ) * 3;
			int out = ( y * m_width + x );

			/* just take red */
			m_imaData[out+0] = pixels[in];
		}
	}
}


void wxGLFont::Save( const wxString& out )
{
	// save to C++ compile-able file
}


class FontFrame : public wxDialog
{
public:
	FontFrame();
	virtual ~FontFrame();

	void initialize();

	void OnQuit( wxCloseEvent& evt );
	void OnSelectFont( wxCommandEvent& evt );

	class wxGLFont* m_font;
	wxSpinButton* m_resGetter;
	int mRes;

	DECLARE_EVENT_TABLE();
};

FontFrame::FontFrame() :
	wxDialog( NULL, wxID_ANY, wxT( "Font Generator" ), wxDefaultPosition, wxSize( SIZEX, SIZEY ), wxDEFAULT_DIALOG_STYLE )
{

	SetMinSize( wxSize( SIZEX, SIZEY ) );
	m_font = 0;
	m_resGetter = 0;
}

FontFrame::~FontFrame()
{
	if ( m_font != 0 )
	{
		delete m_font;
		m_font = 0;
	}

}

void FontFrame::initialize()
{
	Show( true );


	wxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );
	sizer->SetMinSize( wxSize( SIZEY * 2, SIZEY ) );

	wxButton* button = new wxButton( this, ID_FONT,  "Select Font:" );
	sizer->Add( button, wxSizerFlags(0).Align(wxCENTER).Border(wxALL, 10));

	SetSizer( sizer );
	sizer->SetSizeHints( this );

}


void FontFrame::OnQuit( wxCloseEvent& evt )
{
	Destroy();
}

void FontFrame::OnSelectFont( wxCommandEvent& evt )
{
	wxFontDialog dlg( this );

	if ( dlg.ShowModal() ==  wxID_OK )
	{
		wxFont font = dlg.GetFontData().GetChosenFont();
		if ( font.IsOk() )
		{
			wxFileDialog dlg( this, wxT( "Save Font File" ), wxT( "" ), wxT( "" ), wxT( "C++ Font files (*.inl)|*.inl" ) );
			if ( dlg.ShowModal() ==  wxID_OK )
			{
				wxGLFont out(font, font.GetPointSize());
				out.Save(dlg.GetPath());
			}
		}
	}
}

BEGIN_EVENT_TABLE( FontFrame, wxDialog )
	EVT_CLOSE( FontFrame::OnQuit )
	EVT_BUTTON( ID_FONT, FontFrame::OnSelectFont )
END_EVENT_TABLE()



class Application : public wxApp
{
public:
	bool OnInit()
	{
		wxInitAllImageHandlers();

		FontFrame* app = new FontFrame();
		app->initialize();
		return true;
	}
};


IMPLEMENT_APP( Application );
