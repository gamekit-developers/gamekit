
file(GLOB fi_SRCS "*.c")
file(GLOB fi_HDRS "*.h")

include_directories(${FI_INCLUDE} )
add_library(${OGREKIT_ZLIB_TARGET} ${fi_SRCS} ${fi_HDRS})
