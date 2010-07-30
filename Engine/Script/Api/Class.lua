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
--
-- Class derrived: 
--  http://www.lua.org/gems/
--  http://www.lua.org/gems/lpg121.zip
--
-- BaseClass is local code.
--
-------------------------------------------------------------------------------


-------------------------------------------------------------------------------
-- 
-- Derrived functions 
-- function(self, ...) = self.base.function(self.base, ...)
-------------------------------------------------------------------------------
local pconstructor = function(instance, extends, ...)

    status, ret = pcall(extends, ...)
    if (not status) then
        ret = nil
        status, ret = pcall(extends)
        if (not status) then
            error(ret)
            return nil
        end
    end

    instance.base = ret
    local ptable = getmetatable(instance.base)

    for i,j in pairs(ptable) do

        if (type(j) == "table") then
            for k, parTable in pairs(j) do
                if (type(parTable) == "function") then

                    -- manually overload the function
                    instance[k] = function(instance, ...)
 
                        local status, ret = pcall(parTable, instance.base)
                        if (status) then
                            return ret
                        end

                        status, ret = pcall(parTable, instance.base, ...)
                        if (not status) then
                            error(ret)
                        end
                        return ret;
                    end
                end
            end
        end
    end

    instance:constructor(...)
    return instance
end

-------------------------------------------------------------------------------
-- 
-- Class that inherits methods of 'extends'
-- The base class instance is in self.base
-- 
-- This function allows extending internal OgreKit classes
-- 
-- ex; NewGameObjectClass = BaseClass(OgreKit.GameObject)
--        TODO :)
-------------------------------------------------------------------------------
BaseClass = function(extends)
    local this  = {}
    local pthis = {
        __call  = function(object, ...)
            local newt = {}
            local instance  = {
                __index = this
                }
            setmetatable(newt, instance)
            if (newt.constructor) then
                newt = pconstructor(newt, extends, ...)
            end
            return newt
        end,
    }

    setmetatable(this, pthis)
    return this
end


-------------------------------------------------------------------------------
-- 
-- Simple class no inheritance
-- 
-------------------------------------------------------------------------------
Class = function()
    local this      = {}
    local parTable  = {
        __index = nil,
        __call  = function(object, ...)
            local newt      = {}
            local instance  = {__index = this}

            setmetatable(newt, instance)
            if (newt.constructor) then
                newt:constructor(...)
            end
            return newt
        end,
    }
    setmetatable(this, parTable)
    return this
end