
function(config_ogrekit_target1 targetname is_library)   
	#MESSAGE(STATUS "target:" ${targetname})
	if (is_library)
		 ogre_config_common(${targetname})
	endif()

	if (OGREKIT_BUILD_IPHONE)
		include_directories("include/iOS")
		set_target_properties(${targetname} PROPERTIES XCODE_ATTRIBUTE_GCC_UNROLL_LOOPS "YES")
		set_target_properties(${targetname} PROPERTIES XCODE_ATTRIBUTE_GCC_THUMB_SUPPORT "NO")
		set_target_properties(${targetname} PROPERTIES XCODE_ATTRIBUTE_GCC_PRECOMPILE_PREFIX_HEADER "YES")
	
		if (is_library)
			set_target_properties(${targetname} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "bin$(EFFECTIVE_PLATFORM_NAME)")
		else()
			set_target_properties(${targetname} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "bin$(EFFECTIVE_PLATFORM_NAME)")
			set_target_properties(${targetname} PROPERTIES XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "iPhone Developer")
		endif()	
	endif()
endfunction()


function(config_ogrekit_target targetname is_library)   
	config_ogrekit_target1(${targetname} ${is_library})
endfunction()
