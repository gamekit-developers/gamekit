# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)


include(PrecompiledHeader)

project(OgreLite)
set(OgreLite 1.0)

set(OGRE_THREAD_PROVIDER 0)
set(OGRE_LIB_TYPE ${OGREKIT_LIB_TYPE})

include_directories(${OGRELITE_SOURCE_DIR}/OgreMain/include)
include_directories(${OGRELITE_SOURCE_DIR}/Settings)

subdirs(OgreMain)
subdirs(RenderSystems)

