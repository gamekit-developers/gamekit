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
#ifndef _gkPropertyToken_h_
#define _gkPropertyToken_h_


#include "gkPropertyCommon.h"



// ----------------------------------------------------------------------------
struct gkDebugInformation
{
	char*	   source; /// source file that owns this
	size_t	  line;   /// the line this info appears
};


// ----------------------------------------------------------------------------
typedef enum gkPropertyTokenType
{
	TOK_ERROR= -1,
	TOK_END,
	TOK_NAME,
	TOK_ASSIGN,
	TOK_OPEN_BRACE,
	TOK_CLOSE_BRACE,
	TOK_SECTION,
	TOK_INCLUDE,

}gkPropertyTokenType;

typedef Ogre::String gkPropertyTokenBuffer;

// ----------------------------------------------------------------------------
class gkPropertyToken
{

public:

	gkPropertyToken() :
			token(TOK_ERROR), string()
	{
		debug.line= 0;
		debug.source= 0;
	}


	void push(char c)
	{
		string.push_back(c);
	}

	gkPropertyTokenType	   token;  /// current token
	gkDebugInformation		debug;  /// debug info
	gkPropertyTokenBuffer	 string; /// fixed inner memory
};


typedef gkArray<gkPropertyToken*> gkPropertyTokenArray;





#endif//_gkPropertyToken_h_
