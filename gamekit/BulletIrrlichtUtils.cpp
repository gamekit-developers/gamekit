
#include "BulletIrrlichtUtils.h"
#include <irrlicht.h>
#include "btBulletDynamicsCommon.h"

btScalar	physicsWorldScaling = 1.f;
using namespace irr;

//Blender/OpenGL
//left/right = X axis 
//front/back = Y axis 
//top/bottom = Z axis 
//
//In DirectX's and Irrlicht's and many other coordinate systems it's like this 
//
//left/right = X axis 
//top/bottom = Y axis 
//front/back = Z axis 




void	MatrixToEuler(const btMatrix3x3& mat,btVector3& TEuler)
{
	irr::core::matrix4 imat;
	float m[16];
	m[0] = mat[IRR_X][IRR_X];
	m[1] = IRR_X_M*IRR_Y_M*mat[IRR_Y][IRR_X];
	m[2] = IRR_X_M*IRR_Z_M*mat[IRR_Z][IRR_X];
	m[3] = 0;

	m[4] = IRR_X_M*IRR_Y_M*mat[IRR_X][IRR_Y];
	m[5] = mat[IRR_Y][IRR_Y];
	m[6] = IRR_Y_M*IRR_Z_M*mat[IRR_Z][IRR_Y];
	m[7] = 0;

	m[8] = IRR_X_M*IRR_Z_M*mat[IRR_X][IRR_Z];
	m[9] = IRR_Y_M*IRR_Z_M*mat[IRR_Y][IRR_Z];
	m[10] = mat[IRR_Z][IRR_Z];
	m[11] = 0;
	
	m[12] = 0;//IRR_X_M*btr.getOrigin()[IRR_X];
	m[13] = 0;//IRR_Y_M*btr.getOrigin()[IRR_Y];
	m[14] = 0;//IRR_Z_M*btr.getOrigin()[IRR_Z];
	m[15] = 0;

	imat.setM(m);
	
	irr::core::vector3df eulerrot = imat.getRotationDegrees();
	TEuler[0] = eulerrot.X;
	TEuler[1] = eulerrot.Y;
	TEuler[2] = eulerrot.Z;
}




IrrMotionState::IrrMotionState()
{
}

IrrMotionState::~IrrMotionState()
{
}

///synchronizes world transform from user to physics
void	IrrMotionState::getWorldTransform(btTransform& centerOfMassWorldTrans ) const
{
	centerOfMassWorldTrans = m_graphicsWorldTrans;
}

void	IrrMotionState::addIrrlichtNode(irr::scene::ISceneNode* node)
{
	m_irrNodes.push_back(node);
}

///synchronizes world transform from physics to user
///Bullet only calls the update of worldtransform for active objects
void	IrrMotionState::setWorldTransform(const btTransform& centerOfMassWorldTrans)
{
	
	m_startWorldTrans = centerOfMassWorldTrans;
	m_graphicsWorldTrans = centerOfMassWorldTrans;

	const btVector3& Point = centerOfMassWorldTrans.getOrigin();
	// Set rotation
	btVector3 EulerRotation;
	MatrixToEuler(centerOfMassWorldTrans.getBasis(), EulerRotation);

	for (int i=0;i<m_irrNodes.size();i++)
	{
		m_irrNodes[i]->setPosition(irr::core::vector3df((f32)Point[IRR_X], (f32)Point[IRR_Y], (f32)Point[IRR_Z])/physicsWorldScaling);
		m_irrNodes[i]->setRotation(irr::core::vector3df(EulerRotation[0], EulerRotation[1], EulerRotation[2]));
		
	}
}
