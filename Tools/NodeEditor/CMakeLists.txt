#----------------------------------------------------------
cmake_minimum_required(VERSION 2.4)

#----------------------------------------------------------
if(COMMAND cmake_policy)
   cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project(Nodes)
set(Nodes 1.0)

#----------------------------------------------------------
set(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/CMake
    ${CMAKE_SOURCE_DIR}/wxWidgets-2.9.0/wxCMake

    )
set(wxWidgets_SOURCE_DIR            ${Nodes_SOURCE_DIR}/wxWidgets-2.9.0)
include(System)

option(wxWidgets_PRECOMP            "Enable / Disable pre compiled headers under __WXMSW__"     ON)
option(wxWidgets_DEBUG              "Enable / Disable debug mode (__WXDEBUG__)"                 OFF)
option(wxWidgets_GROUP_HEADERS      "Sort headers into groups for the current IDE "             OFF)


subdirs(${wxWidgets_SOURCE_DIR}/wxCMake Editor)