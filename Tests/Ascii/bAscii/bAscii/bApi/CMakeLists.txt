

set(SRC Main.cpp)


include_directories(${BPARSE_EXTERN_DIR})
link_libraries(${BPARSE_EXTERN_LIBS})
add_executable(bApi ${SRC} ${HDR})
