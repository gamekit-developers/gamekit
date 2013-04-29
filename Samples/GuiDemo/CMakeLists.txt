# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

project(AppGuiDemo)

set(DEMO_BLEND logo_text.blend assets.blend)

if (APPLE)
	include_directories(${OGRELITE_SOURCE_DIR}/OgreMain/include/OSX)
	SET(EXETYPE MACOSX_BUNDLE)
	SET_SOURCE_FILES_PROPERTIES(MainMenu.nib  PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
	SET_SOURCE_FILES_PROPERTIES(${DEMO_BLEND} PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
	SET (NibFile MainMenu.nib)
endif()

set(DEMO_SRC
	main.cpp
	${DEMO_BLEND}
)

set(DEMO_HDR
	#${GEN_TEMPLATES_OUT}
)

set(ALL
	${DEMO_SRC}
	${DEMO_HDR}
)


include_directories(
	. 
	#${GAMEKIT_UTILS_PATH} 
	${OGREKIT_INCLUDE}
	${OGREKIT_LIBROCKET_INCLUDE}
)

add_definitions(-DSTATIC_LIB)

#message(STATUS ${OGREKIT_LIBROCKET_LIBS})

link_libraries(
	${OGREKIT_LIB}
	${OGREKIT_LIBROCKET_LIBS}
)

set(HiddenCMakeLists ../CMakeLists.txt)
source_group(ParentCMakeLists FILES ${HiddenCMakeLists})

#use_precompiled_header(${PROJECT_NAME} StdAfx.h StdAfx.cpp)


if (APPLE)    
	add_executable(
		${PROJECT_NAME} 
		MACOSX_BUNDLE
		MainMenu.nib
		${ALL} 
		${HiddenCMakeLists}
	)
else()

	add_executable(${PROJECT_NAME} ${ALL} ${HiddenCMakeLists})

	IF (NOT INTERNAL_CREATE_DISTRIBUTABLE_MSVC_PROJECTFILES)
		foreach(bfile ${DEMO_BLEND})
			ADD_CUSTOM_COMMAND(
				TARGET ${PROJECT_NAME} 
				POST_BUILD
				COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/${bfile} ${CMAKE_CURRENT_BINARY_DIR}/${bfile}
			)
		endforeach()
	ENDIF ()
endif()