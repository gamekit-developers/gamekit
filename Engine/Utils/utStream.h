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
#ifndef _utStream_h_
#define _utStream_h_
#if 0

#include "utCommon.h"


// base class for all streams
class utStream
{
public:
    enum AccessMode
    {
        AM_R   = (1 << 0),
        AM_W   = (1 << 1),
        AM_B   = (1 << 2),
    };

protected:
    UTint32 m_mode;

public:

    utStream();
    virtual ~utStream() {}

    virtual bool eof(void) = 0;
    virtual bool open(const char *name, const char mode[2]) = 0;
    virtual UTint32 read(void *dest, UTint32 nr) = 0;
    virtual UTint32 write(const void *dest, UTint32 nr) = 0;
};


// File access stream
class utFileStream : public utStream
{
protected:
    void *m_file;

public:
    utFileStream();
    virtual ~utFileStream();

    virtual bool eof(void);
    virtual bool open(const char *name, const char mode[2]);
    virtual UTint32 read(void *dest, UTint32 nr);
    virtual UTint32 write(const void *dest, UTint32 nr);
};

#endif

#endif//_utStream_h_
