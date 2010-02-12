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
#ifndef _bAscii_h_
#define _bAscii_h_

#include "bAsciiCommon.h"
#include "bUtils/utAllocator.h"

/// Parser for the .ascii file format
class bAscii : public utAllocObject
{
protected:

	bAsciiMain*			m_main;
	bAsciiBlockArray	m_blocks;
	bAsciiBlockMap		m_findBlocks;
	bAsciiBlockMapI		m_codeMapBlocks;
	bool				m_compiled;

public:

	bAscii();
	~bAscii();

	int load(const char *pathname, bool verbose=false);
	bAsciiMain* loadBlender(const char *pathname, bool verbose);

	void clearBlocks(void);
	void addBlock(bAsciiBlock *block);
	void addBlock(const bStringF &name, bAsciiBlock *block);
	void addBlock(const bCode code, bAsciiBlock *block);

	bAsciiBlock* findBlock(const bStringF &name);
	bAsciiBlock* findBlock(bCode code);

	bAsciiMain* linkBlenderMain(bool verbose, bool freeblocks = true);

	UT_INLINE bAsciiBlockIterator	getBlockIterator(void)	{return bAsciiBlockIterator(m_blocks);}
	UT_INLINE bAsciiBlockArray&		getBlocks(void)			{return m_blocks;}
	UT_INLINE bAsciiMain*			getMain(void)			{return m_main;}
};


#endif//_bAscii_h_
