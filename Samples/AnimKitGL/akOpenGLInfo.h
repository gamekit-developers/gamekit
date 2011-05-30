/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

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

#ifndef AKOPENGLINFO_H
#define AKOPENGLINFO_H

#include "utTypes.h"
#include "utString.h"

class akOpenGLInfo
{
public:
	utString vendor;
	utString renderer;
	utString version;
	utArray <utString> extensions;
	int redBits;
	int greenBits;
	int blueBits;
	int alphaBits;
	int depthBits;
	int stencilBits;
	int maxTextureSize;
	int maxLights;
	int maxAttribStacks;
	int maxModelViewStacks;
	int maxProjectionStacks;
	int maxClipPlanes;
	int maxTextureStacks;

	akOpenGLInfo() : redBits(0), greenBits(0), blueBits(0), alphaBits(0), depthBits(0),
		stencilBits(0), maxTextureSize(0), maxLights(0), maxAttribStacks(0),
		maxModelViewStacks(0), maxClipPlanes(0), maxTextureStacks(0) {}
	
	bool getInfo(void);
	bool isExtensionSupported(const utString& ext);
	void print(void);
};

#endif // AKOPENGLINFO_H
