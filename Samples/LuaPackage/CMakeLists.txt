# ---------------------------------------------------------
include(ConfigApp)
add_ogrekit_module(OgreKitLua   Main.cpp)
ogrekit_copy_target(OgreKitLua  ${CMAKE_CURRENT_SOURCE_DIR})
