set(NS_SRC
	nsClipboard.cpp
	nsColorPalette.cpp
	nsDialogBase.cpp
	nsEventHandler.cpp
	nsFont.cpp
	nsMain.cpp
	nsMainWndow.cpp
	nsNode.cpp
	nsNodes.cpp
	nsNodeCanvas.cpp
	nsNodeManager.cpp
	nsNodeProperties.cpp
	nsNodeReader.cpp
	nsNodeTree.cpp
	nsNodeTypeInfo.cpp
	nsNodeWriter.cpp
	nsMath.cpp
	nsPropertyPage.cpp
	nsRenderSystem.cpp
	nsSocket.cpp
	nsSocketEditors.cpp
	nsSolutionBrowser.cpp
	nsString.cpp
	nsTreeProperties.cpp
	nsVariable.cpp
	nsWorkspace.cpp
	nsXmlUtils.cpp
)


set(NS_HDR
	nsClipboard.h
	nsColorPalette.h
	nsCommon.h
	nsData.h
	nsDialogBase.h
	nsFont.h
	nsEventHandler.h
	nsMainWindow.h
	nsNode.h
	nsNodes.cpp
	nsNodeCanvas.h
	nsNodeEnums.h
	nsNodeManager.h
	nsNodeProperties.h
	nsNodeReader.h
	nsNodeTree.h
	nsNodeTypeInfo.h
	nsNodeWriter.h
	nsMath.h
	nsPropertyPage.h
	nsRenderSystem.h
	nsSingleton.h
	nsSocket.h
	nsSocketEditors.h
	nsSolutionBrowser.h
	nsString.h
	nsTreeProperties.h
	nsVariable.h
	nsWorkspace.h
	nsXmlUtils.h
	nsDefaultStringConverters.inl
)

if (WIN32)
	set(PLAT_RESOURCE Win32/application.rc)
endif()

set(RESOURCE 
	Resource/NodeTreeDef.xml
	Resource/NodeDef.xsd
	Resource/NodeResource.xsd
)
set_source_files_properties(${RESOURCE} PROPERTIES HEADER_FILE_ONLY 1)
source_group(Content\\Resources FILES ${PLAT_RESOURCE} ${RESOURCE})


set(ALL
	${NS_SRC} 
	${NS_HDR}
	${RESOURCE}
	${PLAT_RESOURCE}
)

include_directories(. ${GAMEKIT_UTILS_PATH})
link_libraries(${GAMEKIT_UTILS_TARGET})
setup_wx()

if (WIN32)
	add_executable(AppNodeEditor WIN32 ${ALL})
else()
	add_executable(AppNodeEditor ${ALL})
endif()
