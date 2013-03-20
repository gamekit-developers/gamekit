/*
-------------------------------------------------------------------------------
    This file is part of gkGUIManager.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Alberto Torres Ruiz

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

#ifndef _gkGUIManager_h_
#define _gkGUIManager_h_

#include "utSingleton.h"
#include <gkString.h>

#include "gkGUI.h"

class FileInterfaceOgre3D;
class SystemInterfaceOgre3D;

class gkGUIManager : 
	public utSingleton<gkGUIManager>
{
private:
	FileInterfaceOgre3D*	m_rkFileInterface;
	SystemInterfaceOgre3D*	m_rkOgreSystem;

public:
	void loadFont(const gkString& name);
	gkGUIManager();
	~gkGUIManager();
	
	void setDebug(gkGUI* gui);
	
	UT_DECLARE_SINGLETON(gkGUIManager);
};

#endif // _gkGUIManager_h_
