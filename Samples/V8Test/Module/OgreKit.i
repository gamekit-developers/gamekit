%module OgreKit
%{
#include "OgreKitApplication.h"
%}

%rename(Application) OgreKitApplication;

%include "OgreKitTypeMaps.i"
%include "OgreKitApplication.h"