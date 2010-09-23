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
#include "gkCommon.h"
#include "gkLogger.h"
#include "gkSoundUtil.h"

#if UT_PLATFORM == UT_PLATFORM_WIN32
#include <windows.h>

extern "C" int OpenAL_LoadLibrary( void );
extern "C" void OpenAL_UnloadLibrary( void );
#endif



bool alOpenLibrary(void)
{
#if UT_PLATFORM == UT_PLATFORM_WIN32
	int status = OpenAL_LoadLibrary();

	if (!status)
	{
		gkLogMessage("OpenAL: Failed to load shared library!");
		return false;
	}

	return true;

#else
	return true;
#endif
}


void alCloseLibrary(void)
{
#if UT_PLATFORM == UT_PLATFORM_WIN32
	OpenAL_UnloadLibrary();
#endif
}



bool alIsPlaying(ALuint src)
{
	ALint state;
	alGetSourcei(src, AL_SOURCE_STATE, &state);
	return state == AL_PLAYING;
}


bool alErrorCheck(const char* message)
{
	ALenum err;
	if ((err = alGetError()) != AL_NO_ERROR)
	{
		gkPrintf("AL Error: %s occured, in call to ==> %s\n", alGetString(err), message);
		return true;
	}
	return false;
}


bool alErrorThrow(const char* message)
{
	return alErrorCheck(message);
}



bool alErrorCheck(void)
{
	ALint err;
	ALCdevice* device = alcGetContextsDevice(alcGetCurrentContext());
	if ((err = alcGetError(device)) != ALC_NO_ERROR)
	{
		gkPrintf("ALC Error: %s occured\n", alcGetString(device, err));
		return true;
	}

	if ((err = alGetError()) != AL_NO_ERROR)
	{
		gkPrintf("AL Error: %s occured\n", alGetString(err));
		return true;
	}
	return false;

}



int alGetBufType(const char* magic)
{
	if (!strncmp(magic, "RIFF", 4))
		return GK_BUF_WAV;
	if (!strncmp(magic, "OggS", 4))
		return GK_BUF_OGG;
	return GK_BUF_NULL;
}


int alReadMagic(const char* file)
{
	char buf[5] = {'\0'};

	FILE* fp = fopen(file, "rb");
	if (!fp)
		return GK_BUF_NULL;

	fread(buf, 4, 1, fp);
	int result = alGetBufType(buf);
	fclose(fp);
	return result;
}
