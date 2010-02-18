set(COMPILE_BLENDER_HEADER FALSE)



if (COMPILE_BLENDER_HEADER)
	subdirs(bApi)
endif()


# ---------------------------------------------------------
set(BA_SRC 
	bAscii.cpp
	bAsciiMain.cpp
	bAsciiCompiler.cpp
	bAsciiBlendLinker.cpp
	bAsciiScanner.cpp
	bAsciiKeywords.cpp
	bAsciiBlock.cpp
)

set(BA_HDR 
	bAscii.h
	bAsciiMain.h
	bAsciiCommon.h
	bAsciiCompiler.h
	bAsciiBlendLinker.h
	bAsciiOpCodes.h
	bAsciiScanner.h
	bAsciiKeywords.h
	bAsciiBlock.h
	bAsciiArgument.h
	bApi/Blender.h
)

# ---------------------------------------------------------
# Utility library

set(BA_UT_SRC 
	bUtils/utMemUtils.cpp
	bUtils/utPrint.cpp
)

set(BA_UT_HDR 
	bUtils/utAllocator.h
	bUtils/utArray.h
	bUtils/utCommon.h
	bUtils/utFixedString.h
	bUtils/utHash.h
	bUtils/utHashTable.h
	bUtils/utIntTypes.h
	bUtils/utMemUtils.h
	bUtils/utPrint.h
	bUtils/utStringDefines.h
)


source_group(bUtils\\Source FILES ${BA_UT_SRC})
source_group(bUtils\\Header FILES ${BA_UT_HDR})


include_directories(${CMAKE_CURRENT_SOURCE_DIR}
					${CMAKE_CURRENT_SOURCE_DIR}/bUtils 
					${CMAKE_CURRENT_SOURCE_DIR}/bApi
					)


if (COMPILE_BLENDER_HEADER)

add_custom_command(
	OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/bApi/Blender.h
	COMMAND bApi builtin.blend
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bApi
	DEPENDS ${OGREKIT_BPARSE_TARGET} bApi
	COMMENT "Building Blender Header."
	)

	set(BA_EXTRA_SRC bApi/Blender.cpp)

endif()

add_library(bAscii	${BA_SRC} 
					${BA_HDR}
					${BA_GRM}
					${BA_UT_SRC}
					${BA_EXTRA_SRC}
					${BA_UT_HDR})
 					
