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
#include "Utils/utStreams.h"



// ----------------------------------------------------------------------------
gkV8Manager::gkV8Manager()
{
}


// ----------------------------------------------------------------------------
gkV8Manager::~gkV8Manager()
{
	utListIterator<ScriptList> it(m_scripts);
	while (it.hasMoreElements())
		delete it.getNext();
	v8::V8::Dispose();
}

// ----------------------------------------------------------------------------
gkV8Script *gkV8Manager::create(const gkString &name, const gkString &text)
{
	gkV8Script *scrpt = new gkV8Script(this, name, text);
	m_scripts.push_back(scrpt);
	return scrpt;
}


// ----------------------------------------------------------------------------
gkV8Script *gkV8Manager::create(const gkString &file)
{
	utFileStream fs;
	fs.open(file.c_str(), utStream::SM_READ);
	if (!fs.isOpen())
		return 0;

	gkV8Script *scrpt = 0;

	utMemoryStream ms;
	ms.open(fs, utStream::SM_READ);
	if (ms.size() > 0)
	{
		scrpt = new gkV8Script(this, file, gkString((char *)ms.ptr()));
		m_scripts.push_back(scrpt);
	}
	return scrpt;
}

extern v8Module GameLogic_OpenV8(v8Globals &context);
extern v8Module Math_OpenV8(v8Globals &context);

// ----------------------------------------------------------------------------
void gkV8Manager::initializeBindings(v8BindModule *extras)
{
	// create object / function templates

	m_globals = v8Globals(v8::ObjectTemplate::New());
	v8RegisterBuiltins(m_globals);


	m_modules.push_back(GameLogic_OpenV8(m_globals));
	m_modules.push_back(Math_OpenV8(m_globals));

	if (extras)
	{
		int i = 0;
		v8BindModule mod = extras[i++];
		while (mod)
		{
			m_modules.push_back((mod)(m_globals));
			mod = extras[i++];
		}
	}



	m_context = v8Context(v8::Context::New(NULL, m_globals));
}

GK_IMPLEMENT_SINGLETON(gkV8Manager);
