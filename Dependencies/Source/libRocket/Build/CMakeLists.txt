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
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

#set(CMAKE_DEBUG_POSTFIX  "_d")

#===================================
# Build options ====================
#===================================

# if(NOT CMAKE_BUILD_TYPE)
    # set(CMAKE_BUILD_TYPE Release CACHE STRING
        # "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
        # FORCE)
# endif()

#option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(LIBROCKET_PYTHON_BINDINGS "Build python bindings" OFF)
option(LIBROCKET_LUA_BINDINGS "Build Lua bindings" ON)
#option(BUILD_SAMPLES "Build samples" OFF)

add_definitions(-DSTATIC_LIB)

#on windows, check for VC10 and fix the multiple compile target issue.
IF(WIN32)
  if(MSVC)
    if(${MSVC_VERSION} STREQUAL 1600)  #OR ${MSVC_VERSION} STRGREATER 1600)
      message("Visual Studio 2010 (${MSVC_VERSION}) build fix at play (/FORCE:MULTIPLE)")
      set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /FORCE:MULTIPLE")
    endif()
  endif() 
ENDIF(WIN32)


#===================================
# Find dependencies ================
#===================================

include_directories(${OGREKIT_FREETYPE_INCLUDE})

# Boost and Python
if(LIBROCKET_PYTHON_BINDINGS)
    find_package(PythonInterp REQUIRED)
    find_package(PythonLibs REQUIRED)
    execute_process(
        COMMAND ${PYTHON_EXECUTABLE} -c "from distutils import sysconfig; print(sysconfig.get_python_lib(1,0,prefix=''))"
        OUTPUT_VARIABLE PYTHON_INSTDIR
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(PYTHONLIBS_FOUND)
        include_directories(${PYTHON_INCLUDE_DIR})
    endif()

    #set(Boost_USE_STATIC_LIBS OFF)
    #set(Boost_USE_MULTITHREADED ON)
    find_package(Boost 1.40.0 COMPONENTS python REQUIRED)
    if(Boost_FOUND)
        include_directories(${Boost_INCLUDE_DIR})
        list(APPEND PY_BINDINGS_LINK_LIBS ${Boost_LIBRARIES})
    endif()

endif()

#Lua
if(LIBROCKET_LUA_BINDINGS)    
	include_directories(${OGREKIT_LUA_INCLUDE})
	#list(APPEND LUA_BINDINGS_LINK_LIBS ${OGREKIT_LUA_TARGET})
endif()


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

    add_library(${NAME} STATIC ${${library}_SRC_FILES}
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

# Build python bindings
if(LIBROCKET_PYTHON_BINDINGS)
    set(LIBRARIES core controls)

    foreach(library ${LIBRARIES})
        set(NAME _rocket${library})

        add_library(${NAME} STATIC ${Py${library}_SRC_FILES}
                                   ${Py${library}_HDR_FILES}
                                   ${Py${library}_PUB_HDR_FILES}
        )

        set_target_properties(${NAME} PROPERTIES PREFIX "")

    endforeach(library)
endif()

# Build Lua bindings
if(LIBROCKET_LUA_BINDINGS)
    set(LIBRARIES Core Controls)

    foreach(library ${LIBRARIES})
        set(NAME Rocket${library}Lua)

        add_library(${NAME} STATIC ${Lua${library}_SRC_FILES}
                            ${Lua${library}_HDR_FILES}
                            ${Lua${library}_PUB_HDR_FILES}
        )

        set_target_properties(${NAME} PROPERTIES
                           VERSION ${PROJECT_VERSION}
                           SOVERSION ${LIBROCKET_VERSION_MAJOR}
        )
    endforeach(library)
endif()

#===================================
# Link libraries ===================
#===================================

#target_link_libraries(RocketCore ${CORE_LINK_LIBS})
#target_link_libraries(RocketControls RocketCore)
#target_link_libraries(RocketDebugger RocketCore)

if(LIBROCKET_PYTHON_BINDINGS)
    target_link_libraries(_rocketcore RocketCore ${PY_BINDINGS_LINK_LIBS})
    target_link_libraries(_rocketcontrols RocketControls ${PY_BINDINGS_LINK_LIBS})
endif()

if(LIBROCKET_LUA_BINDINGS)
    target_link_libraries(RocketCoreLua RocketCore ${LUA_BINDINGS_LINK_LIBS})
    target_link_libraries(RocketControlsLua RocketControls RocketCoreLua ${LUA_BINDINGS_LINK_LIBS})
endif()



config_ogrekit_target(RocketCore TRUE)
config_ogrekit_target(RocketControls TRUE)
config_ogrekit_target(RocketDebugger TRUE)

if(LIBROCKET_LUA_BINDINGS)
	config_ogrekit_target(RocketCoreLua TRUE)
	config_ogrekit_target(RocketControlsLua TRUE)
endif()
