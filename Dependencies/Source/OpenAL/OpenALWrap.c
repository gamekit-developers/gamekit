/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

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
#include <windows.h>
#define AL_NO_PROTOTYPES
#define ALC_NO_PROTOTYPES
#include "al.h"
#include "alc.h"

static HMODULE OpenAL = NULL;

#define DO_TRACE 0
#if DO_TRACE
#include <stdio.h>
static int OpenAL_TOTAL   = 0;
static int OpenAL_MISSING = 0;


#define OpenAL_InitSym(FP, T, DEST) \
    FP= (T)GetProcAddress(OpenAL, DEST);\
	if (FP)\
	{printf("Loaded ==> %s\n", DEST); OpenAL_TOTAL += 1;}\
	else\
	{printf("Missing ==> %s\n", DEST); OpenAL_MISSING += 1;}

#else
# define OpenAL_InitSym(FP, T, DEST) FP= (T)GetProcAddress(OpenAL, DEST);
#endif
/******************************************************************************
    Bring symbols into existence
 ******************************************************************************
 */
LPALENABLE __alWrapalEnable = NULL;
LPALDISABLE __alWrapalDisable = NULL;
LPALISENABLED __alWrapalIsEnabled = NULL;
LPALGETSTRING __alWrapalGetString = NULL;
LPALGETBOOLEANV __alWrapalGetBooleanv = NULL;
LPALGETINTEGERV __alWrapalGetIntegerv = NULL;
LPALGETFLOATV __alWrapalGetFloatv = NULL;
LPALGETDOUBLEV __alWrapalGetDoublev = NULL;
LPALGETBOOLEAN __alWrapalGetBoolean = NULL;
LPALGETINTEGER __alWrapalGetInteger = NULL;
LPALGETFLOAT __alWrapalGetFloat = NULL;
LPALGETDOUBLE __alWrapalGetDouble = NULL;
LPALGETERROR __alWrapalGetError = NULL;
LPALISEXTENSIONPRESENT __alWrapalIsExtensionPresent = NULL;
LPALGETPROCADDRESS __alWrapalGetProcAddress = NULL;
LPALGETENUMVALUE __alWrapalGetEnumValue = NULL;
LPALLISTENERF __alWrapalListenerf = NULL;
LPALLISTENER3F __alWrapalListener3f = NULL;
LPALLISTENERFV __alWrapalListenerfv = NULL;
LPALLISTENERI __alWrapalListeneri = NULL;
LPALLISTENER3I __alWrapalListener3i = NULL;
LPALLISTENERIV __alWrapalListeneriv = NULL;
LPALGETLISTENERF __alWrapalGetListenerf = NULL;
LPALGETLISTENER3F __alWrapalGetListener3f = NULL;
LPALGETLISTENERFV __alWrapalGetListenerfv = NULL;
LPALGETLISTENERI __alWrapalGetListeneri = NULL;
LPALGETLISTENER3I __alWrapalGetListener3i = NULL;
LPALGETLISTENERIV __alWrapalGetListeneriv = NULL;
LPALGENSOURCES __alWrapalGenSources = NULL;
LPALDELETESOURCES __alWrapalDeleteSources = NULL;
LPALISSOURCE __alWrapalIsSource = NULL;
LPALSOURCEF __alWrapalSourcef = NULL;
LPALSOURCE3F __alWrapalSource3f = NULL;
LPALSOURCEFV __alWrapalSourcefv = NULL;
LPALSOURCEI __alWrapalSourcei = NULL;
LPALSOURCE3I __alWrapalSource3i = NULL;
LPALSOURCEIV __alWrapalSourceiv = NULL;
LPALGETSOURCEF __alWrapalGetSourcef = NULL;
LPALGETSOURCE3F __alWrapalGetSource3f = NULL;
LPALGETSOURCEFV __alWrapalGetSourcefv = NULL;
LPALGETSOURCEI __alWrapalGetSourcei = NULL;
LPALGETSOURCE3I __alWrapalGetSource3i = NULL;
LPALGETSOURCEIV __alWrapalGetSourceiv = NULL;
LPALSOURCEPLAYV __alWrapalSourcePlayv = NULL;
LPALSOURCESTOPV __alWrapalSourceStopv = NULL;
LPALSOURCEREWINDV __alWrapalSourceRewindv = NULL;
LPALSOURCEPAUSEV __alWrapalSourcePausev = NULL;
LPALSOURCEPLAY __alWrapalSourcePlay = NULL;
LPALSOURCESTOP __alWrapalSourceStop = NULL;
LPALSOURCEREWIND __alWrapalSourceRewind = NULL;
LPALSOURCEPAUSE __alWrapalSourcePause = NULL;
LPALSOURCEQUEUEBUFFERS __alWrapalSourceQueueBuffers = NULL;
LPALSOURCEUNQUEUEBUFFERS __alWrapalSourceUnqueueBuffers = NULL;
LPALGENBUFFERS __alWrapalGenBuffers = NULL;
LPALDELETEBUFFERS __alWrapalDeleteBuffers = NULL;
LPALISBUFFER __alWrapalIsBuffer = NULL;
LPALBUFFERDATA __alWrapalBufferData = NULL;
LPALBUFFERF __alWrapalBufferf = NULL;
LPALBUFFER3F __alWrapalBuffer3f = NULL;
LPALBUFFERFV __alWrapalBufferfv = NULL;
LPALBUFFERI __alWrapalBufferi = NULL;
LPALBUFFER3I __alWrapalBuffer3i = NULL;
LPALBUFFERIV __alWrapalBufferiv = NULL;
LPALGETBUFFERF __alWrapalGetBufferf = NULL;
LPALGETBUFFER3F __alWrapalGetBuffer3f = NULL;
LPALGETBUFFERFV __alWrapalGetBufferfv = NULL;
LPALGETBUFFERI __alWrapalGetBufferi = NULL;
LPALGETBUFFER3I __alWrapalGetBuffer3i = NULL;
LPALGETBUFFERIV __alWrapalGetBufferiv = NULL;
LPALDOPPLERFACTOR __alWrapalDopplerFactor = NULL;
LPALDOPPLERVELOCITY __alWrapalDopplerVelocity = NULL;
LPALSPEEDOFSOUND __alWrapalSpeedOfSound = NULL;
LPALDISTANCEMODEL __alWrapalDistanceModel = NULL;
LPALCCREATECONTEXT __alWrapalcCreateContext = NULL;
LPALCMAKECONTEXTCURRENT __alWrapalcMakeContextCurrent = NULL;
LPALCPROCESSCONTEXT __alWrapalcProcessContext = NULL;
LPALCSUSPENDCONTEXT __alWrapalcSuspendContext = NULL;
LPALCDESTROYCONTEXT __alWrapalcDestroyContext = NULL;
LPALCGETCURRENTCONTEXT __alWrapalcGetCurrentContext = NULL;
LPALCGETCONTEXTSDEVICE __alWrapalcGetContextsDevice = NULL;
LPALCOPENDEVICE __alWrapalcOpenDevice = NULL;
LPALCCLOSEDEVICE __alWrapalcCloseDevice = NULL;
LPALCGETERROR __alWrapalcGetError = NULL;
LPALCISEXTENSIONPRESENT __alWrapalcIsExtensionPresent = NULL;
LPALCGETPROCADDRESS __alWrapalcGetProcAddress = NULL;
LPALCGETENUMVALUE __alWrapalcGetEnumValue = NULL;
LPALCGETSTRING __alWrapalcGetString = NULL;
LPALCGETINTEGERV __alWrapalcGetIntegerv = NULL;
LPALCCAPTUREOPENDEVICE __alWrapalcCaptureOpenDevice = NULL;
LPALCCAPTURECLOSEDEVICE __alWrapalcCaptureCloseDevice = NULL;
LPALCCAPTURESTART __alWrapalcCaptureStart = NULL;
LPALCCAPTURESTOP __alWrapalcCaptureStop = NULL;
LPALCCAPTURESAMPLES __alWrapalcCaptureSamples = NULL;

/******************************************************************************
    Load OpenAL
 ******************************************************************************
 */
int OpenAL_LoadLibrary( void )
{
#if DO_TRACE
    OpenAL_TOTAL= 0;
    OpenAL_MISSING= 0;
	printf("Loading OpenAL32.dll\n");
#endif

    OpenAL = LoadLibrary("OpenAL32.dll");
    if ( OpenAL == NULL ) return 0;
	OpenAL_InitSym(__alWrapalEnable, LPALENABLE, "alEnable");
	OpenAL_InitSym(__alWrapalDisable, LPALDISABLE, "alDisable");
	OpenAL_InitSym(__alWrapalIsEnabled, LPALISENABLED, "alIsEnabled");
	OpenAL_InitSym(__alWrapalGetString, LPALGETSTRING, "alGetString");
	OpenAL_InitSym(__alWrapalGetBooleanv, LPALGETBOOLEANV, "alGetBooleanv");
	OpenAL_InitSym(__alWrapalGetIntegerv, LPALGETINTEGERV, "alGetIntegerv");
	OpenAL_InitSym(__alWrapalGetFloatv, LPALGETFLOATV, "alGetFloatv");
	OpenAL_InitSym(__alWrapalGetDoublev, LPALGETDOUBLEV, "alGetDoublev");
	OpenAL_InitSym(__alWrapalGetBoolean, LPALGETBOOLEAN, "alGetBoolean");
	OpenAL_InitSym(__alWrapalGetInteger, LPALGETINTEGER, "alGetInteger");
	OpenAL_InitSym(__alWrapalGetFloat, LPALGETFLOAT, "alGetFloat");
	OpenAL_InitSym(__alWrapalGetDouble, LPALGETDOUBLE, "alGetDouble");
	OpenAL_InitSym(__alWrapalGetError, LPALGETERROR, "alGetError");
	OpenAL_InitSym(__alWrapalIsExtensionPresent, LPALISEXTENSIONPRESENT, "alIsExtensionPresent");
	OpenAL_InitSym(__alWrapalGetProcAddress, LPALGETPROCADDRESS, "alGetProcAddress");
	OpenAL_InitSym(__alWrapalGetEnumValue, LPALGETENUMVALUE, "alGetEnumValue");
	OpenAL_InitSym(__alWrapalListenerf, LPALLISTENERF, "alListenerf");
	OpenAL_InitSym(__alWrapalListener3f, LPALLISTENER3F, "alListener3f");
	OpenAL_InitSym(__alWrapalListenerfv, LPALLISTENERFV, "alListenerfv");
	OpenAL_InitSym(__alWrapalListeneri, LPALLISTENERI, "alListeneri");
	OpenAL_InitSym(__alWrapalListener3i, LPALLISTENER3I, "alListener3i");
	OpenAL_InitSym(__alWrapalListeneriv, LPALLISTENERIV, "alListeneriv");
	OpenAL_InitSym(__alWrapalGetListenerf, LPALGETLISTENERF, "alGetListenerf");
	OpenAL_InitSym(__alWrapalGetListener3f, LPALGETLISTENER3F, "alGetListener3f");
	OpenAL_InitSym(__alWrapalGetListenerfv, LPALGETLISTENERFV, "alGetListenerfv");
	OpenAL_InitSym(__alWrapalGetListeneri, LPALGETLISTENERI, "alGetListeneri");
	OpenAL_InitSym(__alWrapalGetListener3i, LPALGETLISTENER3I, "alGetListener3i");
	OpenAL_InitSym(__alWrapalGetListeneriv, LPALGETLISTENERIV, "alGetListeneriv");
	OpenAL_InitSym(__alWrapalGenSources, LPALGENSOURCES, "alGenSources");
	OpenAL_InitSym(__alWrapalDeleteSources, LPALDELETESOURCES, "alDeleteSources");
	OpenAL_InitSym(__alWrapalIsSource, LPALISSOURCE, "alIsSource");
	OpenAL_InitSym(__alWrapalSourcef, LPALSOURCEF, "alSourcef");
	OpenAL_InitSym(__alWrapalSource3f, LPALSOURCE3F, "alSource3f");
	OpenAL_InitSym(__alWrapalSourcefv, LPALSOURCEFV, "alSourcefv");
	OpenAL_InitSym(__alWrapalSourcei, LPALSOURCEI, "alSourcei");
	OpenAL_InitSym(__alWrapalSource3i, LPALSOURCE3I, "alSource3i");
	OpenAL_InitSym(__alWrapalSourceiv, LPALSOURCEIV, "alSourceiv");
	OpenAL_InitSym(__alWrapalGetSourcef, LPALGETSOURCEF, "alGetSourcef");
	OpenAL_InitSym(__alWrapalGetSource3f, LPALGETSOURCE3F, "alGetSource3f");
	OpenAL_InitSym(__alWrapalGetSourcefv, LPALGETSOURCEFV, "alGetSourcefv");
	OpenAL_InitSym(__alWrapalGetSourcei, LPALGETSOURCEI, "alGetSourcei");
	OpenAL_InitSym(__alWrapalGetSource3i, LPALGETSOURCE3I, "alGetSource3i");
	OpenAL_InitSym(__alWrapalGetSourceiv, LPALGETSOURCEIV, "alGetSourceiv");
	OpenAL_InitSym(__alWrapalSourcePlayv, LPALSOURCEPLAYV, "alSourcePlayv");
	OpenAL_InitSym(__alWrapalSourceStopv, LPALSOURCESTOPV, "alSourceStopv");
	OpenAL_InitSym(__alWrapalSourceRewindv, LPALSOURCEREWINDV, "alSourceRewindv");
	OpenAL_InitSym(__alWrapalSourcePausev, LPALSOURCEPAUSEV, "alSourcePausev");
	OpenAL_InitSym(__alWrapalSourcePlay, LPALSOURCEPLAY, "alSourcePlay");
	OpenAL_InitSym(__alWrapalSourceStop, LPALSOURCESTOP, "alSourceStop");
	OpenAL_InitSym(__alWrapalSourceRewind, LPALSOURCEREWIND, "alSourceRewind");
	OpenAL_InitSym(__alWrapalSourcePause, LPALSOURCEPAUSE, "alSourcePause");
	OpenAL_InitSym(__alWrapalSourceQueueBuffers, LPALSOURCEQUEUEBUFFERS, "alSourceQueueBuffers");
	OpenAL_InitSym(__alWrapalSourceUnqueueBuffers, LPALSOURCEUNQUEUEBUFFERS, "alSourceUnqueueBuffers");
	OpenAL_InitSym(__alWrapalGenBuffers, LPALGENBUFFERS, "alGenBuffers");
	OpenAL_InitSym(__alWrapalDeleteBuffers, LPALDELETEBUFFERS, "alDeleteBuffers");
	OpenAL_InitSym(__alWrapalIsBuffer, LPALISBUFFER, "alIsBuffer");
	OpenAL_InitSym(__alWrapalBufferData, LPALBUFFERDATA, "alBufferData");
	OpenAL_InitSym(__alWrapalBufferf, LPALBUFFERF, "alBufferf");
	OpenAL_InitSym(__alWrapalBuffer3f, LPALBUFFER3F, "alBuffer3f");
	OpenAL_InitSym(__alWrapalBufferfv, LPALBUFFERFV, "alBufferfv");
	OpenAL_InitSym(__alWrapalBufferi, LPALBUFFERI, "alBufferi");
	OpenAL_InitSym(__alWrapalBuffer3i, LPALBUFFER3I, "alBuffer3i");
	OpenAL_InitSym(__alWrapalBufferiv, LPALBUFFERIV, "alBufferiv");
	OpenAL_InitSym(__alWrapalGetBufferf, LPALGETBUFFERF, "alGetBufferf");
	OpenAL_InitSym(__alWrapalGetBuffer3f, LPALGETBUFFER3F, "alGetBuffer3f");
	OpenAL_InitSym(__alWrapalGetBufferfv, LPALGETBUFFERFV, "alGetBufferfv");
	OpenAL_InitSym(__alWrapalGetBufferi, LPALGETBUFFERI, "alGetBufferi");
	OpenAL_InitSym(__alWrapalGetBuffer3i, LPALGETBUFFER3I, "alGetBuffer3i");
	OpenAL_InitSym(__alWrapalGetBufferiv, LPALGETBUFFERIV, "alGetBufferiv");
	OpenAL_InitSym(__alWrapalDopplerFactor, LPALDOPPLERFACTOR, "alDopplerFactor");
	OpenAL_InitSym(__alWrapalDopplerVelocity, LPALDOPPLERVELOCITY, "alDopplerVelocity");
	OpenAL_InitSym(__alWrapalSpeedOfSound, LPALSPEEDOFSOUND, "alSpeedOfSound");
	OpenAL_InitSym(__alWrapalDistanceModel, LPALDISTANCEMODEL, "alDistanceModel");
	OpenAL_InitSym(__alWrapalcCreateContext, LPALCCREATECONTEXT, "alcCreateContext");
	OpenAL_InitSym(__alWrapalcMakeContextCurrent, LPALCMAKECONTEXTCURRENT, "alcMakeContextCurrent");
	OpenAL_InitSym(__alWrapalcProcessContext, LPALCPROCESSCONTEXT, "alcProcessContext");
	OpenAL_InitSym(__alWrapalcSuspendContext, LPALCSUSPENDCONTEXT, "alcSuspendContext");
	OpenAL_InitSym(__alWrapalcDestroyContext, LPALCDESTROYCONTEXT, "alcDestroyContext");
	OpenAL_InitSym(__alWrapalcGetCurrentContext, LPALCGETCURRENTCONTEXT, "alcGetCurrentContext");
	OpenAL_InitSym(__alWrapalcGetContextsDevice, LPALCGETCONTEXTSDEVICE, "alcGetContextsDevice");
	OpenAL_InitSym(__alWrapalcOpenDevice, LPALCOPENDEVICE, "alcOpenDevice");
	OpenAL_InitSym(__alWrapalcCloseDevice, LPALCCLOSEDEVICE, "alcCloseDevice");
	OpenAL_InitSym(__alWrapalcGetError, LPALCGETERROR, "alcGetError");
	OpenAL_InitSym(__alWrapalcIsExtensionPresent, LPALCISEXTENSIONPRESENT, "alcIsExtensionPresent");
	OpenAL_InitSym(__alWrapalcGetProcAddress, LPALCGETPROCADDRESS, "alcGetProcAddress");
	OpenAL_InitSym(__alWrapalcGetEnumValue, LPALCGETENUMVALUE, "alcGetEnumValue");
	OpenAL_InitSym(__alWrapalcGetString, LPALCGETSTRING, "alcGetString");
	OpenAL_InitSym(__alWrapalcGetIntegerv, LPALCGETINTEGERV, "alcGetIntegerv");
	OpenAL_InitSym(__alWrapalcCaptureOpenDevice, LPALCCAPTUREOPENDEVICE, "alcCaptureOpenDevice");
	OpenAL_InitSym(__alWrapalcCaptureCloseDevice, LPALCCAPTURECLOSEDEVICE, "alcCaptureCloseDevice");
	OpenAL_InitSym(__alWrapalcCaptureStart, LPALCCAPTURESTART, "alcCaptureStart");
	OpenAL_InitSym(__alWrapalcCaptureStop, LPALCCAPTURESTOP, "alcCaptureStop");
	OpenAL_InitSym(__alWrapalcCaptureSamples, LPALCCAPTURESAMPLES, "alcCaptureSamples");

#if DO_TRACE
    printf( "\tLoaded %i symbols.\n", OpenAL_TOTAL );
    printf( "\tAPI is missing %i symbols.\n", OpenAL_MISSING );
#endif
    return 1;
}
/******************************************************************************
    Unload OpenAL
 ******************************************************************************
 */
void OpenAL_UnloadLibrary( void )
{
    if (OpenAL != NULL) FreeLibrary(OpenAL);
}
/******************************************************************************
    Wrap and validate
 ******************************************************************************
 */
AL_API void ALC_APIENTRY alEnable( ALenum capability ){
    if (__alWrapalEnable != NULL) {
        __alWrapalEnable( capability );
    }
}
AL_API void ALC_APIENTRY alDisable( ALenum capability ){
    if (__alWrapalDisable != NULL) {
        __alWrapalDisable( capability );
    }
}
AL_API ALboolean ALC_APIENTRY alIsEnabled( ALenum capability ){
    if (__alWrapalIsEnabled != NULL) {
        return __alWrapalIsEnabled( capability );
    }
    return ((ALboolean)0);
}
AL_API const ALchar* ALC_APIENTRY alGetString( ALenum param ){
    if (__alWrapalGetString != NULL) {
        return __alWrapalGetString( param );
    }
    return ((const ALchar*)0);
}
AL_API void ALC_APIENTRY alGetBooleanv( ALenum param, ALboolean* data ){
    if (__alWrapalGetBooleanv != NULL) {
        __alWrapalGetBooleanv( param, data );
    }
}
AL_API void ALC_APIENTRY alGetIntegerv( ALenum param, ALint* data ){
    if (__alWrapalGetIntegerv != NULL) {
        __alWrapalGetIntegerv( param, data );
    }
}
AL_API void ALC_APIENTRY alGetFloatv( ALenum param, ALfloat* data ){
    if (__alWrapalGetFloatv != NULL) {
        __alWrapalGetFloatv( param, data );
    }
}
AL_API void ALC_APIENTRY alGetDoublev( ALenum param, ALdouble* data ){
    if (__alWrapalGetDoublev != NULL) {
        __alWrapalGetDoublev( param, data );
    }
}
AL_API ALboolean ALC_APIENTRY alGetBoolean( ALenum param ){
    if (__alWrapalGetBoolean != NULL) {
        return __alWrapalGetBoolean( param );
    }
    return ((ALboolean)0);
}
AL_API ALint ALC_APIENTRY alGetInteger( ALenum param ){
    if (__alWrapalGetInteger != NULL) {
        return __alWrapalGetInteger( param );
    }
    return ((ALint)0);
}
AL_API ALfloat ALC_APIENTRY alGetFloat( ALenum param ){
    if (__alWrapalGetFloat != NULL) {
        return __alWrapalGetFloat( param );
    }
    return ((ALfloat)0);
}
AL_API ALdouble ALC_APIENTRY alGetDouble( ALenum param ){
    if (__alWrapalGetDouble != NULL) {
        return __alWrapalGetDouble( param );
    }
    return ((ALdouble)0);
}
AL_API ALenum ALC_APIENTRY alGetError(){
    if (__alWrapalGetError != NULL) {
        return __alWrapalGetError();
    }
    return ((ALenum)0);
}
AL_API ALboolean ALC_APIENTRY alIsExtensionPresent( const ALchar* extname ){
    if (__alWrapalIsExtensionPresent != NULL) {
        return __alWrapalIsExtensionPresent( extname );
    }
    return ((ALboolean)0);
}
AL_API void* ALC_APIENTRY alGetProcAddress( const ALchar* fname ){
    if (__alWrapalGetProcAddress != NULL) {
        return __alWrapalGetProcAddress( fname );
    }
    return ((void*)0);
}
AL_API ALenum ALC_APIENTRY alGetEnumValue( const ALchar* ename ){
    if (__alWrapalGetEnumValue != NULL) {
        return __alWrapalGetEnumValue( ename );
    }
    return ((ALenum)0);
}
AL_API void ALC_APIENTRY alListenerf( ALenum param, ALfloat value ){
    if (__alWrapalListenerf != NULL) {
        __alWrapalListenerf( param, value );
    }
}
AL_API void ALC_APIENTRY alListener3f( ALenum param, ALfloat value1, ALfloat value2, ALfloat value3 ){
    if (__alWrapalListener3f != NULL) {
        __alWrapalListener3f( param, value1, value2, value3 );
    }
}
AL_API void ALC_APIENTRY alListenerfv( ALenum param, const ALfloat* values ){
    if (__alWrapalListenerfv != NULL) {
        __alWrapalListenerfv( param, values );
    }
}
AL_API void ALC_APIENTRY alListeneri( ALenum param, ALint value ){
    if (__alWrapalListeneri != NULL) {
        __alWrapalListeneri( param, value );
    }
}
AL_API void ALC_APIENTRY alListener3i( ALenum param, ALint value1, ALint value2, ALint value3 ){
    if (__alWrapalListener3i != NULL) {
        __alWrapalListener3i( param, value1, value2, value3 );
    }
}
AL_API void ALC_APIENTRY alListeneriv( ALenum param, const ALint* values ){
    if (__alWrapalListeneriv != NULL) {
        __alWrapalListeneriv( param, values );
    }
}
AL_API void ALC_APIENTRY alGetListenerf( ALenum param, ALfloat* value ){
    if (__alWrapalGetListenerf != NULL) {
        __alWrapalGetListenerf( param, value );
    }
}
AL_API void ALC_APIENTRY alGetListener3f( ALenum param, ALfloat* value1, ALfloat* value2, ALfloat* value3 ){
    if (__alWrapalGetListener3f != NULL) {
        __alWrapalGetListener3f( param, value1, value2, value3 );
    }
}
AL_API void ALC_APIENTRY alGetListenerfv( ALenum param, ALfloat* values ){
    if (__alWrapalGetListenerfv != NULL) {
        __alWrapalGetListenerfv( param, values );
    }
}
AL_API void ALC_APIENTRY alGetListeneri( ALenum param, ALint* value ){
    if (__alWrapalGetListeneri != NULL) {
        __alWrapalGetListeneri( param, value );
    }
}
AL_API void ALC_APIENTRY alGetListener3i( ALenum param, ALint* value1, ALint* value2, ALint* value3 ){
    if (__alWrapalGetListener3i != NULL) {
        __alWrapalGetListener3i( param, value1, value2, value3 );
    }
}
AL_API void ALC_APIENTRY alGetListeneriv( ALenum param, ALint* values ){
    if (__alWrapalGetListeneriv != NULL) {
        __alWrapalGetListeneriv( param, values );
    }
}
AL_API void ALC_APIENTRY alGenSources( ALsizei n, ALuint* sources ){
    if (__alWrapalGenSources != NULL) {
        __alWrapalGenSources( n, sources );
    }
}
AL_API void ALC_APIENTRY alDeleteSources( ALsizei n, const ALuint* sources ){
    if (__alWrapalDeleteSources != NULL) {
        __alWrapalDeleteSources( n, sources );
    }
}
AL_API ALboolean ALC_APIENTRY alIsSource( ALuint sid ){
    if (__alWrapalIsSource != NULL) {
        return __alWrapalIsSource( sid );
    }
    return ((ALboolean)0);
}
AL_API void ALC_APIENTRY alSourcef( ALuint sid, ALenum param, ALfloat value ){
    if (__alWrapalSourcef != NULL) {
        __alWrapalSourcef( sid, param, value );
    }
}
AL_API void ALC_APIENTRY alSource3f( ALuint sid, ALenum param, ALfloat value1, ALfloat value2, ALfloat value3 ){
    if (__alWrapalSource3f != NULL) {
        __alWrapalSource3f( sid, param, value1, value2, value3 );
    }
}
AL_API void ALC_APIENTRY alSourcefv( ALuint sid, ALenum param, const ALfloat* values ){
    if (__alWrapalSourcefv != NULL) {
        __alWrapalSourcefv( sid, param, values );
    }
}
AL_API void ALC_APIENTRY alSourcei( ALuint sid, ALenum param, ALint value ){
    if (__alWrapalSourcei != NULL) {
        __alWrapalSourcei( sid, param, value );
    }
}
AL_API void ALC_APIENTRY alSource3i( ALuint sid, ALenum param, ALint value1, ALint value2, ALint value3 ){
    if (__alWrapalSource3i != NULL) {
        __alWrapalSource3i( sid, param, value1, value2, value3 );
    }
}
AL_API void ALC_APIENTRY alSourceiv( ALuint sid, ALenum param, const ALint* values ){
    if (__alWrapalSourceiv != NULL) {
        __alWrapalSourceiv( sid, param, values );
    }
}
AL_API void ALC_APIENTRY alGetSourcef( ALuint sid, ALenum param, ALfloat* value ){
    if (__alWrapalGetSourcef != NULL) {
        __alWrapalGetSourcef( sid, param, value );
    }
}
AL_API void ALC_APIENTRY alGetSource3f( ALuint sid, ALenum param, ALfloat* value1, ALfloat* value2, ALfloat* value3 ){
    if (__alWrapalGetSource3f != NULL) {
        __alWrapalGetSource3f( sid, param, value1, value2, value3 );
    }
}
AL_API void ALC_APIENTRY alGetSourcefv( ALuint sid, ALenum param, ALfloat* values ){
    if (__alWrapalGetSourcefv != NULL) {
        __alWrapalGetSourcefv( sid, param, values );
    }
}
AL_API void ALC_APIENTRY alGetSourcei( ALuint sid, ALenum param, ALint* value ){
    if (__alWrapalGetSourcei != NULL) {
        __alWrapalGetSourcei( sid, param, value );
    }
}
AL_API void ALC_APIENTRY alGetSource3i( ALuint sid, ALenum param, ALint* value1, ALint* value2, ALint* value3 ){
    if (__alWrapalGetSource3i != NULL) {
        __alWrapalGetSource3i( sid, param, value1, value2, value3 );
    }
}
AL_API void ALC_APIENTRY alGetSourceiv( ALuint sid, ALenum param, ALint* values ){
    if (__alWrapalGetSourceiv != NULL) {
        __alWrapalGetSourceiv( sid, param, values );
    }
}
AL_API void ALC_APIENTRY alSourcePlayv( ALsizei ns, const ALuint* sids ){
    if (__alWrapalSourcePlayv != NULL) {
        __alWrapalSourcePlayv( ns, sids );
    }
}
AL_API void ALC_APIENTRY alSourceStopv( ALsizei ns, const ALuint* sids ){
    if (__alWrapalSourceStopv != NULL) {
        __alWrapalSourceStopv( ns, sids );
    }
}
AL_API void ALC_APIENTRY alSourceRewindv( ALsizei ns, const ALuint* sids ){
    if (__alWrapalSourceRewindv != NULL) {
        __alWrapalSourceRewindv( ns, sids );
    }
}
AL_API void ALC_APIENTRY alSourcePausev( ALsizei ns, const ALuint* sids ){
    if (__alWrapalSourcePausev != NULL) {
        __alWrapalSourcePausev( ns, sids );
    }
}
AL_API void ALC_APIENTRY alSourcePlay( ALuint sid ){
    if (__alWrapalSourcePlay != NULL) {
        __alWrapalSourcePlay( sid );
    }
}
AL_API void ALC_APIENTRY alSourceStop( ALuint sid ){
    if (__alWrapalSourceStop != NULL) {
        __alWrapalSourceStop( sid );
    }
}
AL_API void ALC_APIENTRY alSourceRewind( ALuint sid ){
    if (__alWrapalSourceRewind != NULL) {
        __alWrapalSourceRewind( sid );
    }
}
AL_API void ALC_APIENTRY alSourcePause( ALuint sid ){
    if (__alWrapalSourcePause != NULL) {
        __alWrapalSourcePause( sid );
    }
}
AL_API void ALC_APIENTRY alSourceQueueBuffers( ALuint sid, ALsizei numEntries, const ALuint* bids ){
    if (__alWrapalSourceQueueBuffers != NULL) {
        __alWrapalSourceQueueBuffers( sid, numEntries, bids );
    }
}
AL_API void ALC_APIENTRY alSourceUnqueueBuffers( ALuint sid, ALsizei numEntries, ALuint* bids ){
    if (__alWrapalSourceUnqueueBuffers != NULL) {
        __alWrapalSourceUnqueueBuffers( sid, numEntries, bids );
    }
}
AL_API void ALC_APIENTRY alGenBuffers( ALsizei n, ALuint* buffers ){
    if (__alWrapalGenBuffers != NULL) {
        __alWrapalGenBuffers( n, buffers );
    }
}
AL_API void ALC_APIENTRY alDeleteBuffers( ALsizei n, const ALuint* buffers ){
    if (__alWrapalDeleteBuffers != NULL) {
        __alWrapalDeleteBuffers( n, buffers );
    }
}
AL_API ALboolean ALC_APIENTRY alIsBuffer( ALuint bid ){
    if (__alWrapalIsBuffer != NULL) {
        return __alWrapalIsBuffer( bid );
    }
    return ((ALboolean)0);
}
AL_API void ALC_APIENTRY alBufferData( ALuint bid, ALenum format, const ALvoid* data, ALsizei size, ALsizei freq ){
    if (__alWrapalBufferData != NULL) {
        __alWrapalBufferData( bid, format, data, size, freq );
    }
}
AL_API void ALC_APIENTRY alBufferf( ALuint bid, ALenum param, ALfloat value ){
    if (__alWrapalBufferf != NULL) {
        __alWrapalBufferf( bid, param, value );
    }
}
AL_API void ALC_APIENTRY alBuffer3f( ALuint bid, ALenum param, ALfloat value1, ALfloat value2, ALfloat value3 ){
    if (__alWrapalBuffer3f != NULL) {
        __alWrapalBuffer3f( bid, param, value1, value2, value3 );
    }
}
AL_API void ALC_APIENTRY alBufferfv( ALuint bid, ALenum param, const ALfloat* values ){
    if (__alWrapalBufferfv != NULL) {
        __alWrapalBufferfv( bid, param, values );
    }
}
AL_API void ALC_APIENTRY alBufferi( ALuint bid, ALenum param, ALint value ){
    if (__alWrapalBufferi != NULL) {
        __alWrapalBufferi( bid, param, value );
    }
}
AL_API void ALC_APIENTRY alBuffer3i( ALuint bid, ALenum param, ALint value1, ALint value2, ALint value3 ){
    if (__alWrapalBuffer3i != NULL) {
        __alWrapalBuffer3i( bid, param, value1, value2, value3 );
    }
}
AL_API void ALC_APIENTRY alBufferiv( ALuint bid, ALenum param, const ALint* values ){
    if (__alWrapalBufferiv != NULL) {
        __alWrapalBufferiv( bid, param, values );
    }
}
AL_API void ALC_APIENTRY alGetBufferf( ALuint bid, ALenum param, ALfloat* value ){
    if (__alWrapalGetBufferf != NULL) {
        __alWrapalGetBufferf( bid, param, value );
    }
}
AL_API void ALC_APIENTRY alGetBuffer3f( ALuint bid, ALenum param, ALfloat* value1, ALfloat* value2, ALfloat* value3 ){
    if (__alWrapalGetBuffer3f != NULL) {
        __alWrapalGetBuffer3f( bid, param, value1, value2, value3 );
    }
}
AL_API void ALC_APIENTRY alGetBufferfv( ALuint bid, ALenum param, ALfloat* values ){
    if (__alWrapalGetBufferfv != NULL) {
        __alWrapalGetBufferfv( bid, param, values );
    }
}
AL_API void ALC_APIENTRY alGetBufferi( ALuint bid, ALenum param, ALint* value ){
    if (__alWrapalGetBufferi != NULL) {
        __alWrapalGetBufferi( bid, param, value );
    }
}
AL_API void ALC_APIENTRY alGetBuffer3i( ALuint bid, ALenum param, ALint* value1, ALint* value2, ALint* value3 ){
    if (__alWrapalGetBuffer3i != NULL) {
        __alWrapalGetBuffer3i( bid, param, value1, value2, value3 );
    }
}
AL_API void ALC_APIENTRY alGetBufferiv( ALuint bid, ALenum param, ALint* values ){
    if (__alWrapalGetBufferiv != NULL) {
        __alWrapalGetBufferiv( bid, param, values );
    }
}
AL_API void ALC_APIENTRY alDopplerFactor( ALfloat value ){
    if (__alWrapalDopplerFactor != NULL) {
        __alWrapalDopplerFactor( value );
    }
}
AL_API void ALC_APIENTRY alDopplerVelocity( ALfloat value ){
    if (__alWrapalDopplerVelocity != NULL) {
        __alWrapalDopplerVelocity( value );
    }
}
AL_API void ALC_APIENTRY alSpeedOfSound( ALfloat value ){
    if (__alWrapalSpeedOfSound != NULL) {
        __alWrapalSpeedOfSound( value );
    }
}
AL_API void ALC_APIENTRY alDistanceModel( ALenum distanceModel ){
    if (__alWrapalDistanceModel != NULL) {
        __alWrapalDistanceModel( distanceModel );
    }
}
AL_API ALCcontext * ALC_APIENTRY alcCreateContext( ALCdevice* device, const ALCint* attrlist ){
    if (__alWrapalcCreateContext != NULL) {
        return __alWrapalcCreateContext( device, attrlist );
    }
    return ((ALCcontext *)0);
}
AL_API ALCboolean ALC_APIENTRY alcMakeContextCurrent( ALCcontext* context ){
    if (__alWrapalcMakeContextCurrent != NULL) {
        return __alWrapalcMakeContextCurrent( context );
    }
    return ((ALCboolean)0);
}
AL_API void ALC_APIENTRY alcProcessContext( ALCcontext* context ){
    if (__alWrapalcProcessContext != NULL) {
        __alWrapalcProcessContext( context );
    }
}
AL_API void ALC_APIENTRY alcSuspendContext( ALCcontext* context ){
    if (__alWrapalcSuspendContext != NULL) {
        __alWrapalcSuspendContext( context );
    }
}
AL_API void ALC_APIENTRY alcDestroyContext( ALCcontext* context ){
    if (__alWrapalcDestroyContext != NULL) {
        __alWrapalcDestroyContext( context );
    }
}
AL_API ALCcontext * ALC_APIENTRY alcGetCurrentContext(){
    if (__alWrapalcGetCurrentContext != NULL) {
        return __alWrapalcGetCurrentContext();
    }
    return ((ALCcontext *)0);
}
AL_API ALCdevice * ALC_APIENTRY alcGetContextsDevice( ALCcontext* context ){
    if (__alWrapalcGetContextsDevice != NULL) {
        return __alWrapalcGetContextsDevice( context );
    }
    return ((ALCdevice *)0);
}
AL_API ALCdevice * ALC_APIENTRY alcOpenDevice( const ALCchar* devicename ){
    if (__alWrapalcOpenDevice != NULL) {
        return __alWrapalcOpenDevice( devicename );
    }
    return ((ALCdevice *)0);
}
AL_API ALCboolean ALC_APIENTRY alcCloseDevice( ALCdevice* device ){
    if (__alWrapalcCloseDevice != NULL) {
        return __alWrapalcCloseDevice( device );
    }
    return ((ALCboolean)0);
}
AL_API ALCenum ALC_APIENTRY alcGetError( ALCdevice* device ){
    if (__alWrapalcGetError != NULL) {
        return __alWrapalcGetError( device );
    }
    return ((ALCenum)0);
}
AL_API ALCboolean ALC_APIENTRY alcIsExtensionPresent( ALCdevice* device, const ALCchar* extname ){
    if (__alWrapalcIsExtensionPresent != NULL) {
        return __alWrapalcIsExtensionPresent( device, extname );
    }
    return ((ALCboolean)0);
}
AL_API void * ALC_APIENTRY alcGetProcAddress( ALCdevice* device, const ALCchar* funcname ){
    if (__alWrapalcGetProcAddress != NULL) {
        return __alWrapalcGetProcAddress( device, funcname );
    }
    return ((void *)0);
}
AL_API ALCenum ALC_APIENTRY alcGetEnumValue( ALCdevice* device, const ALCchar* enumname ){
    if (__alWrapalcGetEnumValue != NULL) {
        return __alWrapalcGetEnumValue( device, enumname );
    }
    return ((ALCenum)0);
}
AL_API const ALCchar* ALC_APIENTRY alcGetString( ALCdevice* device, ALCenum param ){
    if (__alWrapalcGetString != NULL) {
        return __alWrapalcGetString( device, param );
    }
    return ((const ALCchar*)0);
}
AL_API void ALC_APIENTRY alcGetIntegerv( ALCdevice* device, ALCenum param, ALCsizei size, ALCint* dest ){
    if (__alWrapalcGetIntegerv != NULL) {
        __alWrapalcGetIntegerv( device, param, size, dest );
    }
}
AL_API ALCdevice * ALC_APIENTRY alcCaptureOpenDevice( const ALCchar* devicename, ALCuint frequency, ALCenum format, ALCsizei buffersize ){
    if (__alWrapalcCaptureOpenDevice != NULL) {
        return __alWrapalcCaptureOpenDevice( devicename, frequency, format, buffersize );
    }
    return ((ALCdevice *)0);
}
AL_API ALCboolean ALC_APIENTRY alcCaptureCloseDevice( ALCdevice* device ){
    if (__alWrapalcCaptureCloseDevice != NULL) {
        return __alWrapalcCaptureCloseDevice( device );
    }
    return ((ALCboolean)0);
}
AL_API void ALC_APIENTRY alcCaptureStart( ALCdevice* device ){
    if (__alWrapalcCaptureStart != NULL) {
        __alWrapalcCaptureStart( device );
    }
}
AL_API void ALC_APIENTRY alcCaptureStop( ALCdevice* device ){
    if (__alWrapalcCaptureStop != NULL) {
        __alWrapalcCaptureStop( device );
    }
}
AL_API void ALC_APIENTRY alcCaptureSamples( ALCdevice* device, ALCvoid* buffer, ALCsizei samples ){
    if (__alWrapalcCaptureSamples != NULL) {
        __alWrapalcCaptureSamples( device, buffer, samples );
    }
}

