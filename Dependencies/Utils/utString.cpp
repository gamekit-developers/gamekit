/*
-------------------------------------------------------------------------------
    General Purpose Utility Library, should be kept dependency free.
    Unless the dependency can be compiled along with this library.

    Copyright (c) 2009-2010 Charlie C.
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
#include "utString.h"
#include <iomanip>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <stdio.h>
#include <stdarg.h>



void utStringUtils::trim( utString &in, const utString &expr )
{
	in.erase( in.find_last_not_of( expr ) + 1 );
	in.erase( 0, in.find_first_not_of( expr ) );
}


void utStringUtils::split(utStringArray &rval,  const utString &spl, const utString &expr )
{
	utString string= spl;
	rval.reserve( 32 );

	for ( ;; )
	{
		size_t pos= string.find_first_of( expr );
		if ( pos != utString::npos )
		{
			// chop first
			if ( pos == 0 )
				pos= pos + 1;

			utString sub= string.substr( 0, pos );
			if ( !sub.empty() && expr.find( sub ) == utString::npos )
				rval.push_back( sub );

			string.erase( 0, pos );
		}
		else
		{
			if ( !string.empty() )
				rval.push_back( string );

			break;
		}
	}
}


void utStringUtils::lower( utString &str )
{
	std::transform( str.begin(), str.end(), str.begin(), tolower );
}


utString utStringUtils::lower( const utString &str )
{
	utString ret= str;
	std::transform( ret.begin(), ret.end(), ret.begin(), tolower );
	return ret;
}


void utStringUtils::upper( utString &str )
{
	std::transform( str.begin(), str.end(), str.begin(), toupper );
}


utString utStringUtils::upper( const utString &str )
{
	utString ret= str;
	std::transform( ret.begin(), ret.end(), ret.begin(), toupper );
	return ret;
}


void utStringUtils::replace( utString &in, const utString &from, const utString &to )
{

	if ( !from.empty() && from != to )
	{
		// erase
		if ( to.empty() )
		{
			size_t pos= 0;
			while ( pos != utString::npos )
			{
				pos= in.find( from );
				if ( pos != utString::npos )
					in.erase( pos, from.size() );
			}
		}
		else
		{
			size_t pos= 0;
			while ( pos != utString::npos )
			{
				pos= in.find( from );
				if ( pos != utString::npos )
				{
					in.erase( pos, from.size() );
					in.insert( pos, to );
				}
			}
		}
	}
}


utString utStringFormat(const char* format, ...)
{
#ifdef _MSC_VER
# define ut_vsnprintf _vsnprintf_s
#else
# define ut_vsnprintf vsnprintf
#endif

	va_list args;
	va_start(args, format);

	int nBuf = 0;
	const int BUF_SIZE = 1024;
	char szBuffer[BUF_SIZE+1];

	nBuf = ut_vsnprintf(szBuffer, BUF_SIZE, format, args);

	if (nBuf < 0) 	
		szBuffer[BUF_SIZE] = 0;

	va_end(args);

	return utString(szBuffer);

#undef ut_vsnprintf
}