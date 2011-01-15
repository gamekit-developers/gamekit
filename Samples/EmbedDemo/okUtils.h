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

#ifndef _okUtils_h_
#define _okUtils_h_

gkString getNativeWinHandleFromWxWin(wxWindow* win);
gkString getBaseFileName(const gkString& s);
void dumpGkInfo();

GK_INLINE gkString WX2GK(const wxString& s) { return gkString(s.mb_str(wxConvUTF8)); }
GK_INLINE wxString GK2WX(const gkString& s) { return wxString(s.c_str()); }

GK_INLINE wxColor GK2WX(const gkColor &s) { return wxColor(s.r * 255, s.g * 255, s.b * 255, s.a * 255); }
GK_INLINE gkColor WX2GK(const wxColor &s) { return gkColor(s.Red() / 255.0f, s.Green() / 255.0f, s.Blue() / 255.0f, s.Alpha() / 255.0f); }

GK_INLINE wxString GK2STR(const UTint32 v)			{ return wxString::Format("%d", v);			}
GK_INLINE wxString GK2STR(const UTuint32 v)			{ return wxString::Format("%u", v);			}
GK_INLINE wxString GK2STR(const gkScalar v)			{ return wxString::Format("%g", double(v));	}

GK_INLINE UTint32	WX2INT(const wxString& s)		{ long v = 0; s.ToLong(&v); return UTint32(v);				}
GK_INLINE UTuint32	WX2UINT(const wxString& s)		{ unsigned long v = 0; s.ToULong(&v); return UTuint32(v);	}
GK_INLINE gkScalar	WX2SCALAR(const wxString& s)	{ double v = 0; s.ToDouble(&v); return gkScalar(v);			}


#endif //_okUtils_h_