# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)


file(GLOB fi_SRCS "*.cpp")
file(GLOB fi_HDRS "*.h")

include_directories(${FI_INCLUDE})
link_libraries(LibJPEG LibMNG LibOpenJPEG LibPNG OpenEXR LibTIFF MetaData Zlib FreeImageToolkit)
add_library(FreeImage SHARED ${fi_SRCS} ${fi_HDRS})

install(TARGETS FreeImage 
        RUNTIME DESTINATION .
        )
