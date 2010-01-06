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

#ifdef WIN32
	#ifndef NT_APP
		#define NT_APP
	#endif
	#pragma warning(disable:4786)
#endif

#include <iostream>
#include <fstream>

#include "BlendShape.h"

enum BlendShapeBinding
{
	NONE,
	BASE,
	SKINED_BASE,
	TARGET
};

class BlendShapeList
{
public:
	BlendShapeList(void);
	~BlendShapeList(void);

	void extractAllBlendShapes();

	BlendShapeBinding getBlendShape(const std::string& meshName, BlendShape** blendShape) const;

	int			m_NumBlendShapes;
	BlendShape*	m_BlendShapes;

	void outputBlendShapeAnims(std::ostream& ofs, int startFrame, int endFrame, SAnimation* anim);
};
