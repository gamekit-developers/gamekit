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


// Base class representing a loadable object
class gkObject
{
public:
	enum InitState
	{
		ST_INITIALIZING  = (1 << 0),
		ST_INITIALIZED   = (1 << 1),
		ST_FINALIZING    = (1 << 2),
		ST_FINALIZED     = (1 << 3),
		ST_INITFAILED    = (1 << 4),
	};


protected:
	const gkString      m_name;
	int                 m_initState;

	virtual void preInitializeImpl(void) {}
	virtual void preFinalizeImpl(void) {}
	virtual void initializeImpl(void) {}
	virtual void finalizeImpl(void) {}
	virtual void postInitializeImpl(void) {}
	virtual void postFinalizeImpl(void) {}

public:

	gkObject(const gkString &name);
	virtual ~gkObject();

	void initialize(void);
	void finalize(void);
	void reinitialize(void);


	GK_INLINE const gkString &getName(void)               {return m_name;}
	GK_INLINE bool           isInitialized(void)  const   {return (m_initState & ST_INITIALIZED) != 0;}
	GK_INLINE bool           isInitializing(void) const   {return (m_initState & ST_INITIALIZING) != 0;}
	GK_INLINE bool           isFinalizing(void)   const   {return (m_initState & ST_FINALIZING) != 0;}
	GK_INLINE int            getInitState(void)   const   {return m_initState;}


};



#endif//_gkObject_h_
