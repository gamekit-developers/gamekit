/*
-------------------------------------------------------------------------------
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#include "bAsciiBlock.h"
#include "bAsciiArgument.h"


// ---------------------------------------------------------------------------
bAsciiBlock::bAsciiBlock(const bStringF& type, const int code) :
	m_type(type), m_code(code), m_name(), m_len(bNumber(-1)), m_parent(0), m_children()
{
}

// ---------------------------------------------------------------------------
bAsciiBlock::~bAsciiBlock()
{
	for (size_t i=0; i<m_arguments.size(); ++i)
		delete m_arguments[i];
}

// ---------------------------------------------------------------------------
bAsciiArgument* bAsciiBlock::getArgument(const bCode& arg)
{

	size_t pos = UT_NPOS;
	if ((pos = m_arguments.find(arg)) != UT_NPOS)
		return m_arguments.at(pos);

	return 0;
}

// ---------------------------------------------------------------------------
void bAsciiBlock::addArgument(bAsciiArgument *argument)
{
	if (m_arguments.find(argument->m_name) == UT_NPOS)
		m_arguments.insert(argument->m_name, argument);
}
