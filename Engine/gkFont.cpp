/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Charlie C.

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
#include "gkSettings.h"

#if defined(OGREKIT_COMPILE_OGRE_SCRIPTS) || defined(OGREKIT_COMPILE_LIBROCKET)

#include "gkFont.h"
#include "OgreFont.h"
#include "OgreFontManager.h"
#include "OgreBitwise.h"
#include "OgreLogManager.h"
#include "OgreMaterialManager.h"
#include "OgreTextureManager.h"
#include "OgreTechnique.h"

#define generic _generic	// keyword for C++/CX
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#undef generic

gkFont::gkFont(gkResourceManager* creator, const gkResourceName& name, const gkResourceHandle& handle)
	:    gkResource(creator, name, handle), m_data(0)
{
}




gkFont::~gkFont()
{
	UTsize i;
	for (i = 0; i < m_loaders.size(); ++i)
		delete m_loaders[i];
	m_loaders.clear();
	delete m_data;
}


void gkFont::setData(void* data, UTsize size)
{
	if (size == 0 || size == UT_NPOS)
		return;

	if (m_data)
		delete m_data;

	m_data = new utMemoryStream();
	m_data->open(data, size, utStream::SM_READ);
}



gkFont::Loader* gkFont::addLoader(UTsize size, UTsize resolution)
{
	Loader* loader = new Loader(this, size, resolution);
	m_loaders.push_back(loader);
	return loader;
}



void gkFont::Loader::loadResource(Ogre::Resource* resource)
{
	Ogre::Font* font = (Ogre::Font*)resource;
	if (!m_parent || !m_parent->m_data || m_parent->m_data->size() == 0 || font != m_font)
		return;

	m_font->setType(Ogre::FT_TRUETYPE);
	m_font->setTrueTypeSize(Ogre::Real(m_size));
	m_font->setTrueTypeResolution(m_resolution);

	Ogre::DataStreamPtr memStream(OGRE_NEW Ogre::MemoryDataStream((void*)m_parent->m_data->ptr(), m_parent->m_data->size(), false, true));
	loadManualOgreFont(m_font, memStream);
}


gkFont::Loader::~Loader()
{
	if (m_font)
	{
		m_font->unload();
		Ogre::FontManager::getSingleton().remove(m_font->getHandle());
	}
}


void gkFont::loadManualOgreFont(Ogre::Font* font, Ogre::DataStreamPtr &dataStreamPtr)
{
	class OgreFontInterface : public Ogre::Font
	{
	public:
		static void loadManualResource(OgreFontInterface* font, Ogre::DataStreamPtr &dataStreamPtr)
		{
			using namespace Ogre;

			GK_ASSERT(font);

			// ManualResourceLoader implementation - load the texture
			FT_Library ftLibrary;
			// Init freetype
			if( FT_Init_FreeType( &ftLibrary ) )
				OGRE_EXCEPT( Exception::ERR_INTERNAL_ERROR, "Could not init FreeType library!",
				"Font::Font");

			FT_Face face;
			// Add a gap between letters vert and horz
			// prevents nasty artefacts when letters are too close together
			Ogre::uint char_spacer = 5;

			// Locate ttf file, load it pre-buffered into memory by wrapping the
			// original DataStream in a MemoryDataStream
			MemoryDataStream ttfchunk(dataStreamPtr);

			// Load font
			if( FT_New_Memory_Face( ftLibrary, ttfchunk.getPtr(), (FT_Long)ttfchunk.size() , 0, &face ) )
				OGRE_EXCEPT( Exception::ERR_INTERNAL_ERROR,
				"Could not open font face!", "Font::createTextureFromFont" );


			// Convert our point size to freetype 26.6 fixed point format
			FT_F26Dot6 ftSize = (FT_F26Dot6)(font->mTtfSize * (1 << 6));
			if( FT_Set_Char_Size( face, ftSize, 0, font->mTtfResolution, font->mTtfResolution ) )
				OGRE_EXCEPT( Exception::ERR_INTERNAL_ERROR,
				"Could not set char size!", "Font::createTextureFromFont" );

			//FILE *fo_def = stdout;

			int max_height = 0, max_width = 0;

			// Backwards compatibility - if codepoints not supplied, assume 33-166
			if (font->mCodePointRangeList.empty())
			{
				font->mCodePointRangeList.push_back(Font::CodePointRange(33, 166));
			}

			// Calculate maximum width, height and bearing
			size_t glyphCount = 0;
			for (Font::CodePointRangeList::const_iterator r = font->mCodePointRangeList.begin();
				r != font->mCodePointRangeList.end(); ++r)
			{
				const Font::CodePointRange& range = *r;
				for(Font::CodePoint cp = range.first; cp <= range.second; ++cp, ++glyphCount)
				{
					FT_Load_Char( face, cp, FT_LOAD_RENDER );

					if( ( 2 * ( face->glyph->bitmap.rows << 6 ) - face->glyph->metrics.horiBearingY ) > max_height )
						max_height = ( 2 * ( face->glyph->bitmap.rows << 6 ) - face->glyph->metrics.horiBearingY );
					if( face->glyph->metrics.horiBearingY > font->mTtfMaxBearingY )
						font->mTtfMaxBearingY = face->glyph->metrics.horiBearingY;

					if( (face->glyph->advance.x >> 6 ) + ( face->glyph->metrics.horiBearingX >> 6 ) > max_width)
						max_width = (face->glyph->advance.x >> 6 ) + ( face->glyph->metrics.horiBearingX >> 6 );
				}

			}

			// Now work out how big our texture needs to be
			size_t rawSize = (max_width + char_spacer) *
								((max_height >> 6) + char_spacer) * glyphCount;

			uint32 tex_side = static_cast<uint32>(Math::Sqrt((Real)rawSize));
			// just in case the size might chop a glyph in half, add another glyph width/height
			tex_side += std::max(max_width, (max_height>>6));
			// Now round up to nearest power of two
			uint32 roundUpSize = Bitwise::firstPO2From(tex_side);

			// Would we benefit from using a non-square texture (2X width(
			size_t finalWidth, finalHeight;
			if (roundUpSize*roundUpSize*0.5 >= rawSize)
			{
				finalHeight = static_cast<size_t>(roundUpSize * 0.5);
			}
			else
			{
				finalHeight = roundUpSize;
			}
			finalWidth = roundUpSize;

			Real textureAspect = (Real)finalWidth / (Real)finalHeight;

			const size_t pixel_bytes = 2;
			size_t data_width = finalWidth * pixel_bytes;
			size_t data_size = finalWidth * finalHeight * pixel_bytes;

			LogManager::getSingleton().logMessage("Font " + font->mName + "using texture size " +
				StringConverter::toString(finalWidth) + "x" + StringConverter::toString(finalHeight));

			uchar* imageData = OGRE_ALLOC_T(uchar, data_size, MEMCATEGORY_GENERAL);
			// Reset content (White, transparent)
			for (size_t i = 0; i < data_size; i += pixel_bytes)
			{
				imageData[i + 0] = 0xFF; // luminance
				imageData[i + 1] = 0x00; // alpha
			}

			size_t l = 0, m = 0;
			for (Font::CodePointRangeList::const_iterator r = font->mCodePointRangeList.begin();
				r != font->mCodePointRangeList.end(); ++r)
			{
				const Font::CodePointRange& range = *r;
				for(Font::CodePoint cp = range.first; cp <= range.second; ++cp )
				{
					FT_Error ftResult;

					// Load & render glyph
					ftResult = FT_Load_Char( face, cp, FT_LOAD_RENDER );
					if (ftResult)
					{
						// problem loading this glyph, continue
						LogManager::getSingleton().logMessage("Info: cannot load character " +
							StringConverter::toString(cp) + " in font " + font->mName);
						continue;
					}

					FT_Int advance = face->glyph->advance.x >> 6;

					unsigned char* buffer = face->glyph->bitmap.buffer;

					if (!buffer)
					{
						// Yuck, FT didn't detect this but generated a null pointer!
						LogManager::getSingleton().logMessage("Info: Freetype returned null for character " +
							StringConverter::toString(cp) + " in font " + font->mName);
						continue;
					}

					int y_bearnig = ( font->mTtfMaxBearingY >> 6 ) - ( face->glyph->metrics.horiBearingY >> 6 );
					int x_bearing = face->glyph->metrics.horiBearingX >> 6;

					for(int j = 0; j < face->glyph->bitmap.rows; j++ )
					{
						size_t row = j + m + y_bearnig;
						uchar* pDest = &imageData[(row * data_width) + (l + x_bearing) * pixel_bytes];
						for(int k = 0; k < face->glyph->bitmap.width; k++ )
						{
							if (font->mAntialiasColour)
							{
								// Use the same greyscale pixel for all components RGBA
								*pDest++= *buffer;
							}
							else
							{
								// Always white whether 'on' or 'off' pixel, since alpha
								// will turn off
								*pDest++= 0xFF;
							}
							// Always use the greyscale value for alpha
							*pDest++= *buffer++;
						}
					}

					font->setGlyphTexCoords(cp,
						(Real)l / (Real)finalWidth,  // u1
						(Real)m / (Real)finalHeight,  // v1
						(Real)( l + ( face->glyph->advance.x >> 6 ) ) / (Real)finalWidth, // u2
						( m + ( max_height >> 6 ) ) / (Real)finalHeight, // v2
						textureAspect
						);

					// Advance a column
					l += (advance + char_spacer);

					// If at end of row
					if( finalWidth - 1 < l + ( advance ) )
					{
						m += ( max_height >> 6 ) + char_spacer;
						l = 0;
					}
				}
			}

			DataStreamPtr memStream(
				OGRE_NEW MemoryDataStream(imageData, data_size, true));

			Image img;
			img.loadRawData( memStream, finalWidth, finalHeight, PF_BYTE_LA );


			{
				font->mMaterial =  MaterialManager::getSingleton().create("Fonts/" + font->mName,  font->mGroup);
				if (font->mMaterial.isNull())
				{
					OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR,
						"Error creating new material!", "Font::load" );
				}

				TextureUnitState *texLayer;
				{
					String texName = font->mName + "Texture";
					// Create, setting isManual to true and passing self as loader
					font->mTexture = TextureManager::getSingleton().createManual(
						texName, font->mGroup, TEX_TYPE_2D, finalWidth, finalHeight, 1, 0, PF_BYTE_LA);

					TextureUnitState* t = font->mMaterial->getTechnique(0)->getPass(0)->createTextureUnitState( texName );
					// Allow min/mag filter, but no mip
					t->setTextureFiltering(FO_LINEAR, FO_LINEAR, FO_NONE);


					// Call internal _loadImages, not loadImage since that's external and
					// will determine load status etc again, and this is a manual loader inside load()
					ConstImagePtrList imagePtrs;
					imagePtrs.push_back(&img);
					font->mTexture->_loadImages( imagePtrs );

				}
				texLayer = font->mMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0);

				// Make sure material is aware of colour per vertex.
				font->mMaterial->getTechnique(0)->getPass(0)->setVertexColourTracking(TVC_DIFFUSE);
				// Clamp to avoid fuzzy edges
				texLayer->setTextureAddressingMode( TextureUnitState::TAM_CLAMP );
				// Allow min/mag filter, but no mip
				texLayer->setTextureFiltering(FO_LINEAR, FO_LINEAR, FO_NONE);


				// Set up blending
				font->mMaterial->setSceneBlending( SBT_TRANSPARENT_ALPHA );
			}

			FT_Done_FreeType(ftLibrary);
		}
	};

	OgreFontInterface::loadManualResource(static_cast<OgreFontInterface*>(font), dataStreamPtr);
}


#endif
