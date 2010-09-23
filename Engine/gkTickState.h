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
#ifndef _gkTickState_h_
#define _gkTickState_h_

#include "gkCommon.h"
#include "gkMathUtils.h"
#include "LinearMath/btQuickprof.h"



///Timer for running loops at a fixed rate
class gkTickState
{
private:

	unsigned long   m_ticks, m_rate;
	unsigned long   m_skip, m_loop;
	unsigned long   m_cur, m_next;
	gkScalar        m_fixed, m_invt;
	btClock*         m_clock;
	bool            m_lock, m_init;

protected:

	virtual void tickImpl(gkScalar delta) = 0;

	virtual void beginTickImpl(void) {}
	virtual void endTickImpl(void) {}

public:

	gkTickState();
	gkTickState(int rate);
	~gkTickState();

	void reset(void);
	void initialize(int rate);
	void tick(void);
};


#endif//_gkTickState_h_
