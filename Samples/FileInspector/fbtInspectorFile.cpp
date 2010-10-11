/*
-------------------------------------------------------------------------------
    This file is part of FBT (File Binary Tables).
    http://gamekit.googlecode.com/

    Copyright (c) 2010 Charlie C & Erwin Coumans.

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
#include <wx/filename.h>
#define FBT_IN_SOURCE
#include "fbtInspectorFile.h"
#include "fbtTables.h"
#include "fbtStreams.h"
#include "fbtPlatformHeaders.h"


namespace fbtIdCodes
{
const FBTuint32 GLOB = FBT_ID('P', 'R', 'O', 'J');
const FBTuint32 ENDB = FBT_ID('E', 'N', 'D', 'B');
const FBTuint32 FBT1 = FBT_ID('D', 'N', 'A', '1');
const FBTuint32 DATA = FBT_ID('D', 'A', 'T', 'A');

const FBTuint16 FP   = FBT_ID2('F', 'P');
}



fbtInspectorFile::fbtInspectorFile()
	:   fbtFile("INSPECT"),
	    m_project(0)
{
	m_project   = new fbtProjectFile();
	m_memory    = new fbtBinTables();
	m_dataIsData = true;

	initializeTables(m_memory);
}



fbtInspectorFile::~fbtInspectorFile()
{
	if (m_project)
	{
		delete m_project;
		m_project = 0;
	}
}



void fbtInspectorFile::writeStruct(fbtStream* stream, FBTtype index, FBTuint32 code, FBTsize len, void* writeData)
{
	Chunk ch;
	ch.m_code   = code;
	ch.m_len    = len;
	ch.m_nr     = 1;
	ch.m_old    = (FBTsize)writeData;
	ch.m_typeid = index;

	stream->write(&ch, sizeof(Chunk));
	stream->write(writeData, ch.m_len);
}


void fbtInspectorFile::writeData(fbtStream* stream, FBTsize len, void* writeData)
{
	Chunk ch;
	ch.m_code   = fbtIdCodes::DATA;
	ch.m_len    = len;
	ch.m_nr     = 1;
	ch.m_old    = (FBTsize)writeData;
	ch.m_typeid = 0;

	stream->write(&ch, sizeof(Chunk));
	stream->write(writeData, ch.m_len);
}


void fbtInspectorFile::writeGlobal(fbtStream* stream)
{
	m_project->m_version = INSPECTOR_VERSION_FILE;

	writeStruct(stream, m_memory->findTypeId("fbtProjectFile"), fbtIdCodes::GLOB, sizeof(fbtProjectFile), m_project);
	if (m_project->m_windowLayout)
		writeData(stream, strlen(m_project->m_windowLayout), m_project->m_windowLayout);


	if (m_project->m_projectFiles.m_first)
	{
		FBTuint32 code = m_memory->findTypeId("fbtText");


		for (fbtText* fp = m_project->m_projectFiles.m_first; fp; fp = fp->m_next)
		{
			int tf = fp->m_flag;
			fp->m_flag &= ~(fbtText::FILE_OWN | fbtText::FILE_MODIFIED);

			void* tfd = fp->m_textFile;
			fp->m_textFile = 0;

			writeStruct(stream, code, fbtIdCodes::FP, sizeof(fbtText), fp);
			if (fp->m_data)
				writeData(stream, fp->m_size, fp->m_data);


			fp->m_textFile = tfd;
			fp->m_flag = tf;
		}
	}
}



void fbtInspectorFile::save(const char* path)
{
	Chunk ch;
	fbtFileStream fs;
	fs.open(path, fbtStream::SM_WRITE);



	FBTuint8 cp = FBT_VOID8 ? FM_64_BIT : FM_32_BIT;
	FBTuint8 ce = ((FBTuint8)fbtGetEndian()) == FBT_ENDIAN_IS_BIG ? FM_BIG_ENDIAN : FM_LITTLE_ENDIAN;


	// put magic
	fs.writef("INSPECT%c%c%1i%1i%1i", cp, ce, INSP_VERSION_MAJOR, INSP_VERSION_MINOR, INSP_VERSION_SUB);

	writeGlobal(&fs);

	// write DNA1
	ch.m_code   = fbtIdCodes::FBT1;
	ch.m_len    = getFBTlength();
	ch.m_nr     = 1;
	ch.m_old    = 0;
	ch.m_typeid = 0;
	fs.write(&ch, sizeof(Chunk));
	fs.write(getFBT(), ch.m_len);


	// write ENDB (End Byte | EOF )
	ch.m_code   = fbtIdCodes::ENDB;
	ch.m_len    = 0;
	ch.m_nr     = 0;
	ch.m_old    = 0;
	ch.m_typeid = 0;
	fs.write(&ch, sizeof(Chunk));
}


int fbtInspectorFile::notifyData(void* p, const Chunk& id)
{
	if (id.m_code == fbtIdCodes::GLOB)
	{
		if (m_project)
			m_project->copy(((const fbtProjectFile*)p));

	}

	if (id.m_code == fbtIdCodes::FP)
	{
		if (m_project)
			m_project->addFile((fbtText*)p);
	}


	return FS_OK;
}



extern unsigned char fbtInspectorFBT[];
extern int fbtInspectorLen;

int fbtInspectorFile::initializeTables(fbtBinTables* tables)
{
	return tables->read(fbtInspectorFBT, fbtInspectorLen, false)  ? FS_OK : FS_FAILED;
}


void* fbtInspectorFile::getFBT(void)
{
	return fbtInspectorFBT;
}

FBTsize fbtInspectorFile::getFBTlength(void)
{
	return fbtInspectorLen;
}
