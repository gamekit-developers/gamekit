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

#include "I16Aligned.h"
#include "XTransformation.h"
#include "XSkeleton.h"

#include "../SharedStructures.h"

using namespace std;

class XJointAnim : public I16Aligned
{
	friend std::istream& operator >> (std::istream& ifs, XJointAnim &ja);

public:
	uint				m_NumJointAnims;
	uint				m_NumFrames;
	uint*				m_JointNameHashes;
	vector<string>		m_JointNames;
	XTransformation*	m_JointXforms;

	XJointAnim(void);
	~XJointAnim(void);

	// update skeleton's joint transformations using timeinfo
	void updateJointXforms(const XTimeInfo& timeInfo, XSkeleton& skeleton);
};
