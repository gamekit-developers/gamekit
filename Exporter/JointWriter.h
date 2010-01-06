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

#include <fstream>

//#include <maya/MObject.h>
#include <maya/MDagPath.h>

#include "../SharedStructures.h"

class JointWriter : public Joint
{
public:
	//MObject& 	m_Obj;
	MDagPath	m_DagPath;

	//JointWriter();
	JointWriter( MDagPath& path );
	//JointWriter( JointWriter const& jw );
	//JointWriter & operator = ( const JointWriter & jw );
	~JointWriter(void);

	// extract transformation of a joint
	void extractJointTransformation();

	// write joint transformation
	void writeJointTransformation(std::ostream& ofs);

	// compare joint transformation with another joint
	bool isEqual(const JointWriter* other);
};
