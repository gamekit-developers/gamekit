cmake_minimum_required(VERSION 2.4)


if(COMMAND cmake_policy)
	cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)
	
project(HeaderGenerator)

if (MSVC)
	add_definitions( -D_CRT_SECURE_NO_WARNINGS )
	add_definitions( -D_CRT_SECURE_NO_DEPRECATE )
endif()

set(SRC Main.cpp)

add_executable(HeaderGenerator ${SRC})
