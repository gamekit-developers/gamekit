/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

    Contributor(s): none yet.
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

#include "akTextureLoader.h"

#include "Blender.h"
#include "akBlenderDefines.h"

#include "utTypes.h"
#include "utStreams.h"

#include"akBLoader.h"

#include "../akDemoBase.h"

//Header containing information on the image data and palette
//Optional image identification field
//Optional color map
//Bitmap data

typedef struct TgaHeader
{
	UTbyte   IDLength;        /* 00h  Size of Image ID field */
	UTbyte   ColorMapType;    /* 01h  Color map type */
	UTbyte   ImageType;       /* 02h  Image type code */
	UTbyte   CMapStart[2];    /* 03h  Color map origin */
	UTbyte   CMapLength[2];   /* 05h  Color map length */
	UTbyte   CMapDepth;       /* 07h  Depth of color map entries */
	UTuint16 XOffset;         /* 08h  X origin of image */
	UTuint16 YOffset;         /* 0Ah  Y origin of image */
	UTuint16 Width;           /* 0Ch  Width of image */
	UTuint16 Height;          /* 0Eh  Height of image */
	UTbyte   PixelDepth;      /* 10h  Image pixel size */
	UTbyte   ImageDescriptor; /* 11h  Image descriptor UTbyte */
} TgaHeader;

bool checkHeader(TgaHeader& head)
{
	if(head.ColorMapType != 0)
		return false;
	if(head.ImageType != 2)
		return false;
	if(head.XOffset != 0)
		return false;
	if(head.YOffset != 0)
		return false;
	if(head.PixelDepth != 32 && head.PixelDepth != 24)
		return false;
	return true;
}

GLuint LoadRawTargaTexture(utStream& stream)
{
	GLuint texture;
	TgaHeader header;
	UTuint32 size;
	void* data;

//	Debug should be 18
//	UTuint32 hsize = sizeof(TgaHeader); 

	stream.read(&header, sizeof(TgaHeader));
	
	if(!checkHeader(header))
		return 0;
	
	if(header.IDLength)
		stream.seek(header.IDLength, SEEK_CUR);
	
	size = header.Width * header.Height * header.PixelDepth / 8;
	data = malloc(size);
	stream.read(data, size);
	
	GLint intform = 4;
	GLint form = GL_BGRA;
	if(header.PixelDepth == 24)
	{
		intform = 3;
		form = GL_BGR;
	}
	
	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );
	
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, intform, header.Width, header.Height, 0, form, GL_UNSIGNED_BYTE, data);
//	gluBuild2DMipmaps( GL_TEXTURE_2D, textype, header.Width, header.Height, textype, GL_UNSIGNED_BYTE, data );
	
	free(data);
	return texture;
}

void akTextureLoader::load(void)
{
	Blender::PackedFile* pack = m_image->packedfile;
	if (pack)
	{
		utMemoryStream stream;
		stream.open(pack->data, pack->size, utStream::SM_READ);
		GLuint tex = LoadRawTargaTexture(stream);
		if(tex)
			m_demo->addTexture(AKB_IDNAME(m_image), tex);
		
	}
}
