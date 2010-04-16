#----------------------------------------------------------
cmake_minimum_required(VERSION 2.4)

#----------------------------------------------------------
if(COMMAND cmake_policy)
   cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project(Nodes)
set(Nodes 1.0)

set(PARENT_SOURCE_DIR  ${Nodes_SOURCE_DIR}/../../)

#----------------------------------------------------------
set(CMAKE_MODULE_PATH
    ${PARENT_SOURCE_DIR}/CMake
    ${PARENT_SOURCE_DIR}/CMake
    ${PARENT_SOURCE_DIR}/CMake/Utils
    ${PARENT_SOURCE_DIR}/CMake/Packages
    ${PARENT_SOURCE_DIR}/CMake/Templates
    ${CMAKE_SOURCE_DIR}/CMake
    ${CMAKE_SOURCE_DIR}/wxWidgets-2.9.0/wxCMake
    )
set(wxWidgets_SOURCE_DIR            ${Nodes_SOURCE_DIR}/wxWidgets-2.9.0)
include(System)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)
include(ConfigOgreKit)
include(ConfigGameKit)
include(FindOpenAL)

option(wxWidgets_PRECOMP            "Enable / Disable pre compiled headers under __WXMSW__"     ON)
option(wxWidgets_DEBUG              "Enable / Disable debug mode (__WXDEBUG__)"                 OFF)
option(wxWidgets_GROUP_HEADERS      "Sort headers into groups for the current IDE "             OFF)


configure_gamekit(${PARENT_SOURCE_DIR}/bullet/Extras)
configure_ogrekit(${PARENT_SOURCE_DIR} ${PARENT_SOURCE_DIR}/OgreLite)


set(OGREKIT_BULLET_LIBS BulletDynamics BulletCollision BulletSoftBody LinearMath)
set(OGREKIT_BULLET_INCLUDE ${PARENT_SOURCE_DIR}/bullet/src)
set(OGREKIT_INCLUDE ${PARENT_SOURCE_DIR}/Engine ${OGREKIT_OGRE_INCLUDE} ${OGREKIT_BULLET_INCLUDE})

subdirs(${PARENT_SOURCE_DIR}/bullet)
subdirs(${PARENT_SOURCE_DIR}/Dependencies)
subdirs(${PARENT_SOURCE_DIR}/OgreLite)
subdirs(${PARENT_SOURCE_DIR}/Engine)
subdirs( ${wxWidgets_SOURCE_DIR}/wxCMake Editor)
