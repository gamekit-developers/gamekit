
INCLUDE_DIRECTORIES(
	${BULLET_PHYSICS_SOURCE_DIR}/src 
)

SET(LinearMath_SRCS
	btAlignedAllocator.cpp
		btConvexHull.cpp
		btGeometryUtil.cpp
	btQuickprof.cpp
)

SET(LinearMath_HDRS
	btAabbUtil2.h
	btAlignedAllocator.h
		btAlignedObjectArray.h
	btConvexHull.h
		btDefaultMotionState.h
	btGeometryUtil.h
		btHashMap.h
	btIDebugDraw.h
	btList.h
	btMatrix3x3.h
		btMinMax.h
	btMotionState.h
	btPoolAllocator.h
	btQuadWord.h
		btQuaternion.h
	btQuickprof.h
	btRandom.h
	btScalar.h
		btStackAlloc.h
		btTransform.h
		btTransformUtil.h
	btVector3.h
)

ADD_LIBRARY(LinearMath ${LinearMath_SRCS} ${LinearMath_HDRS})
SET_TARGET_PROPERTIES(LinearMath PROPERTIES VERSION ${BULLET_VERSION})
SET_TARGET_PROPERTIES(LinearMath PROPERTIES SOVERSION ${BULLET_VERSION})


IF (APPLE AND BUILD_SHARED_LIBS AND FRAMEWORK)
	SET_TARGET_PROPERTIES(LinearMath PROPERTIES FRAMEWORK true)
	SET_TARGET_PROPERTIES(LinearMath PROPERTIES PUBLIC_HEADER ${LinearMath_HDRS})
ENDIF (APPLE AND BUILD_SHARED_LIBS AND FRAMEWORK)
