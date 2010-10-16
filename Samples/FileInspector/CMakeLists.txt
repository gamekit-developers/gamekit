include(MakeFBT)


set(INSP_SRC
    fbtApp.cpp
    fbtTextFile.cpp
    fbtMainFrame.cpp
    fbtDataView.cpp
    fbtInspectorFile.cpp
    fbtIcons.cpp
    Serialize/fbtDataApi.cpp
)


set(INSP_HDR
    fbtApp.h
    fbtTextFile.h
    fbtDataView.h
    fbtMainFrame.h
    fbtInspectorFile.h
    fbtIcons.h
)

set(INSP_API Serialize/fbtDataApi.h)
if (WIN32)
	set(PLAT_RESOURCE Win32/application.rc)
else()
	if (NOT APPLE)
		link_libraries(X11 SM)
	endif()
endif()

add_fbt(fbtInspector ${INSP_API})


include_directories(${FBT_SOURCE_DIR}/File ${FBT_SOURCE_DIR}/FileFormats/Blend ${FBT_SOURCE_DIR}/FileFormats/Bullet)
link_libraries(fbtFile bfBlend btBulletFile)


set(ALL ${INSP_SRC} ${INSP_HDR} ${INSP_API} ${fbtInspector} ${PLAT_RESOURCE})
setup_wx()


if (WIN32)
    add_executable(AppInspector WIN32 ${ALL})
else()
    add_executable(AppInspector ${ALL})
endif()
