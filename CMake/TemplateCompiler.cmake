# ----------------------------------------------------------------------------#
#                                                                             #
#  Compile time template generator                                            #
#   Compile N source files to C strings                                       #
#   Usage: ${TARGET} ${OUTFILE} ${SOURCE0} ... ${SOURCE[N]}                   #
#                                                                             #
# ----------------------------------------------------------------------------#
subdirs(${OGREKIT_SOURCE_DIR}/Tools/TemplateCompiler)
set(TCL_EXECUTABLE tcl)
# ----------------------------------------------------------------------------#
#                                                                             #
# Get a list of absolute paths                                                #
#                                                                             #
# ----------------------------------------------------------------------------#
macro(TMPL_ABSOLUTE_SRC OUT)
    foreach (it ${ARGN})
        get_filename_component(N ${it} ABSOLUTE)
        set(${OUT} ${${OUT}} ${N})
    endforeach(it)
endmacro(TMPL_ABSOLUTE_SRC)
# ----------------------------------------------------------------------------#
#                                                                             #
#   Usage: ${TCL_EXECUTABLE} ${OUTFILE} {ARGN}                                #
#                                                                             #
# ----------------------------------------------------------------------------#
macro(ADD_TEMPLATES BINARY OUT GENERATED)
		
    get_filename_component(OUTFILE ${GENERATED} ABSOLUTE)
    tmpl_absolute_src(TEMPLATES ${ARGN})
    get_filename_component(ONAME ${OUTFILE} NAME)

	if (${BINARY})
		set(TCL_CMD ${TCL_EXECUTABLE} -b ${OUTFILE} ${TEMPLATES})
	else()
		set(TCL_CMD ${TCL_EXECUTABLE} ${OUTFILE} ${TEMPLATES})
	endif()	
	
    add_custom_command(
	    OUTPUT ${OUTFILE}
	    COMMAND ${TCL_CMD}
	    DEPENDS ${TCL_EXECUTABLE} ${TEMPLATES}
	    COMMENT "Building ${ONAME}"
	    )
    
	set(${OUT} ${OUTFILE})
	set(TEMPLATES)	
	
endmacro(ADD_TEMPLATES)
