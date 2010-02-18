# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.4)

if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)
    
# ---------------------------------------------------------
project(bAscii)
set(bAscii_VERSION 1.0)


if (NOT BPARSE_EXTERN_DIR)
	set(BPARSE_EXTERN_DIR ${CMAKE_SOURCE_DIR}/Core/bParse)
endif()


# ---------------------------------------------------------
subdirs(bAscii)
#subdirs(blen2Ascii)

