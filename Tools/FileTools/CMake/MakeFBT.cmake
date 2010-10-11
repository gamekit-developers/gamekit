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
set(FBT_EXECUTABLE makefbt)
# ----------------------------------------------------------------------------#
#                                                                             #
# Get a list of absolute paths                                                #
#                                                                             #
# ----------------------------------------------------------------------------#
macro(FBT_ABSOLUTE_SRC TARGET)
    foreach (it ${ARGN})
        get_filename_component(N ${it} ABSOLUTE)
        set(${TARGET} ${${TARGET}} ${N})
    endforeach(it)
endmacro(FBT_ABSOLUTE_SRC)

# ----------------------------------------------------------------------------#
#                                                                             #
# Get a list of names paths                                                   #
#                                                                             #
# ----------------------------------------------------------------------------#
macro(FBT_BASE_SRC TARGET)
    foreach (it ${ARGN})
        get_filename_component(N ${it} NAME)
        set(${TARGET} ${${TARGET}} ${N})
    endforeach(it)
endmacro(FBT_BASE_SRC)
# ----------------------------------------------------------------------------#
#                                                                             #
#   Usage: ${FBT_EXECUTABLE} ${OUTFILE} {ARGN}                                #
#                                                                             #
# ----------------------------------------------------------------------------#
macro(ADD_FBT TARGET)
    
    set(SRC_FILES )
    set(BASE_FILES )
    set(OUTFILE )

    fbt_absolute_src(SRC_FILES ${ARGN})
    fbt_base_src(BASE_FILES ${ARGN})

    get_filename_component(TARNAME ${TARGET} NAME)
    set(OUTFILE ${CMAKE_CURRENT_BINARY_DIR}/${TARNAME}.cpp)


    add_custom_command(
	    OUTPUT ${OUTFILE}
	    COMMAND ${FBT_EXECUTABLE} ${TARNAME} ${OUTFILE} ${SRC_FILES}
	    DEPENDS ${FBT_EXECUTABLE} ${SRC_FILES}
	    COMMENT "${BASE_FILES}"
	    )
    set(${TARGET} ${OUTFILE})
endmacro(ADD_FBT)

# ----------------------------------------------------------------------------#
#                                                                             #
#   Usage: ${FBT_EXECUTABLE} ${OUTFILE} {ARGN}                                #
#          Writes a validation target                                         #
#                                                                             #
# ----------------------------------------------------------------------------#
macro(ADD_FBT_VALIDATOR TARGET)
    
    set(SRC_FILES )
    set(BASE_FILES )
    set(OUTFILE )

    fbt_absolute_src(SRC_FILES ${ARGN})
    fbt_base_src(BASE_FILES ${ARGN})

    get_filename_component(TARNAME ${TARGET} NAME)
    set(OUTFILE  ${CMAKE_CURRENT_BINARY_DIR}/${TARNAME}.cpp)
    set(OUTFILEV ${CMAKE_CURRENT_BINARY_DIR}/${TARNAME}Validator.cpp)


    add_custom_command(
	    OUTPUT ${OUTFILE} ${OUTFILEV}
	    COMMAND ${FBT_EXECUTABLE} ${TARNAME} ${OUTFILE} ${SRC_FILES}
	    DEPENDS ${FBT_EXECUTABLE} ${SRC_FILES}
	    COMMENT "${BASE_FILES}"
	    )


    add_executable(${TARNAME}Validator ${OUTFILEV})

    add_custom_command(
        TARGET ${TARNAME}Validator
        POST_BUILD
	    COMMAND ${TARNAME}Validator
	    COMMENT "Validating ${OUTFILE}"
	    )


    set(${TARGET} ${OUTFILE})
endmacro(ADD_FBT_VALIDATOR)

