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

#ifndef GKPARENTACTUATOR_H
#define GKPARENTACTUATOR_H

#include "gkLogicActuator.h"

class gkParentActuator : public gkLogicActuator
{
public:
	enum Mode
	{
		PA_SET,
		PA_CLEAR,
	};

private:
	int m_mode;
	gkString m_obj;
	bool m_compound, m_ghost;

public:
	gkParentActuator(gkGameObject* object, gkLogicLink* link, const gkString& name);
	virtual ~gkParentActuator() {}

	gkLogicBrick* clone(gkLogicLink* link, gkGameObject* dest);

	void execute(void);

	GK_INLINE void            setMode(int v)               {m_mode = v;}
	GK_INLINE void            setParent(const gkString& v) {m_obj = v;}
	GK_INLINE void            setCompound(bool v)          {m_compound = v;}
	GK_INLINE void            setGhost(bool v)             {m_ghost = v;}

	GK_INLINE int             getMode(void)                const {return m_mode;}
	GK_INLINE const gkString& getParent(void)              const {return m_obj;}
	GK_INLINE bool            getCompound(void)            const {return m_compound;}
	GK_INLINE bool            getGhost(void)               const {return m_ghost;}
};


#endif // GKPARENTACTUATOR_H
