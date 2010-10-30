
-- Extend the engine.
OgreKitApp = BaseClass(OgreKit.Engine)


function OgreKitApp:constructor()


    self.prefs              = self:getUserDefs()
    self.prefs.winTitle     = "Engine Setup"
    self.prefs.verbose      = false
    self.prefs.grabInput    = false
    self.prefs.blenderMat   = true
    self.prefs.debugFps     = true
    self.keyboard           = OgreKit.Keyboard()


    -- call internal engine setup.
    self:initialize()


    -- Loads the first scene, or the active one.
    self.scene = self:loadBlendFile("Test0.blend")


    -- Converts blender data to Ogre. 
    self.scene:createInstance()


    -- We now have access to all the objects within the OgreKit scene. 

    -- Grab the default cube.
    self.object = self.scene:getObject("Cube")
end



--- Engine tick listener.
function OgreKitApp:OnTick(delta)

    if self.keyboard:isKeyDown(OgreKit.KC_ESCKEY) then
        self:requestExit()
    end

    self.object:rotate(0, 0, -90 * delta)

end

--- Print input arguments
function printArgs()	
	for i=1, #arg do
		print(i, arg[i])
	end
end

printArgs()

-- Create the application.
local app = OgreKitApp()

-- Connect Events
app:connect(OgreKit.EVT_TICK, app, OgreKitApp.OnTick)

-- Start the main loop.
app:run()

