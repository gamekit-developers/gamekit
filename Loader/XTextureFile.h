/*
Maya Character Exporter
Copyright (c) 2009 Jun Murakawa

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. 
   If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#pragma once

#include <stdio.h>
#include <math.h>
#include <iostream>

struct DDS_IMAGE_DATA
{
    int				width;
    int				height;
    int				components;
    unsigned int	format;
    int				numMipMaps;
    unsigned char*	pixels;
};

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#include <ddraw.h>    // Required for DirectX's DDSURFACEDESC2 structure definition
#else // Mac OS
	// define types for DDSURFACEDESC2_32BIT 
	#define LONG int
	#define DWORD unsigned int 
	#define WORD  unsigned short
	#define BYTE  unsigned char
	#define DUMMYUNIONNAMEN(n)  //u##n

	#ifndef MAKEFOURCC
		#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
		((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |                   \
		((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
	#endif

	/*
	 * FOURCC codes for DX compressed-texture pixel formats
	 */
	#define FOURCC_DXT1  (MAKEFOURCC('D','X','T','1'))
	#define FOURCC_DXT2  (MAKEFOURCC('D','X','T','2'))
	#define FOURCC_DXT3  (MAKEFOURCC('D','X','T','3'))
	#define FOURCC_DXT4  (MAKEFOURCC('D','X','T','4'))
	#define FOURCC_DXT5  (MAKEFOURCC('D','X','T','5'))

	/*
	 * DDPIXELFORMAT
	 */
	typedef struct _DDPIXELFORMAT
		{
			DWORD       dwSize;                 // size of structure
			DWORD       dwFlags;                // pixel format flags
			DWORD       dwFourCC;               // (FOURCC code)
			union
			{
				DWORD   dwRGBBitCount;          // how many bits per pixel
				DWORD   dwYUVBitCount;          // how many bits per pixel
				DWORD   dwZBufferBitDepth;      // how many total bits/pixel in z buffer (including any stencil bits)
				DWORD   dwAlphaBitDepth;        // how many bits for alpha channels
				DWORD   dwLuminanceBitCount;    // how many bits per pixel
				DWORD   dwBumpBitCount;         // how many bits per "buxel", total
				DWORD   dwPrivateFormatBitCount;// Bits per pixel of private driver formats. Only valid in texture
				// format list and if DDPF_D3DFORMAT is set
			} DUMMYUNIONNAMEN(1);
			union
			{
				DWORD   dwRBitMask;             // mask for red bit
				DWORD   dwYBitMask;             // mask for Y bits
				DWORD   dwStencilBitDepth;      // how many stencil bits (note: dwZBufferBitDepth-dwStencilBitDepth is total Z-only bits)
				DWORD   dwLuminanceBitMask;     // mask for luminance bits
				DWORD   dwBumpDuBitMask;        // mask for bump map U delta bits
				DWORD   dwOperations;           // DDPF_D3DFORMAT Operations
			} DUMMYUNIONNAMEN(2);
			union
			{
				DWORD   dwGBitMask;             // mask for green bits
				DWORD   dwUBitMask;             // mask for U bits
				DWORD   dwZBitMask;             // mask for Z bits
				DWORD   dwBumpDvBitMask;        // mask for bump map V delta bits
				struct
				{
					WORD    wFlipMSTypes;       // Multisample methods supported via flip for this D3DFORMAT
					WORD    wBltMSTypes;        // Multisample methods supported via blt for this D3DFORMAT
				} MultiSampleCaps;
				
			} DUMMYUNIONNAMEN(3);
			union
			{
				DWORD   dwBBitMask;             // mask for blue bits
				DWORD   dwVBitMask;             // mask for V bits
				DWORD   dwStencilBitMask;       // mask for stencil bits
				DWORD   dwBumpLuminanceBitMask; // mask for luminance in bump map
			} DUMMYUNIONNAMEN(4);
			union
			{
				DWORD   dwRGBAlphaBitMask;      // mask for alpha channel
				DWORD   dwYUVAlphaBitMask;      // mask for alpha channel
				DWORD   dwLuminanceAlphaBitMask;// mask for alpha channel
				DWORD   dwRGBZBitMask;          // mask for Z channel
				DWORD   dwYUVZBitMask;          // mask for Z channel
			} DUMMYUNIONNAMEN(5);
		} DDPIXELFORMAT;

	/*
	 * DDCOLORKEY
	 */
	typedef struct _DDCOLORKEY
		{
			DWORD       dwColorSpaceLowValue;   // low boundary of color space that is to
			// be treated as Color Key, inclusive
			DWORD       dwColorSpaceHighValue;  // high boundary of color space that is
			// to be treated as Color Key, inclusive
		} DDCOLORKEY;

	/*
	 * DDSCAPS2
	 */
	typedef struct _DDSCAPS2
		{
			DWORD       dwCaps;         // capabilities of surface wanted
			DWORD       dwCaps2;
			DWORD       dwCaps3;
			union
			{
				DWORD       dwCaps4;
				DWORD       dwVolumeDepth;
			} DUMMYUNIONNAMEN(1);
		} DDSCAPS2;

#endif

// workaround for DDS load issue on 64 bit system
typedef struct _DDSURFACEDESC2_32BIT
{
    DWORD dwSize;                 // size of the DDSURFACEDESC structure
    DWORD dwFlags;                // determines what fields are valid
    DWORD dwHeight;               // height of surface to be created
    DWORD dwWidth;                // width of input surface
union
{
    LONG lPitch;                 // distance to start of next line (return value only)
    DWORD dwLinearSize;           // Formless late-allocated optimized surface size
}           DUMMYUNIONNAMEN( 1 );
union
{
    DWORD dwBackBufferCount;      // number of back buffers requested
    DWORD dwDepth;                // the depth if this is a volume texture 
}           DUMMYUNIONNAMEN( 5 );
union
{
    DWORD dwMipMapCount;          // number of mip-map levels requestde
    // dwZBufferBitDepth removed, use ddpfPixelFormat one instead
    DWORD dwRefreshRate;          // refresh rate (used when display mode is described)
    DWORD dwSrcVBHandle;          // The source used in VB::Optimize
}           DUMMYUNIONNAMEN( 2 );
DWORD       dwAlphaBitDepth;        // depth of alpha buffer requested
DWORD       dwReserved;             // reserved
DWORD       lpSurface32;            // this ptr isn't 64bit safe in the ddraw.h header
union
{
    DDCOLORKEY ddckCKDestOverlay;      // color key for destination overlay use
    DWORD dwEmptyFaceColor;       // Physical color for empty cubemap faces
}           DUMMYUNIONNAMEN( 3 );
DDCOLORKEY  ddckCKDestBlt;          // color key for destination blt use
DDCOLORKEY  ddckCKSrcOverlay;       // color key for source overlay use
DDCOLORKEY  ddckCKSrcBlt;           // color key for source blt use
union
{
    DDPIXELFORMAT ddpfPixelFormat;        // pixel format description of the surface
    DWORD dwFVF;                  // vertex format description of vertex buffers
}           DUMMYUNIONNAMEN( 4 );
DDSCAPS2    ddsCaps;                // direct draw surface capabilities
DWORD       dwTextureStage;         // stage in multitexture cascade
} DDSURFACEDESC2_32BIT;

// functions
DDS_IMAGE_DATA* loadDDSTextureFile( const char *filename );
void loadCompressedTexture( unsigned int texObjID, const char *filename );



