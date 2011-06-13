#------------------------------------------------------------------------------
#   This file is part of FBT (File Binary Tables).
#   http://gamekit.googlecode.com/
#
#   Copyright (c) 2010 Charlie C & Erwin Coumans.
#
#------------------------------------------------------------------------------
# This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgment in the product documentation would be
#    appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.
#------------------------------------------------------------------------------
include(FindFLEX)
include(FindBISON)



macro(FB_SOURCE_ABSOLUTE_SRC OUT)
    foreach (it ${ARGN})
        get_filename_component(N ${it} ABSOLUTE)
        set(${OUT} ${${OUT}} ${N})
    endforeach(it)
endmacro(FB_SOURCE_ABSOLUTE_SRC)


macro(ADD_FLEX_BISON OUT DEPS FLEX_SRC BISON_SRC PREFIX)

    fb_source_absolute_src(ABS_DEPS ${${DEPS}})

    get_filename_component(FSA ${FLEX_SRC} ABSOLUTE)
    get_filename_component(BSA ${BISON_SRC} ABSOLUTE)

    get_filename_component(FSP ${FSA} PATH)
    get_filename_component(BSP ${BSA} PATH)
    get_filename_component(FSN ${FSA} NAME)
    get_filename_component(BSN ${BSA} NAME)

    string(REPLACE ".l" ".inl" FSD ${FSN})
    string(REPLACE ".y" ".cpp" BSD ${BSN})

    set(FSO "${FSP}/Generated/${FSD}")
    set(BSO "${BSP}/Generated/${BSD}")
    
    set(AFB_DEBUG FALSE)
    
    if (AFB_DEBUG)
   
        message("Flex   = ${FSA}") 
        message("Bison  = ${BSA}") 
        message("ResF   = ${FSO}") 
        message("ResB   = ${BSO}") 
    
    endif() 

    add_custom_command(
        OUTPUT  ${FSO}
        COMMAND ${FLEX_EXECUTABLE} -L -P${PREFIX} -o${FSO} ${FSA}
        DEPENDS ${FSA} ${ABS_DEPS}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "${FLEX_EXECUTABLE}  > ${FSA}"
    )

    add_custom_command(
        OUTPUT  ${BSO}
        COMMAND ${BISON_EXECUTABLE} --no-lines -p ${PREFIX} -o${BSO} ${BSA}
        DEPENDS ${BSA} ${ABS_DEPS} ${FSO}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "${BISON_EXECUTABLE} > ${BSA}"
    )

    source_group("Parser\\Header" FILES ${FSA} ${BSA})
    source_group("Parser\\Source" FILES ${FSO} ${BSO})


    list(APPEND ${OUT} ${BSO} ${FSO} ${BSA} ${FSA})


endmacro(ADD_FLEX_BISON)


macro(ADD_FLEX OUT DEPS FLEX_SRC PREFIX)

    fb_source_absolute_src(ABS_DEPS ${${DEPS}})

    get_filename_component(FSA ${FLEX_SRC} ABSOLUTE)
    get_filename_component(FSP ${FSA} PATH)
    get_filename_component(FSN ${FSA} NAME)
    string(REPLACE ".l" ".inl" FSD ${FSN})
    set(FSO "${FSP}/Generated/${FSD}")

    add_custom_command(
        OUTPUT  ${FSO}
        COMMAND ${FLEX_EXECUTABLE} -L -P${PREFIX} -o${FSO} ${FSA}
        DEPENDS ${FSA} ${ABS_DEPS}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "${FLEX_EXECUTABLE}  > ${FSA}"
    )

    source_group("Parser\\Header" FILES ${FSA})
    source_group("Parser\\Source" FILES ${FSO})

    list(APPEND ${OUT} ${FSO} ${FSA})

endmacro(ADD_FLEX)
