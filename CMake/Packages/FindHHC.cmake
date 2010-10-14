#-------------------------------------------------------------------
# This file is part of the CMake build system for Gamekit
# For the latest info, see http://code.google.com/p/gamekit/
#
# The contents of this file are placed in the public domain. Feel
# free to make use of it in any way you like.
#-------------------------------------------------------------------

macro(find_hhc_location)
	set(hhc_location)
	if (WIN32)
		set(hhc_path2 "HTML Help Workshop/hhc.exe")
		string(REPLACE "\\" "/" hhc_path1 $ENV{ProgramFiles})
		
		if (EXISTS "${hhc_path1}/${hhc_path2}")
			set (hhc_location "${hhc_path1}/${hhc_path2}")
		else()
			string(REPLACE "\\" "/" hhc_path1 $ENV{ProgramFiles})
			if (EXISTS "${hhc_path1}/${hhc_path2}")
				set (hhc_location "${hhc_path1}/${hhc_path2}")
			endif()
		endif()
	endif()
endmacro()
