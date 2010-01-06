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

#include <vector>
#include <ostream>

#include <maya/MItDependencyNodes.h>
#include <maya/MObjectArray.h>
#include <maya/MItGeometry.h>
#include <maya/MAnimControl.h>
#include <maya/MFnMesh.h>
#include <maya/MPointArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MGlobal.h>

#include <assert.h>
#include "BlendShapeWeight.h"

BlendShapeWeight::BlendShapeWeight(void)
{
	m_Positions = NULL;
	m_Normals = NULL;
	m_Weights = NULL;
}

BlendShapeWeight::~BlendShapeWeight(void)
{
	if (m_Positions)
		delete [] m_Positions;
	if (m_Normals)
		delete [] m_Normals;
	if (m_Weights)
		delete [] m_Weights;
}

void BlendShapeWeight::extractTarget(MFnBlendShapeDeformer& fnBlendShape, MObject& target, int weightIndex)
{
	MFnDependencyNode fnDep( target );
	m_TargetName = fnDep.name().asChar();

	MPointArray			vertexArray;
	MFloatVectorArray	normalArray;

	MStatus status = MStatus::kSuccess;

	MFnMesh m_Mesh( target );
	if (MStatus::kFailure == m_Mesh.getPoints(vertexArray, MSpace::kWorld)) {
		MGlobal::displayError("MFnMesh::getPoints"); 
		status = MStatus::kFailure;
	}

	if (MStatus::kFailure == m_Mesh.getNormals(normalArray, MSpace::kWorld)) {
		MGlobal::displayError("MFnMesh::getNormals"); 
		status = MStatus::kFailure;
	}

	// attach the function set to the object
	MItGeometry it(target);

	// extract points
	m_NumPositions = it.count();
	assert( vertexArray.length() == m_NumPositions );
	m_Positions = new float[ m_NumPositions * 3 ];

	int i = 0;
	// iterate through all geometry points
	while(!it.isDone()) 
	{
		MPoint P = it.position();

		m_Positions[i++] = (float)P.x; 
		m_Positions[i++] = (float)P.y; 
		m_Positions[i++] = (float)P.z;

		it.next();
	}
	assert( i == m_NumPositions*3 );

	// extract normals
	m_NumNormals = normalArray.length();
	m_Normals = new float[ m_NumNormals * 3 ];
	for ( unsigned int i = 0; i < m_NumNormals; i++ )
	{
		m_Normals[i*3 + 0] = normalArray[i].x;
		m_Normals[i*3 + 1] = normalArray[i].y;
		m_Normals[i*3 + 2] = normalArray[i].z;
	}
}

void BlendShapeWeight::extractWeightAnimation(MFnBlendShapeDeformer& fnBlendShape, MObject& target, int weightIndex)
{
	// get start and end times
	int startFrame = static_cast<int>(MAnimControl::minTime().as(MTime::kNTSCField));
	int endFrame   = static_cast<int>(MAnimControl::maxTime().as(MTime::kNTSCField));
	m_StartTime = static_cast<float>(MAnimControl::minTime().as(MTime::kSeconds));
	m_EndTime   = static_cast<float>(MAnimControl::maxTime().as(MTime::kSeconds));

	m_NumFrames = endFrame - startFrame + 1;

	if( m_NumFrames > 0 )
	{
		m_Weights = new float[ m_NumFrames ];
		
		//for(int j = startFrame; j <= endFrame; ++j)
		for( unsigned int i = 0; i < m_NumFrames; i++)
		{
			// set the current time
			MAnimControl::setCurrentTime( MTime( i+startFrame, MTime::kNTSCField ) );
			m_Weights[i] = (float)fnBlendShape.weight( weightIndex );
		}
	}
}
