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
#include "nsString.h"
#include <stdio.h>
#include <iomanip>
#include <sstream>
#include <cctype>
#include <algorithm>

char nsStringConverter::nsStringSplitter = ' ';


void nsStringUtils::trim( nsString& in, const nsString& expr )
{
	in.erase( in.find_last_not_of( expr ) + 1 );
	in.erase( 0, in.find_first_not_of( expr ) );
}


void nsStringUtils::split(nsStringArray& rval,  const nsString& spl, const nsString& expr )
{
	nsString string = spl;
	rval.reserve( 32 );

	for ( ;; )
	{
		size_t pos = string.find_first_of( expr );
		if ( pos != nsString::npos )
		{
			// chop first
			if ( pos == 0 )
				pos = pos + 1;

			nsString sub = string.substr( 0, pos );
			if ( !sub.empty() && expr.find( sub ) == nsString::npos )
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


void nsStringUtils::lower( nsString& str )
{
	std::transform( str.begin(), str.end(), str.begin(), tolower );
}


nsString nsStringUtils::lower( const nsString& str )
{
	nsString ret = str;
	std::transform( ret.begin(), ret.end(), ret.begin(), tolower );
	return ret;
}


void nsStringUtils::upper( nsString& str )
{
	std::transform( str.begin(), str.end(), str.begin(), toupper );
}


nsString nsStringUtils::upper( const nsString& str )
{
	nsString ret = str;
	std::transform( ret.begin(), ret.end(), ret.begin(), toupper );
	return ret;
}


void nsStringUtils::replace( nsString& in, const nsString& from, const nsString& to )
{

	if ( !from.empty() && from != to )
	{
		// erase
		if ( to.empty() )
		{
			size_t pos = 0;
			while ( pos != nsString::npos )
			{
				pos = in.find( from );
				if ( pos != nsString::npos )
					in.erase( pos, from.size() );
			}
		}
		else
		{
			size_t pos = 0;
			while ( pos != nsString::npos )
			{
				pos = in.find( from );
				if ( pos != nsString::npos )
				{
					in.erase( pos, from.size() );
					in.insert( pos, to );
				}
			}
		}
	}
}

void nsStringUtils::loadFileAsString( nsString& dest, const nsString& file)
{
	FILE* fp = fopen(file.c_str(), "rb");
	if (!fp)
		return;

	fseek(fp, 0L, SEEK_END);
	int len = (int)ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	char* buf = new char[len + 1];
	fread(buf, len, 1, fp);
	fclose(fp);

	dest = nsString(buf, len);
	delete []buf;
}
