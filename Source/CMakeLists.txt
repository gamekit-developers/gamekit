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
	akBufferInfo.cpp
	akDualQuat.cpp
	akGeometryDeformer.cpp
	akMathUtils.cpp
	akMesh.cpp
	akMorphTarget.cpp
	akPose.cpp
	akPoseBlender.cpp
	akSkeleton.cpp
	akSkeletonPose.cpp
	akTransitionBlender.cpp
	akVertexGroup.cpp
	
	btAlignedAllocator.cpp
)

set(ANIMKIT_HDR
	akAnimationChannel.h
	akAnimationClip.h
	akAnimationCurve.h
	akAnimationPlayer.h
	akAnimationPlayerSet.h
	akBufferInfo.h
	akColor.h
	akCommon.h
	akDualQuat.h
	akEuler.h
	akGeometryDeformer.h
	akMathUtils.h
	akMesh.h
	akMorphTarget.h
	akPose.h
	akPoseBlender.h
	akSkeleton.h
	akSkeletonPose.h
	akTransformState.h
	akTransitionBlender.h
	akVertexGroup.h
	
	btAlignedAllocator.h
	btAlignedObjectArray.h
)

include_directories(${GAMEKIT_UTILS_PATH} ${ANIMKIT_VECTORMATH_PATH})

add_library(${PROJECT_NAME} ${ANIMKIT_HDR} ${ANIMKIT_SRC})

