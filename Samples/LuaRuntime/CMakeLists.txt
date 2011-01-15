# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

set(CONTENT 
	Test0.blend
	Test1.blend
	Test2.blend
	Test0.lua
	Test1.lua
)


if (APPLE)
	include_directories(${OGRELITE_SOURCE_DIR}/OgreMain/include/OSX)
	SET(EXETYPE MACOSX_BUNDLE)
	SET_SOURCE_FILES_PROPERTIES( MainMenu.nib  PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
	SET_SOURCE_FILES_PROPERTIES(${CONTENT} PROPERTIES MACOSX_PACKAGE_LOCATION Resources )
	SET (NibFile MainMenu.nib)
endif()

set(SRC Lua.cpp ${CONTENT})
include_directories(
	${OGREKIT_INCLUDE}
	${OGREKIT_OPTS_INCLUDE}
	)
link_libraries(${OGREKIT_LIB} ${OGREKIT_OPTS_TARGET})


if (APPLE)

	add_executable(
		AppLuaRuntime 
		MACOSX_BUNDLE
		MainMenu.nib
		${SRC} 
		${HiddenCMakeLists}
	)

else ()

	add_executable(AppLuaRuntime ${SRC} ${HiddenCMakeLists})
	
	
	IF (NOT INTERNAL_CREATE_DISTRIBUTABLE_MSVC_PROJECTFILES)
		ADD_CUSTOM_COMMAND(
			TARGET AppLuaRuntime 
			POST_BUILD
			COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/Test0.blend ${CMAKE_CURRENT_BINARY_DIR}/Test0.blend
			COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/Test1.blend ${CMAKE_CURRENT_BINARY_DIR}/Test1.blend
			COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/Test2.blend ${CMAKE_CURRENT_BINARY_DIR}/Test2.blend
			COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/Test0.lua   ${CMAKE_CURRENT_BINARY_DIR}/Test0.lua
			COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/Test1.lua   ${CMAKE_CURRENT_BINARY_DIR}/Test1.lua
		)
	ENDIF ()

endif()
