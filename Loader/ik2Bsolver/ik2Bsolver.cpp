//-
// ==========================================================================
// Copyright 1995,2006,2008 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

//////////////////////////////////////////////////////////////////
//
// ik2Bsolver: IK 2 Bone Solver
//
// This IK solver solves for 2 bones with
// rotate plane capability.
//
// To create the solver, load the plugin, and then type
// the following in the command window:
//
//   createNode -n ik2Bsolver ik2Bsolver;
//
// To use the solver, create two bones using the Joint Tool.
// Then either use the IK Handle Tool, 
// or type the following in the command window:
//
//   ikHandle -sol ik2Bsolver -sj joint1 -ee joint3;
//
// Moving the handle will cause the IK solver to solve.
//
// For convenience, the command "addIK2BsolverCallbacks"
// will set up callbacks to recreate the ik2Bsolver after a 
// File->New or File->Open, so that the solver will 
// appear to be persistent.
//
//////////////////////////////////////////////////////////////////


#include <math.h>
//#include <iostream>
//
//#include <maya/MFnPlugin.h>
//#include <maya/MObject.h>
//#include <maya/MDagPath.h>
//
//#include <maya/MString.h>
//#include <maya/MPoint.h>
//#include <maya/MVector.h>
//#include <maya/MMatrix.h>
//#include <maya/MQuaternion.h>
//#include <AwPoint.h>
//#include <AwVector.h>
//#include <AwMatrix.h>
//#include <AwQuaternion.h>
#include "ik2Bsolver.h"

//#include <maya/MPxIkSolverNode.h>
//#include <maya/MIkHandleGroup.h>
//#include <maya/MFnIkHandle.h>
//#include <maya/MFnIkEffector.h>
//#include <maya/MFnIkJoint.h>
//
//#include <maya/MArgList.h>
//#include <maya/MPxCommand.h>
//#include <maya/MGlobal.h>
//#include <maya/MSceneMessage.h>
//#include <maya/MSelectionList.h>


#define kSolverType "ik2Bsolver"
#define kPi 3.14159265358979323846264338327950
#define kEpsilon 1.0e-5
#define absoluteValue(x) ((x) < 0 ? (-(x)) : (x))


//////////////////////////////////////////////////////////////////
//
// IK 2 Bone Solver Node
//
//////////////////////////////////////////////////////////////////
//class ik2Bsolver : public MPxIkSolverNode {
//
//public:
//					ik2Bsolver();
//    virtual			~ik2Bsolver();
//	void			postConstructor();
//
//	virtual MStatus	doSolve();
//	virtual MString	solverTypeName() const;
//
//	static	void*	creator();
//	static  MStatus initialize();
//
//	static	MTypeId	id;
//
//private:
//	AwVector poleVectorFromHandle(const MDagPath &handlePath);
//	double	twistFromHandle(const MDagPath &handlePath);
//};

//void solveIK(const AwPoint &startJointPos,
//			 const AwPoint &midJointPos,
//			 const AwPoint &effectorPos,
//			 const AwPoint &handlePos,
//			 const AwVector &poleVector,
//			 double twistValue,
//			 AwQuaternion &qStart,
//			 AwQuaternion &qMid);

//MTypeId ik2Bsolver::id(0x58000030);
//
//ik2Bsolver::ik2Bsolver()
//	: MPxIkSolverNode()
//{
//}
//
//ik2Bsolver::~ik2Bsolver() {}
//
//void ik2Bsolver::postConstructor()
//{
//	setRotatePlane(true);
//}
//
//void* ik2Bsolver::creator()
//{
//	return new ik2Bsolver;
//}
//
//MStatus ik2Bsolver::initialize()
//{ 
//	return MS::kSuccess;
//}
//
//MString ik2Bsolver::solverTypeName() const
////
//// This method returns the type name used to identify this solver.
////
//{
//	return MString(kSolverType);
//}
//
//MStatus ik2Bsolver::doSolve()
////
//// This is the doSolve method which calls solveIK.
////
//{
//	MStatus stat;
//
//	// Handle Group
//	//
//	MIkHandleGroup * handle_group = handleGroup();
//	if (NULL == handle_group) {
//		return MS::kFailure;
//	}
//
//	// Handle
//	//
//	// For single chain types of solvers, get the 0th handle.
//	// Single chain solvers are solvers which act on one handle only, 
//	// i.e. the	handle group for a single chain solver
//	// has only one handle
//	//
//	MObject handle = handle_group->handle(0);
//	MDagPath handlePath = MDagPath::getAPathTo(handle);
//	MFnIkHandle handleFn(handlePath, &stat);
//
//	// Effector
//	//
//	MDagPath effectorPath;
//	handleFn.getEffector(effectorPath);
//	MFnIkEffector effectorFn(effectorPath);
//
//	// Mid Joint
//	//
//	effectorPath.pop();
//	MFnIkJoint midJointFn(effectorPath);
//
//	// Start Joint
//	//
//	MDagPath startJointPath;
//	handleFn.getStartJoint(startJointPath);
//	MFnIkJoint startJointFn(startJointPath);
//
//	// Preferred angles
//	//
//	double startJointPrefAngle[3];
//	double midJointPrefAngle[3];
//	startJointFn.getPreferedAngle(startJointPrefAngle);
//	midJointFn.getPreferedAngle(midJointPrefAngle);
//
//	// Set to preferred angles
//	//
//	startJointFn.setRotation(startJointPrefAngle, 
//							 startJointFn.rotationOrder());
//	midJointFn.setRotation(midJointPrefAngle, 
//						   midJointFn.rotationOrder());
//
//	AwPoint handlePos = handleFn.rotatePivot(MSpace::kWorld);
//	AwPoint effectorPos = effectorFn.rotatePivot(MSpace::kWorld);
//	AwPoint midJointPos = midJointFn.rotatePivot(MSpace::kWorld);
//	AwPoint startJointPos = startJointFn.rotatePivot(MSpace::kWorld);
//	AwVector poleVector = poleVectorFromHandle(handlePath);
//	poleVector *= handlePath.exclusiveMatrix();
//	double twistValue = twistFromHandle(handlePath);
//	
//	AwQuaternion qStart, qMid;
//
//	solveIK(startJointPos,
//			midJointPos,
//			effectorPos,
//			handlePos,
//			poleVector,
//			twistValue,
//			qStart,
//			qMid);
//
//	midJointFn.rotateBy(qMid, MSpace::kWorld);
//	startJointFn.rotateBy(qStart, MSpace::kWorld);
//
//	return MS::kSuccess;
//}

void solveIK(const AwPoint &startJointPos,
			 const AwPoint &midJointPos,
			 const AwPoint &effectorPos,
			 const AwPoint &handlePos,
			 const AwVector &poleVector,
			 double twistValue,
			 AwQuaternion &qStart,
			 AwQuaternion &qMid)
//
// This is method that actually computes the IK solution.
//
{
	// vector from startJoint to midJoint
	AwVector vector1 = midJointPos - startJointPos;
	// vector from midJoint to effector
	AwVector vector2 = effectorPos - midJointPos;
	// vector from startJoint to handle
	AwVector vectorH = handlePos - startJointPos;
	// vector from startJoint to effector
	AwVector vectorE = effectorPos - startJointPos;
	// lengths of those vectors
	double length1 = vector1.length();
	double length2 = vector2.length();
	double lengthH = vectorH.length();
	// component of the vector1 orthogonal to the vectorE
	AwVector vectorO =
		vector1 - vectorE*((vector1*vectorE)/(vectorE*vectorE));

	//////////////////////////////////////////////////////////////////
	// calculate q12 which solves for the midJoint rotation
	//////////////////////////////////////////////////////////////////
	// angle between vector1 and vector2
	double vectorAngle12 = vector1.angle(vector2);
	// vector orthogonal to vector1 and 2
	AwVector vectorCross12 = vector1^vector2;
	double lengthHsquared = lengthH*lengthH;
	// angle for arm extension 
	double cos_theta = 
		(lengthHsquared - length1*length1 - length2*length2)
		/(2*length1*length2);
	if (cos_theta > 1) 
		cos_theta = 1;
	else if (cos_theta < -1) 
		cos_theta = -1;
	double theta = acos(cos_theta);
	// quaternion for arm extension
	AwQuaternion q12(theta - vectorAngle12, vectorCross12);
	
	//////////////////////////////////////////////////////////////////
	// calculate qEH which solves for effector rotating onto the handle
	//////////////////////////////////////////////////////////////////
	// vector2 with quaternion q12 applied
	vector2 = vector2.rotateBy(q12);
	// vectorE with quaternion q12 applied
	vectorE = vector1 + vector2;
	// quaternion for rotating the effector onto the handle
	AwQuaternion qEH(vectorE, vectorH);

	//////////////////////////////////////////////////////////////////
	// calculate qNP which solves for the rotate plane
	//////////////////////////////////////////////////////////////////
	// vector1 with quaternion qEH applied
	vector1 = vector1.rotateBy(qEH);
	if (vector1.isParallel(vectorH))
		// singular case, use orthogonal component instead
		vector1 = vectorO.rotateBy(qEH);
	// quaternion for rotate plane
	AwQuaternion qNP;
	if (!poleVector.isParallel(vectorH) && (lengthHsquared != 0)) {
		// component of vector1 orthogonal to vectorH
		AwVector vectorN = 
			vector1 - vectorH*((vector1*vectorH)/lengthHsquared);
		// component of pole vector orthogonal to vectorH
		AwVector vectorP = 
			poleVector - vectorH*((poleVector*vectorH)/lengthHsquared);
		double dotNP = (vectorN*vectorP)/(vectorN.length()*vectorP.length());
		if (absoluteValue(dotNP + 1.0) < kEpsilon) {
			// singular case, rotate halfway around vectorH
			AwQuaternion qNP1(kPi, vectorH);
			qNP = qNP1;
		}
		else {
			AwQuaternion qNP2(vectorN, vectorP);
			qNP = qNP2;
		}
	}

	//////////////////////////////////////////////////////////////////
	// calculate qTwist which adds the twist
	//////////////////////////////////////////////////////////////////
	AwQuaternion qTwist(twistValue, vectorH);

	// quaternion for the mid joint
	qMid = q12;	
	// concatenate the quaternions for the start joint
	qStart = qEH*qNP*qTwist;
}

//AwVector ik2Bsolver::poleVectorFromHandle(const MDagPath &handlePath)
////
//// This method returns the pole vector of the IK handle.
////
//{
//	MStatus stat;
//	MFnIkHandle handleFn(handlePath, &stat);
//	MPlug pvxPlug = handleFn.findPlug("pvx");
//	MPlug pvyPlug = handleFn.findPlug("pvy");
//	MPlug pvzPlug = handleFn.findPlug("pvz");
//	double pvxValue, pvyValue, pvzValue;
//	pvxPlug.getValue(pvxValue);
//	pvyPlug.getValue(pvyValue);
//	pvzPlug.getValue(pvzValue);
//	AwVector poleVector(pvxValue, pvyValue, pvzValue);
//	return poleVector;
//}
//
//double ik2Bsolver::twistFromHandle(const MDagPath &handlePath)
////
//// This method returns the twist of the IK handle.
////
//{
//	MStatus stat;
//	MFnIkHandle handleFn(handlePath, &stat);
//	MPlug twistPlug = handleFn.findPlug("twist");
//	double twistValue;
//	twistPlug.getValue(twistValue);
//	return twistValue;
//}
//
//
////////////////////////////////////////////////////////////////////
////
//// IK 2 Bone Solver Callbacks
////
////////////////////////////////////////////////////////////////////
//
//class addIK2BsolverCallbacks : public MPxCommand {
//public:															
//					addIK2BsolverCallbacks() {};
//	virtual MStatus	doIt (const MArgList &);
//	static void*	creator();
//	
//	// callback IDs for the solver callbacks
//	static MCallbackId afterNewId;
//	static MCallbackId afterOpenId;
//};
//
//MCallbackId addIK2BsolverCallbacks::afterNewId;
//MCallbackId addIK2BsolverCallbacks::afterOpenId;															
//
//void *addIK2BsolverCallbacks::creator()
//{
//	return new addIK2BsolverCallbacks;
//}
//
//void createIK2BsolverAfterNew(void *clientData)
////
//// This method creates the ik2Bsolver after a File->New.
////
//{
//	MSelectionList selList;
//	MGlobal::getActiveSelectionList( selList );
//	MGlobal::executeCommand("createNode -n ik2Bsolver ik2Bsolver");
//	MGlobal::setActiveSelectionList( selList );
//}
//
//void createIK2BsolverAfterOpen(void *clientData)
////
//// This method creates the ik2Bsolver after a File->Open
//// if the ik2Bsolver does not exist in the loaded file.
////
//{
//	MSelectionList selList;
//	MGlobal::getSelectionListByName("ik2Bsolver", selList);
//	if (selList.length() == 0) {
//		MGlobal::getActiveSelectionList( selList );
//		MGlobal::executeCommand("createNode -n ik2Bsolver ik2Bsolver");
//		MGlobal::setActiveSelectionList( selList );
//	}
//}
//
//MStatus addIK2BsolverCallbacks::doIt(const MArgList &args)
////
//// This method adds the File->New and File->Open callbacks
//// used to recreate the ik2Bsolver.
////
//{
//    // Get the callback IDs so we can deregister them 
//	// when the plug-in is unloaded.
//	afterNewId = MSceneMessage::addCallback(MSceneMessage::kAfterNew, 
//							   createIK2BsolverAfterNew);
//	afterOpenId = MSceneMessage::addCallback(MSceneMessage::kAfterOpen, 
//							   createIK2BsolverAfterOpen);
//	return MS::kSuccess;
//}
//
//
///////////////////////////////////////////////////////////
////
//// Register the IK 2 Bone Solver and Callback Command
////
///////////////////////////////////////////////////////////
//MStatus initializePlugin(MObject obj)
//{
//	MStatus		status;
//	MFnPlugin	plugin(obj, "Autodesk", "2.5", "Any");
//
//	status = plugin.registerNode("ik2Bsolver", 
//								 ik2Bsolver::id,
//								 &ik2Bsolver::creator,
//								 &ik2Bsolver::initialize,
//								 MPxNode::kIkSolverNode);
//	if (!status) {
//		status.perror("registerNode");
//		return status;
//	}
//
//	status = plugin.registerCommand("addIK2BsolverCallbacks",
//	                                addIK2BsolverCallbacks::creator);
//	if (!status) {
//		status.perror("registerCommand");
//		return status;
//	}
//
//	// Register post-load MEL proc
//	//
//	// Note: We make use of the MFnPlugin::registerUI() method which executes
//	// the given MEL procedures following the plugin load to execute our method.
//	// This method will ensure that the solver node is created on plugin load.
//	//
//	//status = plugin.registerUI("ik2Bsolver", "");
//
//	return status;
//}
//
//MStatus uninitializePlugin(MObject obj)
//{
//	MStatus		status;
//	MFnPlugin	plugin(obj);
//
//	status = plugin.deregisterNode(ik2Bsolver::id);
//	if (!status) {
//		status.perror("deregisterNode");
//		return status;
//	}
//
//	status = plugin.deregisterCommand("addIK2BsolverCallbacks");
//	if (!status) {
//		status.perror("deregisterCommand");
//		return status;
//	}
//	
//	// Remove callbacks when plug-in is unloaded.
//	MMessage::removeCallback(addIK2BsolverCallbacks::afterNewId);
//	MMessage::removeCallback(addIK2BsolverCallbacks::afterOpenId);
//
//	return status;
//}
//
