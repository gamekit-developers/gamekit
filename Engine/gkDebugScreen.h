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
#ifndef _gkDebugScreen_h_
#define _gkDebugScreen_h_

#include "gkCommon.h"
#include "gkHashedString.h"
#include "OgreStringVector.h"
#include "OgreStdHeaders.h"
#include "utMemoryPool.h"

class gkDebugScreen
{
public:
	gkDebugScreen();
	~gkDebugScreen();

	void initialize();
	void finalize();

	static void printTo(const gkString& buf);

	GK_INLINE void lock(bool doit)      { m_isLocked = doit; }
	GK_INLINE bool isInitialized()      { return m_isInit; }
	GK_INLINE bool isLocked()           { return m_isLocked; }
	GK_INLINE bool isShown()            { return m_isShown; }
	GK_INLINE const gkString& getText() { return m_txtBuffer;  }

	GK_INLINE void setText(const gkString& txt)
	{
		m_txtBuffer = txt;
		_getClipped();
	}


	void show(bool doit);
	void pushText(const gkString& text);
	void clear();
	void _getClipped();


private:
	bool m_isInit, m_isLocked, m_isShown;
	utString m_txtBuffer;

	Ogre::Overlay*               m_over;
	Ogre::OverlayElement*        m_ele;
	Ogre::ManualResourceLoader*  m_font;


	class ScreenBufferItem : public utListClass<ScreenBufferItem>::Link
	{
	public:
		ScreenBufferItem()
		{
		}

		void clear()
		{
			m_next = m_prev = 0;
		}

		utString m_buf;
	};

	typedef     utMemoryPool<ScreenBufferItem, 0> ScreenBufferPool;
	typedef     utListClass<ScreenBufferItem> ScreenBufferList;
	ScreenBufferList    m_lines;
	ScreenBufferPool    m_lineBuffer;
	utStringArray       m_splitBuf;
};

extern void dsPrintf(const char* fmt, ...);

#endif//_gkDebugScreen_h_
