/*
-------------------------------------------------------------------------------
--                                                                           --
--  This file is part of GameKit.                                            --
--  http://gamekit.googlecode.com/                                           --
--                                                                           --
--                                                                           --
--  Released under the zlib license.                                         --
--  http://www.gzip.org/zlib/zlib_license.html                               --
--                                                                           --
--                                                                           --
-------------------------------------------------------------------------------
-- Include file for AI
*/
%{
#include "OgreKitAI.h"
%}

%include "OgreKitAI.h"
%template(WhenEventPtr)    OgreKit::Pointer<OgreKit::WhenEvent>;
