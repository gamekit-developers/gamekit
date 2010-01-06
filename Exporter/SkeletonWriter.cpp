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

#ifdef WIN32
	#ifndef NT_APP
		#define NT_APP
	#endif
	
	#pragma warning(disable:4786)
#endif

#include <iostream>
#include <assert.h>

#include <maya/MFnIkJoint.h>
#include <maya/MVector.h>
#include <maya/MQuaternion.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MPlug.h>
#include <maya/MGlobal.h>
#include <maya/MAnimControl.h>

#include "SkeletonWriter.h"

// hash function
inline unsigned int GenerateNameHash(const char* name)
{
	unsigned int crcValue = ~0UL;

	if (name) 
	{
		while(*name) 
		{
			unsigned int tableTemp = (unsigned int)((crcValue & 0xff) ^ (unsigned char)*name);
	
			for (int bitLoop = 0; bitLoop < 8; bitLoop++) 
			{
				if (tableTemp & 0x01)
					tableTemp = (tableTemp >> 1) ^ 0xedb88320;
				else
					tableTemp = (tableTemp >> 1);
			}

			crcValue = (crcValue >> 8) ^ tableTemp;
			name++;
		}
	}

	return crcValue;
}

SkeletonWriter::SkeletonWriter(void)
{
	m_numJoints = 0;
}

SkeletonWriter::~SkeletonWriter(void)
{
}

void SkeletonWriter::buildSkeleton(MObject& obj, int parentID)
{

	MFnTransform fnX( obj );
	MDagPath path;
	fnX.getPath( path );

	JointWriter joint( path );

	// get the name 
	const char* name = fnX.name().asChar();
	strcpy_s( joint.name, name );

	// check the name
	size_t nameLen = strlen( name );
	if ( nameLen == 0 )
	{
		std::cerr << "Error! Trying to add joint with no name.\n";
		assert(0); exit(1);
	}

	// make sure this joint has not been added yet
	if ( findJoint( name ) != -1 )
		return;

	// extract joint info
	unsigned int nameHash = GenerateNameHash( name );
	std::string nameStr( name );

	joint.extractJointTransformation();

	// add joint to the list
	m_jointNames.push_back( nameStr );
	m_jointWriters.push_back( joint );
	m_parentIndexList.push_back( parentID );
	m_basePoseList.push_back( joint );
	m_jointNameHashList.push_back( nameHash );
	m_numJoints++;

	parentID = (int)m_parentIndexList.size() - 1;

	// extract child joints
	for(int i = 0; i != fnX.childCount(); ++i)
	{
		MObject child = fnX.child(i);
		if ( child.hasFn( MFn::kTransform ) )
			buildSkeleton( child, parentID );
	}
}

MObject SkeletonWriter::findRootJoint(MObject& obj) 
{
	if ( !obj.hasFn( MFn::kJoint ) )
	{
		std::cerr << "Error! Trying to add non-joint object to Skeltons\n";
		assert(0); exit(1);
	}

	MFnTransform joint( obj );
	const char* name = joint.name().asChar();

	size_t nameLen = strlen( name );
	if ( nameLen == 0 )
	{
		std::cerr << "Error! Trying to add joint with no name.\n";
		assert(0); exit(1);
	}

	if ( joint.parentCount() == 0 )
		return obj;

	MObject parentObj = joint.parent(0);
	if ( !parentObj.hasFn( MFn::kTransform ) )
		return obj;

	MObject thisObj = parentObj;
	while( MFnTransform( thisObj ).parentCount() != 0 )
	{
		parentObj = MFnTransform( thisObj ).parent(0);
		if ( !parentObj.hasFn( MFn::kTransform ) )
			return thisObj;
		thisObj = parentObj;
	}
	return thisObj;
}

// find joint with given name and return the id
int SkeletonWriter::findJoint(const char* name)
{
	unsigned int nameHash = GenerateNameHash( name );
	for ( unsigned int i = 0; i < m_jointNameHashList.size(); i++ )
	{
		if ( m_jointNameHashList[i] == nameHash )
			return i;
	}
	return -1;
}

unsigned int SkeletonWriter::addJoint(MObject& obj)
{
	if ( !obj.hasFn( MFn::kJoint ) )
	{
		std::cerr << "Error! Trying to add non-joint object to Skeltons\n";
		assert(0); exit(1);
	}

	// check if this joint is already in a skeleton
	MFnIkJoint joint(obj);
	const char* name = joint.name().asChar();

	size_t nameLen = strlen( name );
	if ( nameLen == 0 )
	{
		std::cerr << "Error! Trying to add joint with no name.\n";
		assert(0); exit(1);
	}

	int jointID = findJoint(name);
	if ( jointID == -1 )
	{
		if ( m_jointNames.size() > 0 )
		{
			std::cerr << "Error! Trying to export multiple skeltons for joint " << name << "\n";
			assert(0); exit(1);
		}
		MObject root = findRootJoint(obj);
		buildSkeleton( root, -1 );
	}

	return GenerateNameHash( name );
}

void SkeletonWriter::OutputSkeleton(std::ofstream& ofs)
{
	ofs << "SKELETON" << std::endl;
	ofs << "numJoints " << m_numJoints << std::endl;
	for( unsigned int i = 0; i < m_numJoints; i++ )
	{
		ofs << "\t" << "JOINT " << m_jointNames[i] << std::endl;
		ofs << "\t\t" << "nameHash        " << m_jointNameHashList[i] << std::endl;
		ofs << "\t\t" << "parentIndex     " << m_parentIndexList[i] << std::endl;
		ofs << "\t\t" << "Transformation  ";

		JointWriter joint = m_jointWriters[i];
		joint.writeJointTransformation( ofs );
	}
}

void SkeletonWriter::writeJointAnimation(std::ostream& ofs, int startFrame, int endFrame)
{
	int numFrames = endFrame - startFrame + 1;
	if ( numFrames > 0 )
	{
		JointWriter** jointAnims = new JointWriter*[ m_numJoints * numFrames ];
		bool*  isAnimated = new bool[ m_numJoints ];
		for ( uint j = 0; j < m_numJoints; j++ )
			isAnimated[j] = false;

		// extract joint trasformations for each frame
		for( int i = 0; i < numFrames; i++ )
		{
			MAnimControl::setCurrentTime(MTime(i+startFrame,MTime::kNTSCField));

			for( uint j = 0; j < m_numJoints; j++ )
			{
				JointWriter* jw = new JointWriter( m_jointWriters[j].m_DagPath );
				jw->extractJointTransformation();
				jointAnims[ i * m_numJoints + j ] = jw;
			}
		}

		// check if joint is animated or not
		for ( int i = 1; i < numFrames; i++ )
		{
			for ( uint j = 0; j < m_numJoints; j++ )
			{
				JointWriter* prev = jointAnims[ (i-1) * m_numJoints + j ];
				JointWriter* curr = jointAnims[  i    * m_numJoints + j ];
				isAnimated[j] |= !curr->isEqual( prev );
			}
		}

		// count animated joint and find out max length of joint names
		uint count = 0;
		uint maxNameLen = 0;
		for ( uint j = 0; j < m_numJoints; j++ )
		{
			if ( isAnimated[j] )
			{
				count++;
				uint nameLen = (uint)m_jointNames[j].length();
				if ( nameLen > maxNameLen )
					maxNameLen = nameLen;
			}
		}

		// write animated joints to text file
		ofs << "\t\t" << "JOINTANIM" << std::endl;
		ofs << "\t\t" << "NumJointAnims " << count << std::endl;
		for ( uint j = 0; j < m_numJoints; j++ )
		{
			if ( isAnimated[j] )
			{
				ofs << "\t\t\t";
				ofs.fill(' ');
				ofs.width(maxNameLen + 1);
				ofs << std::left << m_jointNames[j];
				ofs << m_jointNameHashList[j] << std::endl;
			}
		}
		ofs	<< std::endl;;

		for( int i = 0; i < numFrames; i++ )
		{
			for( uint j = 0; j < m_numJoints; j++ )
			{
				if ( isAnimated[j] )
				{
					JointWriter* jw = jointAnims[ i * m_numJoints + j ];
					ofs << "\t\t\t";
					jw->writeJointTransformation( ofs );
				}
			}
			ofs << std::endl;
		}

		// clean joint animation data
		for ( uint i = 0; i < m_numJoints * numFrames; i++ )
			delete jointAnims[i];

		delete [] jointAnims;
		delete [] isAnimated;
	}
}
