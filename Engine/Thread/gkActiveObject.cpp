/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) Nestor Silveira.

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
#include "gkActiveObject.h"
#include "gkLogger.h"

gkActiveObject::gkActiveObject(const gkString& name)
	: m_name(name),
	  m_thread(0),
	  m_queue(name)
{
	m_thread = new gkThread(this);
}

gkActiveObject::~gkActiveObject()
{
	delete m_thread;
}

void gkActiveObject::run()
{
	gkPtrRef<gkCall> pCall;

	while (m_queue.pop(pCall))
	{
		try
		{
			pCall->run();

			pCall = gkPtrRef<gkCall>(0);
		}
		catch (...) // catch all the exceptions.
		{
			gkLogMessage(m_name.c_str() << "call error." );
		}
	}
}

void gkActiveObject::enqueue(gkPtrRef<gkCall> pCall, bool front)
{
	m_queue.push(pCall, front);
}

void gkActiveObject::reset()
{
	m_queue.reset();
}

void gkActiveObject::resetButKeepLast()
{
	m_queue.resetButKeepLast();
}

void gkActiveObject::join()
{
	m_queue.petitionToFinish();

	m_thread->join();
}

bool gkActiveObject::isEmpty() const
{
	return m_queue.isEmpty();
}
