/*
-------------------------------------------------------------------------------
    This file is part of FBT (File Binary Tables).
    http://gamekit.googlecode.com/

    Copyright (c) 2010 Charlie C & Erwin Coumans.

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
#define FBT_IN_SOURCE
#include "fbtStreams.h"
#include "fbtPlatformHeaders.h"



#if FBT_COMPILER == FBT_COMPILER_MSVC
# define fbtp_printf(ptr, size, fmt, lst)   _vsnprintf_s(ptr, size, size, fmt, static_cast<va_list>(lst))
#else
# define fbtp_printf    vsnprintf
#endif

#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif


#if FBT_USE_GZ_FILE == 1
#include "zlib.h"
#endif



class fbtFileWrapper
{
public:
	static fbtFileHandle open(const char* filename, int mode);
	static void close(fbtFileHandle& fh);
	static FBTsize read(fbtFileHandle fh, void* dest, FBTsize nr);
	static FBTsize write(fbtFileHandle fh, const void* src, FBTsize nr);
	static FBTsize size(fbtFileHandle fh);
	static int seek(fbtFileHandle fh, FBTsize pos, int way);
	static void flush(fbtFileHandle fh);
};



FBTsize fbtStream::write(const fbtStream& cpy)
{
	if (size() != cpy.size() && cpy.isOpen() && cpy.size() > 0)
	{
		FBTsize size = cpy.size();
		FBTsize old_pos = cpy.position();
		cpy.seek(0, SEEK_SET);

		char* tbuf = new char[size+1];
		cpy.read(tbuf, size);
		size = write(tbuf, size);
		delete []tbuf;

		cpy.seek(old_pos, SEEK_SET);
		return size;
	}
	return 0;
}



fbtFileStream::fbtFileStream() :
	m_file(), m_handle(0), m_pos(0), m_size(0), m_mode(0)
{
}


void fbtFileStream::open(const char* p, fbtStream::StreamMode mode)
{
	if (m_handle != 0 && m_file != p)
		fbtFileWrapper::close(m_handle);


	m_file = p;
	m_handle = fbtFileWrapper::open(m_file.c_str(), mode);
	if (m_handle)
	{
		if (!(mode & SM_WRITE))
			m_size = fbtFileWrapper::size(m_handle);
	}
}


void fbtFileStream::close(void)
{
	if (m_handle != 0)
	{
		fbtFileWrapper::close(m_handle);
		m_handle = 0;
	}

	m_file.clear();
}



fbtFileStream::~fbtFileStream()
{
	close();
}

FBTsize fbtFileStream::read(void* dest, FBTsize nr) const
{
	if (m_mode == fbtStream::SM_WRITE) return -1;
	if (!dest || !m_handle) return -1;
	if (m_pos > m_size) return 0;

	if ((m_size - m_pos) < nr) nr = m_size - m_pos;


	FBTsize br = fbtFileWrapper::read(m_handle, dest, nr);
	m_pos += br;
	return br;
}



FBTsize fbtFileStream::write(const void* src, FBTsize nr)
{
	if (m_mode == fbtStream::SM_READ) return -1;
	if (!src || !m_handle) return -1;

	FBTsize bw = fbtFileWrapper::write(m_handle, src, nr);
	m_pos   += bw;
	m_size  += bw;
	return bw;
}


FBTsize fbtFileStream::writef(const char* fmt, ...)
{
	static char tmp[1024];

	va_list lst;
	va_start(lst, fmt);
	int size = fbtp_printf(tmp, 1024, fmt, lst);
	va_end(lst);

	if (size > 0)
	{
		tmp[size] = 0;
		return write(tmp, size);
	}
	return -1;

}



void fbtFileStream::seek(const FBTsize pos, int dir) const
{
	if (!m_handle)
		return;

	if (dir == SEEK_END)
	{
		m_pos = m_size;
		return;
	}

	int ret = fbtFileWrapper::seek(m_handle, pos, dir);
	if (dir == SEEK_SET)
		m_pos = fbtClamp<FBTsize>(ret, 0, m_size);
	else if (dir == SEEK_CUR)
		m_pos = fbtClamp<FBTsize>(m_pos + ret, 0, m_size);
}


#if FBT_USE_GZ_FILE == 1


fbtGzStream::fbtGzStream() :
	m_file(), m_handle(0), m_pos(0), m_size(0), m_mode(0)
{
}


void fbtGzStream::open(const char* p, fbtStream::StreamMode mode)
{
	if (m_handle != 0 && m_file != p)
		gzclose(m_handle);

	char fm[3] = {0, 0, 0};
	char* mp = &fm[0];
	if (mode & fbtStream::SM_READ)
		*mp++ = 'r';
	else if (mode & fbtStream::SM_WRITE)
		*mp++ = 'w';
	*mp++ = 'b';
	fm[2] = 0;


	m_file = p;
	m_handle = gzopen(m_file.c_str(), fm);
}


void fbtGzStream::close(void)
{
	if (m_handle != 0)
	{
		gzclose(m_handle);
		m_handle = 0;
	}
	m_file.clear();
}



fbtGzStream::~fbtGzStream()
{
	close();
}

FBTsize fbtGzStream::read(void* dest, FBTsize nr) const
{
	if (m_mode == fbtStream::SM_WRITE) return -1;
	if (!dest || !m_handle) return -1;
	if (m_pos > m_size) return 0;

	if ((m_size - m_pos) < nr) nr = m_size - m_pos;


	FBTsize br = gzread(m_handle, dest, nr);
	m_pos += br;
	return br;
}



FBTsize fbtGzStream::write(const void* src, FBTsize nr)
{
	if (m_mode == fbtStream::SM_READ) return -1;
	if (!src || !m_handle) return -1;

	FBTsize bw = gzwrite(m_handle, src, nr);
	m_pos   += bw;
	m_size  += bw;
	return bw;
}

FBTsize fbtGzStream::writef(const char* fmt, ...)
{
	static char tmp[1024];

	va_list lst;
	va_start(lst, fmt);
	int size = fbtp_printf(tmp, 1024, fmt, lst);
	va_end(lst);

	if (size > 0)
	{
		tmp[size] = 0;
		return write(tmp, size);
	}
	return -1;

}



void fbtGzStream::seek(const FBTsize pos, int dir) const
{
}


#endif

fbtMemoryStream::fbtMemoryStream()
	:   m_buffer(0), m_pos(0), m_size(0), m_capacity(0), m_mode(0)
{
}


void fbtMemoryStream::open(fbtStream::StreamMode mode)
{
	m_mode = mode;
}

void fbtMemoryStream::open(const char* path, fbtStream::StreamMode mode)
{
	fbtFileStream fs;
	fs.open(path, fbtStream::SM_READ);
	if (fs.isOpen()) open(fs, mode);
}


void fbtMemoryStream::open(const fbtFileStream& fs, fbtStream::StreamMode mode)
{
	if (fs.isOpen())
	{
		m_size = fs.size();

		if (m_size > 0)
		{
			seek(0, SEEK_SET);
			reserve(m_size); fs.read(m_buffer, m_size);
			m_mode = mode;
		}
	}
}



void fbtMemoryStream::open(const void* buffer, FBTsize size, fbtStream::StreamMode mode)
{
	if (buffer && size > 0 && size != FBT_NPOS)
	{
		m_mode = mode;
		m_size = size;

		seek(0, SEEK_SET); reserve(m_size);
		fbtMemcpy(m_buffer, buffer, m_size);
	}
}


fbtMemoryStream::~fbtMemoryStream()
{
	if (m_buffer != 0)
	{
		delete []m_buffer;
		m_buffer = 0;
	}
	m_size = m_pos = 0;
	m_capacity = 0;
}



void fbtMemoryStream::clear(void)
{
	m_size = m_pos = 0;
	if (m_buffer)
		m_buffer[0] = 0;
}



FBTsize fbtMemoryStream::read(void* dest, FBTsize nr) const
{
	if (m_mode == fbtStream::SM_WRITE) return -1;
	if (m_pos > m_size) return 0;
	if (!dest || !m_buffer) return 0;

	if ((m_size - m_pos) < nr) nr = m_size - m_pos;

	char* cp = (char*)dest;
	char* op = (char*)&m_buffer[m_pos];
	fbtMemcpy(cp, op, nr);
	m_pos += nr;
	return nr;
}



FBTsize fbtMemoryStream::write(const void* src, FBTsize nr)
{
	if (m_mode == fbtStream::SM_READ || !src)
		return -1;

	if (m_pos > m_size) return 0;

	if (m_buffer == 0)
		reserve(m_pos + (nr));
	else if (m_pos + nr > m_capacity)
		reserve(m_pos + (nr > 65535 ? nr : nr + 65535));

	char* cp = &m_buffer[m_pos];
	fbtMemcpy(cp, (char*)src, nr);

	m_pos   += nr;
	m_size  += nr;
	return nr;
}



FBTsize fbtMemoryStream::writef(const char* fmt, ...)
{
	static char tmp[1024];

	va_list lst;
	va_start(lst, fmt);
	int size = fbtp_printf(tmp, 1024, fmt, lst);
	va_end(lst);


	if (size > 0)
	{
		tmp[size] = 0;
		return write(tmp, size);
	}

	return -1;

}
void fbtMemoryStream::reserve(FBTsize nr)
{
	if (m_capacity < nr)
	{
		char* buf = new char[nr + 1];
		if (m_buffer != 0)
		{
			fbtMemcpy(buf, m_buffer, m_size);
			delete [] m_buffer;
		}

		m_buffer = buf;
		m_buffer[m_size] = 0;
		m_capacity = nr;
	}
}


void fbtMemoryStream::seek(const FBTsize pos, int dir) const
{
	if (dir == SEEK_SET)
		m_pos = fbtClamp<FBTsize>(pos, 0, m_size);
	else if (dir == SEEK_CUR)
		m_pos = fbtClamp<FBTsize>(m_pos + pos, 0, m_size);
	else if (dir == SEEK_END)
		m_pos = m_size;
}



fbtFileHandle fbtFileWrapper::open(const char* filename, int mode)
{

	char fm[3] = {0, 0, 0};
	char* mp = &fm[0];
	if (mode & fbtStream::SM_READ)
		*mp++ = 'r';
	else if (mode & fbtStream::SM_WRITE)
		*mp++ = 'w';
	*mp++ = 'b';
	fm[2] = 0;

	return fopen(filename, fm);
}



void fbtFileWrapper::close(fbtFileHandle& fh)
{
	if (fh)
		fclose((FILE*)fh);
}



FBTsize fbtFileWrapper::read(fbtFileHandle fh, void* dest, FBTsize nr)
{
	return (FBTsize)fread(dest, 1, nr, (FILE*)fh);
}


FBTsize fbtFileWrapper::write(fbtFileHandle fh, const void* src, FBTsize nr)
{
	return (FBTsize)fwrite(src, 1, nr, (FILE*)fh);
}



FBTsize fbtFileWrapper::size(fbtFileHandle fh)
{
	if (fh)
	{
		fseek((FILE*)fh, 0L, SEEK_END);
		FBTsize r = (FBTsize)ftell((FILE*)fh);
		fseek((FILE*)fh, 0L, SEEK_SET);
		return r;
	}
	return (FBTsize)0;
}


int fbtFileWrapper::seek(fbtFileHandle fh, FBTsize pos, int way)
{
	if (fh) return (int)fseek((FILE*)fh, pos, way);
	return 0;
}


void fbtFileWrapper::flush(fbtFileHandle fh)
{
}
