# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)



set(ZZIP_SRC zzip/dir.c zzip/fetch.c zzip/err.c zzip/file.c zzip/info.c zzip/plugin.c zzip/stat.c zzip/write.c zzip/zip.c)
file(GLOB ZZIP_HDR "zzip/*.h")

include_directories(./ zzip ${OGREKIT_ZLIB_INCLUDE})
add_library(${OGREKIT_ZZIP_TARGET} ${ZZIP_SRC} ${ZZIP_HDR})

config_target_if_iphone_build(${OGREKIT_ZZIP_TARGET} TRUE)
