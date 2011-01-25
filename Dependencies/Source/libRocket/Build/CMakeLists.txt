#===================================
# Build script for libRocket =======
#===================================

if(APPLE)
# We use the new OSX_ARCHITECTURES property
cmake_minimum_required(VERSION 2.8.1)
else()
cmake_minimum_required(VERSION 2.6)
endif()

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0015 NEW)
endif(COMMAND cmake_policy)

project(libRocket C CXX)

set(LIBROCKET_VERSION_MAJOR 1)
set(LIBROCKET_VERSION_MINOR 2)
set(LIBROCKET_VERSION_PATCH 1)
set(PROJECT_VERSION ${LIBROCKET_VERSION_MAJOR}.${LIBROCKET_VERSION_MINOR}.${LIBROCKET_VERSION_PATCH})

# Search in the 'cmake' directory for additional CMake modules.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)


#===================================
# Environment tests ================
#===================================

include(TestForANSIForScope)
include(TestForANSIStreamHeaders)
include(TestForSTDNamespace)


#===================================
# Build options ====================
#===================================

add_definitions(-DSTATIC_LIB)

#===================================
# Find dependencies ================
#===================================

include_directories(${OGREKIT_FREETYPE_INCLUDE})
#message(${OGREKIT_FREETYPE_INCLUDE})


#===================================
# Setup paths ======================
#===================================

set(PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/..)

include_directories(
    ${PROJECT_SOURCE_DIR}/Include
)

# Include list of source files
include(FileList)

#===================================
# Build libraries ==================
#===================================

set(LIBRARIES Core Controls Debugger)

foreach(library ${LIBRARIES})
    set(NAME Rocket${library})

    add_library(${NAME} ${${library}_SRC_FILES}
                        ${${library}_HDR_FILES}
                        ${${library}_PUB_HDR_FILES}
    )

    set_target_properties(${NAME} PROPERTIES
                           VERSION ${PROJECT_VERSION}
                           SOVERSION ${LIBROCKET_VERSION_MAJOR}
    )
    
    if(APPLE)
    	set_target_properties(${NAME} PROPERTIES
    					OSX_ARCHITECTURES "i386;x86_64;"
    	)
    endif()

    install(TARGETS ${NAME}
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
            RUNTIME DESTINATION bin
    )
endforeach(library)

#===================================
# Link libraries ===================
#===================================

#target_link_libraries(RocketCore ${CORE_LINK_LIBS})
#target_link_libraries(RocketControls RocketCore)
#target_link_libraries(RocketDebugger RocketCore)


config_target_if_iphone_build(RocketCore TRUE)
config_target_if_iphone_build(RocketControls TRUE)
config_target_if_iphone_build(RocketDebugger TRUE)