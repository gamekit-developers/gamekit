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
#ifndef _bAsciiScanner_h_
#define _bAsciiScanner_h_

#include "bAsciiCommon.h"
#include "bAsciiOpCodes.h"
#include "bUtils/utFixedString.h"


// ----------------------------------------------------------------------------
class bAsciiToken
{
public:

	bAsciiToken() : m_string(), m_tok(OP_UNKNOWN) {}

	bAsciiToken(const bStringF& b, int op, int line) : 
		m_string(b), m_tok(op), m_line(line)
	{
	}


	UT_INLINE const bAsciiToken& operator = (const bAsciiToken& o)
	{
		m_string = o.m_string;
		m_tok = o.m_tok;
		m_line = o.m_line;
		return *this;
	}

	bStringF	m_string;
	int			m_tok;
	int			m_line;
};


// ----------------------------------------------------------------------------
class bAsciiScanner
{
protected:
	int				m_line;
	char*			m_buffer;
	int				m_bufSize;
	int				m_curTok;
	int				m_error;
	const char*		m_source;

public:

	bAsciiScanner(const char *file);
	bAsciiScanner(const char *source, const char *memBuf, size_t size);
	~bAsciiScanner();

	int getError(void)				{return m_error;}
	const char *getSource(void)		{return m_source;}

	int scan(bAsciiToken &tokd, bool verbose);

};


#endif//_bAsciiScanner_h_
