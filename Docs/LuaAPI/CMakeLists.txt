
# ---------------------------------------------------------
include(FindDoxygen)


file(GLOB_RECURSE SRC Source/*.inl Source/*.h)



if (DOXYGEN_FOUND)
	add_custom_target(LuaApiManual
		COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.txt
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} 
		COMMENT "Building OgreKit Documentation" 
		SOURCES ${SRC}
	)
endif()