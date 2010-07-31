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
