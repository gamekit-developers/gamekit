/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Charlie C.

    Contributor(s): Thomas Trocha(dertom).
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
#ifndef _gkHUDElement_h_
#define _gkHUDElement_h_

#include "gkCommon.h"
#include "gkVariable.h"
#include "OgreOverlayElement.h"


class gkHUDElement
{
public:

	gkHUDElement(const gkString& name);
	~gkHUDElement();


	void setValue(const gkVariable& v);
	void setValue(const gkString& v);

	gkString getValue();

	void setUvCoords(float u1, float v1, float u2, float v2);

	gkString getParameter(const gkString& name);
	void setParameter(const gkString& name, const gkString& value);

	gkVector2 getPosition(void);
	void setPosition(float x, float y);

	void show(bool v);

	const gkString& getName(void) {return m_name;}

	gkString getMaterialName(void);
	void setMaterialName(const gkString& material);

	void  setMaterialAlpha(float factor);
	float getMaterialAlpha();

	void setMaterialAlphaRejectValue(int val, bool isGreater);
	int  getMaterialAlphaRejectValue();

	Ogre::OverlayElement* _getElement(void);
	void _setElement(Ogre::OverlayElement* ele);

protected:

	const gkString m_name;
	Ogre::OverlayElement* m_element;
	gkHUD* m_parent;
	float m_alphaBlend;
	gkVector2 m_position;
};

#endif//_gkHUDElement_h_
