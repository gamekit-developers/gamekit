# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

project(Recast)
SET(${PROJECT_NAME} 1.0)

set(RECAST_SRC 
	Source/Recast.cpp
	Source/RecastArea.cpp
	Source/RecastContour.cpp
	Source/RecastFilter.cpp
	Source/RecastLog.cpp
	Source/RecastMesh.cpp
	Source/RecastMeshDetail.cpp
	Source/RecastRasterization.cpp
	Source/RecastRegion.cpp
	Source/RecastTimer.cpp
)

set(RECAST_HDR
	Include/Recast.h
	Include/RecastLog.h
	Include/RecastTimer.h
)

include_directories( Include )
add_library(${PROJECT_NAME} ${RECAST_HDR} ${RECAST_SRC})

config_ogrekit_target(${PROJECT_NAME}  TRUE)
