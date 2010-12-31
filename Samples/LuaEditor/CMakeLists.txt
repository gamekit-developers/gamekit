# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

project(AppLuaEditor)



set(APP_PROJ0	Data/Test0.blend Data/Test0.lua Data/Test0.okproj Data/Test0.xpm)
set(APP_PROJ1	Data/Test1.blend Data/Test1.lua Data/Test1.okproj Data/Test1.xpm)
set(APP_HELP	Help/OgreKitLuaApi.chm Help/index.hhk)
set(APP_ETC		Data/base.blend Data/Empty.xpm Data/Test0m.blend)
set(APP_CONFIG Config/luconfig.xml Config/lutemplate.xml Config/config.xsl Config/config.xsd)

set(APP_DATA
	${APP_PROJ0}
	${APP_PROJ1}
	${APP_HELP}
	${APP_ETC}
	${APP_CONFIG}
)

if (APPLE)
	include_directories(${OGRELITE_SOURCE_DIR}/OgreMain/include/OSX)
	SET(EXETYPE MACOSX_BUNDLE)
	SET_SOURCE_FILES_PROPERTIES(MainMenu.nib  PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
	SET (NibFile MainMenu.nib)

	foreach (f ${APP_DATA})
		SET_SOURCE_FILES_PROPERTIES(${f}  PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
	endforeach()
		
endif()

set(OK_UTILS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../EmbedDemo)

set(LIB_SRC
	Lib/liTypes.cpp
	Lib/liTypes.h
	Lib/liUtils.cpp
	Lib/liUtils.h
	Lib/liXmlConfig.cpp
	Lib/liXmlConfig.h
	Lib/liXmlUtils.cpp
	Lib/liXmlUtils.h
	#Lib/liBlendFile.cpp
	#Lib/liBlendFile.h
	Lib/liLuaUtils.cpp
	Lib/liLuaUtils.h
	Lib/liOgreUtils.h
	Lib/liOgreUtils.cpp
)

set(APP_SRC
	luMainApp.cpp
	luMainFrame.cpp
	luLog.cpp	
	luProjPanel.cpp
	luPropsPanel.cpp
	luProps.cpp
	luUtils.cpp
	luHelp.cpp
	luEdit.cpp
	luFile.cpp
	luProcess.cpp
	luWizard.cpp
	luConfig.cpp
	luFindRepDlg.cpp
	luDefs.cpp
	luOutputs.cpp
	luAnimDlg.cpp
	luStateFrame.cpp	
	${OK_UTILS_PATH}/okApp.cpp
	${OK_UTILS_PATH}/okWin.cpp
	${OK_UTILS_PATH}/okUtils.cpp
	${OK_UTILS_PATH}/okCamera.cpp
	StdAfx.cpp
)

set(APP_HDR
	luMainApp.h
	luMainFrame.h
	luLog.h
	luProjPanel.h
	luPropsPanel.h
	luProps.h
	luUtils.h
	luHelp.h
	luEdit.h
	luDefs.h
	luFile.h
	luProcess.h
	luWizard.h
	luConfig.h
	luFindRepDlg.h
	luOutputs.h
	luAnimDlg.h
	luStateFrame.h
	${OK_UTILS_PATH}/okApp.h
	${OK_UTILS_PATH}/okWin.h
	${OK_UTILS_PATH}/okUtils.h
	${OK_UTILS_PATH}/okCamera.h
	StdAfx.h
)


if (WIN32)
	set(PLAT_RESOURCE Win32/app.rc)
endif()

set(RESOURCE 
)

set_source_files_properties(${RESOURCE} PROPERTIES HEADER_FILE_ONLY 1)
source_group(Content\\Resources FILES ${PLAT_RESOURCE} ${RESOURCE})
source_group("Library Files" FILES ${LIB_SRC})
source_group("Data Files" FILES ${APP_DATA})

set(ALL
	${APP_SRC}
	${APP_HDR}
	${LIB_SRC}
	${RESOURCE}
	${PLAT_RESOURCE}
	${APP_DATA}
)

include_directories(
	. 
	#${GAMEKIT_UTILS_PATH} 
	${OGREKIT_INCLUDE}
	${OK_UTILS_PATH}
	${OGREKIT_TINYXML_INCLUDE}
)

link_libraries(
	#${GAMEKIT_UTILS_TARGET} 
	${OGREKIT_LIB}
	${OGREKIT_TINYXML_TARGET}
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
		ADD_CUSTOM_COMMAND( TARGET ${PROJECT_NAME}  POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/Data)
		ADD_CUSTOM_COMMAND( TARGET ${PROJECT_NAME}  POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/Help)
		ADD_CUSTOM_COMMAND( TARGET ${PROJECT_NAME}  POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/Config)
		ADD_CUSTOM_COMMAND( TARGET ${PROJECT_NAME}  POST_BUILD COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/Work)
		foreach (f ${APP_DATA})
			ADD_CUSTOM_COMMAND(
				TARGET ${PROJECT_NAME} 
				POST_BUILD
				COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/${f} ${CMAKE_CURRENT_BINARY_DIR}/${f}
			)
		endforeach()
	ENDIF ()
endif()
