
file(GLOB fi_SRCS "*.cpp")
file(GLOB fi_HDRS "*.h")

include_directories(${FI_INCLUDE} )
add_library(FreeImageToolkit ${fi_SRCS} ${fi_HDRS})
