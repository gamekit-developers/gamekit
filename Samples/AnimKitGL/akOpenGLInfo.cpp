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

#include "akOpenGLInfo.h"

#ifdef WIN32
#include <Windows.h>
#include <GL/glut.h>
#elif defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <sstream>
#include <algorithm>


bool akOpenGLInfo_extSort(const utString& a, const utString& b)
{
	if(b.compare(a)>0)
		return false;
	return true;
}

bool akOpenGLInfo::getInfo(void)
{
	char* str = 0;
	char* tok = 0;
	
	// get vendor string
	str = (char*)glGetString(GL_VENDOR);
	if(str) vendor = str;                  // check NULL return value
	else return false;
	
	// get renderer string
	str = (char*)glGetString(GL_RENDERER);
	if(str) renderer = str;                // check NULL return value
	else return false;
	
	// get version string
	str = (char*)glGetString(GL_VERSION);
	if(str) version = str;                 // check NULL return value
	else return false;
	
	// get all extensions as a string
	str = (char*)glGetString(GL_EXTENSIONS);
	
	// split extensions
	if(str)
	{
		tok = strtok((char*)str, " ");
		while(tok)
		{
			extensions.push_back(tok);    // put a extension into struct
			tok = strtok(0, " ");               // next token
		}
	}
	else
	{
		return false;
	}
	
	// sort extension by alphabetical order
	extensions.sort(akOpenGLInfo_extSort);
	
	// get number of color bits
	glGetIntegerv(GL_RED_BITS, &redBits);
	glGetIntegerv(GL_GREEN_BITS, &greenBits);
	glGetIntegerv(GL_BLUE_BITS, &blueBits);
	glGetIntegerv(GL_ALPHA_BITS, &alphaBits);
	
	// get depth bits
	glGetIntegerv(GL_DEPTH_BITS, &depthBits);
	
	// get stecil bits
	glGetIntegerv(GL_STENCIL_BITS, &stencilBits);
	
	// get max number of lights allowed
	glGetIntegerv(GL_MAX_LIGHTS, &maxLights);
	
	// get max texture resolution
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
	
	// get max number of clipping planes
	glGetIntegerv(GL_MAX_CLIP_PLANES, &maxClipPlanes);
	
	// get max modelview and projection matrix stacks
	glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, &maxModelViewStacks);
	glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH, &maxProjectionStacks);
	glGetIntegerv(GL_MAX_ATTRIB_STACK_DEPTH, &maxAttribStacks);
	
	// get max texture stacks
	glGetIntegerv(GL_MAX_TEXTURE_STACK_DEPTH, &maxTextureStacks);
	
	return true;
}


bool akOpenGLInfo::isExtensionSupported(const std::string& ext)
{
	utArray<utString>::Iterator iter = extensions.iterator();
	while (iter.hasMoreElements())
	{
		if(ext == iter.getNext())
			return true;
	}
	return false;
}

void akOpenGLInfo::print(void)
{
	std::stringstream ss;
	
	ss << std::endl; // blank line
	ss << "OpenGL Driver Info" << std::endl;
	ss << "==================" << std::endl;
	ss << "Vendor: " << vendor << std::endl;
	ss << "Version: " << version << std::endl;
	ss << "Renderer: " << renderer << std::endl;
	
	ss << std::endl;
	ss << "Color Bits(R,G,B,A): (" << redBits << ", " << greenBits
	   << ", " << blueBits << ", " << alphaBits << ")\n";
	ss << "Depth Bits: " << depthBits << std::endl;
	ss << "Stencil Bits: " << stencilBits << std::endl;
	
	ss << std::endl;
	ss << "Max Texture Size: " << maxTextureSize << "x" << maxTextureSize << std::endl;
	ss << "Max Lights: " << maxLights << std::endl;
	ss << "Max Clip Planes: " << maxClipPlanes << std::endl;
	ss << "Max Modelview Matrix Stacks: " << maxModelViewStacks << std::endl;
	ss << "Max Projection Matrix Stacks: " << maxProjectionStacks << std::endl;
	ss << "Max Attribute Stacks: " << maxAttribStacks << std::endl;
	ss << "Max Texture Stacks: " << maxTextureStacks << std::endl;
	
	ss << std::endl;
	ss << "Total Number of Extensions: " << extensions.size() << std::endl;
	ss << "===============================" << std::endl;
	for(unsigned int i = 0; i < extensions.size(); ++i)
		ss << extensions.at(i) << std::endl;
	
	ss << "===============================" << std::endl;
	
	std::cout << ss.str() << std::endl;
}
