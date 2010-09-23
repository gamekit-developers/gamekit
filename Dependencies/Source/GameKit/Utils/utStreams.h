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
#ifndef _utStreams_h_
#define _utStreams_h_


#include "utString.h"


class utStream
{
public:


	enum StreamMode
	{
		SM_READ =1,
		SM_WRITE=2,
	};



public:

	utStream() {}
	virtual ~utStream() {}

	virtual void clear(void) {};
	virtual void flush(void) {}


	virtual bool isOpen(void) const = 0;
	virtual bool eof(void) const = 0;

	virtual UTsize  read(void *dest, UTsize nr) const = 0;
	virtual UTsize  write(const void *src, UTsize nr) = 0;
	UTsize          write(const utStream &cpy);

	virtual UTsize  position(void) const =0;
	virtual void    seek(const UTsize pos, int dir) const = 0;
	virtual UTsize  size(void) const = 0;

#ifdef UT_USE_ZLIB
	void inflate(utStream &dest);
#endif

protected:

	virtual void reserve(UTsize nr) {}
#ifdef UT_USE_ZLIB
	int tryInflate(utStream &dest);
#endif
};



class           utMemoryStream;
typedef void   *utFileHandle;


class utFileStream : public utStream
{
public:
	utFileStream();
	~utFileStream();

	void flush(void);
	void open(const char *path, utStream::StreamMode mode);
	void close(void);

	bool isOpen(void)   const {return m_handle != 0;}
	bool eof(void)      const {return !m_handle || m_pos >= m_size;}

	UTsize  read(void *dest, UTsize nr) const;
	UTsize  write(const void *src, UTsize nr);

	UTsize  position(void)          const {return m_pos;}
	void    seek(const UTsize pos, int dir)  const;

	UTsize size(void) const {return m_size;}

protected:


	utString        m_file;
	utFileHandle    m_handle;
	mutable UTsize  m_pos;
	UTsize          m_size;
	int             m_mode;
};




class utMemoryStream : public utStream
{
public:
	utMemoryStream(int mode = 0);
	~utMemoryStream();

	void clear(void);

	void open(const char *path, utStream::StreamMode mode);
	void open(const utFileStream &fs, utStream::StreamMode mode);
	void open(const void *buffer, UTsize size, utStream::StreamMode mode);


	bool    isOpen(void)    const   {return m_buffer != 0;}
	bool    eof(void)       const   {return !m_buffer || m_pos >= m_size;}
	UTsize  position(void)  const   {return m_pos;}
	UTsize  size(void)      const   {return m_size;}

	UTsize read(void *dest, UTsize nr) const;
	UTsize write(const void *src, UTsize nr);


	void    seek(const UTsize pos, int dir) const;

	void            *ptr(void)          {return m_buffer;}
	const void      *ptr(void) const    {return m_buffer;}

protected:

	void reserve(UTsize nr);

	char            *m_buffer;
	mutable UTsize  m_pos;
	UTsize          m_size, m_capacity;
	int             m_mode;
};


#endif//_utStreams_h_
