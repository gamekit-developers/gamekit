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
		cmake_policy(SET CMP0017 NEW)
		find_package(GTK2 REQUIRED)
		_GTK2_FIND_LIBRARY(GTK2_GTHREAD_LIBRARY gthread false true)
		_GTK2_FIND_LIBRARY(GTK2_PANGOCAIRO_LIBRARY pangocairo false true)

		set(wxGTK2_PATHS
			${GTK2_GTK_INCLUDE_DIR}
			${GTK2_GDK_INCLUDE_DIR}
			${GTK2_GDK_PIXBUF_INCLUDE_DIR}
			${GTK2_GLIBCONFIG_INCLUDE_DIR}
			${GTK2_GDKCONFIG_INCLUDE_DIR}
			${GTK2_PANGO_INCLUDE_DIR}
			${GTK2_GLIB_INCLUDE_DIR}
			${GTK2_ATK_INCLUDE_DIR}
			${GTK2_CAIRO_INCLUDE_DIR}
			)
		set(wxGTK2_LIBS
			${GTK2_ATK_LIBRARY}
			${GTK2_CAIRO_LIBRARY}
			${GTK2_GDK_LIBRARY}
			${GTK2_GDK_PIXBUF_LIBRARY}
			${GTK2_GLIB_LIBRARY}
			${GTK2_GTK_LIBRARY}
			${GTK2_PANGO_LIBRARY}
			${GTK2_GOBJECT_LIBRARY}
			${GTK2_GTHREAD_LIBRARY}
			${GTK2_PANGOCAIRO_LIBRARY}
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
