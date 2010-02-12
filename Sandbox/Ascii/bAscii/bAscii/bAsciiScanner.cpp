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
#include <stdio.h>
#include "bAsciiScanner.h"
#include "utPrint.h"


// ----------------------------------------------------------------------------
bAsciiScanner::bAsciiScanner(const char *file) :
	m_line(0), m_buffer(0), m_bufSize(0), m_curTok(-1), m_error(-1),
	m_source(file)
{
	FILE *fp = fopen(file, "rb");
	if (fp)
	{

		fseek(fp, 0L, SEEK_END);
		m_bufSize = (int)ftell(fp);
		fseek(fp, 0L, SEEK_SET);

		m_buffer = static_cast<char*>(utMemUtils::alloc(m_bufSize + 1));

		fread(m_buffer, m_bufSize, 1, fp);
		fclose(fp);

		m_buffer[m_bufSize] = 0;

		m_curTok = 0;
		m_error = 0;
		m_line = 1;
	}
}

// ----------------------------------------------------------------------------
bAsciiScanner::bAsciiScanner(const char *source, const char *memBuf, size_t size) :
	m_line(0), m_buffer(0), m_bufSize(0), m_curTok(-1), m_error(-1),
	m_source(source)
{
	if (memBuf && size > 0 && size != UT_NPOS)
	{
		m_bufSize = (int)size;
		m_buffer = static_cast<char*>(utMemUtils::alloc(m_bufSize + 1));

		utMemcpy(m_buffer, memBuf, m_bufSize);

		m_buffer[m_bufSize] = 0;
		m_curTok = 0;
		m_error = 0;
		m_line = 1;
	}
}

// ----------------------------------------------------------------------------
bAsciiScanner::~bAsciiScanner()
{
	if (m_buffer)
	{
		utMemUtils::dealloc(m_buffer);
		m_buffer = 0;
	}
}

// ----------------------------------------------------------------------------
#define CTOK ((m_curTok < m_bufSize && m_curTok >= 0) ? m_buffer[m_curTok] : OP_END)
#define PTOK ((m_curTok < m_bufSize && m_curTok >= 1) ? m_buffer[m_curTok-1] : OP_END)
#define NTOK (((m_curTok+1) < m_bufSize && m_curTok >= 0) ? m_buffer[m_curTok+1] : OP_END)
#define ADVANCE (m_curTok += 1)
#define REWIND  (m_curTok -= 1)
#define STCMT	'#'
#define STOK	'"'
#define ALPHA(c) (((c) >= 'a' && (c) <= 'z'  || (c) >= 'A' && (c) <='Z'))
#define DIGIT(c) (((c) >= '0' && (c) <= '9') || (c) == '.')

#define ALPHAS(c) (ALPHA(c) || ((c) >= '0' && (c) <= '9') || (c) == '_')


// ----------------------------------------------------------------------------
int bAsciiScanner::scan(bAsciiToken &tokd, bool verbose)
{
	tokd.m_string.clear();
	tokd.m_tok = OP_UNKNOWN;
	tokd.m_line = -1;


	for (;;)
	{
		char tok = CTOK;
		if (tok == OP_END) 
		{

			tokd.m_tok = OP_END;
			tokd.m_line = m_line;
			tokd.m_string = "EOF";
			return OP_END;
		}

		if (tok == '\n' || tok == '\r')
		{
			if (NTOK == '\r')
				ADVANCE;
			m_line += 1;
			ADVANCE;
		}
		else if (tok == ' ' || tok == '\t')
			ADVANCE;
		else if (tok == STCMT)
		{
			do 
			{
				ADVANCE;
			}
			while (CTOK != '\n' && CTOK != '\r' && CTOK != OP_END);

			if (CTOK == '\n')
			{
				if (NTOK == '\r')
					ADVANCE;
				m_line += 1;
				ADVANCE;
			}
		}
		else if (tok == ':') 
		{
			ADVANCE;
			tokd.m_string.push_back(':');
			tokd.m_tok = OP_SECTION;
			tokd.m_line = m_line;

			return OP_SECTION;
		}
		else if (tok == ',') 
		{
			ADVANCE;
			tokd.m_string.push_back(',');
			tokd.m_tok = OP_SEPERATOR;
			tokd.m_line = m_line;
			return OP_SEPERATOR;
		}
		else if (tok == '=') 
		{
			ADVANCE;
			tokd.m_string.push_back('=');
			tokd.m_tok = OP_ASSIGNMENT;
			tokd.m_line = m_line;
			return OP_ASSIGNMENT;
		}
		else if (tok == ';') 
		{
			ADVANCE;
			tokd.m_string.push_back(';');
			tokd.m_tok = OP_TERM;
			tokd.m_line = m_line;
			return OP_TERM;
		}
		else if (tok == '{') 
		{
			ADVANCE;
			tokd.m_string.push_back('{');
			tokd.m_tok = OP_BRACKET_OPEN;
			tokd.m_line = m_line;
			return OP_BRACKET_OPEN;
		}
		else if (tok == '}') 
		{
			ADVANCE;
			tokd.m_string.push_back('}');
			tokd.m_tok = OP_BRACKET_CLOSE;
			tokd.m_line = m_line;
			return OP_BRACKET_CLOSE;
		}
		else if (tok == STOK)
		{
			ADVANCE;
			do
			{
				tokd.m_string.push_back(CTOK);
				ADVANCE;
			}
			while (CTOK != STOK && CTOK != OP_END);

			if (CTOK == OP_END)
			{
				// eofscan
				tokd.m_string = "end of file";
				tokd.m_tok = OP_UNKNOWN;
				tokd.m_line = m_line;
				return OP_UNKNOWN;
			}
			else
			{
				ADVANCE;
				tokd.m_tok = OP_STRING;
				tokd.m_line = m_line;
				return OP_STRING;
			}
		}
		else if (ALPHA(tok))
		{
			do 
			{
				tokd.m_string.push_back(CTOK);
				ADVANCE;
			}
			while (ALPHAS(CTOK) && CTOK != OP_END);

			tokd.m_tok = OP_ALPHA;
			tokd.m_line = m_line;
			return OP_ALPHA;
		}
		else if (tok == '-' || DIGIT(tok))
		{
			if (tok == '-' && !DIGIT(NTOK))
			{
				tokd.m_string = tokd.m_string.format("unknown token %c", NTOK);
				tokd.m_tok = OP_UNKNOWN;
				tokd.m_line = m_line;
				return OP_UNKNOWN;
			}
			do 
			{
				tokd.m_string.push_back(CTOK);
				ADVANCE;
			}
			while (DIGIT(CTOK) && CTOK != OP_END);

			tokd.m_tok = OP_NUMBER;
			tokd.m_line = m_line;
			return OP_NUMBER;
		}
		else
		{
			tokd.m_string = tokd.m_string.format("unknown token %c", tok);
			tokd.m_tok = OP_UNKNOWN;
			tokd.m_line = m_line;
			return OP_UNKNOWN;
		}
	}

	return OP_UNKNOWN;
}
