# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

project(AppEmbedDemo)

set(DEMO_BLEND logo_text.blend logo_text2.blend)

if (APPLE)
	include_directories(${OGRELITE_SOURCE_DIR}/OgreMain/include/OSX)
	SET(EXETYPE MACOSX_BUNDLE)
	SET_SOURCE_FILES_PROPERTIES(MainMenu.nib  PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
	foreach(bfile ${DEMO_BLEND})
		SET_SOURCE_FILES_PROPERTIES(${bfile} PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
	endforeach()
	SET (NibFile MainMenu.nib)
endif()

set(DEMO_SRC
	Main.cpp
	okApp.cpp
	okWin.cpp
	okCamera.cpp
	okUtils.cpp
	StdAfx.cpp
	${DEMO_BLEND}
)

set(DEMO_HDR
	okApp.h
	okWin.h
	okCamera.h
	okUtils.h
	StdAfx.h
)

set(ALL
	${DEMO_SRC}
	${DEMO_HDR}
)

include_directories(
	. 
	${GAMEKIT_UTILS_PATH} 
	${OGREKIT_INCLUDE}
)

link_libraries(
	${GAMEKIT_UTILS_TARGET} 
	${OGREKIT_LIB}
)

setup_wx()

set(HiddenCMakeLists ../CMakeLists.txt)
source_group(ParentCMakeLists FILES ${HiddenCMakeLists})

use_precompiled_header(${PROJECT_NAME} StdAfx.h StdAfx.cpp)


if (APPLE)    
	add_executable(
		${PROJECT_NAME} 
		MACOSX_BUNDLE
		MainMenu.nib
		${ALL} 
		${HiddenCMakeLists}
	)
else()
	if (WIN32)
		add_executable(${PROJECT_NAME} WIN32 ${ALL} ${HiddenCMakeLists})
	else()
		add_executable(${PROJECT_NAME} ${ALL} ${HiddenCMakeLists})
	endif()

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