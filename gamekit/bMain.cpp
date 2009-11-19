/* Copyright (C) 2006 Charlie C
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would be
*    appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/
#include "bMain.h"
#include "bBlenderFile.h"
#include "bDefines.h"
#include "bChunk.h"
#include "bDNA.h"

using namespace bParse;


// ----------------------------------------------------- //
bMain::bMain(bBlenderFile *filePtr, const char *baseName, int fileVersion)
	:	mFP(filePtr),
		mVersion(fileVersion),
		mName(baseName)	
{
	mData[ID_SCE]  = bListBasePtr();
	mData[ID_LI]   = bListBasePtr();
	mData[ID_OB]   = bListBasePtr();
	mData[ID_ME]   = bListBasePtr();
	mData[ID_CU]   = bListBasePtr();
	mData[ID_MB]   = bListBasePtr();
	mData[ID_MA]   = bListBasePtr();
	mData[ID_TE]   = bListBasePtr();
	mData[ID_IM]   = bListBasePtr();
	mData[ID_WV]   = bListBasePtr();
	mData[ID_LT]   = bListBasePtr();
	mData[ID_LA]   = bListBasePtr();
	mData[ID_CA]   = bListBasePtr();
	mData[ID_IP]   = bListBasePtr();
	mData[ID_KE]   = bListBasePtr();
	mData[ID_WO]   = bListBasePtr();
	mData[ID_SCR]  = bListBasePtr();
	mData[ID_VF]   = bListBasePtr();
	mData[ID_TXT]  = bListBasePtr();
	mData[ID_SO]   = bListBasePtr();
	mData[ID_GR]   = bListBasePtr();
	mData[ID_AR]   = bListBasePtr();
	mData[ID_AC]   = bListBasePtr();
	mData[ID_NT]   = bListBasePtr();
	mData[ID_BR]   = bListBasePtr();
	mData[ID_SCRIPT] = bListBasePtr();
}


// ----------------------------------------------------- //
bMain::~bMain()
{
	// allocated data blocks!
	bListBasePtr::iterator it = mPool.begin();
	while (it != mPool.end())
	{
		delete [](*it);
		it++;
	}
}

// ----------------------------------------------------- //
int bMain::getVersion()
{
	return mVersion;
}

// ----------------------------------------------------- //
const char *bMain::getName()
{
	return mName;
}

// ----------------------------------------------------- //
void bMain::addDatablock(void *allocated)
{
	assert(allocated);
	mPool.push_back((bStructHandle*)allocated);
}

// ----------------------------------------------------- //
void *bMain::findLibPointer(void *ptr)
{
	bPtrMap::iterator it = mFP->getLibPointers().find(ptr);
	if (it != mFP->getLibPointers().end())
		return it->second;
	return 0;
}


// ------------------------------------------------------------//
void bMain::linkList(void *listBasePtr)
{

	struct ListBase // local Blender::ListBase
	{
		void *first;
		void *last;
	};

	struct Link // local Blender::Link
	{
		void *next;
		void *prev;
	};


	ListBase *base = (ListBase*)listBasePtr;

	if (!base || !base->first)
		return;

	base->first = findLibPointer(base->first);
	if (!base->first)
	{
		base->last = 0;
		return;
	}

	void *prev = 0;
	Link *l = (Link*)base->first;
	while (l)
	{
		l->next = findLibPointer(l->next);
		l->prev = l->next;
		prev = l->next;
		l = (Link*)l->next;
	}
}

// ------------------------------------------------------------//
bListBasePtr* bMain::getListBasePtr(int listBaseCode)
{
	bListBasePtr *ptr = _findCode(listBaseCode);
	if (!ptr)
		return 0;
	return ptr;
}

// ------------------------------------------------------------//
bListBasePtr *bMain::_findCode(int code)
{
	bMainDataMap::iterator it = mData.find(code);
	if (it != mData.end())
		return &it->second;
	return 0;
}


// ------------------------------------------------------------//
bListBasePtr *bMain::getScene()
{
	bListBasePtr *ptr = _findCode(ID_SCE);
	if (!ptr)
		return 0;
	return ptr;
}

// ------------------------------------------------------------//
bListBasePtr *bMain::getLibrary()
{
	bListBasePtr *ptr = _findCode(ID_LI);
	if (!ptr)
		return 0;
	return ptr;
}
// ------------------------------------------------------------//
bListBasePtr *bMain::getObject()
{
	bListBasePtr *ptr = _findCode(ID_OB);
	if (!ptr)
		return 0;
	return ptr;
}

// ------------------------------------------------------------//
bListBasePtr *bMain::getMesh()
{
	bListBasePtr *ptr = _findCode(ID_ME);
	if (!ptr)
		return 0;
	return ptr;
}

// ------------------------------------------------------------//
bListBasePtr *bMain::getCurve()
{
	bListBasePtr *ptr = _findCode(ID_CU);
	if (!ptr)
		return 0;
	return ptr;
}



// ------------------------------------------------------------//
bListBasePtr *bMain::getMball()
{
	bListBasePtr *ptr = _findCode(ID_MB);
	if (!ptr)
		return 0;
	return ptr;
}

// ------------------------------------------------------------//
bListBasePtr *bMain::getMat()
{
	bListBasePtr *ptr = _findCode(ID_MA);
	if (!ptr)
		return 0;
	return ptr;
}

// ------------------------------------------------------------//
bListBasePtr *bMain::getTex()
{
	bListBasePtr *ptr = _findCode(ID_TE);
	if (!ptr)
		return 0;
	return ptr;
}


// ------------------------------------------------------------//
bListBasePtr *bMain::getImage()
{
	bListBasePtr *ptr = _findCode(ID_IM);
	if (!ptr)
		return 0;
	return ptr;
}

// ------------------------------------------------------------//
bListBasePtr *bMain::getWave()
{
	bListBasePtr *ptr = _findCode(ID_WV);
	if (!ptr)
		return 0;
	return ptr;
}

// ------------------------------------------------------------//
bListBasePtr *bMain::getLatt()
{
	bListBasePtr *ptr = _findCode(ID_LT);
	if (!ptr)
		return 0;
	return ptr;
}

// ------------------------------------------------------------//
bListBasePtr *bMain::getLamp()
{
	bListBasePtr *ptr = _findCode(ID_LA);
	if (!ptr)
		return 0;
	return ptr;
}

// ------------------------------------------------------------//
bListBasePtr *bMain::getCamera()
{
	bListBasePtr *ptr = _findCode(ID_CA);
	if (!ptr)
		return 0;
	return ptr;
}

// ------------------------------------------------------------//
bListBasePtr *bMain::getIpo()
{
	bListBasePtr *ptr = _findCode(ID_IP);
	if (!ptr)
		return 0;
	return ptr;
}

// ------------------------------------------------------------//
bListBasePtr *bMain::getKey()
{
	bListBasePtr *ptr = _findCode(ID_KE);
	if (!ptr)
		return 0;
	return ptr;
}

// ------------------------------------------------------------//
bListBasePtr *bMain::getWorld()
{
	bListBasePtr *ptr = _findCode(ID_WO);
	if (!ptr)
		return 0;
	return ptr;
}


// ------------------------------------------------------------//
bListBasePtr *bMain::getScreen()
{
	bListBasePtr *ptr = _findCode(ID_SCR);
	if (!ptr)
		return 0;
	return ptr;
}

// ------------------------------------------------------------//
bListBasePtr *bMain::getScript()
{
	bListBasePtr *ptr = _findCode(ID_SCRIPT);
	if (!ptr)
		return 0;
	return ptr;
}

// ------------------------------------------------------------//
bListBasePtr *bMain::getVfont()
{
	bListBasePtr *ptr = _findCode(ID_VF);
	if (!ptr)
		return 0;
	return ptr;
}

// ------------------------------------------------------------//
bListBasePtr *bMain::getText()
{
	bListBasePtr *ptr = _findCode(ID_TXT);
	if (!ptr)
		return 0;
	return ptr;
}

// ------------------------------------------------------------//
bListBasePtr *bMain::getSound()
{
	bListBasePtr *ptr = _findCode(ID_SO);
	if (!ptr)
		return 0;
	return ptr;
}

// ------------------------------------------------------------//
bListBasePtr *bMain::getGroup()
{
	bListBasePtr *ptr = _findCode(ID_GR);
	if (!ptr)
		return 0;
	return ptr;
}

// ------------------------------------------------------------//
bListBasePtr *bMain::getArmature()
{
	bListBasePtr *ptr = _findCode(ID_AR);
	if (!ptr)
		return 0;
	return ptr;
}

// ------------------------------------------------------------//
bListBasePtr *bMain::getAction()
{
	bListBasePtr *ptr = _findCode(ID_AC);
	if (!ptr)
		return 0;
	return ptr;
}


// ------------------------------------------------------------//
bListBasePtr *bMain::getNodetree()
{
	bListBasePtr *ptr = _findCode(ID_NT);
	if (!ptr)
		return 0;
	return ptr;
}

// ------------------------------------------------------------//
bListBasePtr *bMain::getBrush()
{
	bListBasePtr *ptr = _findCode(ID_BR);
	if (!ptr)
		return 0;
	return ptr;
}

void	bMain::dumpChunks(bParse::bDNA* dna)
{
	int i;

	for (i=0;i<m_chunks.size();i++)
	{
		bChunkInd& dataChunk = m_chunks[i];
		char* codeptr = (char*)&dataChunk.code;
		char codestr[5] = {codeptr[0],codeptr[1],codeptr[2],codeptr[3],0};
		
		short* newStruct = dna->getStruct(dataChunk.dna_nr);
		char* typeName = dna->getType(newStruct[0]);
		printf("%3d: %s  ",i,typeName);

		printf("code=%s  ",codestr);
		
		printf("ptr=%p  ",dataChunk.oldPtr);
		printf("len=%d  ",dataChunk.len);
		printf("nr=%d  ",dataChunk.nr);
		if (dataChunk.nr!=1)
		{
			printf("not 1\n");
		}
		printf("\n");

		
		

	}

#if 0
	IDFinderData ifd;
	ifd.success = 0;
	ifd.IDname = NULL;
	ifd.just_print_it = 1;
	for (i=0; i<bf->m_blocks.size(); ++i) 
	{
		BlendBlock* bb = bf->m_blocks[i];
		printf("tag='%s'\tptr=%p\ttype=%s\t[%4d]",		bb->tag, bb,bf->types[bb->type_index].name,bb->m_array_entries_.size());
		block_ID_finder(bb, bf, &ifd);
		printf("\n");
	}
#endif

}


//eof
