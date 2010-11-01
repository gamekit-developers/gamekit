/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Nestor Silveira & Charlie C.

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
#include "gkOgg.h"
#include "gkSoundUtil.h"
#include "gkLogger.h"


#define OV_FIXED_BUF 32768



static size_t gkOggRead(void* ptr, size_t bl, size_t nr, void* blk)
{
	utStream* block = (utStream*)blk;
	if (!block)
		return ((size_t) - 1);

	size_t nl = bl * nr;
	return block->read(ptr, nl);
}



static int gkOggSeek(void* blk, ogg_int64_t off, int way)
{
	utStream* block = (utStream*)blk;
	if (!block)
		return -1;

	block->seek((UTsize)off, way);
	return block->position();
}



static long gkOggTell(void* blk)
{
	utStream* block = (utStream*)blk;
	if (!block)
		return -1;

	return block->position();
}



gkOgg::gkOgg()
	:   m_reader(0),
	    m_stream(),
	    m_inf(0),
	    m_eos(false)
{
}



gkOgg::~gkOgg()
{
	ov_clear(&m_stream);


	if (m_reader)
	{
		delete m_reader;
		m_reader = 0;
	}

}


bool gkOgg::load(const char* fname)
{

	m_reader = new utFileStream();
	static_cast<utFileStream*>(m_reader)->open(fname, utStream::SM_READ);


	if (!m_reader->isOpen())
	{
		gkPrintf("OggVorbis: File %s loading failed.\n", fname);
		delete m_reader;
		m_reader = 0;
		return false;

	}

	m_callbacks.read_func   = gkOggRead;
	m_callbacks.seek_func   = gkOggSeek;
	m_callbacks.close_func  = 0;
	m_callbacks.tell_func   = gkOggTell;

	ov_open_callbacks(m_reader, &m_stream, 0, 0, m_callbacks);
	m_inf = ov_info(&m_stream, -1);
	m_eos = m_inf == 0;
	return !m_eos;
}




bool gkOgg::load(const char* buf, int len)
{
	m_reader = new utMemoryStream();
	static_cast<utMemoryStream*>(m_reader)->open(buf, len, utStream::SM_READ);

	if (!m_reader->isOpen())
	{
		gkPrintf("OggVorbis: Buffer loading failed.\n");
		delete m_reader;
		m_reader = 0;
		return false;
	}

	m_callbacks.read_func   = gkOggRead;
	m_callbacks.seek_func   = gkOggSeek;
	m_callbacks.close_func  = 0;
	m_callbacks.tell_func   = gkOggTell;

	ov_open_callbacks(m_reader, &m_stream, 0, 0, m_callbacks);

	m_inf = ov_info(&m_stream, -1);
	m_eos = m_inf == 0;
	return !m_eos;

}



const char* gkOgg::read(UTsize pos, UTsize len, UTsize& br)
{
	if (pos != UT_NPOS)
		seek(pos, SEEK_SET);
	return read(len, br);
}



const char* gkOgg::read(UTsize len, UTsize& br)
{
	static char blk[OV_FIXED_BUF] = {0};
	br = 0;
	int bs;

	if (len > OV_FIXED_BUF)
		len = OV_FIXED_BUF;

	while (br < len)
	{
		long v = ov_read(&m_stream, blk + br, len - br, 0, 2, 1, &bs);
		if (v == 0)     // end of stream
			break;
		else if (v > 0) // append
			br += v;
		else
		{
			gkPrintf("OggVorbis:: Block Read error!\n");
			br = 0;
			break;
		}
	}
	if (br == 0)
		m_eos = true;
	return blk;
}


bool gkOgg::eos(void)
{
	return m_eos;
}


void gkOgg::seek(UTsize pos, int way)
{
	m_eos = false;
	ov_pcm_seek(&m_stream, pos * ov_pcm_total(&m_stream, -1));
}



int gkOgg::getFormat(void) const
{
	if (m_inf != 0)
		return m_inf->channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
	return -1;
}


int gkOgg::getSampleRate(void) const
{
	if (m_inf)
		return m_inf->rate;
	return 0;
}



int gkOgg::getBitsPerSecond(void)  const
{
	return OV_FIXED_BUF;
}
