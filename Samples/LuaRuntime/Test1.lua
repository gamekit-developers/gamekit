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

	Player test Using OgreKit.FSM
--]]



-------------------------------------------------------------------------------
-- Global Input codes
-------------------------------------------------------------------------------
local input_codes = {
	forward  = OgreKit.KC_WKEY,
	reverse  = OgreKit.KC_SKEY,
	jump     = OgreKit.KC_SPACEKEY,
	jump_end = 12,
	exit_app = OgreKit.KC_ESCKEY,
}

-------------------------------------------------------------------------------
--
-- Utilities
--
-------------------------------------------------------------------------------
function Clamp(v, mi, ma)
	if v < mi then return mi end
	if v > ma then return ma end
	return v
end


--[[
-------------------------------------------------------------------------------
	Camera Logic Setup

	.blend file requirements:
		Camera         - named 'MainCamera'
							 Default blender camera.
		Object         - named 'Physics'
							   For synchronizing / tracking this camera to the Object

	See: Player for the rest
-------------------------------------------------------------------------------
--]]
MainCamera = Class()


-------------------------------------------------------------------------------
function MainCamera:constructor(scene)

	self.scene    = scene
	self.camera   = scene:getObject("MainCamera")
	self.player   = scene:getObject("Physics")
	self.mouse    = OgreKit.Mouse()
	self.cray     = OgreKit.RayTest()


	local old = self.player:getWorldPosition()

	self.xrot = scene:createEmpty("xRot")
	self.zrot = scene:createEmpty("zRot")
	self.zrot:createInstance();
	self.xrot:createInstance();



	self.xrot:setParent(self.zrot)
	self.camera:setParent(self.xrot)


	self.zrot:setPosition(old)
	self.camera:setRotation(90, 0, 0)
	self.camera:setPosition(0, -1, 0)


end

-------------------------------------------------------------------------------
function MainCamera:getRootRotation()

	return self.zrot:getRotation()
end


-------------------------------------------------------------------------------
function MainCamera:getRootOrientation()

	return self.zrot:getOrientation()

end


-------------------------------------------------------------------------------
function MainCamera:getXWorldOrientation()

	return self.xrot:getWorldOrientation()

end


-------------------------------------------------------------------------------
function MainCamera:getXWorldRotation()

	return self.xrot:getWorldRotation()

end


-------------------------------------------------------------------------------
function MainCamera:GetRay(len)

	if len == 0 then 
		len = 1 
	end


	local offsVec   = OgreKit.Vector3(0, 0, 0.025)
	local offsVec2  = OgreKit.Vector3(0, -offsVec.z/2.0, offsVec.z)
	local playloc   = self.player:getPosition()       + offsVec
	local rayFrom   = self.camera:getWorldPosition()  - offsVec2
	local rayDir    = (playloc - rayFrom) / len
	local dist      = (rayDir:length2() * len)
	return OgreKit.Ray(rayFrom, rayDir), dist
end

-------------------------------------------------------------------------------
function MainCamera:OnUpdate(delta)


	local zoffs     = 0.025
	local camTol    = 0.05

	self.mouse:capture()

	if self.mouse.moved then
		
		if self.mouse.xrel ~= 0 then
			self.zrot:roll( -(self.mouse.xrel * delta) * 0.5)

		end

		if self.mouse.yrel ~= 0 then
		
			self.xrot:pitch( -(self.mouse.yrel * delta) * 0.5)

			local rot = self.xrot:getRotation()

			if (rot.x > 45) then
				rot.x = 45
				self.xrot:setRotation(rot)
			else
				if (rot.x < -90) then
					rot.x = -90
					self.xrot:setRotation(rot)
				end
			end
		end
	end

	-- move camera to player
	self.zrot:translate((self.player:getPosition() - (self.zrot:getPosition() + OgreKit.Vector3(0, 0, -zoffs))) * camTol)
	

	local rayLen = 1.5
	local ray, dist = self:GetRay(rayLen)

	-- do ray cast
	if self.cray:cast(ray) then

		if self.cray:getObject():getName() ~= "Physics" then

			-- move forward
			self.camera:translate(0, dist * self.cray:getHitFraction(), 0)

		end
	end


	if dist <= rayLen then
		self.camera:translate(0, -(camTol * 0.5), 0)
	end

	-- ensure it's always behind the root parent
	local vpos = self.camera:getPosition();

	if vpos.y >= 0 then
		self.camera:setPosition(0, -rayLen / 2.0, 0)
	end

end


--[[
-------------------------------------------------------------------------------
	Player Logic Setup

	.blend file requirements:
	Object   -  named 'Physics'
					As dynamic physics type.
	Entity   -  named 'MeshMomo' + Armature 'RigMomo'
					Main mesh needed for animations.
	Hierarchy:
		Physics (Root)
			|- MeshMomo
				|- RigMomo
-------------------------------------------------------------------------------
--]]

Player = BaseClass(OgreKit.FSM)

-------------------------------------------------------------------------------
function Player:constructor(scene)

	if (not scene) then error("Expected scene as first argument to Player") end

	self.states = {
		MAIN        =0,
		IDLE        =1,
		WALK        =2,
		RUN         =3,
		BACK        =4,
		JUMP        =5,
		HOVER       =6,
		FALL        =7,
	}

	self.keyboard = OgreKit.Keyboard()
	self.scene    = scene
	self.physics  = scene:getObject("Physics")
	self.mesh     = scene:getObject("MeshMomo")
	self.camera   = MainCamera(self.scene);
	self.hangtime = 0
	self.jumpTog  = 0
	self.iFly     = 0.0
	self.iFlyMax  = 25.0;

	local avoid = OgreKit.ObjectList()
	avoid:push(self.physics)


	self.gTest    = OgreKit.SweptTest(avoid)
	self.debugger = OgreKit.Debugger(scene)

	-- INITIAL
	self:setState(self.states.IDLE)


	-----------------------------------
	-- IDLE TRANSITIONS
	self:addTransition(self.states.IDLE,    self.states.IDLE)
	self:addTransition(self.states.IDLE,    self.states.RUN, 5):when(self, Player.IsForward)
	self:addTransition(self.states.IDLE,    self.states.BACK, 5):when(self, Player.IsReverse)
	self:addTransition(self.states.IDLE,    self.states.JUMP):when(self, Player.IsJump)
	self:addEvent(self.states.IDLE, self,   Player.OnIdle)

	-----------------------------------
	-- RUN TRANSITIONS
	self:addTransition(self.states.RUN,     self.states.RUN )
	self:addTransition(self.states.RUN,     self.states.IDLE):when(self, Player.IsNotForward)
	self:addTransition(self.states.RUN,     self.states.JUMP):when(self, Player.IsJump)
	self:addEvent(self.states.RUN, self,    Player.OnRun)


	-----------------------------------
	-- BACK TRANSITIONS
	self:addTransition(self.states.BACK,    self.states.BACK)
	self:addTransition(self.states.BACK,    self.states.IDLE):when(self, Player.IsNotReverse)
	self:addEvent(self.states.BACK, self,   Player.OnBackup)

	-----------------------------------
	-- JUMP TRANSITIONS
	self:addTransition(self.states.JUMP,    self.states.JUMP)
	self:addTransition(self.states.JUMP,    self.states.IDLE):when(self,  Player.OnGround)
	self:addTransition(self.states.JUMP,    self.states.HOVER):when(self,  Player.IsDbJump)
	self:addEvent(self.states.JUMP,         self, Player.OnJump)
	self:addEndTrigger(self.states.JUMP,    self, Player.ClearJump)


	-----------------------------------
	-- HOVER TRANSITIONS
	self:addTransition(self.states.HOVER,   self.states.IDLE):when(self,  Player.OnGround)
	self:addTransition(self.states.HOVER,   self.states.FALL):when(self,  Player.IsHoverEnd)
	self:addEvent(self.states.HOVER,        self, Player.OnHover)
	self:addEndTrigger(self.states.HOVER,   self, Player.ClearHover)

	-----------------------------------
	-- FALL TRANSITIONS
	self:addTransition(self.states.FALL,    self.states.FALL)
	self:addTransition(self.states.FALL,    self.states.IDLE ):when(self,  Player.OnGround)
	self:addTransition(self.states.FALL,    self.states.HOVER):when(self,  Player.IsJump)
	self:addEvent(self.states.FALL, self, 	Player.OnFall)

end


-------------------------------------------------------------------------------
--
-- State Conditions
--
-------------------------------------------------------------------------------

-------------------------------------------------------------------------------
function Player:IsForward()
	return self.keyboard:isKeyDown(input_codes.forward)
end


-------------------------------------------------------------------------------
function Player:IsNotForward()
	return not self.keyboard:isKeyDown(input_codes.forward)
end



-------------------------------------------------------------------------------
function Player:IsReverse()
	return self.keyboard:isKeyDown(input_codes.reverse)
end

-------------------------------------------------------------------------------
function Player:IsNotReverse()
	return not self.keyboard:isKeyDown(input_codes.reverse)
end


-------------------------------------------------------------------------------
function Player:IsJump()
	return self.keyboard:isKeyDown(input_codes.jump)
end


-------------------------------------------------------------------------------
function Player:IsNotJump()
	return not self:IsJump()
end


-------------------------------------------------------------------------------
function Player:IsDbJump()
	return self.keyboard:isKeyDown(input_codes.jump) and self.jumpTog == 1
end

-------------------------------------------------------------------------------
function Player:IsJumpEnd()
	return self.hangtime == input_codes.jump_end
end


-------------------------------------------------------------------------------
function Player:IsHoverEnd()
	return self:IsNotForward() and self:IsNotJump()
end



-------------------------------------------------------------------------------
function Player:OnGround()

	
	local dist  = 0.35;

	local origin = self.physics:getPosition()
	local direction = OgreKit.Vector3(0, 0, dist)
	origin = origin - direction;

	if (self.gTest:collides(OgreKit.Ray(origin, direction), dist)) then
		return true
	end

	return false
end




-------------------------------------------------------------------------------
function Player:ClearJump(from, to)
	-- End trigger, clear jump variables

	self.hangtime = 0;
	self.jumpTog = 0;
end


-------------------------------------------------------------------------------
function Player:ClearHover(from, to)
	-- End trigger, clear hover states


	self.mesh:setRotation(0, 0, self.mesh:getRotation().z)
	self.iFly = 0;

	self.physics:setLinearVelocity(0, 0, 0)
end





-------------------------------------------------------------------------------
function Player:Move(mvec)

	local rootOri = self.camera:getRootOrientation()
	mvec = rootOri * mvec
	self.mesh:setRotation(0, 0, self.camera:getRootRotation().z)

	mvec.z = self.physics:getLinearVelocity().z
	self.physics:setLinearVelocity(mvec)

end

-------------------------------------------------------------------------------
--
-- State Actions
--
-------------------------------------------------------------------------------


-------------------------------------------------------------------------------
function Player:OnIdle()
	
	-- No input, just sit and wait 

	self.mesh:playAnimation("Momo_IdleNasty", 20)
end


-------------------------------------------------------------------------------
function Player:OnRun()

	-- Travel forward


	self:Move(OgreKit.Vector3(0, 3, 0))
	
	self.mesh:playAnimation("Momo_Run", 7)
end


-------------------------------------------------------------------------------
function Player:OnBackup()

	-- Travel back


	self:Move(OgreKit.Vector3(0, -1.5, 0))


	self.mesh:playAnimation("Momo_WalkBack", 7)
end




-------------------------------------------------------------------------------
function Player:OnJump()


	-- Double jump test
	if (not self:IsJump()) then
		self.jumpTog = 1;
	end



	-- Play half of the jump action
	if (self.hangtime > input_codes.jump_end) then
		return
	end


	-- apply initial velocity
	if (self.hangtime == 0) then
		local vel = self.physics:getLinearVelocity()
		self.physics:setLinearVelocity(vel.x, vel.y, vel.z + 2)
	end



	self.mesh:playAnimation("Momo_Jump", 10)
	
	-- Advance time
	self.hangtime = self.hangtime + 1

end


-------------------------------------------------------------------------------
function Player:OnFall()

	self.mesh:playAnimation("Momo_Fall", 20)

end


-------------------------------------------------------------------------------
function Player:OnHover()


	local yvel = 0.01
	local zvel = -3 / 9.81
	local accel = self.physics:getLinearVelocity().z / 2.0

	if self:IsForward() then
		-- forward speed
		yvel = yvel + 3
		zvel = -yvel / 9.81
	end

	local mouse = self.camera.mouse

	-- clamp side to side sway
	self.iFly = Clamp( self.iFly - 0.3*(mouse.xrel), -self.iFlyMax, self.iFlyMax)

	if not mouse.moved then
		self.iFly = self.iFly * 0.95
	end


	local mvec  = self.camera:getXWorldOrientation() * OgreKit.Vector3(0, yvel, zvel);
	local xeul  = self.camera:getXWorldRotation()


	-- add extra velocity
	mvec.z = mvec.z + accel

	self.mesh:setRotation(-xeul.x, self.iFly, xeul.z)
	self.physics:setLinearVelocity(mvec)

	self.mesh:playAnimation("Momo_Glide", 20)

end


-------------------------------------------------------------------------------
function Player:OnUpdate(delta)

	-- update camera tree
	self.camera:OnUpdate(delta)

	-- call base FSM:update()
	self:update()

end


-------------------------------------------------------------------------------
--
-- OgreKitApp Setup
--
-------------------------------------------------------------------------------


OgreKitApp = BaseClass(OgreKit.Engine)

-------------------------------------------------------------------------------
function OgreKitApp:constructor()

	self.prefs                  = self:getUserDefs()
	self.prefs.winTitle         = "Lua Runtime Test [Test1.lua]";
	self.prefs.renderSystem	    = OgreKit.OGRE_RS_GL
	self.prefs.verbose          = true;
	self.prefs.grabInput        = true;
	self.prefs.blenderMat       = true;
	self.prefs.debugPhysics     = false;
	self.prefs.debugFps         = false;
	self.prefs.animSpeed        = 25
	self.prefs.enableShadows    = true
	self.prefs.shadowTechnique  = "stencilmodulative"
	self.prefs.colourShadowR    = 0.5
	self.prefs.colourShadowG    = 0.5
	self.prefs.colourShadowB    = 0.5
	
	self:initialize()

	self.keyboard   = OgreKit.Keyboard()
	self.scene      = self:loadBlendFile("Test1.blend");
	self.scene:createInstance();

	self:loadBlendFile("Test2.blend");
	
	self.player = Player(self.scene)

	self.cubeTemplate = self.scene:getObject("Cube.001")
	self.cubeClone    = self.scene:cloneObject(self.cubeTemplate, 0, true)
	self.cubeClone:setPosition(0, 2, 5);

end



-------------------------------------------------------------------------------
function OgreKitApp:OnTick(delta)


	-- poll for exit request 
	if (self.keyboard:isKeyDown(input_codes.exit_app)) then
		self:requestExit()
	end


	-- Update player state
	self.player:OnUpdate(delta)
end



-------------------------------------------------------------------------------
function main()

	-- Load all
	local app = OgreKitApp()

	-- Connect Tick Listener
	app:connect(OgreKit.EVT_TICK, app, OgreKitApp.OnTick)


	-- start engine
	app:run()

end
main()
