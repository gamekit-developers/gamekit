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
#ifndef _gkWaveform_h_
#define _gkWaveform_h_

#include "gkSoundStream.h"
#include "utStreams.h"


class gkWaveform : public gkSoundStream
{
public:

	typedef struct Header
	{
		// 'fmt ' chunk
		short           m_tag;              // WaveFmtCode in WAVE_FORMAT_PCM
		short           m_channels;         // Number of channels
		int             m_samplesPerSec;    // Samples per second
		int             m_bytesPerSec;      // Data rate
		short           m_bklAlign;         // data block size
		short           m_bits;             // Bits per sample
	} Header;

private:

	Header      m_header;       // Format header
	int         m_sampleStart;  // start of 'data' chunk
	int         m_totalLen;     // length of 'data' chunk
	utStream*   m_reader;
	char*       m_data;

	bool        loadStreamImpl(void);


public:

	gkWaveform();
	virtual ~gkWaveform();

	bool load(const char* fname);
	bool load(const char* buf, int len);


	const Header&   getHeader(void)    const   { return m_header; }

	const char*     read(UTsize len, UTsize& br);
	const char*     read(UTsize pos, UTsize len, UTsize& br);
	bool            eos(void);
	void            seek(UTsize pos, int way);

	int             getFormat(void)         const;
	int             getSampleRate(void)     const { return m_header.m_samplesPerSec; }
	int             getBitsPerSecond(void)  const;
};

#endif//_gkWaveform_h_
