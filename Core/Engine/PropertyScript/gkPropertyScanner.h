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
#ifndef _gkPropertyScanner_h_
#define _gkPropertyScanner_h_

#include "gkPropertyToken.h"
#include "gkStack.h"




// ----------------------------------------------------------------------------
class gkPropertyScanner
{
public:
	gkPropertyScanner(const char *srcname, const Ogre::String &data, size_t len);
	virtual ~gkPropertyScanner();

	gkPropertyToken* scan(void);

	/// prevent unnecessary memory usage
	void doneToken(gkPropertyToken *tok);
protected:

	typedef gkArray<gkPropertyToken*> TokenAlloc;


	typedef char* CharPtr;
	void addCharacter(gkPropertyToken* ptr, CharPtr &tok);
	gkPropertyToken* makeToken(void);


	const char *source;	 /// source file name
	char *buffer, *tok;	 /// source buffer, current token iterator
	size_t len, line;	   /// buffer length, current line
	int m_state;			/// internal scan state
	TokenAlloc m_tokens;	/// token memory
	bool first;
};





#endif//_gkPropertyScanner_h_
