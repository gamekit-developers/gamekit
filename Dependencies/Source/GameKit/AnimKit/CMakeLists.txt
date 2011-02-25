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
	akGeometryDeformer.cpp
	akMesh.cpp
	akPoseBlender.cpp
	akSkeleton.cpp
	akSkeletonPose.cpp
	akTransitionBlender.cpp
	akVertexBuffer.cpp
)

set(ANIMKIT_HDR
	akAnimationChannel.h
	akAnimationClip.h
	akAnimationCurve.h
	akAnimationPlayer.h
	akAnimationPlayerSet.h
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
	
	vectormathlibrary/include/vectormath/scalar/cpp/vectormath_aos.h
	vectormathlibrary/include/vectormath/SSE/cpp/vectormath_aos.h
)

include_directories(${GAMEKIT_UTILS_PATH})

add_library(${PROJECT_NAME} ${ANIMKIT_HDR} ${ANIMKIT_SRC})

config_target_if_iphone_build(${PROJECT_NAME} TRUE)
