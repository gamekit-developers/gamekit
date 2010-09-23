include(wxSource)
include(wxSupport)
include(wxConfig)

#----------------------------------------------------------
# Base source directories
set(wxROOT_SRC ${wxWidgets_SOURCE_DIR}/src)
set(wxROOT_INC ${wxWidgets_SOURCE_DIR}/include)


#----------------------------------------------------------
# Default support directories
set(wxZLIB_INC  ${wxROOT_SRC}/zlib)
set(wxJPEG_INC  ${wxROOT_SRC}/jpeg)
set(wxPNG_INC   ${wxROOT_SRC}/png)
set(wxTIFF_INC  ${wxROOT_SRC}/tiff/libtiff)
set(wxEXPAT_INC ${wxROOT_SRC}/expat/lib)
set(wxALL_INC  ${wxROOT_INC}/setup  ${wxROOT_INC} ${wxZLIB_INC} ${wxJPEG_INC} ${wxPNG_INC} ${wxTIFF_INC} ${wxEXPAT_INC})



if (WIN32)

    if (MSVC)
        add_definitions(/EHa)
        # disable msvc specific warnings
	    add_definitions( -D_CRT_SECURE_NO_WARNINGS )
	    add_definitions( -D_CRT_SECURE_NO_DEPRECATE )
	    add_definitions( -D_SCL_SECURE_NO_WARNINGS )
	else ()
        message("WARNING: This platform is untested and may not currently work")
    endif()


    # Using MSW Toolkit

    set(BASE_PLATFORM_SRC               ${BASE_WIN32_SRC})
    set(BASE_PLATFORM_HDR               ${BASE_WIN32_HDR})
    set(BASE_AND_GUI_PLATFORM_SRC       ${BASE_AND_GUI_WIN32_SRC})
    set(BASE_AND_GUI_PLATFORM_HDR       ${BASE_AND_GUI_WIN32_HDR})

    set(LOWLEVEL_SRC                    ${MSW_LOWLEVEL_SRC} ${MSW_DESKTOP_LOWLEVEL_SRC})
    set(LOWLEVEL_HDR                    ${MSW_LOWLEVEL_HDR} ${MSW_DESKTOP_LOWLEVEL_HDR})

    set(GUI_SRC                         ${MSW_SRC}          ${MSW_DESKTOP_SRC})
    set(GUI_HDR                         ${MSW_HDR}          ${MSW_DESKTOP_HDR})

    set(ADVANCED_PLATFORM_SRC           ${ADVANCED_MSW_SRC} ${ADVANCED_MSW_DESKTOP_SRC})
    set(ADVANCED_PLATFORM_HDR           ${ADVANCED_MSW_HDR} ${ADVANCED_MSW_DESKTOP_HDR})

    set(ADVANCED_PLATFORM_NATIVE_SRC    ${ADVANCED_MSW_NATIVE_SRC})
    set(ADVANCED_PLATFORM_NATIVE_HDR    ${ADVANCED_MSW_NATIVE_HDR})

    set(NET_PLATFORM_SRC                ${NET_WIN32_SRC})
    set(NET_PLATFORM_HDR                ${NET_WIN32_HDR})


else()
    if (APPLE)
        message("WARNING: This platform is untested and may not currently work")

        # using OSX_COCOA Toolkit

        set(BASE_PLATFORM_SRC           ${BASE_OSX_SHARED_SRC} )
        set(BASE_PLATFORM_HDR           ${BASE_OSX_HDR} )


        set(BASE_OSX_SRC                ${BASE_OSX_SHARED_SRC} )
        set(BASE_OSX_HDR                ${BASE_OSX_SHARED_HDR})

        set(BASE_AND_GUI_TOOLKIT_SRC    ${BASE_AND_GUI_OSX_COCOA_SRC})
        set(BASE_AND_GUI_TOOLKIT_HDR    ${BASE_AND_GUI_OSX_COCOA_HDR})

        set(LOWLEVEL_SRC                ${OSX_LOWLEVEL_SRC})
        set(LOWLEVEL_HDR                ${OSX_LOWLEVEL_HDR})

        set(GUI_SRC                     ${OSX_COCOA_SRC})
        set(GUI_HDR                     ${OSX_COCOA_HDR})

        set(PLATFORM_SRC                ${UNIX_SRC})
        set(PLATFORM_HDR                ${UNIX_HDR})

        set(ADVANCED_PLATFORM_SRC       ${ADVANCED_OSX_COCOA_SRC})
        set(ADVANCED_PLATFORM_HDR       ${ADVANCED_OSX_COCOA_HDR})

        set(NET_PLATFORM_SRC            ${NET_UNIX_SRC} ${NET_OSX_SRC})
        set(NET_PLATFORM_HDR            ${NET_UNIX_HDR} ${NET_OSX_HDR})

    else()
        #message("WARNING: This platform is untested and may not currently work")

        # Using GTK2 Toolkit
        set(BASE_PLATFORM_SRC               ${BASE_UNIX_SRC} )
        set(BASE_PLATFORM_HDR               ${BASE_UNIX_HDR} )

        set(LOWLEVEL_SRC                    ${GTK_LOWLEVEL_SRC})
        set(LOWLEVEL_HDR                    ${GTK_LOWLEVEL_HDR})

        set(GUI_SRC                         ${GTK_SRC})
        set(GUI_HDR                         ${GTK_HDR})

        set(PLATFORM_SRC                    ${UNIX_SRC})
        set(PLATFORM_HDR                    ${UNIX_HDR})

        set(ADVANCED_PLATFORM_SRC           ${ADVANCED_UNIX_SRC} ${ADVANCED_GTK_SRC})
        set(ADVANCED_PLATFORM_HDR           ${ADVANCED_UNIX_HDR} ${ADVANCED_GTK_HDR})

        set(ADVANCED_PLATFORM_NATIVE_SRC    ${ADVANCED_GTK_NATIVE_SRC})
        set(ADVANCED_PLATFORM_NATIVE_HDR    ${ADVANCED_GTK_NATIVE_HDR})

        set(NET_PLATFORM_SRC                ${NET_UNIX_SRC})
        set(NET_PLATFORM_HDC                ${NET_UNIX_HDR})

    endif()
endif ()


if (0)

    # Test to make sure headers are found by cmake
    set(FAKE_TEST_OSX
        ${BASE_OSX_SHARED_SRC}
        ${BASE_OSX_SHARED_SRC}
        ${BASE_AND_GUI_OSX_COCOA_SRC}
        ${OSX_LOWLEVEL_SRC}
        ${OSX_COCOA_SRC}
        ${ADVANCED_OSX_COCOA_SRC}
        ${NET_OSX_SRC}
        )

    set(FAKE_TEST_OSX_HDR
        ${BASE_OSX_SHARED_HDR}
        ${BASE_OSX_SHARED_HDR}
        ${BASE_AND_GUI_OSX_COCOA_HDR}
        ${OSX_LOWLEVEL_HDR}
        ${OSX_COCOA_HDR}
        ${ADVANCED_OSX_COCOA_HDR}
        ${NET_OSX_HDR}
        )


    set(FakeSRC ${FAKE_TEST_OSX})
    set(FakeHDR ${FAKE_TEST_OSX_HDR})

 endif()


set(BASE_SRC                ${BASE_CMN_SRC} ${BASE_PLATFORM_SRC} )
set(BASE_AND_GUI_SRC        ${BASE_AND_GUI_CMN_SRC} ${BASE_AND_GUI_PLATFORM_SRC} ${BASE_AND_GUI_TOOLKIT_SRC} )
set(wxBASE_SRC              ${BASE_SRC} ${BASE_AND_GUI_SRC})
set(wxCORE_SRC              ${LOWLEVEL_SRC} ${PLATFORM_SRC} ${GUI_SRC} ${GUI_CMN_SRC})
set(wxADV_SRC               ${ADVANCED_SRC} ${ADVANCED_PLATFORM_NATIVE_SRC} ${ADVANCED_CMN_SRC})
set(wxAUI_SRC               ${AUI_SRC})
set(wxPropGrid_SRC          ${PROPGRID_SRC})
set(wxGLCanvas_SRC          ${OPENGL_SRC})
set(wxXML_SRC               ${XML_SRC})
set(wxNET_SRC               ${NET_CMN_SRC} ${NET_PLATFORM_SRC})


set(wxALL_SRC
    ${wxBASE_SRC}
    ${wxCORE_SRC}
    ${wxADV_SRC}
    ${wxAUI_SRC}
    ${wxPropGrid_SRC}
    ${wxGLCanvas_SRC}
    ${wxXML_SRC}
    ${wxNET_SRC}
    ${FakeSRC}
    )

set(BASE_HDR                ${BASE_CMN_HDR} ${BASE_PLATFORM_HDR} )
set(BASE_AND_GUI_HDR        ${BASE_AND_GUI_CMN_HDR} ${BASE_AND_GUI_PLATFORM_HDR} ${BASE_AND_GUI_TOOLKIT_HDR} )
set(wxBASE_HDR              ${BASE_HDR} ${BASE_AND_GUI_HDR})
set(wxCORE_HDR              ${LOWLEVEL_HDR} ${PLATFORM_HDR} ${GUI_HDR} ${GUI_CMN_HDR})
set(wxADV_HDR               ${ADVANCED_HDR} ${ADVANCED_PLATFORM_NATIVE_HDR} ${ADVANCED_CMN_HDR})
set(wxAUI_HDR               ${AUI_HDR})
set(wxPropGrid_HDR          ${PROPGRID_HDR})
set(wxGLCanvas_HDR          ${OPENGL_HDR})
set(wxXML_HDR               ${XML_HDR})
set(wxNET_HDR               ${NET_CMN_HDR} ${NET_PLATFORM_HDR})

set(wxALL_HDR
    ${wxBASE_HDR}
    ${wxCORE_HDR}
    ${wxADV_HDR}
    ${wxAUI_HDR}
    ${wxPropGrid_HDR}
    ${wxGLCanvas_HDR}
    ${wxXML_HDR}
    ${wxNET_HDR}
    ${FakeHDR}
    )

set(wxSUPPORT_SOURCE
    ${wxJPEG}
    ${wxPNG}
    ${wxTIFF}
    ${wxZLIB}
    ${wxEXPAT}
)

#----------------------------------------------------------
# Copied from ogre
macro(use_precompiled_header TARGET HEADER_FILE SRC_FILE)
    get_filename_component(HEADER ${HEADER_FILE} NAME)

    if (MSVC)
        add_definitions(/Yuwx/${HEADER})
        set_source_files_properties(${SRC_FILE} PROPERTIES COMPILE_FLAGS /Ycwx/${HEADER})
    endif()

endmacro(use_precompiled_header)


macro(SORT_ARGS OUT2)
    foreach (it ${ARGN})
        set(${OUT2} ${${OUT2}} ${it})
    endforeach(it)

    if (${OUT2})
        list(SORT ${OUT2})
    endif()
endmacro(SORT_ARGS)


#----------------------------------------------------------
macro(MAKE_SOURCE OUT)
    # Append Root Path to src

    if (wxWidgets_GROUP_HEADERS)
        sort_args(GROUP_LIST_S ${ARGN})
    else()
        set(GROUP_LIST_S ${ARGN})
    endif()

    foreach (it ${GROUP_LIST_S})
        set(${OUT} ${${OUT}} ${wxWidgets_SOURCE_DIR}/${it})
        if (wxWidgets_GROUP_HEADERS)
            get_filename_component(PATH_COMPS ${it} PATH)
            string(REGEX REPLACE "/" "\\\\" PATH_NEWS ${PATH_COMPS})
            source_group("Source Files\\${PATH_NEWS}" FILES ${wxWidgets_SOURCE_DIR}/${it})
        endif()
    endforeach(it)

endmacro(MAKE_SOURCE)

#----------------------------------------------------------
macro(MAKE_HEADER OUT1)

    if (wxWidgets_GROUP_HEADERS)
        sort_args(GROUP_LIST_H ${ARGN})
    else()
        set(GROUP_LIST_H ${ARGN})
    endif()

    foreach (it ${GROUP_LIST_H})

        get_filename_component(PATH_FILE_EXT ${it} EXT)
        set(EXT .cpp)
        if ( NOT ${PATH_FILE_EXT} STREQUAL ${EXT})
            set(${OUT1} ${${OUT1}} ${wxWidgets_SOURCE_DIR}/include/${it})
        endif()

        if (wxWidgets_GROUP_HEADERS AND ${OUT1})
            get_filename_component(PATH_COMPH ${it} PATH)
            string(REGEX REPLACE "/" "\\\\" PATH_NEWH ${PATH_COMPH})
            source_group("Header Files\\${PATH_NEWH}" FILES ${wxWidgets_SOURCE_DIR}/include/${it})
        endif()
    endforeach(it)

endmacro(MAKE_HEADER)


#----------------------------------------------------------
macro(SETUP_WX_ALL TARGET)

    include_directories(${wxALL_INC})

    add_definitions(-DWXBUILDING -D_UNICODE -DwxUSE_BASE=1)

    if (wxWidgets_DEBUG)
        add_definitions(-D__WXDEBUG__)
    endif()

    if (WIN32)
        add_definitions(-D__WXMSW__)
        link_libraries(Comctl32 Rpcrt4)
    else()
        if (APPLE)
            add_definitions(-D__WXCOCOA__)
            ## XXX link_libraries(May need some extra system libs here)
        else()
            include_directories(${wxGTK2_PATHS})
			add_definitions(${wxGTK2_PREPROCESSOR})
        endif()
    endif()

endmacro(SETUP_WX_ALL)




#----------------------------------------------------------
macro(SETUP_WX_SUPPORT TARGET)
    ## all support libraries

    include_directories(${wxALL_INC})

    ## for expat
    if (WIN32)
        add_definitions(-DCOMPILED_FROM_DSP)
    endif()

    if (APPLE)
        add_definitions(-DOS2_32)
    endif()


endmacro(SETUP_WX_SUPPORT)

