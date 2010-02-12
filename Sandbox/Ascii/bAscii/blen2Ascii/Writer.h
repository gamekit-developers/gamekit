/*
-------------------------------------------------------------------------------
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#ifndef _Writer_h_
#define _Writer_h_

#include "utCommon.h"
#include <stdio.h>
#include "utArray.h"
#include "utFixedString.h"

typedef utArray<float>	FloatArray;
typedef utArray<int>	IntArray;
typedef utArray<char*>	CharArray;
typedef utFixedString<128> String;


class Writer
{
protected:

	FILE*	m_out;
	int		m_indent;
	bool	m_compress;
	int		m_cur;

	void compression(void);
public:
	Writer(const char *fname, bool compress);
	~Writer();

	bool isOpen(void) const {return m_out != 0;}


	void setIndent(int indent) {m_indent = indent;}

	void writeIndent(void);
	void writef(const char *fmt, ...);
	void writef(const char *name, const char *fmt, ...);

	void writeCommentf(const char *fmt, ...);
	void writeComment(const char *cmt);
	void writeSeperator(int len=60);
	void writeNewLine(int nr=1);

	void writeBool(const char *name, bool v);
	void writeInt(const char *name, int v);
	void writeFloat(const char *name, float v);
	void writeFloat2(const char *name, float *v);
	void writeFloat3(const char *name, float *v);
	void writeFloat9(const char *name, float *v);
	void writeFloat16(const char *name, float *v);
	void writeString(const char *name, const char *str);


	void writeEnumArray(const char *name, CharArray& v, int max = 10);
	void writeStringArray(const char *name, CharArray& v, int max = 10);
	void writeFloatArray(const char *name, FloatArray& v, int max = 10);
	void writeIntArray(const char *name, IntArray& v, int max = 10);

	void openStruct(const char *name, int v);
	void openStruct(const char *name, const char *strname=0);
	void closeStruct(void);



};


#endif//_Writer_h_
