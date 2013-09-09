# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

project(OpenSteer)
SET(${PROJECT_NAME} 1.0)

set(OPENSTEER_SRC 
	src/Color.cpp
	src/Obstacle.cpp
	src/Pathway.cpp
	src/Vec3.cpp
	src/PolylineSegmentedPath.cpp
	src/PolylineSegmentedPathwaySegmentRadii.cpp
	src/PolylineSegmentedPathwaySingleRadius.cpp
	src/SegmentedPath.cpp
	src/SegmentedPathway.cpp   
	src/Path.cpp 
	src/Vec3Utilities.cpp
	src/lq.c
)

set(OPENSTEER_HDR
	include/AbstractVehicle.h
	include/Vec3.h
	include/Utilities.h
	include/SteerLibrary.h
	include/Pathway.h
	include/Obstacle.h
	include/Color.h
	include/PolylineSegmentedPath.h
	include/PolylineSegmentedPathwaySegmentRadii.h
	include/PolylineSegmentedPathwaySingleRadius.h
	include/SegmentedPath.h
	include/SegmentedPathAlikeUtilities.h
	include/SegmentedPathway.h
	include/QueryPathAlike.h
	include/QueryPathAlikeBaseDataExtractionPolicies.h
	include/QueryPathAlikeMappings.h
	include/QueryPathAlikeUtilities.h
	include/SegmentedPath.h
	include/SegmentedPathAlikeUtilities.h
	include/SegmentedPathway.h
	include/StandardTypes.h
	include/Path.h
	include/Vec3Utilities.h
	include/UnusedParameter.h
	include/lq.h
	include/Proximity.h
)

include_directories( 
	include 
	${OGREKIT_OGRE_INCLUDE}
	${GAMEKIT_UTILS_PATH}
	../../../Engine
	../../../bullet/src
)
add_library(${PROJECT_NAME} ${OPENSTEER_HDR} ${OPENSTEER_SRC})

config_ogrekit_target(${PROJECT_NAME}  TRUE)
