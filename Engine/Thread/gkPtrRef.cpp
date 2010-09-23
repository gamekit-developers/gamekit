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
#include "gkPtrRef.h"

gkReferences::gkReferences()
	: m_references(1)
{
}

gkReferences::~gkReferences()
{
}

int gkReferences::addRef()
{
	gkCriticalSection::Lock guard(m_cs);

	return ++m_references;
}

int gkReferences::release()
{
	{
		gkCriticalSection::Lock tmp(m_cs);

		if (--m_references != 0)
		{
			return m_references;
		}
	}

	delete this;

	return 0;
}

int gkReferences::getReferences() const
{
	gkCriticalSection::Lock tmp(m_cs);

	return m_references;
}
