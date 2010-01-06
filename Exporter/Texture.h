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

#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <fstream>

#include <maya/MString.h>

#include "../SharedStructures.h"


/// \brief	This function returns the texture ID of the texture node specified.
///			If the name specified does not exist it will return 0.
/// \param	fnn	-	the file node name. NOTE This is not the name of the texture
///			file, but the name of the fileTexture node in maya.
unsigned int GetTextureID(MString&);

void ClearTextures(SModel* model);

void ExtractTextures(SModel* model);
void OutputTextures(std::ostream& ofs, SModel* model);

#endif
