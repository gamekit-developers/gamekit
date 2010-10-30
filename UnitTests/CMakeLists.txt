cmake_minimum_required(VERSION 2.4)

list(APPEND CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/CMake
)

#setup gtest
include(ConfigGTest)
setup_gtest()

subdirs(${GTEST_DIR})

subdirs(OgreKitUnitTests)
subdirs(FbtUnitTests)

if (SAMPLES_LUA_EDITOR)
subdirs(LuaEditorUnitTests)
endif()
