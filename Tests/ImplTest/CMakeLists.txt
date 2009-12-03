# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)



set(SRC Main.cpp)
include_directories(
    ${OGREKIT_OGRE_INCLUDE}
    ${OGREKIT_SOURCE_DIR}/Core/Engine
)

link_libraries(
    OgreKitCore 
    ${OGREKIT_OGRE_LIBS}
)

add_executable(TestApp ${SRC})
install(TARGETS TestApp RUNTIME DESTINATION ${OGREKIT_INSTALL_PREFIX})
