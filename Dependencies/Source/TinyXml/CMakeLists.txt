# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)

project(TinyXml)
SET(${PROJECT_NAME} 1.0)

set(LIB_SRC 
	tinyxmlparser.cpp
	tinyxmlerror.cpp
	tinyxml.cpp
)

set(LIB_HDR
	tinyxml.h
)

add_library(${PROJECT_NAME} ${LIB_HDR} ${LIB_SRC})

config_ogrekit_target(${PROJECT_NAME} TRUE)
