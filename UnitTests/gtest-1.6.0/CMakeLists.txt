# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

project(gtest)
SET(${PROJECT_NAME} 1.0)

set(LIB_SRC 
	src/gtest-all.cc
	src/gtest_main.cc
)

set(LIB_HDR
	include/gtest/gtest.h
)

include_directories(
	${GTEST_INCLUDE}
)

add_library(${PROJECT_NAME} STATIC
	${LIB_HDR} ${LIB_SRC})

#config_ogrekit_target(${PROJECT_NAME} TRUE)
