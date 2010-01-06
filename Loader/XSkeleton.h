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

#include <string>
#include <vector>
#include <fstream>

#include "I16Aligned.h"
#include "XTransformation.h"
#include "../SharedStructures.h"

using namespace std;

class XSkeleton : public Skeleton, public I16Aligned
{
	friend std::istream& operator >> (std::istream& ifs, XSkeleton &s);

public:
	vector<string>		m_jointNames;

	XTransformation*	m_bindXforms;
	XTransformation*	m_animXforms;
	bool*				m_animVisibility;

	Matrix4*			m_worldMatrices;

	XSkeleton(void);
	~XSkeleton(void);

	// update world matricies for each joint
	void updateWorldMatrices(const XTransformation& rootXform, bool rootVisibility);

	// return world metrix for a joint
	Matrix4 getJointWorldMatrix(uint jointNameHash);

	// return visibility of a joint
	bool getJointVisibility(uint jointNameHash);

	// draw skeleton for debug
	void draw(const Matrix4& rootMatrix);

	// update joint transformation
	void updateXform(uint jointNameHash, const XTransformation& transformation);
};
