if (MSVC)
    # disable msvc specific warnings
	add_definitions( -D_CRT_SECURE_NO_WARNINGS )
	add_definitions( -D_CRT_SECURE_NO_DEPRECATE )
	add_definitions( -D_SCL_SECURE_NO_WARNINGS )
endif()


find_package(OpenGL REQUIRED)

if (UNIX AND NOT APPLE)

    # will not work without
    find_package(GTK2 REQUIRED)


    set(wxGTK2_PATHS
        ${GTK2_gtk_INCLUDE_PATH}
        ${GTK2_gdk_INCLUDE_PATH}
        ${GTK2_glibconfig_INCLUDE_PATH}
        ${GTK2_pango_INCLUDE_PATH}
        ${GTK2_glib_INCLUDE_PATH}
        ${GTK2_atk_INCLUDE_PATH}
        ${GTK2_cairo_INCLUDE_PATH}
        )
    set(wxGTK2_LIBS
        ${GTK2_atk_LIBRARY}
        ${GTK2_cairo_LIBRARY}
        ${GTK2_gdk_LIBRARY}
        ${GTK2_glib_LIBRARY}
        ${GTK2_gtk_LIBRARY}
        ${GTK2_pango_LIBRARY}
        ${GTK2_gthread_LIBRARY}
        ${X11_SM_LIB}
        )

    set(wxGTK2_PREPROCESSOR
        -DUSE_UNIX_SETUP
        -D__WXGTK__
        -D__WXGTK20__=1
	-D_UNICODE
	-DUNICODE
        )
endif()

include(wxSystem)

macro(SETUP_WX)
    include_directories(${wxALL_INC})

    if (WIN32)
        link_libraries( wxAll wxSupport ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} )
    else()
        if (APPLE)
            add_definitions(-D__WXCOCOA__)
            ## XXX link_libraries(May need some extra system libs here)
        else()
            include_directories(${wxGTK2_PATHS})
            link_libraries( wxAll wxSupport ${wxGTK2_LIBS} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} )
            add_definitions(${wxGTK2_PREPROCESSOR})
        endif()
    endif()

endmacro(SETUP_WX)
