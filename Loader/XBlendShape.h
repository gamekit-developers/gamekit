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
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <math.h>
#include <iostream>
#include <vector>
#include <float.h>

#include "XBlendShapeWeight.h"
#include "XMesh.h"

class XBlendShape : public SBlendAnim
{
	friend std::istream &operator>>(std::istream&,XBlendShape&);

public:
	XBlendShape(XTimeInfo* timeInfo);
	~XBlendShape(void);
	
	void updateWeights(); // runtime

	std::string				m_Name;
	std::string				m_BaseName;  // Assuming one base object for now
	XBlendShapeWeight		m_Weight;

	// actual connection to the base mesh
	XMesh					*m_pTheMesh;

};
