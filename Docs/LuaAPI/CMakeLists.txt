
# ---------------------------------------------------------
include(FindDoxygen)


file(GLOB_RECURSE SRC Source/*.inl Source/*.h)

set(OGREKIT_DOC_IMAGES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Images)

set(OGREKIT_DOC_IMAGES 
	${CMAKE_CURRENT_SOURCE_DIR}/bg_t.gif
	${CMAKE_CURRENT_SOURCE_DIR}/top_bg.gif
	${CMAKE_CURRENT_SOURCE_DIR}/top__sm.gif
	)

if (DOXYGEN_FOUND)

	set(CPCMD ${CMAKE_COMMAND} -E copy_directory ${OGREKIT_DOC_IMAGES_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/local/html)
	
	add_custom_target(LuaApiManual
		COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.txt && ${CPCMD}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} 
		COMMENT "Building OgreKit Lua Documentation" 
		SOURCES ${SRC}
	)
endif()
