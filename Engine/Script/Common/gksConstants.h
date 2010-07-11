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
-- Generic Script API Binding                                                --
-------------------------------------------------------------------------------
*/
#ifndef _gksConstants_h_
#define _gksConstants_h_



// ----------------------------------------------------------------------------
enum gksTransformSpace
{
    TS_LOCAL,
    TS_PARENT,
    TS_WORLD,
};

// ----------------------------------------------------------------------------
enum gksGameObjectTypes
{
    OB_UNKNOWN=0,
    OB_CAMERA,
    OB_LIGHT,
    OB_ENTITY,
    OB_EMPTY,
    OB_SKELETON,
};

#endif//_gksConstants_h_
