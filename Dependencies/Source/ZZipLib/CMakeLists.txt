# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)



set(ZZIP_SRC zzip/dir.c zzip/fetch.c zzip/err.c zzip/file.c zzip/info.c zzip/plugin.c zzip/stat.c zzip/write.c zzip/zip.c)
file(GLOB ZZIP_HDR "zzip/*.h")

include_directories(./ zzip ${OGREKIT_ZLIB_INCLUDE})
add_library(${OGREKIT_ZZIP_TARGET} ${ZZIP_SRC} ${ZZIP_HDR})

config_ogrekit_target(${OGREKIT_ZZIP_TARGET} TRUE)


if (OGREKIT_CREATE_OGRE_DEPENDENCY_DIR)
	file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/zzip/ DESTINATION ${OGRE_PREBUILT_DEPENDENCY_INCLUDE_DIR}/zzip PATTERN "*.h")
	GET_TARGET_PROPERTY(FILEPATH ${OGREKIT_ZZIP_TARGET} LOCATION)
	ADD_CUSTOM_COMMAND(TARGET ${OGREKIT_ZZIP_TARGET} POST_BUILD
		COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${FILEPATH} ${OGRE_PREBUILT_DEPENDENCY_LIB_DIR}
		COMMAND mv ${OGRE_PREBUILT_DEPENDENCY_LIB_DIR}/libZZipLib.a ${OGRE_PREBUILT_DEPENDENCY_LIB_DIR}/libzzip.a
	)
endif()
