
set(NS_SRC
    nsClipboard.cpp
    nsEventHandler.cpp
    nsFont.cpp
    nsMain.cpp
    nsMainWndow.cpp
    nsNodeCanvas.cpp
    nsNodeManager.cpp
    nsNodeProperties.cpp
    nsNodeTypeInfo.cpp
    nsMath.cpp
    nsPropertyPage.cpp
    nsRenderSystem.cpp
    nsSocketEditors.cpp
    nsSolutionBrowser.cpp
    nsTreeProperties.cpp
    nsVariable.cpp
    nsWorkspace.cpp
    Utils/utString.cpp
)


set(NS_HDR
    nsClipboard.h
    nsCommon.h
    nsFont.h
    nsEventHandler.h
    nsMainWindow.h
    nsNodeCanvas.h
    nsNodeManager.h
    nsNodeProperties.h
    nsNodeTypeInfo.h
    nsMath.h
    nsPropertyPage.h
    nsRenderSystem.h
    nsSingleton.h
    nsSocketEditors.h
    nsSolutionBrowser.h
    nsTreeProperties.h
    nsVariable.h
    nsWorkspace.h
    Utils/utCommon.h
    Utils/utTypes.h
    Utils/utString.h
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

include_directories(. Utils)
setup_wx()

if (WIN32)
    add_executable(NodeEditor WIN32 ${ALL})
else()
    add_executable(NodeEditor ${ALL})
endif()
