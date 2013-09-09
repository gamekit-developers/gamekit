

set (BPARSE_FILES 
					bBlenderFile.cpp  
					bBlenderFile.h 
					bMain.cpp 
					bMain.h
)

message(STATUS "BPARSE-Setting ${BPARSE_BLEND_DIR}")
if (BPARSE_FILE_FORMAT EQUAL 1)
	set (BPARSE_BLENDER_HEADER_DIR blend25  CACHE INTERNAL "")
elseif (BPARSE_FILE_FORMAT EQUAL 2)	
	set (BPARSE_BLENDER_HEADER_DIR blend263  CACHE INTERNAL "")
else()
	message(FATAL_ERROR "Invalid bparse-file-format.")
endif()

message(STATUS "BPARSE-DIR ${BPARSE_BLENDER_HEADER_DIR}")


LIST(APPEND BPARSE_FILES 
    				${BPARSE_BLENDER_HEADER_DIR}/dna_32.cpp 
    				${BPARSE_BLENDER_HEADER_DIR}/dna_64.cpp
)


INCLUDE_DIRECTORIES(
 	${BULLET_PHYSICS_SOURCE_DIR}/src 
 	../BulletFileLoader 
 	../BlenderSerialize 
 	../BlenderSerialize/${BPARSE_BLENDER_HEADER_DIR} 
)

ADD_LIBRARY(BlenderSerialize  ${BPARSE_FILES} )

config_ogrekit_target(BlenderSerialize TRUE)
