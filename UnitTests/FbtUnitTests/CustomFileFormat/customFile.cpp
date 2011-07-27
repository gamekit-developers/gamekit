/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

    Contributor(s): none yet.
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

#include "customFile.h"

#include "fbtTables.h"
#include "fbtStreams.h"

#include <malloc.h>

extern unsigned char cstmFileFBT[];
extern int cstmFileLen;

static char cstmFile_Header[8] = {'C', 'U', 'S', 'T', 'F', 'B', 'T', '\0'};

namespace cstmIdCodes
{
const FBTuint32 GLOB = FBT_ID('G', 'L', 'O', 'B');
const FBTuint32 ENDB = FBT_ID('E', 'N', 'D', 'B');
const FBTuint32 FBT1 = FBT_ID('D', 'N', 'A', '1');
const FBTuint32 DATA = FBT_ID('D', 'A', 'T', 'A');
const FBTuint16 CSTR = FBT_ID2('S', 'T');

}



cstmFile::cstmFile() : fbtFile(cstmFile_Header)
{
	m_version = 1;
	
	m_memory = new fbtBinTables();
	initializeTables(m_memory);
	
	//m_global = (Custom::cstmGlobal*) malloc(sizeof(Custom::cstmGlobal));
	m_global = new Custom::cstmGlobal();
}

cstmFile::~cstmFile()
{
	if(m_global)
	{
		delete m_global;
		m_global = 0;
	}
}

int cstmFile::initializeTables(fbtBinTables* tables)
{
	return tables->read(cstmFileFBT, cstmFileLen, false) ? FS_OK : FS_FAILED;
}

void* cstmFile::getFBT(void)
{
	return cstmFileFBT;
}

FBTsize cstmFile::getFBTlength(void)
{
	return cstmFileLen;
}

int cstmFile::writeData(fbtStream* stream)
{
	writeStruct(stream, m_memory->findTypeId("cstmGlobal"), cstmIdCodes::GLOB, sizeof(Custom::cstmGlobal), m_global);
	
	Custom::cstmStruct* s = m_global->main.structList.m_first;
	while(s)
	{
		writeStruct(stream, m_memory->findTypeId("cstmStruct"), cstmIdCodes::CSTR, sizeof(Custom::cstmStruct), s);
		s = s->m_next;
	}
	
	return FS_OK;
}

int cstmFile::notifyData(void* p, const Chunk& id)
{
	if (id.m_code == cstmIdCodes::GLOB)
	{
		if (m_global)
			memcpy(m_global, p, sizeof(Custom::cstmGlobal));
	}
	
	return FS_OK;
}

bool cstmFile::skip(const FBTuint32& id)
{
	return false;
}

