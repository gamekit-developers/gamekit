# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

project(opts)
SET(${PROJECT_NAME} 1.0)

set(LIB_SRC 
	opts.c
)

set(LIB_HDR
	opts.h
)

add_library(${PROJECT_NAME} ${LIB_HDR} ${LIB_SRC})

config_target_if_iphone_build(${PROJECT_NAME} TRUE)
