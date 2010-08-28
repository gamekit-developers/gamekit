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
#ifndef _gkObject_h_
#define _gkObject_h_

#include "gkCommon.h"
#include "gkHashedString.h"


///Base class for objects that are repetitively removed or added to Ogre
class gkObject
{
public:


	enum CreationState
	{
		ST_CREATING   = (1 << 0),
		ST_CREATED    = (1 << 1),
		ST_DESTROYING = (1 << 2),
		ST_DESTROYED  = (1 << 3),
		ST_ERROR      = (1 << 4),
	};


protected:
	const gkString      m_name;
	int                 m_instanceState;


	// Create and destroy events

	virtual void preCreateInstanceImpl(void) {}
	virtual void preDstroyInstanceImpl(void) {}
	virtual void createInstanceImpl(void) {}
	virtual void destroyInstanceImpl(void) {}
	virtual void postCreateInstanceImpl(void) {}
	virtual void postDestroyInstanceImpl(void) {}

public:

	gkObject(const gkString &name);
	virtual ~gkObject();



	void createInstance(void);
	void destroyInstance(void);
	void reinstance(void);


	GK_INLINE const gkString &getName(void)                    { return m_name;}
	GK_INLINE bool           isInstanced(void) const           { return (m_instanceState & ST_CREATED) != 0;}
	GK_INLINE bool           isBeingCreated(void) const        { return (m_instanceState & ST_CREATING) != 0;}
	GK_INLINE bool           isBeingDestroyed(void) const      { return (m_instanceState & ST_DESTROYING) != 0;}
	GK_INLINE int            getInstanceState(void) const      { return m_instanceState;}


};



#endif//_gkObject_h_
