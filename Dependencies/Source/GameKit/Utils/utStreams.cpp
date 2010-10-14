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
#include "utStreams.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

#ifdef UT_USE_ZLIB
#include "zlib.h"
#endif





//#define UT_WIN32_FILE 1
#if UT_PLATFORM == UT_PLATFORM_WIN32 && defined(UT_WIN32_FILE)
#include <windows.h>
#endif



class utFileWrapper
{
public:
	static utFileHandle open(const char *filename, int mode);
	static void close(utFileHandle &fh);
	static UTsize read(utFileHandle fh, void *dest, UTsize nr);
	static UTsize write(utFileHandle fh, const void *src, UTsize nr);
	static UTsize size(utFileHandle fh);
	static int seek(utFileHandle fh, UTsize pos, int way);
	static void flush(utFileHandle fh);
};



#ifdef UT_USE_ZLIB

void utStream::inflate(utStream &dest)
{
	if (!isOpen() || eof())
		return;

	dest.reserve(size());

	if (tryInflate(dest) != Z_OK)
	{
		if (dest.size() > 0)
			dest.seek(0, SEEK_SET);
		dest.write(*this);
	}

}

int utStream::tryInflate(utStream &dest)
{
	int ret;
	z_stream strm;

	const int BlkSize = 32767;

	Bytef in[BlkSize];
	Bytef out[BlkSize];

	memset(&strm, Z_NULL, sizeof(z_stream));

	ret = ::inflateInit2(&strm, 31);
	if (ret != Z_OK) return ret;

	do
	{
		strm.avail_in   = read(in, BlkSize);

		if (strm.avail_in == 0)
			break;

		strm.next_in    = in;

		do
		{
			strm.avail_out  = BlkSize;
			strm.next_out   = out;


			ret = ::inflate(&strm, Z_NO_FLUSH);
			UT_ASSERT(ret != Z_STREAM_ERROR);

			switch (ret)
			{
			case Z_NEED_DICT:
				ret = Z_DATA_ERROR;
			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
				inflateEnd(&strm);
				return ret;
			}

			if (dest.write(out, BlkSize - strm.avail_out) < 0)
			{
				::inflateEnd(&strm);
				return Z_ERRNO;
			}
		}

		while (strm.avail_out == 0);
	}
	while (ret != Z_STREAM_END);

	::inflateEnd(&strm);
	return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}




#endif


UTsize utStream::write(const utStream &cpy)
{
	// copy from stream
	if (size() != cpy.size() && cpy.isOpen() && cpy.size() > 0)
	{
		UTsize size= cpy.size();
		UTsize old_pos= cpy.position();

		cpy.seek(0, SEEK_SET);

		char *tbuf = new char[size+1];
		cpy.read(tbuf, size);
		size = write(tbuf, size);
		delete []tbuf;

		// restore position
		cpy.seek(old_pos, SEEK_SET);
		return size;
	}
	return 0;
}




utFileStream::utFileStream() :
	m_file(), m_handle(0), m_pos(0), m_size(0), m_mode(0)
{
}


void utFileStream::open(const char *p, utStream::StreamMode mode)
{
	if (m_handle != 0 && m_file != p)
		utFileWrapper::close(m_handle);


	m_file = p;
	m_handle= utFileWrapper::open(m_file.c_str(), mode);
	if (m_handle)
	{
		if (!(mode & SM_WRITE))
			m_size= utFileWrapper::size(m_handle);
	}
}


void utFileStream::close(void)
{
	if (m_handle != 0)
	{
		utFileWrapper::close(m_handle);
		m_handle= 0;
	}

	m_file.clear();
}



utFileStream::~utFileStream()
{
	close();
}



void utFileStream::flush(void)
{
	if (m_handle != 0)
		utFileWrapper::flush(m_handle);
}


UTsize utFileStream::read(void *dest, UTsize nr) const
{
	// write only
	if (m_mode == utStream::SM_WRITE)
		return -1;

	// error
	if (!dest || !m_handle)
		return -1;

	// eof
	if (m_pos > m_size)
		return 0;


	// clamp
	if ((m_size - m_pos) < nr)
		nr = m_size - m_pos;


	UTsize br= utFileWrapper::read(m_handle, dest, nr);

	// advance position
	m_pos += br;
	return br;
}



UTsize utFileStream::write(const void *src, UTsize nr)
{
	// read only
	if (m_mode == utStream::SM_READ)
		return -1;

	// error
	if (!src || !m_handle)
		return -1;

	UTsize bw = utFileWrapper::write(m_handle, src, nr);

	// advance position
	m_pos   += bw;
	m_size  += bw;
	return bw;
}



void utFileStream::seek(const UTsize pos, int dir) const
{
	if (!m_handle)
		return;

	if (dir == SEEK_END)
	{
		m_pos = m_size;
		return;
	}

	int ret = utFileWrapper::seek(m_handle, pos, dir);

	if (dir == SEEK_SET)
		m_pos = utClamp<UTsize>(ret, 0, m_size);
	else if (dir == SEEK_CUR)
		m_pos = utClamp<UTsize>(m_pos + ret, 0, m_size);
}



utMemoryStream::utMemoryStream(int mode)
	:   m_buffer(0), m_pos(0), m_size(0), m_capacity(0), m_mode(mode)
{
}



void utMemoryStream::open(const char *path, utStream::StreamMode mode)
{
	utFileStream fs;
	fs.open(path, utStream::SM_READ);
	if (fs.isOpen())
		open(fs, mode);
}




void utMemoryStream::open(const utFileStream &fs, utStream::StreamMode mode)
{
	if (fs.isOpen())
	{
		m_size = fs.size();

		if (m_size > 0)
		{
			seek(0, SEEK_SET);
			reserve(m_size);
			fs.read(m_buffer, m_size);
			m_mode = mode;
		}
	}
}



void utMemoryStream::open(const void *buffer, UTsize size, utStream::StreamMode mode)
{
	if (buffer && size > 0 && size != UT_NPOS)
	{
		m_mode = mode;
		m_size= size;
		seek(0, SEEK_SET);
		reserve(m_size);
		memcpy(m_buffer, buffer, m_size);
	}
}



utMemoryStream::~utMemoryStream()
{
	if (m_buffer != 0)
	{
		delete []m_buffer;
		m_buffer= 0;
	}
	m_size= m_pos= 0;
	m_capacity= 0;
}



void utMemoryStream::clear(void)
{
	m_size= m_pos= 0;
	if (m_buffer)
		m_buffer[0]= 0;
}


UTsize utMemoryStream::read(void *dest, UTsize nr) const
{
	// write only
	if (m_mode == utStream::SM_WRITE)
		return -1;

	// eof
	if (m_pos > m_size)
		return 0;

	if (!dest || !m_buffer)
		return 0;

	// clamp
	if ((m_size - m_pos) < nr)
		nr= m_size - m_pos;


	char *cp= (char *)dest;
	char *op= (char *)&m_buffer[m_pos];
	memcpy(cp, op, nr);
	m_pos += nr;
	return nr;
}



UTsize utMemoryStream::write(const void *src, UTsize nr)
{
	// read only
	if (m_mode == utStream::SM_READ || !src)
		return -1;

	// eof
	if (m_pos > m_size)
		return 0;

	if (m_buffer == 0)
		reserve(m_pos + (nr));
	else if (m_pos + nr > m_capacity)
		reserve(m_pos + (nr > 65535 ? nr : nr + 65535));

	char *cp= &m_buffer[m_pos];
	memcpy(cp, (char *)src, nr);

	// advance position
	m_pos += nr;
	m_size += nr;
	return nr;
}

void utMemoryStream::reserve(UTsize nr)
{
	if (m_capacity < nr)
	{
		char *buf= new char[nr + 1];
		if (m_buffer != 0)
		{
			memcpy(buf, m_buffer, m_size);
			delete [] m_buffer;
		}

		m_buffer= buf;
		m_buffer[m_size]= 0;
		m_capacity= nr;
	}
}


void utMemoryStream::seek(const UTsize pos, int dir) const
{
	if (dir == SEEK_SET)
		m_pos = utClamp<UTsize>(pos, 0, m_size);
	else if (dir == SEEK_CUR)
		m_pos = utClamp<UTsize>(m_pos + pos, 0, m_size);
	else if (dir == SEEK_END)
		m_pos = m_size;
}



utFileHandle utFileWrapper::open(const char *filename, int mode)
{
#if UT_PLATFORM == UT_PLATFORM_WIN32 && defined(UT_WIN32_FILE)

	DWORD dwDesiredAccess= 0;
	if (mode & utStream::SM_READ)
		dwDesiredAccess |= GENERIC_READ;
	if (mode & utStream::SM_WRITE)
		dwDesiredAccess |= GENERIC_WRITE;

	DWORD dwShareMode= 0;
	if (mode & utStream::SM_READ)
		dwShareMode |= FILE_SHARE_READ;
	if (mode & utStream::SM_WRITE)
		dwShareMode |= FILE_SHARE_WRITE;


	DWORD dwCreationDisposition= OPEN_EXISTING;
	if (mode == utStream::SM_WRITE)
		dwCreationDisposition= CREATE_ALWAYS;

	HANDLE h= ::CreateFile((LPCTSTR)filename, dwDesiredAccess, dwShareMode, 0,
	                       dwCreationDisposition, FILE_ATTRIBUTE_NORMAL, 0);

	return h == INVALID_HANDLE_VALUE ? 0 : h;

#else
	char fm[3] = {0,0,0};

	char *mp = &fm[0];

	if (mode & utStream::SM_READ)
		*mp++ = 'r';
	else if (mode & utStream::SM_WRITE)
		*mp++ = 'w';
	*mp++ = 'b';
	fm[2] = 0;
	return fopen(filename, fm);
#endif
}



void utFileWrapper::close(utFileHandle &fh)
{
#if UT_PLATFORM == UT_PLATFORM_WIN32 && defined(UT_WIN32_FILE)

	if (fh)
		::CloseHandle((HANDLE)fh);
#else

	if (fh)
		fclose((FILE *)fh);
#endif
}



UTsize utFileWrapper::read(utFileHandle fh, void *dest, UTsize nr)
{
#if UT_PLATFORM == UT_PLATFORM_WIN32 && defined(UT_WIN32_FILE)
	DWORD out;
	::ReadFile(fh, dest, nr, &out, 0);
	return (UTsize)out;
#else
	return (UTsize)fread(dest, 1, nr, (FILE *)fh);
#endif
}


UTsize utFileWrapper::write(utFileHandle fh, const void *src, UTsize nr)
{
#if UT_PLATFORM == UT_PLATFORM_WIN32 && defined(UT_WIN32_FILE)
	DWORD out;
	::WriteFile(fh, src, nr, &out, 0);
	return (UTsize)out;
#else
	return (UTsize)fwrite(src, 1, nr, (FILE *)fh);
#endif
}


UTsize utFileWrapper::size(utFileHandle fh)
{
#if UT_PLATFORM == UT_PLATFORM_WIN32 && defined(UT_WIN32_FILE)
	if (fh)
	{
		DWORD fs= GetFileSize(fh, 0);

		if (fs == INVALID_FILE_SIZE)
		{
			if (GetLastError())
				return 0;
		}
		return fs;
	}
	return (UTsize)0;

#else

	if (fh)
	{
		fseek((FILE *)fh, 0L, SEEK_END);
		UTsize r = (UTsize)ftell((FILE *)fh);
		fseek((FILE *)fh, 0L, SEEK_SET);
		return r;
	}

	return (UTsize)0;
#endif
}


int utFileWrapper::seek(utFileHandle fh, UTsize pos, int way)
{
#if UT_PLATFORM == UT_PLATFORM_WIN32 && defined(UT_WIN32_FILE)
	if (fh)
	{
		DWORD mov = FILE_BEGIN;
		if (way == SEEK_CUR)
			mov = FILE_CURRENT;
		if (way == SEEK_END)
			mov = FILE_END;

		DWORD np = ::SetFilePointer(fh, pos, 0, mov);
		if (np == INVALID_SET_FILE_POINTER)
			return -1;
		return (int)np;
	}
#else
	if (fh)
		return (int)fseek((FILE *)fh, pos, way);
#endif
	return -1;
}


void utFileWrapper::flush(utFileHandle fh)
{
#if UT_PLATFORM == UT_PLATFORM_WIN32 && defined(UT_WIN32_FILE)
	if (fh)
		FlushFileBuffers(fh);
#else
	if (fh)
		fflush((FILE *)fh);
#endif
}
