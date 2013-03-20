/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Charlie C.

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
    This is a replacement candidate for Engine/Graphics
    - For now it wraps Ogre::Overlays to gk usable objects.
    - Later it will be superseded by custom Ogre vertex buffer usage,
    - abstracted for external GameKit external rendering.
*/
#ifndef _gkHUD_h_
#define _gkHUD_h_


#include "gkResource.h"
#include "OgreOverlay.h"

class gkHUDElement;


class gkHUD : public gkResource
{
public:
	typedef utArray<gkHUDElement*> ChildNodes;


public:
	gkHUD(gkResourceManager* creator, const gkResourceName& name, const gkResourceHandle& handle);
	virtual ~gkHUD();

	void show(bool v = true);

	void addChild(gkHUDElement* hud);
	void removeChild(gkHUDElement* hud);

	gkHUDElement* getChild(const gkString& name);
	ChildNodes&   getChildren(void) {return m_children;}


	Ogre::Overlay* _getOverlay(void);
	void _setOverlay(Ogre::Overlay* over);

protected:
	Ogre::Overlay* m_overlay;
	ChildNodes     m_children;

private:
	UTsize getChildIndex(const gkString& name);

	void newImpl(void);
};



#endif//_gkHUD_h_
