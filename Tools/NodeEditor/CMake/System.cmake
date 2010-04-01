if (MSVC)
    # disable msvc specific warnings
	add_definitions( -D_CRT_SECURE_NO_WARNINGS )
	add_definitions( -D_CRT_SECURE_NO_DEPRECATE )
	add_definitions( -D_SCL_SECURE_NO_WARNINGS )
endif()


find_package(OpenGL REQUIRED)

macro(SETUP_WX)
    include(wxSystem)
    include_directories(${wxROOT_INC} ${wxROOT_INC}/setup)
    link_libraries( wxAll wxSupport ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} )
endmacro(SETUP_WX)
