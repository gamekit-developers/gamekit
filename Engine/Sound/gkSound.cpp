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
#include "gkSound.h"
#include "gkBuffer.h"
#include "gkSoundManager.h"
#include "gkWaveform.h"
#include "gkOgg.h"



gkSound::gkSound(gkResourceManager *creator, const gkResourceName &name, const gkResourceHandle &handle)
	:   gkResource(creator, name, handle),
	    m_stream(0)
{
}


gkSound::~gkSound()
{
	stopPlayback();
	if (m_stream)
	{
		delete m_stream;
		m_stream = 0;
	}
}


bool gkSound::load(const char* fname)
{

	int magic = alReadMagic(fname);
	if (magic == GK_BUF_WAV)
	{
		if (m_stream)
			delete m_stream;

		gkWaveform* wave = new gkWaveform();
		if (!wave->load(fname))
		{
			delete wave;
			wave = 0;
		}
		m_stream = wave;
	}
	else if (magic == GK_BUF_OGG)
	{
		if (m_stream)
			delete m_stream;

		gkOgg* ogg = new gkOgg();
		if (!ogg->load(fname))
		{
			delete ogg;
			ogg = 0;
		}
		m_stream = ogg;
	}
	return m_stream != 0;
}



bool gkSound::loadToMemory(const char* file)
{
	bool result = false;
	if (alReadMagic(file) != GK_BUF_NULL)
	{
		FILE* fp = fopen(file, "rb");
		if (fp)
		{
			fseek(fp, 0L, SEEK_END);
			int len = ftell(fp);
			fseek(fp, 0L, SEEK_SET);


			char* temp = new char[len];
			fread(temp, 1, len, fp);
			result = load(temp, len);
			delete []temp;

			fclose(fp);
		}
	}

	return result;
}



bool gkSound::load(void* handle, UTsize len)
{
	if (handle && len > 0 && len != UT_NPOS)
	{

		char* cp = (char*)handle;
		int magic = alGetBufType(cp);

		if (magic != GK_BUF_NULL)
		{
			if (m_stream)
				delete m_stream;

			if (magic == GK_BUF_WAV)
			{
				gkWaveform* wave = new gkWaveform();
				if (!wave->load((char*)handle, len))
				{
					delete wave;
					wave = 0;
				}
				m_stream = wave;
			}
			else if (magic == GK_BUF_OGG)
			{
				gkOgg* ogg = new gkOgg();
				if (!ogg->load((char*)handle, len))
				{
					delete ogg;
					ogg = 0;
				}
				m_stream = ogg;
			}
			else m_stream = 0;

		}
	}
	return m_stream != 0;
}




void gkSound::stopPlayback(void)
{
	gkSoundManager& mgr = gkSoundManager::getSingleton();
	for (UTsize i = 0; i < m_sources.size(); ++i)
	{
		m_sources[i]->stop();
		GK_ASSERT(!m_sources[i]->isBound());
		mgr.notifySourceDestroyed(m_sources[i]);
	}
	m_sources.clear();
}



gkSource* gkSound::createSource(void)
{
	// add it to the manager
	gkSource* src = new gkSource(this);
	gkSoundManager::getSingleton().notifySourceCreated(src);
	m_sources.push_back(src);
	return src;
}



void gkSound::destroySource(gkSource* src)
{
	UTsize pos;
	if ((pos = m_sources.find(src)) != UT_NPOS)
	{
		gkSoundManager::getSingleton().notifySourceDestroyed(src);
		m_sources.erase(pos);
	}
}
