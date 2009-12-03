# ---------------------------------------------------------
cmake_minimum_required(VERSION 2.6)



project(Dependencies)
SET(Dependencies 1.0)

subdirs(Source/bullet)
subdirs(Source/FreeImage)
subdirs(Source/OIS)
subdirs(Source/ZZipLib)
subdirs(Source/FreeType)

# Later Ogre Threading + OgreKitXML ++ OgreKitNetowrk
# subdirs(Source/Poco)
