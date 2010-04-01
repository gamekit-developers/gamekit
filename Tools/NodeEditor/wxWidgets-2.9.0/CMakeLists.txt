cmake_minimum_required(VERSION 2.4)
project(wxWidgets)
set(wxWidgets 2.9.0)

set(CMAKE_MODULE_PATH ${wxWidgets_SOURCE_DIR}/wxCMake)

option(wxDEBUG              "Enable / Disable debug mode (__WXDEBUG__)"                 OFF)
option(wxPRECOMP            "Enable / Disable pre compiled headers under __WXMSW__"     ON)
option(wxGROUP_HEADERS      "Sort headers into groups for the current IDE "             OFF)
subdirs(wxCMake)
