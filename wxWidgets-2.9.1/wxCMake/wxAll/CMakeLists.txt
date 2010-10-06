setup_wx_all(wxAll)
if (wxWidgets_PRECOMP)	
	use_precompiled_header_wx(wxAll ${wxROOT_INC}/wx/wxprec.h dummy.cpp)
	set(wxPRECOMPSRC dummy.cpp)
	ignore_precompiled_header_wx(${wxWidgets_SOURCE_DIR}/src/common/extended.c)
endif()

make_header(wxALLHDR ${wxALL_HDR})
make_source(wxALLSRC ${wxALL_SRC})


add_library(wxAll ${wxALLSRC} ${wxALLHDR} ${wxPRECOMPSRC})
