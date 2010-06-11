/*
-------------------------------------------------------------------------------
    General Purpose Utility Library, should be kept dependency free.
    Unless the dependency can be compiled along with this library.

    Copyright (c) 2009-2010 Charlie C.
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utStream.h"

#if 0

#ifdef ZLIB_SUPPORT
#include "ZLib/zlib.h"
#define FileEOF(f)          gzeof(f)
#define FileOpen(n, m)      gzopen(n, m)
#define FileClose(f)        gzclose(f)
#define FileRead(f, d, s)   gzread(f, d, s)
#else
#define FileEOF(f)          feof(reinterpret_cast<FILE*>(f))
#define FileOpen(n, m)      (void*)fopen(n, m)
#define FileClose(f)        fclose(reinterpret_cast<FILE*>(f))
#define FileRead(f, d, s)   fread(d, 1, s, reinterpret_cast<FILE*>(f))
#endif




utStream::utStream()
:   m_mode(0)
{
}



// File support



utFileStream::utFileStream()
:   utStream(), m_file(0)
{
}


utFileStream::~utFileStream()
{
    if (m_file)
        FileClose(m_file);
}

bool utFileStream::eof(void)
{
    if (m_file)
        return FileEOF(m_file) != 0;
    return true;
}


bool utFileStream::open(const char *name, const char mode[2])
{
    char m[3] = {'\0', '\0', '\0'};
    switch (mode[0])
    {
    case 'r':
        m_mode |= AM_R;
        break;
    case 'w':
        m_mode |= AM_W;
        break;
    default:
        return false;
    }

    m[0] = mode[0];

    if (mode[1] == 'b')
    {
        m_mode |= AM_B;
        m[1] = 'b';
    }

    void* fp = FileOpen(name, m);
    if (!fp)
        return false;

    m_file = fp;
    return true;
}


UTint32 utFileStream::read(void *dest, UTint32 nr)
{
    if (!m_file || !(m_mode & AM_R))
        return -1;

    return (UTint32)FileRead(m_file, dest, nr);
}

UTint32 utFileStream::write(const void *dest, UTint32 nr)
{
    return -1;
}
#endif