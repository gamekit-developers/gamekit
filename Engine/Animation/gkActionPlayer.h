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

#ifndef GKACTIONPLAYER_H
#define GKACTIONPLAYER_H

#include "gkAction.h"

class gkActionPlayer
{
protected:
	gkAction*            m_action;
	gkScalar             m_evalTime, m_weight;
	bool                 m_enabled;
	int                  m_mode;
	gkGameObject*        m_object;

public:
	gkActionPlayer();
	gkActionPlayer(gkAction* resource, gkGameObject* object);
	~gkActionPlayer() {}

	GK_INLINE gkScalar         getTimePosition(void) const { return m_evalTime; }
	GK_INLINE gkScalar         getWeight(void) const       { return m_weight; }
	GK_INLINE int              getMode(void) const         { return m_mode; }
	GK_INLINE gkScalar         getLength(void) const       { return m_action? m_action->getLength() : 0;}
	GK_INLINE gkScalar         getStart(void) const        { return m_action? m_action->getStart() : 1;}
	GK_INLINE gkScalar         getEnd(void) const          { return m_action? m_action->getEnd() : 1;}
	GK_INLINE gkString         getName(void) const         { return m_action? m_action->getName() : "";}
	GK_INLINE gkGameObject*	   getObject(void) const       { return m_object; }

	GK_INLINE void             setMode(int v)              { m_mode = v; }
	GK_INLINE void             setAction(gkAction* v)      { m_action = v; }
	GK_INLINE void             setObject(gkGameObject * v) { m_object = v; }

	GK_INLINE void             enable(bool v)              { m_enabled = v; }
	GK_INLINE bool             isEnabled(void) const       { return m_enabled; }
	GK_INLINE bool             isDone(void) const          { return !m_enabled || m_evalTime >= getLength(); }

	void setBlendFrames(gkScalar v);
	void setTimePosition(gkScalar v);
	void setWeight(gkScalar w);

	void evaluate(gkScalar time);
	void reset(void);

};

#endif // GKACTIONPLAYER_H
