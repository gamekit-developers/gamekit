# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

project(AnimKit)
SET(${PROJECT_NAME} 1.0)

set(ANIMKIT_SRC 
	akAnimationChannel.cpp
	akAnimationClip.cpp
	akAnimationCurve.cpp
	akAnimationPlayer.cpp
	akAnimationPlayerSet.cpp
	akDualQuat.cpp
	akGeometryDeformer.cpp
	akMesh.cpp
	akPoseBlender.cpp
	akSkeleton.cpp
	akSkeletonPose.cpp
	akTransitionBlender.cpp
	akVertexBuffer.cpp
	
	btAlignedAllocator.cpp
)

set(ANIMKIT_HDR
	akAnimationChannel.h
	akAnimationClip.h
	akAnimationCurve.h
	akAnimationPlayer.h
	akAnimationPlayerSet.h
	akDualQuat.h
	akCommon.h
	akEuler.h
	akGeometryDeformer.h
	akMathUtils.h
	akMesh.h
	akPoseBlender.h
	akSkeleton.h
	akSkeletonPose.h
	akTransformState.h
	akTransitionBlender.h
	akVertexBuffer.h
	
	btAlignedAllocator.h
	btAlignedObjectArray.h
	
	vectormath/scalar/vectormath_aos.h
	vectormath/scalar/quat_aos.h
	vectormath/scalar/vec_aos.h
	vectormath/scalar/mat_aos.h
	vectormath/scalar/boolInVec.h
	vectormath/scalar/floatInVec.h
	
	vectormath/sse/vectormath_aos.h
	vectormath/sse/quat_aos.h
	vectormath/sse/vec_aos.h
	vectormath/sse/mat_aos.h
	vectormath/sse/boolInVec.h
	vectormath/sse/floatInVec.h
	vectormath/sse/vecidx_aos.h
)

include_directories(${GAMEKIT_UTILS_PATH})

add_library(${PROJECT_NAME} ${ANIMKIT_HDR} ${ANIMKIT_SRC})

config_target_if_iphone_build(${PROJECT_NAME} TRUE)
