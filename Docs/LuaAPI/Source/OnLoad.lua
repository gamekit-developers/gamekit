Demo = BaseClass(OgreKit.Engine)

function Demo:constructor()

    self.scene = self:getActiveScene()

end


function Demo:update(delta)

	OgreKit.DebugPrint("Demo main loop running ==> " .. self.scene:getName())

end


demo = Demo()
demo:connect(OgreKit.EVT_TICK, demo, Demo.OnUpdate)
