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
#ifndef _gkOgg_h_
#define _gkOgg_h_

#include "gkSoundStream.h"
#include "utStreams.h"

#include <stdio.h>

#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>



/// Ogg File & memory decoder
class gkOgg : public gkSoundStream
{
private:
	utStream*        m_reader;
	OggVorbis_File  m_stream;
	vorbis_info*    m_inf;

	bool            m_eos;
	ov_callbacks    m_callbacks;

public:

	gkOgg();
	virtual ~gkOgg();

	bool load(const char* fname);
	bool load(const char* buf, int len);


	// stream impl
	const char*      read(UTsize len, UTsize& br);
	const char*      read(UTsize pos, UTsize len, UTsize& br);
	bool            eos(void);
	void            seek(UTsize pos, int way);

	int             getFormat(void)     const;
	int             getSampleRate(void) const;
	int             getBitsPerSecond(void)  const;
};

#endif//_gkOgg_h_
