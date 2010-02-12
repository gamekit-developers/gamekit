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
#include <stdarg.h>
#include "Writer.h"
#include "utStringDefines.h"


// ----------------------------------------------------------------------------
Writer::Writer(const char *fname, bool compress) : m_indent(0), m_compress(compress), m_cur(0)
{
	m_out = fopen(fname, "wb");
}


// ----------------------------------------------------------------------------
Writer::~Writer()
{
	if (m_out)
		fclose(m_out);
}

// ----------------------------------------------------------------------------
void Writer::compression(void)
{
	if (!m_compress) fprintf(m_out, "\n");
	else
	{
		const int max = 80;
		m_cur ++;
		if ((m_cur % max)== (max-1))
			fprintf(m_out, "\n");
	}


}


// ----------------------------------------------------------------------------
void Writer::writeSeperator(int len)
{
	if (m_compress)
		return;

	if (!m_out) return;
	len /= 2;
	fprintf(m_out, "# ");
	for (int i=0; i<len; i++)
		fprintf(m_out, "--");
	fprintf(m_out, "\n");
}

// ----------------------------------------------------------------------------
void Writer::writeIndent(void)
{
	if (m_compress)
		return;

	if (!m_out) return;

	for (int i=0; i<m_indent; i++)
		fprintf(m_out, "    ");
}

// ----------------------------------------------------------------------------
void Writer::writeNewLine(int nr)
{
	if (m_compress)
		return;

	if (!m_out) return;

	for (int i=0; i<nr; i++)
		fprintf(m_out, "\n");
}

// ----------------------------------------------------------------------------
void Writer::writeComment(const char *cmt) 
{
	if (m_compress)
		return;

	if (!m_out) return;
	writeIndent();
	fprintf(m_out, "# %s\n", cmt);
}


// ----------------------------------------------------------------------------
void Writer::openStruct(const char *name, int v)
{
	if (!m_out) return;
	writeIndent();
	fprintf(m_out, "%s: %i {", name, v);
	compression();
	m_indent++;
}

// ----------------------------------------------------------------------------
void Writer::openStruct(const char *name, const char *strname)
{
	if (!m_out) return;
	writeIndent();
	if (strname)
	{
		fprintf(m_out, "%s: \"%s\" {", name, strname);
		compression();
	}

	else
	{
		fprintf(m_out, "%s: {", name);
		compression();
	}
	m_indent++;
}

// ----------------------------------------------------------------------------
void Writer::closeStruct(void)
{
	if (!m_out) return;
	--m_indent;
	if (m_indent < 0) m_indent = 0;

	writeIndent();
	fprintf(m_out, "}");
	compression();
}

// ----------------------------------------------------------------------------
void Writer::writeBool(const char *name, bool v)
{
	if (!m_out) return;

	writeIndent();
	fprintf(m_out, "%s = %s;", name, (v ? "true" : "false"));
	compression();
}


// ----------------------------------------------------------------------------
void Writer::writeInt(const char *name, int v)
{
	if (!m_out) return;

	writeIndent();
	fprintf(m_out, "%s = %i;", name, v);
	compression();
}


// ----------------------------------------------------------------------------
void Writer::writeFloat(const char *name, float v)
{
	if (!m_out) return;

	writeIndent();
	fprintf(m_out, "%s = %f;", name, v);
	compression();
}

// ----------------------------------------------------------------------------
void Writer::writeFloat2(const char *name, float *v)
{
	if (!m_out || !v) return;

	writeIndent();
	fprintf(m_out, "%s = %f, %f;", name, v[0], v[1]);
	compression();
}

// ----------------------------------------------------------------------------
void Writer::writeFloat3(const char *name, float *v)
{
	if (!m_out || !v) return;

	writeIndent();
	fprintf(m_out, "%s = %f, %f, %f;", name, v[0], v[1], v[2]);
	compression();
}


// ----------------------------------------------------------------------------
void Writer::writeFloat9(const char *name, float *v)
{
	FloatArray fa(9, v);
	writeFloatArray(name, fa, 3);
}

// ----------------------------------------------------------------------------
void Writer::writeFloat16(const char *name, float *v)
{
	FloatArray fa(16, v);
	writeFloatArray(name, fa, 4);
}


// ----------------------------------------------------------------------------
void Writer::writeString(const char *name, const char *str)
{
	if (!m_out) return;

	writeIndent();
	fprintf(m_out, "%s = \"%s\";", name, str);
	compression();
}

// ----------------------------------------------------------------------------
void Writer::writeFloatArray(const char *name, FloatArray& v, int max)
{
	if (!m_out) return;
	writeIndent();


	utFixedString<256> str;
	str.format("%s = ", name);

	fprintf(m_out, "%s", str.c_str());


	FloatArray::Pointer ptr = v.ptr();

	if (max == 0) max = v.size();

	size_t size = v.size();
	for (size_t i=0; i < size; i++)
	{
		fprintf(m_out, "%f", ptr[i]);

		if (i < (size-1))
			fprintf(m_out, ", ");

		if (i < (size-1) && (i % max) == (max-1))
		{
			compression();
			if (!m_compress)
			{
				writeIndent();
				for (size_t s=0; s<(size_t)str.size(); s++)
					fprintf(m_out, " ");
			}
		}
	}

	fprintf(m_out, ";");
	compression();
}

// ----------------------------------------------------------------------------
void Writer::writeIntArray(const char *name, IntArray& v, int max)
{
	if (!m_out) return;
	writeIndent();


	utFixedString<256> str;
	str.format("%s = ", name);

	fprintf(m_out, "%s", str.c_str());


	IntArray::Pointer ptr = v.ptr();

	if (max == 0) max = v.size();

	size_t size = v.size();
	for (size_t i=0; i < size; i++)
	{
		fprintf(m_out, "%i", ptr[i]);
		if (i < (size-1))
			fprintf(m_out, ", ");

		if (i < (size-1) && (i % max) == (max-1))
		{
			compression();
			if (!m_compress)
			{
				writeIndent();
				for (size_t s=0; s<(size_t)str.size(); s++)
					fprintf(m_out, " ");
			}
		}
	}
	fprintf(m_out, ";");
	compression();
}


// ----------------------------------------------------------------------------
void Writer::writeStringArray(const char *name, CharArray& v, int max)
{
	if (!m_out) return;
	writeIndent();


	utFixedString<256> str;
	str.format("%s = ", name);

	fprintf(m_out, "%s", str.c_str());


	CharArray::Pointer ptr = v.ptr();

	if (max == 0) max = v.size();

	size_t size = v.size();
	for (size_t i=0; i < size; i++)
	{
		fprintf(m_out, "\"%s\"", ptr[i]);

		if (i < (size-1))
			fprintf(m_out, ", ");

		if (i < (size-1) && (i % max) == (max-1))
		{
			compression();
			if (!m_compress)
			{
				writeIndent();
				for (size_t s=0; s<(size_t)str.size(); s++)
					fprintf(m_out, " ");
			}
		}
	}

	fprintf(m_out, ";");
	compression();
}



// ----------------------------------------------------------------------------
void Writer::writeEnumArray(const char *name, CharArray& v, int max)
{
	if (!m_out) return;
	writeIndent();


	utFixedString<256> str;
	str.format("%s = ", name);

	fprintf(m_out, "%s", str.c_str());


	CharArray::Pointer ptr = v.ptr();

	if (max == 0) max = v.size();

	size_t size = v.size();
	for (size_t i=0; i < size; i++)
	{
		fprintf(m_out, "%s", ptr[i]);

		if (i < (size-1))
			fprintf(m_out, ", ");

		if (i < (size-1) && (i % max) == (max-1))
		{
			compression();
			if (!m_compress)
			{
				writeIndent();
				for (size_t s=0; s<(size_t)str.size(); s++)
					fprintf(m_out, " ");
			}
		}
	}

	fprintf(m_out, ";");
	compression();
}


// ----------------------------------------------------------------------------
void Writer::writeCommentf(const char *fmt, ...) 
{
	static char Buffer[1024+1];

	if (m_compress)
		return;

	if (!m_out) return;

	va_list lst;
	va_start(lst, fmt);
	int size = ut_vsnprintf(Buffer, 1024, fmt, lst);
	va_end(lst);

	if (size < 0)
	{
		Buffer[1024] = 0;
		size = 1024;
	}

	if (size > 0)
	{
		/// out to log stream so user def flags work
		Buffer[size] = 0;

		for (int i=0; i<m_indent; i++)
			fprintf(m_out, "\t");
		fprintf(m_out, "# %s\n", Buffer);
	}
}

// ----------------------------------------------------------------------------
void Writer::writef(const char *fmt, ...) 
{
	static char Buffer[1024+1];

	if (!m_out) return;

	va_list lst;
	va_start(lst, fmt);
	int size = ut_vsnprintf(Buffer, 1024, fmt, lst);
	va_end(lst);

	if (size < 0)
	{
		Buffer[1024] = 0;
		size = 1024;
	}

	if (size > 0)
	{
		/// out to log stream so user def flags work
		Buffer[size] = 0;

		for (int i=0; i<m_indent; i++)
			fprintf(m_out, "\t");
		fprintf(m_out, "%s", Buffer);
	}
}


// ----------------------------------------------------------------------------
void Writer::writef(const char *name, const char *fmt, ...) 
{
	static char Buffer[1024+1];

	if (!m_out) return;

	va_list lst;
	va_start(lst, fmt);
	int size = ut_vsnprintf(Buffer, 1024, fmt, lst);
	va_end(lst);

	if (size < 0)
	{
		Buffer[1024] = 0;
		size = 1024;
	}

	if (size > 0)
	{
		/// out to log stream so user def flags work
		Buffer[size] = 0;

		if (!m_compress)
		{
			for (int i=0; i<m_indent; i++)
				fprintf(m_out, "\t");
		}
		fprintf(m_out, "%s = \"%s\";", name, Buffer);
		compression();
	}
}
