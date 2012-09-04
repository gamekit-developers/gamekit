@echo off

set MAINFOLDER=%CD%

set ABI="armeabi-v7a"

rem - Possible ABIs:
rem - "armeabi" "armeabi-v6 with VFP" "armeabi-v7a" "armeabi-v7a with NEON" "armeabi-v7a with VFPV3" "x86"

set API_LEVEL=9

rem - See SDK/Android/android.toolchain.cmake for a full list of options.

    
if "%1" == "" (
	echo "Enter build target directory."
	@exit /b 1
)

if "%NDK%" == "" (
	echo "NDK Path is empty."
	@exit /b 1
)

if "%LIBPATH%" == "" (
	echo "Warning: vc++ environment is missing."
)

if not exist %1 (
	mkdir %1
)

cd %1

if "%2" == "debug" (	
	cmake %MAINFOLDER% -G "NMake Makefiles" -DGAMEKIT_GRAPHIC_BACKEND=2 -DCMAKE_TOOLCHAIN_FILE="%MAINFOLDER%/SDK/Android/android.toolchain.cmake" -DOGRE_BUILD_PLATFORM_ANDROID=1 -DOGREKIT_BUILD_ANDROID=1 -DANDROID_ABI="%ABI%" -DANDROID_NATIVE_API_LEVEL=%API_LEVEL% -DOGREKIT_MINIMAL_FREEIMAGE_CODEC=1 -DLIBRARY_OUTPUT_PATH= -DCMAKE_BUILD_TYPE="Debug" --debug-trycompile
	cmake %MAINFOLDER% -G "NMake Makefiles" -DGAMEKIT_GRAPHIC_BACKEND=2 -DCMAKE_TOOLCHAIN_FILE="%MAINFOLDER%/SDK/Android/android.toolchain.cmake" -DOGRE_BUILD_PLATFORM_ANDROID=1 -DOGREKIT_BUILD_ANDROID=1 -DANDROID_ABI="%ABI%" -DANDROID_NATIVE_API_LEVEL=%API_LEVEL% -DOGREKIT_MINIMAL_FREEIMAGE_CODEC=1 -DLIBRARY_OUTPUT_PATH= -DCMAKE_BUILD_TYPE="Debug" --debug-trycompile
) else (
	cmake %MAINFOLDER% -G "NMake Makefiles" -DGAMEKIT_GRAPHIC_BACKEND=2 -DCMAKE_TOOLCHAIN_FILE="%MAINFOLDER%/SDK/Android/android.toolchain.cmake" -DOGRE_BUILD_PLATFORM_ANDROID=1 -DOGREKIT_BUILD_ANDROID=1 -DANDROID_ABI="%ABI%" -DANDROID_NATIVE_API_LEVEL=%API_LEVEL% -DOGREKIT_MINIMAL_FREEIMAGE_CODEC=1 -DLIBRARY_OUTPUT_PATH=  -DCMAKE_BUILD_TYPE="MinSizeRel"
	cmake %MAINFOLDER% -G "NMake Makefiles" -DGAMEKIT_GRAPHIC_BACKEND=2 -DCMAKE_TOOLCHAIN_FILE="%MAINFOLDER%/SDK/Android/android.toolchain.cmake" -DOGRE_BUILD_PLATFORM_ANDROID=1 -DOGREKIT_BUILD_ANDROID=1 -DANDROID_ABI="%ABI%" -DANDROID_NATIVE_API_LEVEL=%API_LEVEL% -DOGREKIT_MINIMAL_FREEIMAGE_CODEC=1 -DLIBRARY_OUTPUT_PATH=  -DCMAKE_BUILD_TYPE="MinSizeRel"
)

rem cd %MAINFOLDER%
