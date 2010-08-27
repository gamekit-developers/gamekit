#!/bin/bash

sed -f edit_linker_paths.sed OgreKit.xcodeproj/project.pbxproj > tmp.pbxproj
mv tmp.pbxproj OgreKit.xcodeproj/project.pbxproj
