/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): Nestor Silveira.
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
#ifndef _gkResource_h_
#define _gkResource_h_

#include "gkCommon.h"

class gkResource
{
protected:
	gkResourceManager*		m_creator;
	const gkResourceName	m_name;	
	const gkResourceHandle	m_resourceHandle;	

	friend class gkResourceManager;
	virtual void notifyResourceDestroying(void) {}


public:

	gkResource(gkResourceManager* creator, const gkResourceName& name, const gkResourceHandle& handle);
	virtual ~gkResource();


	GK_INLINE const gkResourceName&		getResourceName(void)		const { return m_name; }
	GK_INLINE const gkString&			getName(void)				const { return m_name.name.str(); }
	GK_INLINE const gkString&			getGroupName(void)			const { return m_name.group.str(); }
	GK_INLINE const gkResourceHandle&	getResourceHandle(void)		const { return m_resourceHandle; }
	GK_INLINE gkResourceManager*		getCreator(void)			{ return m_creator; }



	const gkString& getResourceType(void);
	const gkString& getManagerType(void);
};

#endif//_gkResource_h_
