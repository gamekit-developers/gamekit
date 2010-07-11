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
#include "gkV8Manager.h"
#include "gkV8Script.h"
#include "gkV8CodeTemplates.h"


// ----------------------------------------------------------------------------
gkV8Script::gkV8Script(gkV8Manager *parent, const gkString &name, const gkString &buffer)
    :       m_name(name),
            m_text(buffer),
            m_compiled(false),
            m_isInvalid(false),
            m_owner(parent)
{
}


// ----------------------------------------------------------------------------
gkV8Script::~gkV8Script()
{
    unload();
}


// ----------------------------------------------------------------------------
void gkV8Script::unload(void)
{
    m_script.Dispose();
}


// ----------------------------------------------------------------------------
void gkV8Script::compile(void)
{
    v8::TryCatch caught;

    gkString __main = GKV8HIDDEN;
    utStringUtils::replace(__main, "$MAIN_SCOPE", m_text);

    m_script = v8Script(v8::Script::Compile(v8::String::New(__main.c_str()), 
                                            v8::String::New(m_name.c_str())));
    if (m_script.IsEmpty())
    {
        v8HandleException(caught);
        m_isInvalid = true;
        return;
    }

    m_compiled = true;

}


// ----------------------------------------------------------------------------
bool gkV8Script::execute(void)
{
    v8::Context::Scope __ctx__(m_owner->getContext());
    {
        v8::HandleScope __scope__;

        if (!m_compiled)
            compile();

        if (m_isInvalid)
            return false;


        v8::TryCatch caught;
        v8::Handle<v8::Value> result = m_script->Run();
        if (result.IsEmpty())
        {
            m_isInvalid = true;
           v8HandleException(caught);
        }

    }
    return true;
}
