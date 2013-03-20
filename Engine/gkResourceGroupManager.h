/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 harkon.kr

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

#ifndef _gkResourceGroupManager_h_
#define _gkResourceGroupManager_h_

#include "gkCommon.h"
#include "utSingleton.h"

class gkMaterialLoader;

class gkResourceGroupManager : public utSingleton<gkResourceGroupManager>
{
protected:
	utArray<gkResourceNameString> m_groups;
	gkMaterialLoader* m_materialLoader;

public:
	gkResourceGroupManager();	
	virtual ~gkResourceGroupManager();

	bool createResourceGroup(const gkResourceNameString& group, bool inGlobalPool=true);
	void destroyResourceGroup(const gkResourceNameString& group);
	void clearResourceGroup(const gkResourceNameString& group);
	bool existResourceGroup(const gkResourceNameString& group);
	void destroyAllResourceGroup(void);

	void initialiseAllResourceGroups();
	void initialiseResourceGroup(const gkString& group);

	bool initRTShaderSystem(const gkString& shaderLang, const gkString& shaderCachePath, bool hasFixedCapability);

	GK_INLINE const utArray<gkResourceNameString> getResourceGroupList() { return m_groups; }

	UT_DECLARE_SINGLETON(gkResourceGroupManager)
};

#endif//_gkResourceGroupManager_h_