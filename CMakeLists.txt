# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

project(AK)
SET(${PROJECT_NAME} 1.0)

# ---------------------------------------------------------
set(CMAKE_MODULE_PATH
	${PROJECT_SOURCE_DIR}/CMake
	${PROJECT_SOURCE_DIR}/CMake/Packages
	)

include(CMakeDependentOption)
include(CheckCXXCompilerFlag)
include(ConfigAnimKit)

configure_animkit()

set(GAMEKIT_UTILS_PATH 		${PROJECT_SOURCE_DIR}/Dependencies/Utils/)
set(ANIMKIT_VECTORMATH_PATH	${PROJECT_SOURCE_DIR}/Dependencies/vectormath/)
set(FBT_SOURCE_DIR			${PROJECT_SOURCE_DIR}/Dependencies/FileTools)
set(ZLIB_SOURCE_DIR			${PROJECT_SOURCE_DIR}/Dependencies/ZLib)
set(ANIMKIT_ANIMKIT_PATH	${PROJECT_SOURCE_DIR}/Source/)
set(ANIMKIT_SAMPLES_DIR		${PROJECT_SOURCE_DIR}/Samples/)

set(ANIMKIT_ANIMKIT_TARGET	AnimKit)
set(GAMEKIT_UTILS_TARGET	GameKitUtils)
set(OGREKIT_ZLIB_TARGET		ZLib)

subdirs(Dependencies)
subdirs(Source)
subdirs(Samples)

if (ANIMKIT_ENABLE_UNITTESTS)
	subdirs(UnitTests)
endif()
