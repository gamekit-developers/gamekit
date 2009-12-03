/*
-------------------------------------------------------------------------------
	This file is part of the Ogre GameKit port.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
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
#include "gkPropertyScanner.h"
#include "gkLogger.h"


using namespace Ogre;


// ----------------------------------------------------------------------------
#define MATCH_ALPHA(c)	  ((c >= 'a' && c <= 'z' || c >= 'A' && c <='Z') || c == '_')
#define MATCH_DIGIT(c)	  ((c >= '0' && c <= '9'))
#define MATCH_ALPNU(c)	  (MATCH_ALPHA(c) || MATCH_DIGIT(c))
#define MATCH_NL(c)		 (c == '\n' || c == '\r')
#define MATCH_WS(c)		 (c == ' ' || c == '\t')
#define MATCH_TERM(c)	   (c == ';')

#define TOKEN_PREALLOC	  16

// ----------------------------------------------------------------------------
enum ScanStates
{
	SS_NONE=-1,
	SS_CLOSE,
	SS_TEXT,
};

// ----------------------------------------------------------------------------
gkPropertyScanner::gkPropertyScanner(const char *sname, const String& sdata, size_t slen) :
		source(sname),
		buffer(0),
		tok(0), len(slen), line(1),
		m_state(SS_NONE),
		first(true)
{
	if (slen > 0 && slen != GK_NPOS)
	{
		buffer= new char[slen + 1];
		memcpy(buffer, sdata.c_str(), slen);
		buffer[slen]=0;

		/// move lookahead to the start of the buffer
		if (buffer != 0 && buffer[0] != TOK_END)
			tok= &buffer[0];
	}

	for (int i=0; i<TOKEN_PREALLOC; i++)
		m_tokens.push_back(new gkPropertyToken());
}


// ----------------------------------------------------------------------------
gkPropertyScanner::~gkPropertyScanner()
{
	delete []buffer;

	for (size_t i=0; i<m_tokens.capacity(); i++)
		delete m_tokens.at(i);
}

// ----------------------------------------------------------------------------
void gkPropertyScanner::doneToken(gkPropertyToken *tok)
{
	if (tok)
		m_tokens.push_back(tok);
}

// ----------------------------------------------------------------------------
gkPropertyToken* gkPropertyScanner::makeToken(void)
{
	gkPropertyToken *dst= 0;

	if (m_tokens.empty())
		dst= new gkPropertyToken();
	else
	{
		dst= m_tokens.back();
		m_tokens.pop_back();
	}

	GK_ASSERT(dst);

	dst->token= TOK_END;
	dst->debug.line= line;
	dst->debug.source= (char*)source;
	dst->string.clear();
	return dst;
}


// ----------------------------------------------------------------------------
void gkPropertyScanner::addCharacter(gkPropertyToken* ptr, CharPtr &tok)
{
	if (*tok == TOK_END)
		return;
	ptr->push(*tok);
}


// ----------------------------------------------------------------------------
gkPropertyToken* gkPropertyScanner::scan(void)
{
	gkPropertyToken *pt= makeToken();

	char s=' ';

	if (buffer == 0 || tok == 0 || len == 0)
		goto finally;

	for (;;)
	{
		if (!first)
			tok++;
		first= false;

		if ((*tok) == TOK_END)
			goto finally;

		switch (*tok)
		{
		case ' ':
		case '\t':
		case ';':
			break;
		case '\n':
		case '\r':
			/// skip if crlf pair
			if (MATCH_NL(*(tok+1)))
				++tok;
			line += 1;
			break;
		case '{':
			pt->debug.line= line;
			pt->token= TOK_OPEN_BRACE;
			return pt;
		case '}':
			pt->debug.line= line;
			pt->token= TOK_CLOSE_BRACE;
			return pt;
		case ':':
			pt->debug.line= line;
			pt->token= TOK_SECTION;
			return pt;
		case '#':
		case '/':
			{
				// trim comments
				if (*(tok+1) == '/')
				{
					// c++ style eat till new line
					s= (*tok);

					while (!MATCH_NL((*tok)))
					{
						tok++;
						if (*tok == TOK_END)
							goto error;
					}
					/// skip if crlf pair
					if (MATCH_NL(*(tok+1)))
						++tok;

					line += 1;

				}
				else if ((*tok)=='#')
				{
					if (strncmp(tok, "#include", 8)==0)
					{
						tok += 8;

						while (MATCH_WS((*tok)))
						{
							tok++;
							if ((*tok) == TOK_END)
								goto error;
						}

						for (;;)
						{
							if (MATCH_NL(*tok))
							{

								/// skip if crlf pair
								if (MATCH_NL(*(tok+1)))
									++tok;

								line += 1;
								break;
							}

							if ((*tok) != '"' && (*tok) != '<' && (*tok) != '>')
								pt->string.push_back((*tok));

							++tok;

							if (*tok == TOK_END)
								goto error;
						}
						pt->debug.line= line;
						pt->token= TOK_INCLUDE;
						return pt;
					}
				}

				else if (*(tok+1) == '*')
				{
					// c style
					s= '/';
					do
					{
						tok++;
						if ((*tok) == TOK_END)
							goto error;

						if (MATCH_NL(*tok))
						{

							/// skip if crlf pair
							if (MATCH_NL(*(tok+1)))
								++tok;

							line += 1;

						}
					}
					while ((*tok) != '*' || (*(tok+1)) != '/');
					++tok; // skip '/'
				}
			}
			break;
		case '=':
			{
				/// eat till first non white space character
				tok++;

				while (MATCH_WS((*tok)))
				{
					tok++;
					if ((*tok) == TOK_END)
						goto error;
				}

				/// scan till first term ';'
				if (MATCH_TERM((*tok)))
					goto empty;

				s= ';';

				pt->string.reserve(32);

				while (!MATCH_TERM((*tok)))
				{
					pt->string.push_back((*tok));

					tok++;
					if ((*tok) == TOK_END)
						goto error;


					if (MATCH_NL((*tok)))
					{
						/// skip if crlf pair
						if (MATCH_NL(*(tok+1)))
							++tok;

						line += 1;
					}
				}

				/// successful assignment
				pt->debug.line= line;
				pt->token= TOK_ASSIGN;
				return pt;
			}
		default:

			if (MATCH_ALPHA((*tok)))
			{
				pt->string.reserve(32);

				while (MATCH_ALPNU((*tok)))
				{
					pt->string.push_back((*tok));
					tok++;
					if ((*tok) == TOK_END)
						break;
				}

				/// successful id
				pt->debug.line= line +1;
				pt->token=  TOK_NAME;

				// add back last token
				--tok;
				return pt;

			}

			goto unknown;
		}

	}

unknown:
	/// unknown character parsed
	pt->debug.line= line;
	pt->token= TOK_ERROR;

	gkPrintf("%s(%i): syntax error: '%c' \n", source, line, (*tok));
	return pt;

empty:

	/// empty assignment
	/// x= ;

	pt->debug.line= line;
	pt->token= TOK_ERROR;

	gkPrintf("%s(%i): syntax error: empty statement. \n", source, line);
	return pt;

error:

	/// premature end of file

	pt->debug.line= line;
	pt->token= TOK_ERROR;

	gkPrintf("%s(%i): error: premature end of file.\n\twhile scanning for '%c'\n", source, line, s);
	return pt;

finally:

	/// successful end of buffer
	pt->debug.line=  line;
	pt->token=  TOK_END;
	return pt;

}
