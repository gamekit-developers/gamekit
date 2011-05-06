file(GLOB SOURCE *.cpp *.h)
include_directories(${GAMEKIT_UTILS_PATH})
add_library(${GAMEKIT_ANIMKIT_TARGET} ${SOURCE})
config_target_if_iphone_build(${GAMEKIT_ANIMKIT_TARGET} TRUE)
