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

#include "JointWriter.h"

#include <maya/MFnIkJoint.h>
#include <maya/MVector.h>
#include <maya/MQuaternion.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MPlug.h>
#include <maya/MGlobal.h>
#include <maya/MAnimControl.h>

//JointWriter::JointWriter() : m_Obj( NULL )
//{
//}

//JointWriter::JointWriter( MObject& obj ) : m_Obj( obj )
//{
//}
JointWriter::JointWriter( MDagPath& path ) : m_DagPath( path )
{
}

//JointWriter::JointWriter( JointWriter const& jw ) : m_Obj( jw.m_Obj )
//{
//}
//
//JointWriter & JointWriter::operator = ( const JointWriter & jw )
//{
//	return *(new JointWriter( jw ));
//}

JointWriter::~JointWriter(void)
{
}


static bool isVisible(MFnDagNode& fnDag, MStatus& status) 
{
	if(fnDag.isIntermediateObject())
		return false;

	MPlug visPlug = fnDag.findPlug("visibility", &status);
	if (MStatus::kFailure == status) 
	{
		MGlobal::displayError("MPlug::findPlug");
		return false;
	} 
	else 
	{
		bool visible;
		status = visPlug.getValue(visible);
		if (MStatus::kFailure == status) 
		{
			MGlobal::displayError("MPlug::getValue");
		}
		return visible;
	}
}

void JointWriter::extractJointTransformation()
{
	MFnTransform fnX( m_DagPath );

	// extract visibility
	//MDagPath p;
	//fnX.getPath(p);
	MFnDagNode fnDagNode( m_DagPath );
	MStatus status;
	bool visibility = isVisible( fnDagNode, status );

	// extract transformation
	double Scale[3];
	fnX.getScale(Scale);

	MVector trans = fnX.translation(MSpace::kTransform);

	MQuaternion R(0,0,0,1);
	fnX.getRotation(R);

	// combined rotation for joint
	if ( fnX.object().hasFn( MFn::kJoint ) )
	{
		MFnIkJoint fnJoint( m_DagPath );
		MQuaternion JO(0,0,0,1);
		MQuaternion RO(0,0,0,1);
		fnJoint.getOrientation(JO);
		RO = fnJoint.rotateOrientation(MSpace::kObject);
		R = RO * R * JO;
	}

	// extract transformation values
	m_visibility = visibility;
	m_rotation[0] = (float)R.x;
	m_rotation[1] = (float)R.y;
	m_rotation[2] = (float)R.z;
	m_rotation[3] = (float)R.w;
	m_translation[0] = (float)trans.x;
	m_translation[1] = (float)trans.y;
	m_translation[2] = (float)trans.z;
	m_scale[0] = (float)Scale[0];
	m_scale[1] = (float)Scale[1];
	m_scale[2] = (float)Scale[2];
}


void JointWriter::writeJointTransformation(std::ostream& ofs)
{
	// set float format
	std::streamsize oldPrec = ofs.precision(5);
	ofs.setf(std::ios::fixed,std::ios::floatfield);	// floatfield set to fixed
	ofs.setf(std::ios::showpos);					// write + for positive 

	ofs << m_visibility << "    " 
		<< m_rotation[0] << " " << m_rotation[1] << " " << m_rotation[2] << " " << m_rotation[3] << "    " 
		<< m_translation[0] << " " << m_translation[1] << " " << m_translation[2] << "    " 
		<< m_scale[0] << " " << m_scale[1] << " " << m_scale[2] << std::endl;

	// unset float format
	ofs.precision(oldPrec);
	ofs.unsetf(std::ios::floatfield);
	ofs.unsetf(std::ios::showpos);
}

bool JointWriter::isEqual(const JointWriter* other)
{
	return 
		m_visibility == other->m_visibility &&
		m_rotation[0] == other->m_rotation[0] &&
		m_rotation[1] == other->m_rotation[1] &&
		m_rotation[2] == other->m_rotation[2] &&
		m_rotation[3] == other->m_rotation[3] &&
		m_translation[0] == other->m_translation[0] &&
		m_translation[1] == other->m_translation[1] &&
		m_translation[2] == other->m_translation[2] &&
		m_scale[0] == other->m_scale[0] &&
		m_scale[1] == other->m_scale[1] &&
		m_scale[2] == other->m_scale[2];
}
