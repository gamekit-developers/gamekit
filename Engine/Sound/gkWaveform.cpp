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
#include "gkWaveform.h"
#include "gkSoundUtil.h"
#include "gkLogger.h"
#include "utTypes.h"



static void gkWaveform_SwapInt(int& v)
{
	char* p = (char*)&v;
	utSwap(p[0], p[3]);
	utSwap(p[1], p[2]);
}



static void gkWaveform_SwapShort(short& v)
{
	char* p = (char*)&v;
	utSwap(p[0], p[1]);
}


static bool gkWaveform_SwapEndian(void)
{
	int littleEndian = 1;
	littleEndian = ((char*)&littleEndian)[0];
	return littleEndian == 0;
}


static void gkWaveform_SwapHeader(gkWaveform::Header& v)
{
	gkWaveform_SwapShort(v.m_tag);
	gkWaveform_SwapShort(v.m_channels);
	gkWaveform_SwapInt(v.m_samplesPerSec);
	gkWaveform_SwapInt(v.m_bytesPerSec);
	gkWaveform_SwapShort(v.m_bklAlign);
	gkWaveform_SwapShort(v.m_bits);
}



gkWaveform::gkWaveform()
	:   m_header(),
		m_reader(0),
	    m_sampleStart(0),
	    m_totalLen(0),
	    m_data(0)
{
	memset(&m_header, 0, sizeof(Header));
}



gkWaveform::~gkWaveform()
{
	if (m_reader)
	{
		delete m_reader;
		m_reader = 0;
	}

	if (m_data)
	{
		delete []m_data;
		m_data = 0;
	}
}


bool gkWaveform::loadStreamImpl(void)
{
	// Specs derrived from.
	// http://www-mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html

	if (!m_reader)
		return false;


	m_reader->seek(0, SEEK_SET);

	bool swap = gkWaveform_SwapEndian();
	m_sampleStart = 0;


	char blk[5];
	m_reader->read(blk, 4);
	if (strncmp(blk, "RIFF", 4))
	{
		gkPrintf("Waveform: Invalid RIFF Block\n");
		return false;
	}
	int ti;
	m_reader->read(&ti, sizeof(int));

	m_reader->read(blk, 4);
	if (strncmp(blk, "WAVE", 4))
	{
		gkPrintf("Waveform: Invalid WAVE Block\n");
		return false;
	}


	// wave chunks
	m_sampleStart = m_reader->position();


	for (UTsize i = m_sampleStart; i < m_reader->size(); ++i)
	{
		m_reader->read(blk, 4);

		if (!strncmp(blk, "fmt ", 4))
		{
			// parse format

			int chunkSize;
			m_reader->read(&chunkSize, sizeof(int));

			if (swap)
				gkWaveform_SwapInt(chunkSize);

			assert(chunkSize == 16 || chunkSize == 18 || chunkSize == 40);
			assert(sizeof(Header) == 16);

			// 16 block size
			m_reader->read(&m_header, sizeof(Header));
			if (swap)
				gkWaveform_SwapHeader(m_header);


			// TODO: handle extensions
			m_reader->seek(chunkSize - sizeof(Header), SEEK_CUR);

		}
		else if (!strncmp(blk, "data", 4))
		{
			m_reader->read(&m_totalLen, sizeof(int));
			if (swap)
				gkWaveform_SwapInt(m_totalLen);

			m_sampleStart = m_reader->position();
			return true;
		}
		else
		{
			// skip over unused chunks
			int chunkSize;
			m_reader->read(&chunkSize, sizeof(int));

			if (swap)
				gkWaveform_SwapInt(chunkSize);

			m_reader->seek(chunkSize, SEEK_CUR);
		}
	}
	return false;
}



bool gkWaveform::load(const char* fname)
{
	m_reader = new utFileStream();
	static_cast<utFileStream*>(m_reader)->open(fname, utStream::SM_READ);


	if (!m_reader->isOpen())
	{
		gkPrintf("Waveform: File %s loading failed.\n", fname);
		return false;
	}

	if (!loadStreamImpl())
	{
		gkPrintf("Waveform: File %s loading failed.\n", fname);
		return false;
	}

	// make sure a valid format & block size is present
	if (getFormat() <= 0)
	{
		gkPrintf("Waveform: File %s loading failed. (invalid format read)\n", fname);
		return false;
	}


	if (getBitsPerSecond() <= 0)
	{
		gkPrintf("Waveform: File %s loading failed. (invalid block size (%i))\n", fname, getBitsPerSecond());
		return false;
	}



	m_data = new char[getBitsPerSecond() + 1];
	return true;
}



bool gkWaveform::load(const char* buf, int len)
{
	m_reader = new utMemoryStream();
	static_cast<utMemoryStream*>(m_reader)->open(buf, len, utStream::SM_READ);

	if (!m_reader->isOpen())
	{
		gkPrintf("Waveform: Buffer loading failed.\n");
		return false;
	}

	if (!loadStreamImpl())
	{
		gkPrintf("Waveform: Buffer loading failed.\n");
		return false;
	}

	// make sure a valid format & block size is present
	if (getFormat() <= 0)
	{
		gkPrintf("Waveform: Buffer loading failed. (invalid format read)\n");
		return false;
	}


	if (getBitsPerSecond() <= 0)
	{
		gkPrintf("Waveform: Buffer loading failed. (invalid block size (%i))\n", getBitsPerSecond());
		return false;
	}


	m_data = new char[getBitsPerSecond() + 1];
	return true;
}



int gkWaveform::getFormat(void) const
{
	if (m_header.m_channels <= 2)
	{
		if (m_header.m_bits <= 16)
		{
			return m_header.m_channels == 1 ?
			       (m_header.m_bits == 16 ? AL_FORMAT_MONO16 : AL_FORMAT_MONO8) :
				       (m_header.m_bits == 16 ? AL_FORMAT_STEREO16 : AL_FORMAT_STEREO8);
		}
	}
	return -1;
}



int gkWaveform::getBitsPerSecond(void)  const
{
	return (m_header.m_samplesPerSec * m_header.m_bklAlign ) << 1;
}



const char* gkWaveform::read(UTsize pos, UTsize len, UTsize& br)
{
	if (pos != UT_NPOS)
		seek(pos, SEEK_SET);
	return read(len, br);
}



const char* gkWaveform::read(UTsize len, UTsize& br)
{
	br = 0;
	if (m_reader)
	{
		br = m_reader->read(m_data, len);
		return m_data;
	}
	return 0;
}



bool gkWaveform::eos(void)
{
	return m_reader ? m_reader->eof() : true;
}



void gkWaveform::seek(UTsize pos, int dir)
{
	if (m_reader)
		m_reader->seek(m_sampleStart + pos, dir);
}
