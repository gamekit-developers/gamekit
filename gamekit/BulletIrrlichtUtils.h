
#ifndef BULLET_IRRLICHT_UTILS_H
#define BULLET_IRRLICHT_UTILS_H

namespace irr
{
	namespace scene
	{
		class ISceneNode;
	}
};



class btTransform;
class btMatrix3x3;
class btVector3;

#include "LinearMath/btAlignedObjectArray.h"
#include "LinearMath/btDefaultMotionState.h"

struct	IrrlichtMeshContainer
{
	btAlignedObjectArray<irr::scene::ISceneNode*>	m_sceneNodes;
};

///mapping between right handed and left-handed coordinate system
#define SWAP_COORDINATE_SYSTEMS
#ifdef SWAP_COORDINATE_SYSTEMS

#define IRR_X 0
#define IRR_Y 2
#define IRR_Z 1

#define IRR_X_M 1.f
#define IRR_Y_M 1.f
#define IRR_Z_M 1.f

///also winding is different
#define IRR_TRI_0_X 0
#define IRR_TRI_0_Y 2
#define IRR_TRI_0_Z 1

#define IRR_TRI_1_X 0
#define IRR_TRI_1_Y 3
#define IRR_TRI_1_Z 2
#else
#define IRR_X 0
#define IRR_Y 1
#define IRR_Z 2

#define IRR_X_M 1.f
#define IRR_Y_M 1.f
#define IRR_Z_M 1.f

///also winding is different
#define IRR_TRI_0_X 0
#define IRR_TRI_0_Y 1
#define IRR_TRI_0_Z 2

#define IRR_TRI_1_X 0
#define IRR_TRI_1_Y 2
#define IRR_TRI_1_Z 3
#endif




void	MatrixToEuler(const btMatrix3x3& mat,btVector3& TEuler);

///This class sychronizes the world transform between Bullet rigid bodies and their accompanying Irrlicht nodes
class IrrMotionState : public btDefaultMotionState
{
	btAlignedObjectArray<irr::scene::ISceneNode*> m_irrNodes;
public:

	IrrMotionState();

	virtual ~IrrMotionState();

	///synchronizes world transform from user to physics
	virtual void	getWorldTransform(btTransform& centerOfMassWorldTrans ) const ;

	void	addIrrlichtNode(irr::scene::ISceneNode* node);

	///synchronizes world transform from physics to user
	///Bullet only calls the update of worldtransform for active objects
	virtual void	setWorldTransform(const btTransform& centerOfMassWorldTrans);

};

#endif //BULLET_IRRLICHT_UTILS_H
