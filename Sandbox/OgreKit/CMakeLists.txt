
set(OGREKIT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(OGREKIT_INCLUDE 
    ${OGREKIT_ROOT}/Common
    ${OGREKIT_ROOT}/Common/Loader
    ${OGREKIT_ROOT}/Common/Loader/Blender
    ${OGREKIT_ROOT}/Common/Utils
)

subdirs(Common)
subdirs(TestDemo)
subdirs(Momo)
