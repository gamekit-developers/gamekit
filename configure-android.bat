@echo off

set MAINFOLDER=%CD%

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
	cmake %MAINFOLDER% -G "NMake Makefiles" -DCMAKE_TOOLCHAIN_FILE="%MAINFOLDER%/SDK/Android/android-r5b.toolchain" -DOGRE_BUILD_PLATFORM_ANDROID=1 -DOGREKIT_BUILD_ANDROID=1 -DOGREKIT_MINIMAL_FREEIMAGE_CODEC=1 --debug-trycompile
	cmake %MAINFOLDER% -G "NMake Makefiles" -DCMAKE_TOOLCHAIN_FILE="%MAINFOLDER%/SDK/Android/android-r5b.toolchain" -DOGRE_BUILD_PLATFORM_ANDROID=1 -DOGREKIT_BUILD_ANDROID=1 -DOGREKIT_MINIMAL_FREEIMAGE_CODEC=1 --debug-trycompile
) else (
	cmake %MAINFOLDER% -G "NMake Makefiles" -DCMAKE_TOOLCHAIN_FILE="%MAINFOLDER%/SDK/Android/android-r5b.toolchain" -DOGRE_BUILD_PLATFORM_ANDROID=1 -DOGREKIT_BUILD_ANDROID=1 -DOGREKIT_MINIMAL_FREEIMAGE_CODEC=1 -DCMAKE_BUILD_TYPE="MinSizeRel"
	cmake %MAINFOLDER% -G "NMake Makefiles" -DCMAKE_TOOLCHAIN_FILE="%MAINFOLDER%/SDK/Android/android-r5b.toolchain" -DOGRE_BUILD_PLATFORM_ANDROID=1 -DOGREKIT_BUILD_ANDROID=1 -DOGREKIT_MINIMAL_FREEIMAGE_CODEC=1 -DCMAKE_BUILD_TYPE="MinSizeRel"
)

rem cd %MAINFOLDER%