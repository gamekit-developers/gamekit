/*
-------------------------------------------------------------------------------
--                                                                           --
--  This file is part of GameKit.                                            --
--  http://gamekit.googlecode.com/                                           --
--                                                                           --
--                                                                           --
--  Released under the zlib license.                                         --
--  http://www.gzip.org/zlib/zlib_license.html                               --
--                                                                           --
--                                                                           --
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
    gkV8Script(gkV8Manager *parent, const gkString& name, const gkString& text);
    ~gkV8Script();

    GK_INLINE const gkString& getName(void) {return m_name;}
    GK_INLINE bool  compiled(void)          {return m_compiled;}

    void unload(void);


    // compile & run the script 
    bool execute(void);
};


#endif//_gkV8Script_h_
