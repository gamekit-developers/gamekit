// ==========================================================
// Photoshop Loader
//
// Design and implementation by
// - Floris van den Berg (flvdberg@wxs.nl)
// - Thorsten Radde (support@IdealSoftware.com)
//
// Based on public domain code created and
// published by Thatcher Ulrich (ulrich@world.std.com)
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

#include "FreeImage.h"

// ==========================================================
// Bitmap pointer access macros
// ==========================================================

#define BP_START(DIB, WIDTH, HEIGHT) \
	int xxxscanline = 0; \
	RGBQUAD *xxxbits = (RGBQUAD *)FreeImage_GetScanLine(DIB, HEIGHT - 1 - xxxscanline); \
	RGBQUAD *xxxp = xxxbits;

#define BP_NEXT(DIB, WIDTH, HEIGHT) \
	xxxp++; \
	if (xxxp - xxxbits == WIDTH) { \
		xxxscanline++; \
		xxxbits = (RGBQUAD *)FreeImage_GetScanLine(DIB, HEIGHT - 1 - xxxscanline); \
		xxxp = xxxbits; \
	}

#define BP_SETVALUE(VALUE, OFFSET) \
	((BYTE *)xxxp)[OFFSET] = (BYTE)VALUE;

// ==========================================================
// Internal functions
// ==========================================================

static unsigned
Read8(FreeImageIO *io, fi_handle handle) {
	unsigned char i = 0;
	io->read_proc(&i, 1, 1, handle);
	return i;
}

static unsigned
Read16(FreeImageIO *io, fi_handle handle) {
	// reads a two-byte big-endian integer from the given file and returns its value.
	// assumes unsigned.

	unsigned hi = Read8(io, handle);
	unsigned lo = Read8(io, handle);
	return lo + (hi << 8);
}

static unsigned
Read32(FreeImageIO *io, fi_handle handle) {
	// reads a four-byte big-endian integer from the given file and returns its value.
	// assumes unsigned.

	unsigned b3 = Read8(io, handle);
	unsigned b2 = Read8(io, handle);
	unsigned b1 = Read8(io, handle);
	unsigned b0 = Read8(io, handle);
	return (b3 << 24) + (b2 << 16) + (b1 << 8) + b0;
}

// ----------------------------------------------------------

static void
ScanForResolution(float* hres, float* vres, FreeImageIO *io, fi_handle handle, int byte_count) {
	// scans through the next byte_count bytes of the file, looking for an
	// image resource block encoding the image's resolution.  Returns the resolution(s),
	// if found, in the pointed-to floats.  Units are in pixels/meter.

	while (byte_count) {
		// read the image resource block header.

		if (Read32(io, handle) != 0x3842494D /* "8BIM" */)
			throw "image resource block has unknown signature";

		int	id = Read16(io, handle);

		// skip the name.

		int	name_length = Read8(io, handle) | 1;	// name_length must be odd, so that total including size byte is even.

		io->seek_proc(handle, name_length, SEEK_CUR);

		// get the size of the data block.

		int	data_size = Read32(io, handle);

		if ((data_size & 1) == 1)
			data_size++; // block size must be even.		

		// account for header size.

		byte_count -= 11 + name_length;

		// if this block is a resolution info structure, then get the resolution.

		if (id == 0x03ED) {
			int junk;

			int	hres_fixed = Read32(io, handle);
			junk = Read16(io, handle);	// display units of hres.
			junk = Read16(io, handle);	// display units of width.

			int	vres_fixed = Read32(io, handle);
			junk = Read16(io, handle);	// display units of vres.
			junk = Read16(io, handle);	// display units of height.

			byte_count -= data_size;
			data_size -= 16;

			// skip any extra bytes at the end of this block...

			if (data_size > 0)
				io->seek_proc(handle, data_size, SEEK_CUR);			

			// need to convert resolution figures from fixed point, pixels/inch
			// to floating point, pixels/meter.

			*hres = hres_fixed * ((float)39.4 / (float)65536.0);
			*vres = vres_fixed * ((float)39.4 / (float)65536.0);			
		} else {
			// skip the rest of this block.

			io->seek_proc(handle, data_size, SEEK_CUR);

			byte_count -= data_size;
		}
	}
}

// ----------------------------------------------------------

static FIBITMAP *
LoadPSDRGB(FreeImageIO *io, fi_handle handle, int width, int height, int channel_count) {
	// skip the mode data.  (it's the palette for indexed color; other info for other modes.)
	
	long area = width * height;

	// skip the reserved data

	int	reserved_data_count = Read32(io, handle);

	if (reserved_data_count)
		io->seek_proc(handle, reserved_data_count, SEEK_CUR);

	// find out if the data is compressed
	//   0: no compressiod
	//   1: RLE compressed

	unsigned compression = Read16(io, handle);

	if ((compression > 1) || (compression < 0))
		return NULL;	

	// some formatting information about the channels

	const struct ChannelInfo {
		int	ofs, deflt;
	} Channel[4] = {
		{  FI_RGBA_RED,   0 },	// red
		{  FI_RGBA_GREEN, 0 },	// green
		{  FI_RGBA_BLUE,  0 },	// blue
		{  FI_RGBA_ALPHA, 255 }	// alpha
	};

	// Create the destination bitmap

	FIBITMAP *dib = FreeImage_Allocate(width, height, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK);

	// finally, read the image data.

	if (compression) {
		// the RLE-compressed data is preceeded by a 2-byte data count for each row in the data

		io->seek_proc(handle, height * channel_count * 2, SEEK_CUR);

		// read the RLE data by channel

		for (int channel = 0; channel < 4; channel++) {
			const ChannelInfo &c = Channel[channel];

			BP_START(dib, width, height)

			if (channel >= channel_count) {
				// fill this channel with default data.

				for (int i = 0; i < area; i++) {
					BP_SETVALUE(c.deflt, c.ofs);
					BP_NEXT(dib, width, height)
				}
			} else {
				// read the RLE data.

				int	count = 0;

				while (count < area) {
					int	len = Read8(io, handle);

					if (len == 128) {
						// nop
					} else if (len < 128) {
						// copy next len + 1 bytes literally.

						len++;
						// check for buffer overrun
						if ((count + len) > area) {
							len = area - count; 
						}
						count += len;

						while (len) {
							BP_SETVALUE(Read8(io, handle), c.ofs);
							BP_NEXT(dib, width, height)

							len--;
						}
					} else if (len > 128) {
						// next -len + 1 bytes in the dest are replicated from next source byte.
						// (interpret len as a negative 8-bit int.)

						len ^= 0x0FF;
						len += 2;
						// check for buffer overrun
						if ((count + len) > area) {
							len = area - count; 
						}
						count += len;

						unsigned val = Read8(io, handle);

						while (len) {
							BP_SETVALUE(val, c.ofs);
							BP_NEXT(dib, width, height)

							len--;
						}
					}
				}
			}			
		}		
	} else {
		// we're at the raw image data.  it's each channel in order (Red, Green, Blue, Alpha, ...)
		// where each channel consists of an 8-bit value for each pixel in the image.

		for (int channel = 0; channel < 4; channel++) {
			const ChannelInfo &c = Channel[channel];
			
			BP_START(dib, width, height)

			if (channel >= channel_count) {
				// fill this channel with default data.

				for (int i = 0; i < area; i++) {
					BP_SETVALUE(c.deflt, c.ofs);
					BP_NEXT(dib, width, height)
				}
			} else {
				// read the data

				for (int i = 0; i < area; i++) {
					BP_SETVALUE(Read8(io, handle), c.ofs);
					BP_NEXT(dib, width, height)
				}
			}
		}
	}
	
	return dib;
}

// ==========================================================
// Plugin Interface
// ==========================================================

static int s_format_id;

// ==========================================================
// Plugin Implementation
// ==========================================================

static const char * DLL_CALLCONV
Format() {
	return "PSD";
}

static const char * DLL_CALLCONV
Description() {
	return "Adobe Photoshop";
}

static const char * DLL_CALLCONV
Extension() {
	return "psd";
}

static const char * DLL_CALLCONV
MimeType() {
	return "image/freeimage-psd";
}

static BOOL DLL_CALLCONV
Validate(FreeImageIO *io, fi_handle handle) {
	return (Read32(io, handle) == 0x38425053);
}

static BOOL DLL_CALLCONV
SupportsExportDepth(int depth) {
	return FALSE;
}

static BOOL DLL_CALLCONV 
SupportsExportType(FREE_IMAGE_TYPE type) {
	return FALSE;
}

// ----------------------------------------------------------

static FIBITMAP * DLL_CALLCONV
Load(FreeImageIO *io, fi_handle handle, int page, int flags, void *data) {
	try {
		if (Read32(io, handle) == 0x38425053) {
			if (Read16(io, handle) == 1) {
				// 6 reserved bytes.

				Read32(io, handle);
				Read16(io, handle);

				// Read the number of channels (R, G, B, A, etc).

				unsigned channel_count = Read16(io, handle);

				if (channel_count >= 0 && channel_count <= 16) {
					unsigned height = Read32(io, handle);
					unsigned width = Read32(io, handle);

					if (Read16(io, handle) == 8) {
						unsigned mode = Read16(io, handle);

						// Get Resolution Information
						float hres = 2835, vres = 2835;		// 72 dpi

						int	mode_data_count = Read32(io, handle);
						if (mode_data_count)
							io->seek_proc(handle, mode_data_count, SEEK_CUR);	

						int	resource_data_count = Read32(io, handle);
						ScanForResolution(&hres, &vres, io, handle, resource_data_count);

						// Valid options are:
						//   0: Bitmap (not implemented)
						//   1: Grayscale (not implemented)
						//   2: Indexed color (not implemented)
						//   3: RGB color
						//   4: CMYK color (not implemented)
						//   7: Multichannel (not implemented)
						//   8: Duotone (not implemented)
						//   9: Lab color (not implemented)

						FIBITMAP *pBitmap = NULL;

						switch (mode) {
							case 3 :
								pBitmap = LoadPSDRGB(io, handle, width, height, channel_count);
								break;

							default :
								throw "color mode not supported";
						}

						if (pBitmap) {
							FreeImage_SetDotsPerMeterX(pBitmap, (LONG)hres);
							FreeImage_SetDotsPerMeterY(pBitmap, (LONG)vres);
						}

						return pBitmap;
					}
				}
			}
		}
	} catch(const char *message) {
		FreeImage_OutputMessageProc(s_format_id, message);
	}

	return NULL;
}

// ==========================================================
//   Init
// ==========================================================

void DLL_CALLCONV
InitPSD(Plugin *plugin, int format_id) {
	s_format_id = format_id;

	plugin->format_proc = Format;
	plugin->description_proc = Description;
	plugin->extension_proc = Extension;
	plugin->regexpr_proc = NULL;
	plugin->open_proc = NULL;
	plugin->close_proc = NULL;
	plugin->pagecount_proc = NULL;
	plugin->pagecapability_proc = NULL;
	plugin->load_proc = Load;
	plugin->save_proc = NULL;
	plugin->validate_proc = Validate;
	plugin->mime_proc = MimeType;
	plugin->supports_export_bpp_proc = SupportsExportDepth;
	plugin->supports_export_type_proc = SupportsExportType;
	plugin->supports_icc_profiles_proc = NULL;	// not implemented yet;
}
