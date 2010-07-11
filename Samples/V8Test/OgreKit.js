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
-- Runtime test using the V8 JavaScript engine.                              --
-------------------------------------------------------------------------------
*/
var BlendToLoad = "MomoAnimation.blend";
var MomoActions = [   "Momo_Carry",
                      "Momo_Catch",
                      "Momo_Death",
                      "Momo_DieLava",
                      "Momo_dj",
                      "Momo_Drowning",
                      "Momo_EdgeClimb",
                      "Momo_EdgeIdle",
                      "Momo_Fall",
                      "Momo_FallUp",
                      "Momo_Glide",
                      "Momo_Hit_Lightly",
                      "Momo_HitCarry",
                      "Momo_Idle1",
                      "Momo_IdleCapoeira",
                      "Momo_IdleNasty",
                      "Momo_Jump",
                      "Momo_Kick",
                      "Momo_Revive",
                      "Momo_Run",
                      "Momo_RunFaster",
                      "Momo_ShimmyL",
                      "Momo_ShimmyR",
                      "Momo_TailWhip",
                      "Momo_Throw1",
                      "Momo_ThrowSheep",
                      "Momo_ThrowWith",
                      "Momo_ThrowWithout",
                      "Momo_Turn.L",
                      "Momo_Turn.R",
                      "Momo_Walk",
                      "Momo_WalkBack",
                      "Momo_WalkFast",
                      "Momo_WalkHand",
                      "Momo_WalkSlow",
                      "Momo_WallFlip"
                  ];



// ----------------------------------------------------------------------------
EditCamera = function(parentScene)
{
    this.scene = parentScene;
    this.robj  = this.scene.getObject("roll");
    this.pobj  = this.scene.getObject("pitch");
    this.cobj  = this.scene.getObject("view");
    // ------------------------------------------------------------------------
    this.update = function(mouse, keyboard)
    {
        var shift   = keyboard.isKeyDown(GameLogic.KC_LEFTSHIFTKEY);
        var ctrl    = keyboard.isKeyDown(GameLogic.KC_LEFTCTRLKEY);
        var middle  = mouse.isButtonDown(GameLogic.LEFT);
        var canDrag = mouse.moved && middle;

        var sizeFac = mouse.winx < mouse.winy ? mouse.winx : mouse.winy;

        if (canDrag && !ctrl && !shift)
        {
            this.rotate(
                2.0 * -mouse.xrel * (2.0 / sizeFac),
                2.0 * -mouse.yrel * (2.0 / sizeFac)
            );
        }
        else if (canDrag && ctrl && !shift)
        {
            var zfac = 2.0 * this.getDistanceToRoot();
            this.zoom( 2.0 * mouse.yrel * (zfac / sizeFac));
        }
        else if (canDrag && !ctrl && shift)
        {
            var offs = this.getDistanceToRoot();
            this.pan(
                2.0 * -mouse.xrel * (offs / sizeFac),
                2.0 *  mouse.yrel * (offs / sizeFac)
            )
        }
    }

    // ------------------------------------------------------------------------
    this.rotate = function(dx, dy)
    {
        this.robj.roll(dx);
        this.pobj.pitch(dy);

        // do -90 / 90 clamp

        var doClamp = false;
        var vec = this.pobj.getRotation();

        if (vec.x > 90)
        {
            doClamp = true;
            vec.x = 90;
        }
        else if (vec.x < -90)
        {
            doClamp = true;
            vec.x = -90;
        }

        if (doClamp)
            this.pobj.setRotation(vec);
    }


    // ------------------------------------------------------------------------
    this.pan = function(dx, dy)
    {
        var p = this.pobj.getOrientation();
        var r = this.robj.getOrientation();
        var c = this.cobj.getOrientation();
        var rvec = Math.QuatMulVec3(Math.QuatMulQuat(Math.QuatMulQuat(r, p), c), Math.Vector3(dx, dy, 0));
        this.robj.translate(rvec);
    }


    // ------------------------------------------------------------------------
    this.zoom = function(zfac)
    {
        this.cobj.translate(Math.QuatMulVec3(this.cobj.getOrientation(), Math.Vector3(0, 0, zfac)));
    }

    // ------------------------------------------------------------------------
    this.getDistanceToRoot = function()
    {
        return this.cobj.getWorldPosition().distance(this.pobj.getWorldPosition());
    }

}


// ----------------------------------------------------------------------------

App = function()
{
    this.runtime = OgreKit.Application(BlendToLoad);
    this.defs    = this.runtime.getUserDefs();
    // ----------------------------------------------------------------------------
    this.Run = function()
    {
        var prefs = this.defs;

        prefs.grab       = false;
        prefs.verbose    = true;
        prefs.winx       = 800;
        prefs.winy       = 600;
        prefs.fullscreen = false;
        prefs.wintitle   = "V8 JavaScript - " + BlendToLoad;


        // Launch runtime
        this.runtime.start();
    }

    // ----------------------------------------------------------------------------
    this.OnLoad = function ()
    {
        this.scene      = GameLogic.getCurrentScene();
        this.mouse      = GameLogic.Mouse();
        this.keyboard   = GameLogic.Keyboard();
        this.camera     = new EditCamera(this.scene);
        this.mesh       = this.scene.getObject("MeshMomo").getEntity();
        this.curAction  = 0;
        // create a screen debugger
        this.debug      = GameLogic.Debugger(this.scene);
        this.debug.print("OgreKit.js Usage:");
        this.debug.print("    Left mouse button rotates camera.");
        this.debug.print("    Left mouse button plus shift key moves camera left or right.");
        this.debug.print("    Left mouse button plus ctrl key zooms camera.");
        this.debug.print("    Mouse.wheel(+) = next action.");
        this.debug.print("    Mouse.wheel(-) = prev action.");

    }

    // ----------------------------------------------------------------------------
    this.SetCallbacks = function(_loadfp, _upfp)
    {
        this.runtime.addListener(OgreKit.ON_LOAD, _loadfp);
        this.runtime.addListener(OgreKit.ON_TICK, _upfp);
    }


    // ----------------------------------------------------------------------------
    this.OnUpdate = function(rate)
    {
        this.mouse.capture();
        this.camera.update(this.mouse, this.keyboard);

        // cycle through actions (mouse.wheel+) = next, (mouse.wheel-) = prev

        if (this.mouse.wheel > 0)
        {
            this.curAction++;
            if (this.curAction > MomoActions.length - 1)
                this.curAction = 0;
        }
        if (this.mouse.wheel < 0)
        {
            this.curAction--;
            if (this.curAction < 0)
                this.curAction = MomoActions.length - 1;
        }

        this.mesh.playAction(MomoActions[this.curAction], 60);
    }
}

// ----------------------------------------------------------------------------
//
// Setup Main Application
//
// ----------------------------------------------------------------------------
var localApp = new App();

// setup callbacks
function OnLoad()       { localApp.OnLoad(); }
function OnUpdate(rate) { localApp.OnUpdate(rate); }

localApp.SetCallbacks(OnLoad, OnUpdate);
localApp.Run();
delete localApp;
