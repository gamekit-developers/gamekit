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
#include <wx/font.h>
#include <wx/settings.h>
#include <wx/glcanvas.h>
#include <GL/glu.h>

#ifndef GL_CLAMP_TO_EDGE
# define GL_CLAMP_TO_EDGE 0x812F
#endif

#include "nsRenderSystem.h"
#include "nsNodeManager.h"
#include "nsFont.h"



#define SOCKET_LINK_LINE    0
#define SOCKET_LINK_BLOCK   1
#define SOCKET_LINK_CURVE   2
#define SOCKET_LINK         SOCKET_LINK_CURVE



// ----------------------------------------------------------------------------
NS_IMPLEMENT_SINGLETON(nsRenderSystem);


// ----------------------------------------------------------------------------
// wxColor converters

NScolor ColorFromWxColor(const wxColour &v)
{
    return NScolor(
               NScolorf(v.Red()),
               NScolorf(v.Green()),
               NScolorf(v.Blue()),
               1.f);
}

enum Palette
{
    PAL_DARK=0,
    PAL_LIGHT,
    PAL_SELECTED,
    PAL_SHADOW,
    PAL_HIGHLIGHT,
    PAL_TEXT,
    PAL_TEXT_HEADER,
    PAL_MAX
};

static const NScolor BLACKOPAQUE    = 0x000000FF;
static const NScolor BLACKALPHA     = 0x0000004B;
static const NScolor BLACKALPHA2    = 0x00000080;
static const NScolor WHITEOPAQUE    = 0xFFFFFFFF;
static const NScolor WHITEALPHA     = 0xFFFFFFC0;
static const NScolor SELECTED       = 0xEBE0714B;


// Standard color wheel  hex(RRGGBBAA)     Dark        Light       Selected    Shadow      Highlight   Text        Text Header
static const NScolor RED[]              = {0xA83232FF, 0xEB7171FF, SELECTED, BLACKALPHA, WHITEOPAQUE, WHITEALPHA, BLACKOPAQUE};
static const NScolor YELLOW[]           = {0xA89E32FF, 0xEBE071FF, SELECTED, BLACKALPHA, WHITEOPAQUE, WHITEALPHA, BLACKOPAQUE};
static const NScolor BLUE[]             = {0x335EA8FF, 0x70B1EBFF, SELECTED, BLACKALPHA, WHITEOPAQUE, WHITEALPHA, BLACKALPHA2};
static const NScolor ORANGE[]           = {0xA85A32FF, 0xEBAD71FF, SELECTED, BLACKALPHA, WHITEOPAQUE, WHITEALPHA, BLACKOPAQUE};
static const NScolor GREEN[]            = {0x91A832FF, 0xD2EB71FF, SELECTED, BLACKALPHA, WHITEOPAQUE, WHITEALPHA, BLACKOPAQUE};
static const NScolor PURPLE[]           = {0x5832A8FF, 0x9771EBFF, SELECTED, BLACKALPHA, WHITEOPAQUE, WHITEALPHA, BLACKOPAQUE};
static const NScolor ORANGE_RED[]       = {0xA84232FF, 0xEB8171FF, SELECTED, BLACKALPHA, WHITEOPAQUE, WHITEALPHA, BLACKOPAQUE};
static const NScolor YELLOW_ORANGE[]    = {0xA87132FF, 0xEBB271FF, SELECTED, BLACKALPHA, WHITEOPAQUE, WHITEALPHA, BLACKOPAQUE};
static const NScolor YELLOW_GREEN[]     = {0x6DA832FF, 0xAEEB71FF, SELECTED, BLACKALPHA, WHITEOPAQUE, WHITEALPHA, BLACKOPAQUE};
static const NScolor BLUE_GREEN[]       = {0x3295A8FF, 0x71D6EBFF, SELECTED, BLACKALPHA, WHITEOPAQUE, WHITEALPHA, BLACKOPAQUE};
static const NScolor BLUE_PURPLE[]      = {0x3C32A8FF, 0x7B71EBFF, SELECTED, BLACKALPHA, WHITEOPAQUE, WHITEALPHA, BLACKOPAQUE};
static const NScolor RED_PURPLE[]       = {0xA832A8FF, 0xEB71EBFF, SELECTED, BLACKALPHA, WHITEOPAQUE, WHITEALPHA, BLACKOPAQUE};
static const NScolor GREY[]             = {0x292929FF, 0x808080FF, SELECTED, BLACKALPHA, WHITEOPAQUE, WHITEALPHA, BLACKOPAQUE};

struct NSPalette
{
    GLuint image;
    const NScolor *col;
};


static NSPalette STDPalette[NS_COL_MAX] =
{
    {0, RED},
    {0, YELLOW},
    {0, BLUE},
    {0, ORANGE},
    {0, GREEN},
    {0, PURPLE},
    {0, ORANGE_RED},
    {0, YELLOW_ORANGE},
    {0, YELLOW_GREEN},
    {0, BLUE_GREEN},
    {0, BLUE_PURPLE},
    {0, RED_PURPLE},
    {0, GREY},
};

// ----------------------------------------------------------------------------
// Style info for the node trees, could be loaded/saved later.
class nsStyle
{
public:
    NScolor     m_background;
    NScolor     m_nodeBack1;
    NScolor     m_nodeBack2;
    NScolor     m_nodeHeader1;
    NScolor     m_nodeHeader2;
    NScolor     m_black, m_white;
    NScolor     m_shadow, m_refl;

    NScolor     m_sysHighlight, m_sysHighlightGrad;

    nsStyle() {}

    void loadDefaultSettings(void)
    {
        m_black         = 0x000000FF;
        m_white         = 0xFFFFFFFF;
        m_shadow        = 0x0000004B;
        m_refl          = 0xFFFFFF2B;
        m_background    = GREY[PAL_LIGHT];
        m_nodeBack1     = m_background - 0.125f;
        m_nodeBack1.a   = 1.f;
        m_nodeBack2     = GREY[PAL_LIGHT] + 0.125f;
        m_nodeBack2.a   = 1.f;

        m_nodeBack1.clamp();
        m_nodeBack2.clamp();


        m_nodeHeader1   = 0xA5D90BFF;
        m_nodeHeader2   = 0x6BBB0CFF;



        m_sysHighlight      = ColorFromWxColor(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
        m_sysHighlightGrad  = ColorFromWxColor(wxSystemSettings::GetColour(wxSYS_COLOUR_GRADIENTACTIVECAPTION));
    }
};

static nsStyle paint;


// ----------------------------------------------------------------------------
// vertex decl
struct nsVertex
{
    nsVertex() {}
    nsVertex(const NSvec2 &v) : x(v.x), y(v.y) {}
    NSfloat x, y, u, v;
    // for vertex colors
    unsigned int col;
};

const GLsizei   nsVertexStride      = sizeof( nsVertex );
const NSfloat   nsVertexTOL         = 0.01f;
const int       NS_VERTEX_SIZE      = 2; // x,y
const int       NS_GRADIENT_SIZE    = 4;

// text sizes
const NSfloat   nsHeaderTextSize    = 12.f;
const NSfloat   nsSocketTextSize    = 10.f;


typedef utArray<nsVertex> VertexBuffer;



// ----------------------------------------------------------------------------
// Paths
class nsPath
{
private:

    nsVertex        m_curPos;
    nsVertex        m_movPos;
    nsVertex        m_wrkPos;
    VertexBuffer    m_buffer;
    bool            m_moved;
    NSaabb          m_aabb;

    void pushVert(const nsVertex &vrt)
    {
        if ( NSFuzzyEqT(vrt.x, m_curPos.x, nsVertexTOL) && NSFuzzyEqT(vrt.y, m_curPos.y, nsVertexTOL) )
            return;

        m_aabb.compair(vrt.x, vrt.y);
        m_buffer.push_back(vrt);
    }

    void addSegment(const NSfloat &x, const NSfloat &y)
    {
        if ( NSFuzzyEqT(x, m_curPos.x, nsVertexTOL) && NSFuzzyEqT(y, m_curPos.y, nsVertexTOL) )
            return;

        m_wrkPos.x = x;
        m_wrkPos.y = y;
        pushVert(m_wrkPos);

        // save current
        m_curPos.x = m_wrkPos.x;
        m_curPos.y = m_wrkPos.y;

        if (!m_moved)
        {
            m_moved = true;
            m_movPos.x = m_wrkPos.x;
            m_movPos.y = m_wrkPos.y;
        }
    }

public:
    nsPath()
    {
        m_buffer.reserve(16);
        clear();
    }

    ~nsPath()
    {
    }

    void clear(void)
    {
        m_curPos.x = NS_INFINITY;
        m_curPos.y = NS_INFINITY;
        m_movPos.x = NS_INFINITY;
        m_movPos.y = NS_INFINITY;
        m_moved = false;
        m_buffer.clear(true);
        m_aabb.clear();
    }


    void    moveTo(NSfloat x, NSfloat y)
    {
        m_moved = true;
        m_movPos.x = x;
        m_movPos.y = y;
        addSegment(m_movPos.x, m_movPos.y);
    }

    void    lineTo(NSfloat x, NSfloat y)               { addSegment(x, y); }
    void    close(void)
    {
        if (m_moved)
            addSegment(m_movPos.x, m_movPos.y);
        m_moved = false;
    }


    // simple rect
    void    rect(const NSrect &rct)
    {
        moveTo( rct.x, rct.y );
        lineTo( rct.x, rct.y + rct.height );
        lineTo( rct.x + rct.width, rct.y + rct.height );
        lineTo( rct.x + rct.width, rct.y );
        close();
    }


    // tex coords come from the aabb
    void    makeUV(void)
    {
        VertexBuffer::Pointer ptr = m_buffer.ptr();
        int size = (int)m_buffer.size(), i;

        if (size == 0 || !m_aabb.valid()) return;

        NSfloat dx, dy;
        NSfloat omaxx, omaxy;

        omaxx= 1.f / ( m_aabb.m_max.x - m_aabb.m_min.x );
        omaxy= 1.f / ( m_aabb.m_max.y - m_aabb.m_min.y );

        i = 0;

        while (i < size)
        {
            nsVertex &v = ptr[i++];


            dx = v.x - m_aabb.m_min.x;
            dy = v.y - m_aabb.m_min.y;

            // allign to [0,1]
            v.u = dx * omaxx;
            v.v = dy * omaxy;
        }
    }

    NSaabb                  &getAABB(void)              {return m_aabb;}
    const NSaabb            &getAABB(void) const        {return m_aabb;}
    VertexBuffer            &buf(void)                  {return m_buffer;}
    const VertexBuffer      &buf(void) const            {return m_buffer;}
};


// ----------------------------------------------------------------------------
// Private internal render tools
class nsRenderSystemPrivate
{
private:

    void enableTexture(GLuint tex)
    {
        if (tex != 0)
        {
            glBindTexture( GL_TEXTURE_2D, tex );
            glEnable( GL_TEXTURE_2D );
        }
    }

    void disableTexture(GLuint tex)
    {
        if (tex != 0)
            glDisable( GL_TEXTURE_2D );
    }



    GLuint gradient(  const NScolor &start,
                      const NScolor &stop,
                      int x1, int y1,
                      int x2, int y2,
                      int size
                   )
    {
        int x, y, sqs;
        float A, B, C, C1, C2;
        unsigned int *ptr;

        x1 = NSClamp(x1, 0, size); x2 = NSClamp(x2, 0, size);
        y1 = NSClamp(y1, 0, size); y2 = NSClamp(y2, 0, size);


        A = float(x2 - x1);
        B = float(y2 - y1);

        C1 = A * x1 + B * y1;
        C2 = A * x2 + B * y2;

        sqs = size * size;


        float scale = 1.f / float(sqs);
        unsigned char *tbuf = new unsigned char[sqs*4];
        ptr = (unsigned int *)tbuf;
        NScolor c;

        for (x = 0; x<size; ++x)
        {
            for (y = 0; y<size; y++)
            {
                C = A * (x + .5f) + B * (y + .5f);

                if (C <= C1)
                    c = start;
                else if (C >= C2)
                    c = stop;
                else
                    c = (start * (C2 - C) + stop * (C - C1))/(C2 - C1);

                (*ptr++) =c.ABGR();
            }
        }

        GLuint ret = createTexture(size, size, tbuf, true);
        delete []tbuf;
        return ret;
    }

public:
    nsPath  *m_workPath;
    GLuint   m_nodeBack;
    GLuint   m_curTex;
    nsFont  *m_font;

    VertexBuffer m_lineBuffer;


    nsRenderSystemPrivate()
    {
        m_workPath = new nsPath();

        m_font      = 0;
        m_nodeBack  = 0;
        m_curTex    = 0;

    }

    ~nsRenderSystemPrivate()
    {
        delete m_font;
        m_font = 0;

        delete m_workPath;
        m_workPath = 0;

        finalize();
    }

    void finalize(void)
    {
        if (m_font && m_font->m_tex)
        {
            glDeleteTextures( 1, &m_font->m_tex);
            m_font->m_tex = 0;
        }

        for (int i=0; i<NS_COL_MAX; ++i)
        {
            if (STDPalette[i].image != 0)
            {
                glDeleteTextures( 1, &STDPalette[i].image );
                STDPalette[i].image = 0;
            }
        }

        if (m_nodeBack != 0)
            glDeleteTextures( 1, &m_nodeBack );
        m_nodeBack = 0;
    }


    // default texture impl
    GLuint createTexture(int w, int h, void *pixels, bool alpha=false, bool lum=false, bool mip=true)
    {
        GLuint tex;
        GLenum mode = alpha ? GL_RGBA : GL_RGB;
        if (lum) mode = GL_ALPHA;

        glGenTextures( 1, &tex );
        glBindTexture( GL_TEXTURE_2D, tex );

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mip ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mip ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST);
        glTexImage2D( GL_TEXTURE_2D, 0, mode, w,  h, 0, mode, GL_UNSIGNED_BYTE, pixels );

        if (mip)
            gluBuild2DMipmaps( GL_TEXTURE_2D, mode, w, h, mode, GL_UNSIGNED_BYTE, pixels );

        glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
        return tex;
    }


    // load / convert internal fonts
    void loadFonts(void)
    {
        // try a common web-safe font.
        wxFont font(96, wxSWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
        if (font.IsOk())
            m_font = new nsFont(font, 96);

        if (!m_font)
            m_font = new nsFont(wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT), 96);
    }


    // Setup GL resources
    void initialize(void)
    {
        NScolor st = paint.m_nodeBack1;
        NScolor en = paint.m_nodeBack2;

        // Gradient from [0,0] -> [1, 1]
        m_nodeBack = gradient(st, en, 0, 0, NS_GRADIENT_SIZE, NS_GRADIENT_SIZE, NS_GRADIENT_SIZE);


        for (int i=0; i<NS_COL_MAX; ++i)
        {
            st = STDPalette[i].col[PAL_LIGHT];
            en = STDPalette[i].col[PAL_DARK];

            // Gradient from [0,0] -> [1, 0]
            if (STDPalette[i].image == 0)
                STDPalette[i].image = gradient(st, en, 0, 0, NS_GRADIENT_SIZE, 0, NS_GRADIENT_SIZE);
        }

        if (!m_font->m_tex)
            m_font->m_tex = createTexture(m_font->m_width, m_font->m_height, m_font->m_imaData, false, true, true);
    }

    // current image
    void setImage(GLuint tex) { m_curTex = tex; }

    // Buffer filling. I'm leaving out non-zero / even-odd fills,
    // they are not needed here

    void fill(const VertexBuffer &vbuf, const NScolor &col)
    {
        VertexBuffer::ConstPointer ptr = vbuf.ptr();

        // setup blending
        if(col.a < 1.f) glEnable(GL_BLEND);

        glEnableClientState( GL_VERTEX_ARRAY );
        if (m_curTex != 0)
        {
            enableTexture(m_curTex);
            glEnableClientState( GL_TEXTURE_COORD_ARRAY );
        }


        glColor4f( col.r, col.g, col.b, col.a );
        glVertexPointer(NS_VERTEX_SIZE, GL_FLOAT, nsVertexStride, &ptr[0].x );
        if (m_curTex !=0)
            glTexCoordPointer( 2, GL_FLOAT, nsVertexStride, &ptr[0].u );

        // do fill
        glDrawArrays(GL_TRIANGLE_FAN, 0, GLsizei(vbuf.size()));


        if (m_curTex != 0)
        {
            disableTexture(m_curTex);
            glDisableClientState( GL_TEXTURE_COORD_ARRAY );
        }


        glDisableClientState( GL_VERTEX_ARRAY );
        if(col.a < 1.f)
            glDisable(GL_BLEND);
    }


    void stroke(const VertexBuffer &vbuf, const NScolor &col, bool strip = true, bool colors=false)
    {
        VertexBuffer::ConstPointer ptr = vbuf.ptr();

        if(col.a < 1.f)
            glEnable(GL_BLEND);

        glEnableClientState(GL_VERTEX_ARRAY);
        glColor4f(col.r, col.g, col.b, col.a);
        if (colors)
            glEnableClientState(GL_COLOR_ARRAY);

        glVertexPointer(NS_VERTEX_SIZE, GL_FLOAT, nsVertexStride, &ptr[0].x );
        if (colors)
            glColorPointer(4, GL_UNSIGNED_BYTE, nsVertexStride, &ptr[0].col );
        glDrawArrays(strip ? GL_LINE_STRIP : GL_LINES, 0, GLsizei(vbuf.size()));

        if (colors)
            glDisableClientState( GL_COLOR_ARRAY );

        glDisableClientState( GL_VERTEX_ARRAY );
        if(col.a < 1.f)
            glDisable(GL_BLEND);
    }



    // default display
    void displayString( nsFont *font, NSfloat size, const char *str, int len, const NScolor &col, NSfloat x, NSfloat y)
    {
        if ( !font )
            return;


        const NSfloat font_scale = size / (NSfloat)font->m_res;
        const NSfloat t_xspace = 1.f / NSfloat( font->m_width );
        const NSfloat t_yspace = 1.f / NSfloat( font->m_height );

        NSfloat xoffs = x;
        NSfloat yoffs = y;

        const NSfloat base_height = font->getChar( '0' ).height * font_scale;



        glEnable( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D, font->m_tex );
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        glColor4f(  col.r,
                    col.g,
                    col.b,
                    col.a );
        glPushMatrix();
        glLoadIdentity();
        glBegin( GL_QUADS );

        const char *poin = str;

        for ( int i = 0; i < len; i++ )
        {
            char cCh = poin[i];
            nsChar ch = font->getChar( cCh );

            if ( cCh == ' ' )
            {
                xoffs += ch.width * font_scale;
                continue;
            }

            else if ( cCh == '\n' )
            {
                yoffs += base_height;
                xoffs = x;
                continue;
            }

            // unknown char
            if ( ch.width == 0.0 && ch.height == 0.0 )
                continue;


            NSfloat v_xmin, v_xmax, v_ymin, v_ymax;

            // vert coords
            v_xmin = xoffs;
            v_ymin = yoffs + ( ch.height * font_scale );
            v_xmax = xoffs + ( ch.width * font_scale );
            v_ymax = yoffs;

            NSfloat cx, cy;
            cx = xoffs; cy = yoffs;

            xoffs += ch.width * font_scale;

            // tex coords
            NSfloat t_xmin, t_xmax, t_ymin, t_ymax;
            t_xmin = ch.xco;
            t_ymin = ch.yco;
            t_xmax = ch.xco + ( ch.width );
            t_ymax = ch.yco + ( ch.height );

            t_xmin *= t_xspace; t_xmax *= t_xspace;
            t_ymin *= t_yspace; t_ymax *= t_yspace;

            glTexCoord2f( t_xmin, t_ymin); glVertex2f( v_xmin, v_ymax);
            glTexCoord2f( t_xmax, t_ymin); glVertex2f( v_xmax, v_ymax);
            glTexCoord2f( t_xmax, t_ymax); glVertex2f( v_xmax, v_ymin);
            glTexCoord2f( t_xmin, t_ymax); glVertex2f( v_xmin, v_ymin);
        }

        glEnd();
        glPopMatrix();
        glDisable( GL_BLEND );
        glDisable( GL_TEXTURE_2D );
    }

    void displayStringS( nsFont *font, NSfloat size, const char *str, int len, const NScolor &col, NSfloat x, NSfloat y)
    {
        // drop shadow (makes strings more visible)

        if ( !font )
            return;


        const NSfloat font_scale = size / (NSfloat)font->m_res;
        const NSfloat t_xspace = 1.f / NSfloat( font->m_width );
        const NSfloat t_yspace = 1.f / NSfloat( font->m_height );

        NSfloat xoffs = x;
        NSfloat yoffs = y;

        const NSfloat base_height = font->getChar( '0' ).height * font_scale;



        glEnable( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D, font->m_tex );
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        glPushMatrix();
        glLoadIdentity();
        glBegin( GL_QUADS );

        const char *poin = str;

        for ( int i = 0; i < len; i++ )
        {
            char cCh = poin[i];
            nsChar ch = font->getChar( cCh );

            if ( cCh == ' ' )
            {
                xoffs += ch.width * font_scale;
                continue;
            }

            else if ( cCh == '\n' )
            {
                yoffs += base_height;
                xoffs = x;
                continue;
            }

            // unknown char
            if ( ch.width == 0.0 && ch.height == 0.0 )
                continue;


            NSfloat v_xmin, v_xmax, v_ymin, v_ymax;

            // vert coords
            v_xmin = xoffs;
            v_ymin = yoffs + ( ch.height * font_scale );
            v_xmax = xoffs + ( ch.width * font_scale );
            v_ymax = yoffs;

            NSfloat cx, cy;
            cx = xoffs; cy = yoffs;

            xoffs += ch.width * font_scale;

            // tex coords
            NSfloat t_xmin, t_xmax, t_ymin, t_ymax;
            t_xmin = ch.xco;
            t_ymin = ch.yco;
            t_xmax = ch.xco + ( ch.width );
            t_ymax = ch.yco + ( ch.height );

            t_xmin *= t_xspace; t_xmax *= t_xspace;
            t_ymin *= t_yspace; t_ymax *= t_yspace;

            glColor4f(  0.f,
                        0.f,
                        0.f,
                        col.a - .5f );

            glTexCoord2f( t_xmin, t_ymin); glVertex2f( v_xmin+1, v_ymax+1);
            glTexCoord2f( t_xmax, t_ymin); glVertex2f( v_xmax+1, v_ymax+1);
            glTexCoord2f( t_xmax, t_ymax); glVertex2f( v_xmax+1, v_ymin+1);
            glTexCoord2f( t_xmin, t_ymax); glVertex2f( v_xmin+1, v_ymin+1);

            glColor4f(  col.r,
                        col.g,
                        col.b,
                        col.a );

            glTexCoord2f( t_xmin, t_ymin); glVertex2f( v_xmin, v_ymax);
            glTexCoord2f( t_xmax, t_ymin); glVertex2f( v_xmax, v_ymax);
            glTexCoord2f( t_xmax, t_ymax); glVertex2f( v_xmax, v_ymin);
            glTexCoord2f( t_xmin, t_ymax); glVertex2f( v_xmin, v_ymin);
        }

        glEnd();
        glPopMatrix();
        glDisable( GL_BLEND );
        glDisable( GL_TEXTURE_2D );
    }

    void makeCubicColor(VertexBuffer &vbuf, NSfloat subSteps, const NSvec2 &to, const NSvec2 &fr, const NScolor &a, const NScolor &b)
    {
        const NSfloat SubSteps= NSMax(subSteps, 1.f);
        const NSfloat step= 1.f / SubSteps;

        NSfloat s = 1.f, t = 0.f;
        NSvec2 c0, c1, c2, c3, pr, cr;
        NSfloat hd = to.distance(fr) * .5f;
        nsVertex av, bv;
        NScolor col;


        c0 = fr; c1 = NSvec2(c0.x + hd, c0.y);
        c3 = to; c2 = NSvec2(c3.x - hd, c3.y);
        pr = c0;
        while (t <= 1.0)
        {
            cr = NSvec2(
                     (s * s * s) * c0.x + (3.f * s * s * t) * c1.x + (3.f * t * t * s) * c2.x + (t * t * t) * c3.x,
                     (s * s * s) * c0.y + (3.f * s * s * t) * c1.y + (3.f * t * t * s) * c2.y + (t * t * t) * c3.y
                 );

            col = a.blend(b, s);

            av = pr; bv = cr;
            av.col = col.ABGR();
            bv.col = av.col;

            vbuf.push_back(av);
            vbuf.push_back(bv);
            pr = cr;

            t += step;
            s = 1.f - t;
        }

        av = pr;
        av.col = col.ABGR();
        vbuf.push_back(av);
        av = c3;
        av.col = col.ABGR();
        vbuf.push_back(av);
    }


    void makeCubic(VertexBuffer &vbuf, NSfloat subSteps, const NSvec2 &to, const NSvec2 &fr)
    {
        makeCubicColor(vbuf, subSteps, to, fr, BLACKOPAQUE, BLACKOPAQUE);
    }

    // Nifty looking connector
    // from  ----
    //          |
    //          ---- to
    void makeSquareColor(VertexBuffer &vbuf, const NSvec2 &to, const NSvec2 &from,  const NScolor &ac, const NScolor &bc)
    {
        NSfloat fac = to.distance(from) * .5f;


        NSfloat center = from.x + fac;
        if ((to.x - fac) < center)
            center = (to.x - fac);

        nsVertex a, b;
        a = from;
        b = NSvec2(center, from.y);

        a.col = b.col = ac.ABGR();

        vbuf.push_back(a);
        vbuf.push_back(b);

        a = b;
        b = NSvec2(center, to.y);
        a.col = b.col = ac.blend(bc, .5f).ABGR();
        vbuf.push_back(a);
        vbuf.push_back(b);

        a = b;
        b = to;
        a.col = b.col = bc.ABGR();
        vbuf.push_back(a);
        vbuf.push_back(b);
    }

    void makeSquare(VertexBuffer &vbuf, const NSvec2 &to, const NSvec2 &from)
    {
        makeSquareColor(vbuf, to, from, BLACKOPAQUE, BLACKOPAQUE);
    }

};



// ----------------------------------------------------------------------------
nsRenderSystem::nsRenderSystem() : m_context(new nsRenderSystemPrivate())
{
    paint.loadDefaultSettings();

    // load fonts at start up
    m_context->loadFonts();
}

// ----------------------------------------------------------------------------
nsRenderSystem::~nsRenderSystem()
{
    delete m_context;
}


// ----------------------------------------------------------------------------
// one time setup
void nsRenderSystem::initializeContextData(void)
{
    m_context->initialize();
}


// ----------------------------------------------------------------------------
// cleanup
void nsRenderSystem::finalizeContextData(void)
{
    m_context->finalize();
}


// ----------------------------------------------------------------------------
// setup viewport rendering
void nsRenderSystem::loadViewport(const NSrect &rect)
{
    glViewport((GLint)rect.x, (GLint)rect.y, (GLsizei)rect.width, (GLsizei)rect.height);
}

// ----------------------------------------------------------------------------
// projection
void nsRenderSystem::loadProjection(const NSrect &rect)
{
    glMatrixMode(GL_PROJECTION);

    NSmat4 matrix;
    NSmath::ortho2D(matrix, rect.getLeft(), rect.getTop(), rect.getRight(), rect.getBottom());
    glLoadMatrixf(matrix.transpose().ptr);

    // align pixels
    glTranslatef( 0.375f, 0.375f, 0.0f );


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    m_projection = rect;
    m_clip = rect;

}


// ----------------------------------------------------------------------------
// clear buffers
void nsRenderSystem::clear(void)
{
    glClearColor(paint.m_background.r, paint.m_background.g, paint.m_background.b, paint.m_background.a);
    glClear(GL_COLOR_BUFFER_BIT);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


// ----------------------------------------------------------------------------
void nsRenderSystem::drawSocketLinks(nsNodes &nodes)
{
    VertexBuffer &buf = m_context->m_lineBuffer;
    buf.clear(true);

    nsSocket *sock, *link;
    nsNodeIterator iter(nodes);
    while (iter.hasMoreElements())
    {
        sock = iter.getNext()->getFirstInput();
        while (sock)
        {
            if (sock->isConnected())
            {
                link = sock->getSocketLink();
                const NSvec2 &centA = sock->getDerrivedRect().center();
                const NSvec2 &centB = link->getDerrivedRect().center();


                // Clip the links bounding box, with some extra padding
                NSrect linkBB = NSrect(centA.x, centA.y, centB.x - centA.x, centB.y - centA.y);
                linkBB.makeMinMax(); linkBB.expand(200, 200);
                if (m_projection.contains(linkBB))
                {
                    bool asel, bsel;
                    asel = sock->getParent()->getState() == NDST_ACTIVE;
                    bsel = link->getParent()->getState() == NDST_ACTIVE;

                    const NSPalette &pala = STDPalette[((int)sock->getType()->m_color) % NS_COL_MAX];
                    const NSPalette &palb = STDPalette[((int)link->getType()->m_color) % NS_COL_MAX];

                    if (SOCKET_LINK == SOCKET_LINK_CURVE)
                    {
                        // cubic spline
                        m_context->makeCubicColor(buf, 24, centA, centB,
                                                  asel ? pala.col[PAL_LIGHT] : paint.m_black,
                                                  bsel ? palb.col[PAL_LIGHT] : paint.m_black);
                    }
                    else if (SOCKET_LINK == SOCKET_LINK_BLOCK)
                    {
                        // square spline
                        m_context->makeSquareColor(buf, centA, centB,
                                                   asel ? pala.col[PAL_LIGHT] : paint.m_black,
                                                   bsel ? palb.col[PAL_LIGHT] : paint.m_black);
                    }
                    else
                    {
                        // line
                        nsVertex a = centA, b = centB;
                        a.col = asel ? pala.col[PAL_LIGHT].ABGR() : paint.m_black.ABGR();
                        b.col = bsel ? palb.col[PAL_LIGHT].ABGR() : paint.m_black.ABGR();

                        buf.push_back(a);
                        buf.push_back(b);
                    }
                }
            }
            sock = sock->getNext();
        }
    }

    if (!buf.empty())
    {
        glEnable( GL_LINE_SMOOTH );
        m_context->stroke(buf, 0xFFFFFFC8, false, true);
        glDisable( GL_LINE_SMOOTH );
        buf.clear(true);
    }
}


// ----------------------------------------------------------------------------
void nsRenderSystem::paintSocket(nsNode *parent, nsSocket *sock)
{
    const NSrect &base = sock->getDerrivedRect();
    const nsSocketType *type = sock->getType();
    int curPal = (int)type->m_color;

    nsPath &pth = *m_context->m_workPath;

    NSrect trect = base;

    // test for clipping
    if (m_projection.contains(base))
    {
        const NSPalette &pal = STDPalette[curPal % NS_COL_MAX];

        pth.clear();
        pth.rect(base);
        pth.makeUV();
        m_context->setImage(pal.image);
        m_context->fill(pth.buf(),      paint.m_white);
        m_context->stroke(pth.buf(),    parent->getState() == NDST_INACTIVE ? pal.col[PAL_SHADOW] : pal.col[PAL_LIGHT]);
        m_context->setImage(0);

        if (sock->isConnected() || sock->isConnectedOut())
        {
            trect = base;
            trect.setSize(trect.getSize() * .5f);
            trect.setPosition(base.center() - (trect.getSize() * .5f));
            pth.clear();
            pth.rect(trect);
            m_context->fill(pth.buf(), pal.col[PAL_LIGHT]);
        }
    }

    // display name

    const NSrect &parRect = parent->getRect();

    if (m_projection.contains(parRect))
    {
        const utString &name = type->m_name;
        const NSPalette &pal = STDPalette[curPal % NS_COL_MAX];

        NSvec2 texPos;

        if (type->m_direction == nsSocketType::In)
        {
            texPos.x = base.getRight()  + 3;
            texPos.y = base.y;
        }
        else
        {
            texPos.x = parRect.getRight()   - (m_context->m_font->getPixelWidth(
                                                   nsSocketTextSize, name.c_str(), name.size()) + 3);
            texPos.y = base.y;
        }

        // write contents
        m_context->displayStringS(  m_context->m_font,
                                    nsSocketTextSize,
                                    name.c_str(), name.size(),
                                    pal.col[PAL_TEXT],
                                    texPos.x,
                                    texPos.y);
    }
}


// ----------------------------------------------------------------------------
void nsRenderSystem::drawNode(nsNode *nd)
{
    NSrect trect = nd->getRect();
    trect.expand(20, 20);

    nsPath &pth = *m_context->m_workPath;


    // see if rect is clipped
    if (m_projection.contains(trect))
    {
        const NSrect &base = nd->getRect();
        const utString &name = nd->getType()->m_typename;

        NSPalette pal = STDPalette[ ((int)nd->getType()->m_color) % NS_COL_MAX];

        // drop shadow
        m_context->setImage(0);
        pth.clear();
        trect = base;
        trect.x += 5; trect.y += 5;
        pth.rect(trect);
        m_context->fill(pth.buf(), GREY[PAL_SHADOW]);
        if ( nd->getState() != NDST_INACTIVE )
            m_context->stroke(pth.buf(), GREY[PAL_DARK]);

        nsPath &pth = *m_context->m_workPath;
        pth.clear();
        pth.rect(base);
        pth.makeUV();
        m_context->setImage(m_context->m_nodeBack);
        m_context->fill(pth.buf(),      paint.m_white);
        m_context->stroke(pth.buf(),    nd->getState() == NDST_INACTIVE ? pal.col[PAL_SHADOW] : pal.col[PAL_LIGHT]);
        m_context->setImage(0);

        // header
        trect = base;
        trect.height    = 24;
        pth.clear();
        pth.rect(trect);
        pth.makeUV();
        m_context->setImage(pal.image);
        m_context->fill(pth.buf(), paint.m_white);
        m_context->setImage(0);
        m_context->stroke(pth.buf(), nd->getState() == NDST_INACTIVE ? pal.col[PAL_SHADOW] : pal.col[PAL_LIGHT]);

        m_context->displayString(   m_context->m_font,
                                    nsHeaderTextSize,
                                    name.c_str(),
                                    name.size(),
                                    pal.col[PAL_TEXT_HEADER],
                                    trect.x + 3.5f,
                                    trect.y + 3.5f);
        // header outline
        m_context->stroke(pth.buf(), pal.col[PAL_LIGHT]);

        pth.clear();
        pth.moveTo(trect.x,             trect.getBottom());
        pth.lineTo(trect.getRight(),    trect.getBottom());
        m_context->stroke(pth.buf(), paint.m_black, false);
    }

    nsSocket *sock;
    sock = nd->getFirstInput();
    while (sock)
    {
        paintSocket(nd, sock);
        sock = sock->getNext();
    }

    sock = nd->getFirstOutput();
    while (sock)
    {
        paintSocket(nd, sock);
        sock = sock->getNext();
    }
}

// ----------------------------------------------------------------------------
// socket linking
void nsRenderSystem::drawConnectingSocket(nsSocket *sock, const NSvec2 &pos, const NSvec2 &mouse)
{
    VertexBuffer &buf = m_context->m_lineBuffer;
    buf.clear(true);


    if (SOCKET_LINK == SOCKET_LINK_CURVE)
    {
        if (sock->isOutput())
            m_context->makeCubic(buf, 24, mouse, pos);
        else
            m_context->makeCubic(buf, 24, pos, mouse);
    }
    else if (SOCKET_LINK == SOCKET_LINK_BLOCK)
    {
        // square spline
        if (sock->isOutput())
            m_context->makeSquare(buf, mouse, pos);
        else
            m_context->makeSquare(buf, pos, mouse);
    }
    else
    {
        // line
        buf.push_back(pos);
        buf.push_back(mouse);
    }

    NSPalette pal = STDPalette[ ((int)sock->getType()->m_color) % NS_COL_MAX];
    NScolor selecColor = pal.col[PAL_LIGHT];
    selecColor.a = 0.784f;

    glEnable( GL_LINE_SMOOTH );
    m_context->stroke(buf, selecColor, false);
    glDisable( GL_LINE_SMOOTH );
}

// ----------------------------------------------------------------------------
// selection rect
void nsRenderSystem::drawDragRect(const NSrect &rect)
{
    nsPath &pth = *m_context->m_workPath;

    // use system colors for this
    NScolor outline = paint.m_sysHighlightGrad;
    NScolor fillcol = paint.m_sysHighlight;
    fillcol.a = 0.3f;

    pth.clear();
    pth.rect(rect);
    m_context->fill(pth.buf(), fillcol);
    m_context->stroke(pth.buf(), outline);
}


// ----------------------------------------------------------------------------
void nsRenderSystem::drawBackground(void)
{
    NSfloat step = 25.0f;
    NSfloat xmin,ymin,xmax,ymax;
    m_projection.getBounds(xmin, ymin, xmax, ymax);

    NScolor col = paint.m_background - 0.025f;

    glColor3f(col.r, col.g, col.b);
    glBegin(GL_LINES);
    for (NSfloat start = xmin - NSFmod(xmin, step); start < xmax; start += step)
    {
        glVertex2f(start, ymin);
        glVertex2f(start, ymax);
    }

    for (NSfloat start = ymin - NSFmod(ymin, step); start < ymax; start += step)
    {
        glVertex2f(xmin, start);
        glVertex2f(xmax, start);
    }

    col = col - 0.025f;
    glColor3f(col.r, col.g, col.b);
    glVertex2f(0.f, ymin);
    glVertex2f(0.f, ymax);
    glVertex2f(xmin, 0.f);
    glVertex2f(xmax, 0.f);
    glEnd();
}
