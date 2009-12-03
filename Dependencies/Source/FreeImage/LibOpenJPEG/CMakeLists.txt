
file(GLOB fi_SRCS "*.c")
file(GLOB fi_HDRS "*.h")

include_directories(${FI_INCLUDE} )
add_library(LibOpenJPEG ${fi_SRCS} ${fi_HDRS})
