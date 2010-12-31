/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

    Contributor(s): harkon.kr.
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

#ifndef _gkViewport_h_
#define _gkViewport_h_

class gkViewport
{
public:
	enum
	{
		FRAMING_EXTEND,
		FRAMING_CROP,
		FRAMING_LETTERBOX,
	};

protected:
	gkWindow* m_window;
	Ogre::Viewport* m_viewport;
	int m_framing;

public:
	gkViewport(gkWindow* window, Ogre::Viewport* vp);
	~gkViewport();

	GK_INLINE Ogre::Viewport* getViewport() { return m_viewport; }
	GK_INLINE int getFraming() { return m_framing; }

	void setDimension(int framing);
};

#endif //_gkViewport_h_