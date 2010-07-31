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
#ifndef _gkV8Script_h_
#define _gkV8Script_h_

#include "gkV8Utils.h"

class gkV8Manager;


// ----------------------------------------------------------------------------
class gkV8Script
{
protected:
	const gkString      m_name, m_text;
	v8Script            m_script;
	bool                m_compiled, m_isInvalid;
	gkV8Manager         *m_owner;

	void compile(void);

public:
	gkV8Script(gkV8Manager *parent, const gkString &name, const gkString &text);
	~gkV8Script();

	GK_INLINE const gkString &getName(void) {return m_name;}
	GK_INLINE bool  compiled(void)          {return m_compiled;}

	void unload(void);


	// compile & run the script
	bool execute(void);
};


#endif//_gkV8Script_h_
