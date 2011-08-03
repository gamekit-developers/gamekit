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
#include "fbtBlend.h"
#include "fbtTables.h"
#include "fbtStreams.h"



const FBTuint32 GLOB = FBT_ID('G', 'L', 'O', 'B');

struct fbtIdDB
{
	const FBTuint16     m_code;
	fbtList             fbtBlend::*m_ptr;
};

fbtIdDB fbtData[] =
{
	{ FBT_ID2('S', 'C'), &fbtBlend::m_scene},
	{ FBT_ID2('L', 'I'), &fbtBlend::m_library },
	{ FBT_ID2('O', 'B'), &fbtBlend::m_object },
	{ FBT_ID2('M', 'E'), &fbtBlend::m_mesh },
	{ FBT_ID2('C', 'U'), &fbtBlend::m_curve },
	{ FBT_ID2('M', 'B'), &fbtBlend::m_mball },
	{ FBT_ID2('M', 'A'), &fbtBlend::m_mat },
	{ FBT_ID2('T', 'E'), &fbtBlend::m_tex },
	{ FBT_ID2('I', 'M'), &fbtBlend::m_image },
	{ FBT_ID2('L', 'T'), &fbtBlend::m_latt },
	{ FBT_ID2('L', 'A'), &fbtBlend::m_lamp },
	{ FBT_ID2('C', 'A'), &fbtBlend::m_camera },
	{ FBT_ID2('I', 'P'), &fbtBlend::m_ipo },
	{ FBT_ID2('K', 'E'), &fbtBlend::m_key },
	{ FBT_ID2('W', 'O'), &fbtBlend::m_world },
	{ FBT_ID2('S', 'N'), &fbtBlend::m_screen},
	{ FBT_ID2('P', 'Y'), &fbtBlend::m_script },
	{ FBT_ID2('V', 'F'), &fbtBlend::m_vfont },
	{ FBT_ID2('T', 'X'), &fbtBlend::m_text },
	{ FBT_ID2('S', 'O'), &fbtBlend::m_sound },
	{ FBT_ID2('G', 'R'), &fbtBlend::m_group },
	{ FBT_ID2('A', 'R'), &fbtBlend::m_armature },
	{ FBT_ID2('A', 'C'), &fbtBlend::m_action },
	{ FBT_ID2('N', 'T'), &fbtBlend::m_nodetree },
	{ FBT_ID2('B', 'R'), &fbtBlend::m_brush },
	{ FBT_ID2('P', 'A'), &fbtBlend::m_particle },
	{ FBT_ID2('G', 'D'), &fbtBlend::m_gpencil },
	{ FBT_ID2('W', 'M'), &fbtBlend::m_wm },
	{ 0, 0 }
};




extern unsigned char bfBlenderFBT[];
extern int bfBlenderLen;


fbtBlend::fbtBlend()
	:   fbtFile("BLENDER"), m_stripList(0)
{
	m_aluhid = "BLENDEs"; //a stripped blend file
}



fbtBlend::~fbtBlend()
{
}



int fbtBlend::initializeTables(fbtBinTables* tables)
{
	return tables->read(bfBlenderFBT, bfBlenderLen, false) ? FS_OK : FS_FAILED;
}



int fbtBlend::notifyData(void* p, const Chunk& id)
{
	if (id.m_code == GLOB)
	{
		m_fg = (Blender::FileGlobal*)p;
		return FS_OK;
	}

	if ((id.m_code <= 0xFFFF))
	{
		int i = 0;
		while (fbtData[i].m_code != 0)
		{
			if (fbtData[i].m_code == id.m_code)
			{
				(this->*fbtData[i].m_ptr).push_back(p);
				break;
			}
			++i;
		}
	}
	return FS_OK;
}


int fbtBlend::writeData(fbtStream* stream)
{
	fbtBinTables::OffsM::Pointer md = m_memory->m_offs.ptr();


	for (MemoryChunk* node = (MemoryChunk*)m_chunks.first; node; node = node->m_next)
	{
		if (node->m_newTypeId > m_memory->m_strcNr)
			continue;
		if (!node->m_newBlock)
			continue;

		void* wd = node->m_newBlock;

		Chunk ch;
		ch.m_code   = node->m_chunk.m_code;
		ch.m_nr     = node->m_chunk.m_nr;
		ch.m_len    = node->m_chunk.m_len;
		ch.m_typeid = node->m_newTypeId;
		ch.m_old    = (FBTsize)wd;

		stream->write(&ch, sizeof(Chunk));
		stream->write(wd, ch.m_len);
	}

	return FS_OK;
}



bool fbtBlend::skip(const FBTuint32& id)
{

	if (!m_stripList)
		return false;

	int i = 0;
	while (m_stripList[i] != 0)
	{
		if (m_stripList[i++] == id)
			return true;
	}

	return false;
}


void*   fbtBlend::getFBT(void)
{
	return (void*)bfBlenderFBT;
}

FBTsize fbtBlend::getFBTlength(void)
{
	return bfBlenderLen;
}

int fbtBlend::save(const char *path, const int mode)
{
	m_version = m_fileVersion;
	return reflect(path, mode);
}
