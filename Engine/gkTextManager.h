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
#ifndef _gkTextFileManager_h_
#define _gkTextFileManager_h_

#include "gkCommon.h"
#include "gkResourceManager.h"
#include "utSingleton.h"



class gkTextManager : public gkResourceManager, public utSingleton<gkTextManager>
{
public:
	enum TextType
	{
		TT_ANY,      // Undefined ( no *.ext or an unregistered *.ext )
		TT_MATERIAL, // Ogre material script (*.material)
		TT_PARTICLE, // Ogre particle script (*.particle)
		TT_COMPOSIT, // todo: Ogre Compositor script (*.compositor)
		TT_OVERLAY,  // Ogre overlay script (*.overlay)
		TT_FONT,     // Ogre font script (*.fontdef)
		TT_CG,       // CG source (*.cg)
		TT_GLSL,     // OpenGL Shader Language      (*.glsl)
		TT_HLSL,     // High Level Shader Language  (*.hlsl)
		TT_LUA,      // Lua script (*.lua)
		TT_XML,      // Undefined: Some other XML source (*.xml)
		TT_NTREE,    // todo: NodeTree script (*.ntree)
		TT_BFONT,    // Blender VFont script (Blender::VFont to Ogre::Font (*.bfont) )
	};


	typedef utArray<gkTextFile*> TextArray;


public:
	gkTextManager();
	virtual ~gkTextManager();

	gkResource* createImpl(const gkResourceName& name, const gkResourceHandle& handle);


	int getTextType(const gkString& name);
	void getTextFiles(TextArray& dest, int textType);


	void parseScripts(const gkString& group);


	UT_DECLARE_SINGLETON(gkTextManager);


};


#endif//_gkTextFileManager_h_
