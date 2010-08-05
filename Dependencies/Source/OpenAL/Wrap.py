#------------------------------------------------------------------------------
#   This file is part of OgreKit.
#   http://gamekit.googlecode.com/
#
#   Copyright (c) 2006-2010 Charlie C.
#
#   Contributor(s): none yet.
#------------------------------------------------------------------------------
# This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgment in the product documentation would be
#    appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.
#------------------------------------------------------------------------------

CODE = [
    ['void',            'alEnable',                 [['ALenum', 'capability']]],
    ['void',            'alDisable',                [['ALenum', 'capability']]],
    ['ALboolean',       'alIsEnabled',              [['ALenum', 'capability']]],
    ['const ALchar*',   'alGetString',              [['ALenum', 'param']]],
    ['void',            'alGetBooleanv',            [['ALenum', 'param'], ['ALboolean*', 'data']]],
    ['void',            'alGetIntegerv',            [['ALenum', 'param'], ['ALint*', 'data']]],
    ['void',            'alGetFloatv',              [['ALenum', 'param'], ['ALfloat*', 'data']]],
    ['void',            'alGetDoublev',             [['ALenum', 'param'], ['ALdouble*', 'data']]],
    ['ALboolean',       'alGetBoolean',             [['ALenum', 'param']]],
    ['ALint',           'alGetInteger',             [['ALenum', 'param']]],
    ['ALfloat',         'alGetFloat',               [['ALenum', 'param']]],
    ['ALdouble',        'alGetDouble',              [['ALenum', 'param']]],
    ['ALenum',          'alGetError',               []],
    ['ALboolean',       'alIsExtensionPresent',     [['const ALchar*', 'extname']]],
    ['void*',           'alGetProcAddress',         [['const ALchar*', 'fname']]],
    ['ALenum',          'alGetEnumValue',           [['const ALchar*', 'ename']]],
    ['void',            'alListenerf',              [['ALenum', 'param'], ['ALfloat', 'value']]],
    ['void',            'alListener3f',             [['ALenum', 'param'], ['ALfloat', 'value1'], ['ALfloat', 'value2'], ['ALfloat', 'value3']]],
    ['void',            'alListenerfv',             [['ALenum', 'param'], ['const ALfloat*', 'values']]],
    ['void',            'alListeneri',              [['ALenum', 'param'], ['ALint', 'value']]],
    ['void',            'alListener3i',             [['ALenum', 'param'], ['ALint', 'value1'], ['ALint', 'value2'], ['ALint', 'value3']]],
    ['void',            'alListeneriv',             [['ALenum', 'param'], ['const ALint*', 'values']]],
    ['void',            'alGetListenerf',           [['ALenum', 'param'], ['ALfloat*', 'value']]],
    ['void',            'alGetListener3f',          [['ALenum', 'param'], ['ALfloat*', 'value1'], ['ALfloat*', 'value2'], ['ALfloat*', 'value3']]],
    ['void',            'alGetListenerfv',          [['ALenum', 'param'], ['ALfloat*', 'values']]],
    ['void',            'alGetListeneri',           [['ALenum', 'param'], ['ALint*', 'value']]],
    ['void',            'alGetListener3i',          [['ALenum', 'param'], ['ALint*', 'value1'], ['ALint*', 'value2'], ['ALint*', 'value3']]],
    ['void',            'alGetListeneriv',          [['ALenum', 'param'], ['ALint*', 'values']]],
    ['void',            'alGenSources',             [['ALsizei', 'n'], ['ALuint*', 'sources']]],
    ['void',            'alDeleteSources',          [['ALsizei', 'n'], ['const ALuint*', 'sources']]],
    ['ALboolean',       'alIsSource',               [['ALuint', 'sid']]],
    ['void',            'alSourcef',                [['ALuint', 'sid'], ['ALenum', 'param'], ['ALfloat', 'value']]],
    ['void',            'alSource3f',               [['ALuint', 'sid'], ['ALenum', 'param'], ['ALfloat', 'value1'], ['ALfloat', 'value2'], ['ALfloat', 'value3']]],
    ['void',            'alSourcefv',               [['ALuint', 'sid'], ['ALenum', 'param'], ['const ALfloat*', 'values']]],
    ['void',            'alSourcei',                [['ALuint', 'sid'], ['ALenum', 'param'], ['ALint', 'value']]],
    ['void',            'alSource3i',               [['ALuint', 'sid'], ['ALenum', 'param'], ['ALint', 'value1'], ['ALint', 'value2'], ['ALint', 'value3']]],
    ['void',            'alSourceiv',               [['ALuint', 'sid'], ['ALenum', 'param'], ['const ALint*', 'values']]],
    ['void',            'alGetSourcef',             [['ALuint', 'sid'], ['ALenum', 'param'], ['ALfloat*', 'value']]],
    ['void',            'alGetSource3f',            [['ALuint', 'sid'], ['ALenum', 'param'], ['ALfloat*', 'value1'], ['ALfloat*', 'value2'], ['ALfloat*', 'value3']]],
    ['void',            'alGetSourcefv',            [['ALuint', 'sid'], ['ALenum', 'param'], ['ALfloat*', 'values']]],
    ['void',            'alGetSourcei',             [['ALuint', 'sid'], ['ALenum', 'param'], ['ALint*', 'value']]],
    ['void',            'alGetSource3i',            [['ALuint', 'sid'], ['ALenum', 'param'], ['ALint*', 'value1'], ['ALint*', 'value2'], ['ALint*', 'value3']]],
    ['void',            'alGetSourceiv',            [['ALuint', 'sid'], ['ALenum', 'param'], ['ALint*', 'values']]],
    ['void',            'alSourcePlayv',            [['ALsizei', 'ns'], ['const ALuint*', 'sids']]],
    ['void',            'alSourceStopv',            [['ALsizei', 'ns'], ['const ALuint*', 'sids']]],
    ['void',            'alSourceRewindv',          [['ALsizei', 'ns'], ['const ALuint*', 'sids']]],
    ['void',            'alSourcePausev',           [['ALsizei', 'ns'], ['const ALuint*', 'sids']]],
    ['void',            'alSourcePlay',             [['ALuint', 'sid']]],
    ['void',            'alSourceStop',             [['ALuint', 'sid']]],
    ['void',            'alSourceRewind',           [['ALuint', 'sid']]],
    ['void',            'alSourcePause',            [['ALuint', 'sid']]],
    ['void',            'alSourceQueueBuffers',     [['ALuint', 'sid'], ['ALsizei', 'numEntries'], ['const ALuint*', 'bids']]],
    ['void',            'alSourceUnqueueBuffers',   [['ALuint', 'sid'], ['ALsizei', 'numEntries'], ['ALuint*', 'bids']]],
    ['void',            'alGenBuffers',             [['ALsizei', 'n'], ['ALuint*', 'buffers']]],
    ['void',            'alDeleteBuffers',          [['ALsizei', 'n'], ['const ALuint*', 'buffers']]],
    ['ALboolean',       'alIsBuffer',               [['ALuint', 'bid']]],
    ['void',            'alBufferData',             [['ALuint', 'bid'], ['ALenum', 'format'], ['const ALvoid*', 'data'], ['ALsizei', 'size'], ['ALsizei', 'freq']]],
    ['void',            'alBufferf',                [['ALuint', 'bid'], ['ALenum', 'param'], ['ALfloat', 'value']]],
    ['void',            'alBuffer3f',               [['ALuint', 'bid'], ['ALenum', 'param'], ['ALfloat', 'value1'], ['ALfloat', 'value2'], ['ALfloat', 'value3']]],
    ['void',            'alBufferfv',               [['ALuint', 'bid'], ['ALenum', 'param'], ['const ALfloat*', 'values']]],
    ['void',            'alBufferi',                [['ALuint', 'bid'], ['ALenum', 'param'], ['ALint', 'value']]],
    ['void',            'alBuffer3i',               [['ALuint', 'bid'], ['ALenum', 'param'], ['ALint', 'value1'], ['ALint', 'value2'], ['ALint', 'value3']]],
    ['void',            'alBufferiv',               [['ALuint', 'bid'], ['ALenum', 'param'], ['const ALint*', 'values']]],
    ['void',            'alGetBufferf',             [['ALuint', 'bid'], ['ALenum', 'param'], ['ALfloat*', 'value']]],
    ['void',            'alGetBuffer3f',            [['ALuint', 'bid'], ['ALenum', 'param'], ['ALfloat*', 'value1'], ['ALfloat*', 'value2'], ['ALfloat*', 'value3']]],
    ['void',            'alGetBufferfv',            [['ALuint', 'bid'], ['ALenum', 'param'], ['ALfloat*', 'values']]],
    ['void',            'alGetBufferi',             [['ALuint', 'bid'], ['ALenum', 'param'], ['ALint*', 'value']]],
    ['void',            'alGetBuffer3i',            [['ALuint', 'bid'], ['ALenum', 'param'], ['ALint*', 'value1'], ['ALint*', 'value2'], ['ALint*', 'value3']]],
    ['void',            'alGetBufferiv',            [['ALuint', 'bid'], ['ALenum', 'param'], ['ALint*', 'values']]],
    ['void',            'alDopplerFactor',          [['ALfloat', 'value']]],
    ['void',            'alDopplerVelocity',        [['ALfloat', 'value']]],
    ['void',            'alSpeedOfSound',           [['ALfloat', 'value']]],
    ['void',            'alDistanceModel',          [['ALenum', 'distanceModel']]],
    ['ALCcontext *',    'alcCreateContext',         [['ALCdevice*', 'device'], ['const ALCint*', 'attrlist']]],
    ['ALCboolean',      'alcMakeContextCurrent',    [['ALCcontext*', 'context']]],
    ['void',            'alcProcessContext',        [['ALCcontext*', 'context']]],
    ['void',            'alcSuspendContext',        [['ALCcontext*', 'context']]],
    ['void',            'alcDestroyContext',        [['ALCcontext*', 'context']]],
    ['ALCcontext *',    'alcGetCurrentContext',     []],
    ['ALCdevice *',     'alcGetContextsDevice',     [['ALCcontext*', 'context']]],
    ['ALCdevice *',     'alcOpenDevice',            [['const ALCchar*', 'devicename']]],
    ['ALCboolean',      'alcCloseDevice',           [['ALCdevice*', 'device']]],
    ['ALCenum',         'alcGetError',              [['ALCdevice*', 'device']]],
    ['ALCboolean',      'alcIsExtensionPresent',    [['ALCdevice*', 'device'], ['const ALCchar*', 'extname']]],
    ['void *',          'alcGetProcAddress',        [['ALCdevice*', 'device'], ['const ALCchar*', 'funcname']]],
    ['ALCenum',         'alcGetEnumValue',          [['ALCdevice*', 'device'], ['const ALCchar*', 'enumname']]],
    ['const ALCchar*',  'alcGetString',             [['ALCdevice*', 'device'], ['ALCenum', 'param']]],
    ['void',            'alcGetIntegerv',           [['ALCdevice*', 'device'], ['ALCenum', 'param'], ['ALCsizei', 'size'], ['ALCint*', 'dest']]],
    ['ALCdevice *',     'alcCaptureOpenDevice',     [['const ALCchar*', 'devicename'], ['ALCuint', 'frequency'], ['ALCenum', 'format'], ['ALCsizei', 'buffersize']]],
    ['ALCboolean',      'alcCaptureCloseDevice',    [['ALCdevice*', 'device']]],
    ['void',            'alcCaptureStart',          [['ALCdevice*', 'device']]],
    ['void',            'alcCaptureStop',           [['ALCdevice*', 'device']]],
    ['void',            'alcCaptureSamples',        [['ALCdevice*', 'device'], ['ALCvoid*', 'buffer'], ['ALCsizei', 'samples']]],
]


def saveBuffer(file, buf):
    f = open(file, "wb")
    f.write(buf)
    f.close()


TEMPL = """/*
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


#define OpenAL_InitSym(FP, T, DEST) \\
    FP= (T)GetProcAddress(OpenAL, DEST);\\
	if (FP)\\
	{printf("Loaded ==> %s\\n", DEST); OpenAL_TOTAL += 1;}\\
	else\\
	{printf("Missing ==> %s\\n", DEST); OpenAL_MISSING += 1;}

#else
# define OpenAL_InitSym(FP, T, DEST) FP= (T)GetProcAddress(OpenAL, DEST);
#endif
/******************************************************************************
    Bring symbols into existence
 ******************************************************************************
 */
$TYPEDEFS
/******************************************************************************
    Load OpenAL
 ******************************************************************************
 */
int OpenAL_LoadLibrary( void )
{
#if DO_TRACE
    OpenAL_TOTAL= 0;
    OpenAL_MISSING= 0;
	printf("Loading OpenAL32.dll\\n");
#endif

    OpenAL = LoadLibrary("OpenAL32.dll");
    if ( OpenAL == NULL ) return 0;
$INITSYM
#if DO_TRACE
    printf( "\\tLoaded %i symbols.\\n", OpenAL_TOTAL );
    printf( "\\tAPI is missing %i symbols.\\n", OpenAL_MISSING );
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
$WRAPPERS
"""


TYPEDEFS = ""
INITSYM  = ""
WRAPPERS = ""
TEST_TEMPLVOID = """    if (__alWrap$SYM != NULL) {
        __alWrap$SYM($PARAM_CALL);
    }
"""
TEST_TEMPLRET = """    if (__alWrap$SYM != NULL) {
        return __alWrap$SYM($PARAM_CALL);
    }
    return (($TYPE)0);
"""

for i in CODE:
    TYPEDEFS += "LP" + i[1].upper() + " __alWrap"+i[1]+" = NULL;\n"
    INITSYM += "\tOpenAL_InitSym(__alWrap" + i[1] + ", LP" + i[1].upper() + ", \"" + i[1] + "\");\n"

    PARAM_LIST = i[2]
    PARAM_STR = ""
    PARAM_CALL= "";

    PARAM_SIZE = len(PARAM_LIST)
    I = 0;
    for PARAM in PARAM_LIST:
        J = 0;
        for ITEM in PARAM:
            PARAM_STR += " " + ITEM
            if J == 1:
                PARAM_CALL += ' ' + ITEM;
            J+= 1
        if I < PARAM_SIZE-1:
            PARAM_STR += ","
            PARAM_CALL += ","
        else:
            PARAM_STR += " "
            PARAM_CALL += " "
        I+= 1
    WRAPPERS += "AL_API " + i[0] + " ALC_APIENTRY " + i[1] + "("+PARAM_STR+"){\n";
    WRAPTEMPL = "";

    if (i[0] == "void"):
        WRAPTEMPL = TEST_TEMPLVOID;
        WRAPTEMPL = WRAPTEMPL.replace("$SYM", i[1])
        WRAPTEMPL = WRAPTEMPL.replace("$PARAM_CALL", PARAM_CALL)
    else:
        WRAPTEMPL = TEST_TEMPLRET;
        WRAPTEMPL = WRAPTEMPL.replace("$TYPE",  i[0])
        WRAPTEMPL = WRAPTEMPL.replace("$SYM",   i[1])
        WRAPTEMPL = WRAPTEMPL.replace("$PARAM_CALL", PARAM_CALL)
    WRAPPERS += WRAPTEMPL;
    WRAPPERS+= "}\n"

tmpl = TEMPL
tmpl = tmpl.replace("$TYPEDEFS",    TYPEDEFS)
tmpl = tmpl.replace("$INITSYM",     INITSYM)
tmpl = tmpl.replace("$WRAPPERS",    WRAPPERS)
saveBuffer("OpenALWrap.c", tmpl)
