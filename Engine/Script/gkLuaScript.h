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
#ifndef _gkLuaScript_h_
#define _gkLuaScript_h_


struct lua_State;
class gkLuaManager;


// ----------------------------------------------------------------------------
class gkLuaScript
{
protected:
    const gkString  m_name, m_text;
    int             m_script;
    bool            m_compiled, m_isInvalid;

    gkLuaManager    *m_owner;
    void compile(void);

public:
    gkLuaScript(gkLuaManager *parent, const gkString& name, const gkString& text);
    ~gkLuaScript();

    GK_INLINE const gkString& getName(void) {return m_name;}
    GK_INLINE bool  compiled(void)          {return m_compiled;}

    void unload(void);
    // compile & run the script 
    bool execute(void);
};



#endif//_gkScript_h_
