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

#include <string>
#include <vector>
#include <fstream>

#include <maya/MObject.h>

#include "JointWriter.h"
#include "../SharedStructures.h"

class SkeletonWriter : public Skeleton
{
public:
    std::vector<int>                m_parentIndexList;    
	std::vector<Joint>              m_basePoseList;
    std::vector<unsigned int>       m_jointNameHashList;

	std::vector<JointWriter>		m_jointWriters;
	std::vector<std::string>		m_jointNames;

	SkeletonWriter(void);
	~SkeletonWriter(void);

	// try to add a new joint to the skeleton
	// return joint hash if a joint is already in the skeleton
	unsigned int addJoint(MObject& obj);

	// recursively extract skeleton for the given root joint
	void buildSkeleton(MObject& obj, int parentID);

	// find a root joint for the given joint
	MObject findRootJoint(MObject& obj);

	// find joint with given name and return the id
	int findJoint(const char* name);

	// output skeleton text
	void OutputSkeleton(std::ofstream& ofs);

	// write joint animations to text file
	void writeJointAnimation(std::ostream& ofs, int startFrame, int endFrame);
};
