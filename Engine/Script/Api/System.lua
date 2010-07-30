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


-------------------------------------------------------------------------------
if _G["printf"] == nil then 
    _G["printf"] = function(...)
        print(string.format(...))
    end
end

-------------------------------------------------------------------------------
if _G["dPrintf"] == nil then 
    _G["dPrintf"] = function(...)
        OgreKit.DebugPrint(string.format(...))
    end
end
