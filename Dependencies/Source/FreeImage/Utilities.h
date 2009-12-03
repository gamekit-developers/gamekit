// ==========================================================
// Utility functions
//
// Design and implementation by
// - Floris van den Berg (flvdberg@wxs.nl)
// - Hervé Drolon <drolon@infonie.fr>
// - Ryan Rubley (ryan@lostreality.org)
//
// This file is part of FreeImage 3
//
// COVERED CODE IS PROVIDED UNDER THIS LICENSE ON AN "AS IS" BASIS, WITHOUT WARRANTY
// OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, WITHOUT LIMITATION, WARRANTIES
// THAT THE COVERED CODE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE
// OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED
// CODE IS WITH YOU. SHOULD ANY COVERED CODE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT
// THE INITIAL DEVELOPER OR ANY OTHER CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY
// SERVICING, REPAIR OR CORRECTION. THIS DISCLAIMER OF WARRANTY CONSTITUTES AN ESSENTIAL
// PART OF THIS LICENSE. NO USE OF ANY COVERED CODE IS AUTHORIZED HEREUNDER EXCEPT UNDER
// THIS DISCLAIMER.
//
// Use at your own risk!
// ==========================================================

#ifndef UTILITIES_H
#define UTILITIES_H

// ==========================================================
//   Standard includes used by the library
// ==========================================================

#include <math.h>
#include <stdlib.h> 
#include <memory.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <assert.h>

#include <string>
#include <list>
#include <map>
#include <set>
#include <vector>
#include <stack>
#include <sstream>
#include <algorithm>

// ==========================================================
//   Bitmap palette and pixels alignment
// ==========================================================

#define FIBITMAP_ALIGNMENT	16	// We will use a 16 bytes alignment boundary

// Memory allocation on a specified alignment boundary
// defined in BitmapAccess.cpp

void* FreeImage_Aligned_Malloc(size_t amount, size_t alignment);
void FreeImage_Aligned_Free(void* mem);

// ==========================================================
//   File I/O structs
// ==========================================================

// these structs are for file I/O and should not be confused with similar
// structs in FreeImage.h which are for in-memory bitmap handling

#ifdef _WIN32
#pragma pack(push, 1)
#else
#pragma pack(1)
#endif // _WIN32

typedef struct tagFILE_RGBA {
  unsigned char r,g,b,a;
} FILE_RGBA;

typedef struct tagFILE_BGRA {
  unsigned char b,g,r,a;
} FILE_BGRA;

typedef struct tagFILE_RGB {
  unsigned char r,g,b;
} FILE_RGB;

typedef struct tagFILE_BGR {
  unsigned char b,g,r;
} FILE_BGR;

#ifdef _WIN32
#pragma pack(pop)
#else
#pragma pack()
#endif // _WIN32

// ==========================================================
//   Utility functions
// ==========================================================

#ifndef _WIN32
inline char*
i2a(unsigned i, char *a, unsigned r) {
	if (i/r > 0) a = i2a(i/r,a,r);
	*a = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[i%r];
	return a+1;
}

/** 
 Transforms integer i into an ascii string and stores the result in a; 
 string is encoded in the base indicated by r.
 @param i Number to be converted
 @param a String result
 @param r Base of value; must be in the range 2 - 36
 @return Returns a
*/
inline char *
_itoa(int i, char *a, int r) {
	r = ((r < 2) || (r > 36)) ? 10 : r;
	if(i < 0) {
		*a = '-';
		*i2a(-i, a+1, r) = 0;
	}
	else *i2a(i, a, r) = 0;
	return a;
}

#endif // !_WIN32

inline unsigned char
HINIBBLE (unsigned char byte) {
	return byte & 0xF0;
}

inline unsigned char
LOWNIBBLE (unsigned char byte) {
	return byte & 0x0F;
}

inline int
CalculateUsedBits(int bits) {
	int bit_count = 0;
	unsigned bit = 1;

	for (unsigned i = 0; i < 32; i++) {
		if ((bits & bit) == bit) {
			bit_count++;
		}

		bit <<= 1;
	}

	return bit_count;
}

inline int
CalculateLine(int width, int bitdepth) {
	return ((width * bitdepth) + 7) / 8;
}

inline int
CalculatePitch(int line) {
	return line + 3 & ~3;
}

inline int
CalculateUsedPaletteEntries(int bit_count) {
	if ((bit_count >= 1) && (bit_count <= 8))
		return 1 << bit_count;

	return 0;
}

inline unsigned char *
CalculateScanLine(unsigned char *bits, unsigned pitch, int scanline) {
	return (bits + (pitch * scanline));
}

inline void
ReplaceExtension(char *result, const char *filename, const char *extension) {
	for (size_t i = strlen(filename) - 1; i > 0; --i) {
		if (filename[i] == '.') {
			memcpy(result, filename, i);
			result[i] = '.';
			memcpy(result + i + 1, extension, strlen(extension) + 1);
			return;
		}
	}

	memcpy(result, filename, strlen(filename));
	result[strlen(filename)] = '.';
	memcpy(result + strlen(filename) + 1, extension, strlen(extension) + 1);
}

// ==========================================================
//   Big Endian / Little Endian utility functions
// ==========================================================

inline void
SwapShort(WORD *sp) {
	BYTE *cp = (BYTE *)sp, t = cp[0]; cp[0] = cp[1]; cp[1] = t;
}

inline void
SwapLong(DWORD *lp) {
	BYTE *cp = (BYTE *)lp, t = cp[0]; cp[0] = cp[3]; cp[3] = t;
	t = cp[1]; cp[1] = cp[2]; cp[2] = t;
}

// ==========================================================
//   Greyscale conversion
// ==========================================================

#define GREY(r, g, b) (BYTE)(((WORD)r * 77 + (WORD)g * 150 + (WORD)b * 29) >> 8)	// .299R + .587G + .114B
/*
#define GREY(r, g, b) (BYTE)(((WORD)r * 169 + (WORD)g * 256 + (WORD)b * 87) >> 9)	// .33R + 0.5G + .17B
*/

// ==========================================================
//   Template utility functions
// ==========================================================

/// Max function
template <class T> T MAX(T a, T b) {
	return (a > b) ? a: b;
}

/// Min function
template <class T> T MIN(T a, T b) {
	return (a < b) ? a: b;
}

/// INPLACESWAP adopted from codeguru.com 
template <class T> void INPLACESWAP(T& a, T& b) {
	a ^= b; b ^= a; a ^= b;
}

/** This procedure computes minimum min and maximum max
 of n numbers using only (3n/2) - 2 comparisons.
 min = L[i1] and max = L[i2].
 ref: Aho A.V., Hopcroft J.E., Ullman J.D., 
 The design and analysis of computer algorithms, 
 Addison-Wesley, Reading, 1974.
*/
template <class T> void 
MAXMIN(const T* L, long n, T& max, T& min) {
	long i1, i2, i, j;
	T x1, x2;
	long k1, k2;

	i1 = 0; i2 = 0; min = L[0]; max = L[0]; j = 0;
	if((n % 2) != 0)  j = 1;
	for(i = j; i < n; i+= 2) {
		k1 = i; k2 = i+1;
		x1 = L[k1]; x2 = L[k2];
		if(x1 > x2)	{
			k1 = k2;  k2 = i;
			x1 = x2;  x2 = L[k2];
		}
		if(x1 < min) {
			min = x1;  i1 = k1;
		}
		if(x2 > max) {
			max = x2;  i2 = k2;
		}
	}
}

// ==========================================================
//   Generic error messages
// ==========================================================

static const char *FI_MSG_ERROR_MEMORY = "Not enough memory";


#endif // UTILITIES_H
