cmake_minimum_required(VERSION 2.6)

project(GameKitUtils)
SET(${PROJECT_NAME} 1.0)

set(GAMEKIT_UTILS_SRC 
	utRandom.cpp
	utScript.cpp
	utStreams.cpp
	utString.cpp
)

set(GAMEKIT_UTILS_HDR
	utCommon.h
	utMemoryPool.h
	utRandom.h
	utScript.h
	utSingleton.h
	utStreams.h
	utString.h
	utTypes.h
)


add_library(${PROJECT_NAME} ${GAMEKIT_UTILS_SRC} ${GAMEKIT_UTILS_HDR})
