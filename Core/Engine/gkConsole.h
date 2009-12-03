/*
-------------------------------------------------------------------------------
	This file is part of the Ogre GameKit port.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#ifndef _gkConsole_h_
#define _gkConsole_h_

#include "gkCommon.h"
#include "OgreString.h"
#include "OgreStringVector.h"
#include "OgreStdHeaders.h"





// ----------------------------------------------------------------------------
class gkConsole
{
public:
	gkConsole();
	~gkConsole();

	void initialize();
	void finalize();

	static void printTo(const Ogre::String &buf);

	void lock(bool doit);
	void show(bool doit);
	void pushText(const Ogre::String &text);
	void _getClipped();

	bool isInitialized();
	bool isLocked();
	bool isShown();

	Ogre::String getText();
	void setText(const Ogre::String &txt);
	void clear();


private:
	bool mInitialized, mLocked, mShow;
	Ogre::String mText;
	Ogre::Overlay *mOver;
	Ogre::OverlayElement *mElement;
	Ogre::StringVector mLines;
	Ogre::Real mCharHeight;

};

// ----------------------------------------------------------------------------
GK_INLINE void gkConsole::lock(bool doit)
{
	mLocked= doit;
}

// ----------------------------------------------------------------------------
GK_INLINE bool gkConsole::isInitialized()
{
	return mInitialized;
}

// ----------------------------------------------------------------------------
GK_INLINE bool gkConsole::isLocked()
{
	return mLocked;
}

// ----------------------------------------------------------------------------
GK_INLINE bool gkConsole::isShown()
{
	return mShow;
}

// ----------------------------------------------------------------------------
GK_INLINE Ogre::String gkConsole::getText()
{
	return mText;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkConsole::setText(const Ogre::String &txt)
{
	mText= txt;
	_getClipped();
}

// ----------------------------------------------------------------------------
GK_INLINE void gkConsole::clear()
{
	mText.clear();
	mLines.clear();
}






// ----------------------------------------------------------------------------
#define ConPrint(x) {						   \
	std::stringstream stream;				   \
	stream << x << "\n";						\
	GameKit::gkConsole::printTo(stream.str());	   \
}


#endif//_gkConsole_h_
