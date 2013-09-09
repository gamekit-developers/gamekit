file(GLOB SOURCE *.cpp *.h)
include_directories(${OGREKIT_ZLIB_INCLUDE})
add_library(${GAMEKIT_UTILS_TARGET} ${SOURCE})
config_ogrekit_target(${GAMEKIT_UTILS_TARGET} TRUE)