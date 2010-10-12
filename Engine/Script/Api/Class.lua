--[[
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): none yet.
-------------------------------------------------------------------------------
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
	Class derrived: 
		http://www.lua.org/gems/
		http://www.lua.org/gems/lpg121.zip
	
	BaseClass is local code.
--]]

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