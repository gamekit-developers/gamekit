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
#include "blender.h"

#include "bAscii.h"
#include "bAsciiMain.h"
#include "bAsciiCompiler.h"
#include "bAsciiBlendLinker.h"
#include "bAsciiScanner.h"
#include "bAsciiBlock.h"
#include "utPrint.h"


// ----------------------------------------------------------------------------
bAscii::bAscii() : m_main(0), m_compiled(false)
{
}

// ----------------------------------------------------------------------------
bAscii::~bAscii()
{
	clearBlocks();

	if (m_main) delete m_main;

}

// ----------------------------------------------------------------------------
bAsciiBlock* bAscii::findBlock(const bStringF &name)
{
	size_t pos;
	if ((pos = m_findBlocks.find(name)) != UT_NPOS)
		return m_findBlocks.at(pos);
	return 0;
}

// ----------------------------------------------------------------------------
void bAscii::clearBlocks(void)
{
	for (size_t i=0; i<m_blocks.size(); ++i)
		delete m_blocks[i];
	m_blocks.clear();
	m_findBlocks.clear();
}

// ----------------------------------------------------------------------------
void bAscii::addBlock(bAsciiBlock *block)
{
	if (block)
		m_blocks.push_back(block);
}

// ----------------------------------------------------------------------------
void bAscii::addBlock(const bStringF &name, bAsciiBlock *block)
{
	if (block)
	{
		if (m_findBlocks.find(name) == UT_NPOS)
			m_findBlocks.insert(name, block);
	}
}

// ----------------------------------------------------------------------------
void bAscii::addBlock(const bCode code, bAsciiBlock *block)
{
	if (code >= OP_LIB_GB && code < OP_LIB_MAX)
	{
		if (m_codeMapBlocks.find(code) == UT_NPOS)
			m_codeMapBlocks.insert(code, block);
	}
}

// ----------------------------------------------------------------------------
bAsciiBlock* bAscii::findBlock(bCode code)
{
	size_t pos;
	if ((pos = m_codeMapBlocks.find(code)) != UT_NPOS)
		return m_codeMapBlocks.at(pos);
	return 0;
}

// ----------------------------------------------------------------------------
int bAscii::load(const char *pathname, bool verbose)
{
	if (verbose)
		utPrintf("Parsing file: %s\n", pathname);

	bAsciiScanner sc(pathname);
	if (sc.getError() < 0)
	{
		utPrintf("Parsing file: %s failed\n", pathname);
		return -1;
	}

	if (verbose)
		utPrintf("Starting scan...\n");

	bAscciBuildTables();


	bAsciiCompiler cmplr(this, verbose);
	if (!cmplr.compile(sc))
		return -1;

	if (verbose)
		utPrintf("Scan complete...\n");

	m_compiled = true;
	return 0;
}

// ----------------------------------------------------------------------------
bAsciiMain* bAscii::linkBlenderMain(bool verbose, bool freeblocks)
{
	if (m_compiled && !m_main)
	{
		bAsciiMain *mainPtr = new bAsciiMain(this);


		BlendLinker lnk;
		int status = lnk.link(this, mainPtr, verbose);

		if (status < 0) delete mainPtr;
		else m_main = mainPtr;

		if (freeblocks) clearBlocks();
	}
	return m_main;
}


// ----------------------------------------------------------------------------
bAsciiMain* bAscii::loadBlender(const char *pathname, bool verbose)
{
	if (load(pathname, verbose) == 0)
		return linkBlenderMain(verbose, true);
	return 0;
}
