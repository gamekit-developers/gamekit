# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

project(Detour)
SET(${PROJECT_NAME} 1.0)

set(DETOUR_SRC 
	Source/DetourCommon.cpp
	Source/DetourNavMesh.cpp
	Source/DetourNavMeshBuilder.cpp
	Source/DetourNode.cpp
)

set(DETOUR_HDR
	Include/DetourCommon.h
	Include/DetourNavMesh.h
	Include/DetourNavMeshBuilder.h
	Include/DetourNode.h
)

include_directories( Include )

add_library(${PROJECT_NAME} ${DETOUR_HDR} ${DETOUR_SRC})

config_ogrekit_target(${PROJECT_NAME} TRUE)
