macro(configure_animkit)

option(ANIMKIT_DOUBLE_PRECISION		"Use double instead of floats" OFF)
option(ANIMKIT_USE_SSE_IF_AVAILABLE	"Use SIMD optimisation if available" ON)


if(ANIMKIT_DOUBLE_PRECISION)

	add_definitions(-DANIMKIT_DOUBLE_PRECISION)
	if(ANIMKIT_USE_SSE_IF_AVAILABLE)
		message("SSE not available with double precission SSE will not be used.")
	endif(ANIMKIT_USE_SSE_IF_AVAILABLE)
	
else(ANIMKIT_DOUBLE_PRECISION)

	if(ANIMKIT_USE_SSE_IF_AVAILABLE)
	
		add_definitions(-DANIMKIT_USE_SSE_IF_AVAILABLE)
		
		if(CMAKE_COMPILER_IS_GNUCXX)
                        check_cxx_compiler_flag(-msse2 ANIMKIT_GCC_SUPPORT_SSE)
			if(ANIMKIT_GCC_SUPPORT_SSE)
                                add_definitions(-msse2)
				add_definitions(-DANIMKIT_GCC_SUPPORT_SSE)
			endif(ANIMKIT_GCC_SUPPORT_SSE)
		endif(CMAKE_COMPILER_IS_GNUCXX)
		
	endif(ANIMKIT_USE_SSE_IF_AVAILABLE)
	
endif(ANIMKIT_DOUBLE_PRECISION)

endmacro()
