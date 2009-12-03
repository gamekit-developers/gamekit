
INCLUDE_DIRECTORIES(
${BULLET_PHYSICS_SOURCE_DIR}/src 
)

SET(BulletSoftBody_SRCS
	btSoftBody.cpp
	btSoftBodyConcaveCollisionAlgorithm.cpp
	btSoftBodyHelpers.cpp
	btSoftBodyRigidBodyCollisionConfiguration.cpp
	btSoftRigidCollisionAlgorithm.cpp
	btSoftRigidDynamicsWorld.cpp
	btSoftSoftCollisionAlgorithm.cpp
)

SET(BulletSoftBody_HDRS
	btSoftBody.h
	btSoftBodyConcaveCollisionAlgorithm.h
	btSoftBodyHelpers.h
	btSoftBodyRigidBodyCollisionConfiguration.h
	btSoftRigidCollisionAlgorithm.h
	btSoftRigidDynamicsWorld.h
	btSoftSoftCollisionAlgorithm.h
	btSparseSDF.h
)



ADD_LIBRARY(BulletSoftBody  ${BulletSoftBody_SRCS} ${BulletSoftBody_HDRS})
SET_TARGET_PROPERTIES(BulletSoftBody PROPERTIES VERSION ${BULLET_VERSION})
SET_TARGET_PROPERTIES(BulletSoftBody PROPERTIES SOVERSION ${BULLET_VERSION})
IF (BUILD_SHARED_LIBS)
	TARGET_LINK_LIBRARIES(BulletSoftBody BulletDynamics)
ENDIF (BUILD_SHARED_LIBS)

IF (APPLE AND BUILD_SHARED_LIBS AND FRAMEWORK)
	SET_TARGET_PROPERTIES(BulletSoftBody PROPERTIES FRAMEWORK true)
	SET_TARGET_PROPERTIES(BulletSoftBody PROPERTIES PUBLIC_HEADER ${BulletSoftBody_HDRS})
ENDIF (APPLE AND BUILD_SHARED_LIBS AND FRAMEWORK)
