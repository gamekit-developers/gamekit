#-------------------------------------------------------------------
# 
# Setup script for linking wxALL and wxSupport 
# 
# use: 
# setup_wx()
# add_executable(Some_wxWidgets_app ... )
#-------------------------------------------------------------------
set(WX_VERSION wxWidgets-2.9.1)


macro(configure_wx_build ROOT)

	list(APPEND CMAKE_MODULE_PATH ${ROOT}/${WX_VERSION}/wxCMake)

	option(wxWidgets_DEBUG "Enable / Disable debug mode (__WXDEBUG__)" OFF)
	option(wxWidgets_PRECOMP "Enable / Disable pre compiled headers under __WXMSW__" ON)
	option(wxWidgets_GROUP_HEADERS "Sort headers into groups for the current IDE " OFF)
	

endmacro(configure_wx_build)




macro(setup_wx)
	include(wxSystem)

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


endmacro(setup_wx)
