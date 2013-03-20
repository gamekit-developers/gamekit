This is an experimental approach to use the most recent ogre-mercurial-repository (at the moment v1-9) by dossantos 
(including compiling ogre's samples)
This is tested on linux, might work on osx and will not work on windows (at least not without doing other actions)

To access configure-android.sh and configure-desktop.sh:
1) make a copy of the current configure-android.sh  
2) unpack the two files from exp-configures.tar.gz in the gamekit-root-dir (overwriting configure-android.sh)

For further reading about the scripts: http://gamekit.org/forum/viewtopic.php?f=2&t=431&start=30

These scripts are in a very early stage. If you have problems please tell on the above mentioned forum-thread...

--------------------------------------------------------------------------------------------------------------------


Steps to build OgreKit against the latest Ogre 1.9 revision from mercurial (branch v1-9 tip)


Preparation of a working copy (choose a good project location, I will use a 'workspace' directory in my home dir):

	tim@Caledonia:~$ mkdir workspace
	tim@Caledonia:~$ mkdir workspace/gamekit
	tim@Caledonia:~$ cd workspace/gamekit

Get GameKit head from subversion:

	tim@Caledonia:~/workspace/gamekit$ svn co http://gamekit.googlecode.com/svn/trunk sources_svn
	tim@Caledonia:~/workspace/gamekit$ cd sources_svn

Clone the 1.9 branch of Ogre into the GameKit directory:

	tim@Caledonia:~/workspace/gamekit/sources_svn$ hg clone -r v1-9 https://bitbucket.org/sinbad/ogre Ogre-1.9-mercurial
	tim@Caledonia:~/workspace/gamekit/sources_svn$ cd Ogre-1.9-mercurial

Patch Ogre (the patch has been tested to work against Ogre 1.9, commit 23cd42f):

	tim@Caledonia:~/workspace/gamekit/sources_svn/Ogre-1.9-mercurial$ patch -p1 < ../ogrekit.ogre.v1-9.patch 
	patching file CMake/Templates/Android.mk.in
	patching file CMake/toolchain/AndroidJNI.cmake
	patching file CMake/Utils/OgreAddTargets.cmake
	patching file Components/Overlay/include/OgreFont.h
	patching file Components/Overlay/src/OgreFont.cpp
	patching file OgreMain/include/OgreUTFString.h
	patching file Samples/Browser/CMakeLists.txt
	patching file Samples/DeferredShading/src/LightMaterialGenerator.cpp

	tim@Caledonia:~/workspace/gamekit/sources_svn/Ogre-1.9-mercurial$ hg status
	M CMake/Templates/Android.mk.in
	M CMake/Utils/OgreAddTargets.cmake
	M CMake/toolchain/AndroidJNI.cmake
	M Components/Overlay/include/OgreFont.h
	M Components/Overlay/src/OgreFont.cpp
	M OgreMain/include/OgreUTFString.h
	M Samples/Browser/CMakeLists.txt
	M Samples/DeferredShading/src/LightMaterialGenerator.cpp

	tim@Caledonia:~/workspace/gamekit/sources_svn/Ogre-1.9-mercurial$ hg summary
	parent: 4419:23cd42fab1fe tip
	 Merge with v1-8
	Zweig: v1-9
	Übernehme: 8 modifiziert, 2 unbekannt
	Aktualisiere: (aktuell)

Now create a dedicated build directory (here as an example for android on x86 / intel atom):

	tim@Caledonia:~/workspace/gamekit/sources_svn/Ogre-1.9-mercurial$ cd ~/workspace/gamekit
	tim@Caledonia:~/workspace/gamekit$ mkdir android_x86; cd android_x86
	tim@Caledonia:~/workspace/gamekit/android_x86$ mkdir src; cd src
	tim@Caledonia:~/workspace/gamekit/android_x86/src$ ln -s ~/workspace/gamekit/sources_svn/* .
	tim@Caledonia:~/workspace/gamekit/android_x86/src$ cd ..
	tim@Caledonia:~/workspace/gamekit/android_x86$ mkdir build; cd build
	tim@Caledonia:~/workspace/gamekit/android_x86/build$ 

For building for android, some environment vars have to be set. I made meself a little shell script for that that can
be sourced; you will need to adapt to your environment, of course. On my machine, the NDK version r7c works; if you
get compile errors (e.g. in Bullet) further below, you may want to try other NDK versions / toolchains.

	tim@Caledonia:~/workspace/gamekit/android_x86/build$ cat ~/workspace/gamekit/setup-android-paths.sh 
	#!/bin/sh

	NDKVER=r7c
	TOOLCHAIN=x86-4.4.3

	export NDK=/opt/android-ndk-$NDKVER
	export NDK_BIN=/opt/android-ndk-$NDKVER/toolchains/$TOOLCHAIN/prebuilt/linux-x86/bin
	export PATH=/opt/android-sdk/tools:/opt/android-sdk/platform-tools:/opt/android-ndk-$NDKVER:$PATH
	export ANDROID_NDK=/opt/android-ndk-$NDKVER
	export ANDROID_SDK=/opt/android-sdk

	tim@Caledonia:~/workspace/gamekit/android_x86/build$ source ~/workspace/gamekit/setup-android-paths.sh 

After these preparations, the build can be started:

	tim@Caledonia:~/workspace/gamekit/android_x86/build$ ../src/configure-android . x86 debug onlydeps
	...
	tim@Caledonia:~/workspace/gamekit/android_x86/build$ make

The mandatory arguments for the configure-script are the build path (in our case the current dir) and the android ABI
to compile against (x86, armeabi, armeabi_v7...); the "debug"-argument configures a debugging build, and the "onlydeps"-
argument makes the build skip Ogre and GameKit, and only builds the dependencies (OIS, FreeImage, FreeType, zzlib, etc)

I the build succeeds, it can be run a second time, this time without the "onlydeps" arg - and such, this time including
Ogre and GameKit. Important: You need to remove the CMake cache first:

	tim@Caledonia:~/workspace/gamekit/android_x86/build$ rm CMakeCache.txt; ../src/configure-android . x86 debug
	...
	tim@Caledonia:~/workspace/gamekit/android_x86/build$  make

If everything worked, you will find the Ogre Samplebrowser:
	
	tim@Caledonia:~/workspace/gamekit/android_x86/build$ cd SampleBrowserNDK/bin/
	tim@Caledonia:~/workspace/gamekit/android_x86/build/SampleBrowserNDK/bin$ ls *.apk
	NativeActivity-debug.apk  NativeActivity-debug-unaligned.apk

And Ogre's new JNI-activity (work in progress):

	tim@Caledonia:~/workspace/gamekit/android_x86/build/SampleBrowserNDK/bin$ cd ../../OgreJNI/bin/
	tim@Caledonia:~/workspace/gamekit/android_x86/build/OgreJNI/bin$ ls *.apk
	MainActivity-debug.apk  MainActivity-debug-unaligned.apk

For linux build instead of android, the process is quite similar:

    tim@Caledonia:~/workspace/gamekit$ mkdir linux; mkdir linux/src; mkdir linux/build
    tim@Caledonia:~/workspace/gamekit$ cd linux/src
    tim@Caledonia:~/workspace/gamekit/linux/src$ ln -s ~/workspace/gamekit/sources_svn/* .
    tim@Caledonia:~/workspace/gamekit/linux/src$ cd ../build
    tim@Caledonia:~/workspace/gamekit/linux/build$ ../src/configure-desktop . debug onlydeps && make
    tim@Caledonia:~/workspace/gamekit/linux/build$ rm CMakeCache.txt; ../src/configure-desktop . debug && make

This will (if everything worked) result in GameKit's stuff in ./Samples
