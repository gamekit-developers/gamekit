/*! \page Engine Core
\LuaSectionUp{index}


\beginmenu{Globals}
\LuaClassMenu{DebugPrint}
\endmenu

\beginmenu{Constants}
\LuaClassMenu{RenderSystem}
\LuaClassMenu{PropertyType}
\LuaClassMenu{TransformSpace}
\LuaClassMenu{GameObjectTypes}
\LuaClassMenu{EngineEvents}
\LuaClassMenu{MouseButton}
\LuaClassMenu{ScanCode}
\LuaClassMenu{ShadowType}
\endmenu


\beginmenu{Classes}
\LuaClassMenu{Camera}
\LuaClassMenu{Debugger}
\LuaClassMenu{Engine}
\LuaClassMenu{Entity}
\LuaClassMenu{GameObject}
\LuaClassMenu{Keyboard}
\LuaClassMenu{Loadable}
\LuaClassMenu{Mouse}
\LuaClassMenu{ObjectList}
\LuaClassMenu{Light}
\LuaClassMenu{Property}
\LuaClassMenu{Scene}
\LuaClassMenu{Skeleton}
\LuaClassMenu{UserDefs}
\endmenu


<!-- ============================================ Property ============================================ -->
\LuaClass{Property}
\LuaSectionUp{Engine}

\beginmenu{Methods}
\LuaMethodMenu{Property,constructor}
\LuaMethodMenu{Property,getName}
\LuaMethodMenu{Property,getValue}
\LuaMethodMenu{Property,makeDebug}
\LuaMethodMenu{Property,getType}
\LuaMethodMenu{Property,toBool}
\LuaMethodMenu{Property,toNumber}
\LuaMethodMenu{Property,toString}
\LuaMethodMenu{Property,fromBool}
\LuaMethodMenu{Property,fromNumber}
\LuaMethodMenu{Property,fromString}
\endmenu

\endpage


<!-- ======================================== -->
\LuaMethod{Property,constructor}
\LuaClassUp{Property}

Creates a nil property.

\code
function Property:constructor()
\endcode


<!-- ================= -->
\sectionseperator{Overload 1:}

Creates a new bool property.

\code
function Property:constructor(name, bool)
\endcode

\param name The property identifier.
\param bool The boolean value.


\endpage

<!-- ================= -->
\sectionseperator{Overload 2:}

Creates a new numerical property.

\code
function Property:constructor(name, number)
\endcode

\param name The property identifier.
\param number The numerical value.


\endpage


<!-- ================= -->
\sectionseperator{Overload 3:}

Creates a new string property.

\code
function Property:constructor(name, string)
\endcode

\param name The property identifier.
\param string The string value.

\endpage



<!-- ======================================== -->
\LuaMethod{Property,getName}
\LuaClassUp{Property}

Returns the property identifier.

\code
function Property:getName()
\endcode

\returns string.

\endpage



<!-- ======================================== -->
\LuaMethod{Property,getValue}
\LuaClassUp{Property}

Returns the internal string value.

\code
function Property:getValue()
\endcode

\returns string.

\endpage




<!-- ======================================== -->
\LuaMethod{Property,makeDebug}
\LuaClassUp{Property}

Adds this property to the render window.  

\code
function Property:makeDebug(val)
\endcode

\param val Add or remove.

\endpage



<!-- ======================================== -->
\LuaMethod{Property,getType}
\LuaClassUp{Property}

Returns the internal type. 

\code
function Property:getType()
\endcode

\returns \LuaClassRef{PropertyType}.

\endpage



<!-- ======================================== -->
\LuaMethod{Property,toBool}
\LuaClassUp{Property}

Return value in boolean form.

\code
function Property:toBool()
\endcode

\returns bool The new value.

\endpage



<!-- ======================================== -->
\LuaMethod{Property,toNumber}
\LuaClassUp{Property}

Return value in numerical form.

\code
function Property:toNumber()
\endcode

\returns number The new value.

\endpage


<!-- ======================================== -->
\LuaMethod{Property,toString}
\LuaClassUp{Property}

Return value in string form.

\code
function Property:toString()
\endcode

\returns string The new value.

\endpage


<!-- ======================================== -->
\LuaMethod{Property,fromBool}
\LuaClassUp{Property}

Applies value from a boolean value.

\code
function Property:fromBool(bool)
\endcode

\param bool The new value.

\endpage


<!-- ======================================== -->
\LuaMethod{Property,fromNumber}
\LuaClassUp{Property}

Applies value from a number.

\code
function Property:fromNumber(number)
\endcode

\param number The new value.

\endpage



<!-- ======================================== -->
\LuaMethod{Property,fromString}
\LuaClassUp{Property}

Applies value from string.

\code
function Property:fromString(string)
\endcode

\param string The new value.

\endpage


<!-- ============================================ UserDefs ============================================ -->
\LuaClass{UserDefs}
\LuaSectionUp{Engine}


\LuaGlobalRef{CDefs}


\beginmenu{Methods}
\LuaMethodMenu{UserDefs,getProperty}
\LuaMethodMenu{UserDefs,addProperty}
\LuaMethodMenu{UserDefs,hasProperty}
\endmenu

\beginmenu{Members}
\LuaMethodMenu{UserDefs,renderSystem}
\LuaMethodMenu{UserDefs,verbose}
\LuaMethodMenu{UserDefs,winx}
\LuaMethodMenu{UserDefs,winy}
\LuaMethodMenu{UserDefs,winTitle}
\LuaMethodMenu{UserDefs,fullScreen}
\LuaMethodMenu{UserDefs,blenderMat}
\LuaMethodMenu{UserDefs,grabInput}
\LuaMethodMenu{UserDefs,debugFps}
\LuaMethodMenu{UserDefs,debugPhysics}
\LuaMethodMenu{UserDefs,debugPhysicsAABB}
\LuaMethodMenu{UserDefs,debugSounds}
\LuaMethodMenu{UserDefs,showDebugProps}
\LuaMethodMenu{UserDefs,enableShadows}
\LuaMethodMenu{UserDefs,shadowTechnique}
\LuaMethodMenu{UserDefs,colourShadowR}
\LuaMethodMenu{UserDefs,colourShadowG}
\LuaMethodMenu{UserDefs,colourShadowB}
\LuaMethodMenu{UserDefs,farDistanceShadow}
\endmenu


\beginmenu{Operators}
\LuaMethodMenu{UserDefs,__index}
\LuaMethodMenu{UserDefs,__newindex}
\endmenu


\endpage



<!-- ======================================== -->
\LuaMethod{UserDefs,getProperty}
\LuaClassUp{UserDefs}

Finds a preexisting property.

\code
function UserDefs:getProperty(name)
\endcode

\param name lookup.
\returns \LuaClassRef{Property}.


\endpage


<!-- ======================================== -->
\LuaMethod{UserDefs,addProperty}
\LuaClassUp{UserDefs}

Adds a new user defined property.

\code
function UserDefs:addProperty(property)
\endcode

\param property \LuaClassRef{Property}.

\endpage


<!-- ======================================== -->
\LuaMethod{UserDefs,hasProperty}
\LuaClassUp{UserDefs}

Looks for a preexisting property.

\code
function UserDefs:hasProperty(name)
\endcode

\param name lookup.
\returns bool


\endpage



<!-- ======================================== -->
\LuaMethod{UserDefs,renderSystem}
\LuaClassUp{UserDefs}

The render system to use.

\code
number UserDefs.renderSystem
\endcode

\see \LuaClassRef{RenderSystem}.

\endpage


<!-- ======================================== -->
\LuaMethod{UserDefs,verbose}
\LuaClassUp{UserDefs}

Enables full Ogre log messages.

\code
bool UserDefs.verbose
\endcode

\endpage



<!-- ======================================== -->
\LuaMethod{UserDefs,winx}
\LuaClassUp{UserDefs}

Window width.

\code
number UserDefs.winx
\endcode

\endpage



<!-- ======================================== -->
\LuaMethod{UserDefs,winy}
\LuaClassUp{UserDefs}

Window height.

\code
number UserDefs.winy
\endcode

\endpage



<!-- ======================================== -->
\LuaMethod{UserDefs,winTitle}
\LuaClassUp{UserDefs}

Window display text.

\code
string UserDefs.winTitle
\endcode

\endpage



<!-- ======================================== -->
\LuaMethod{UserDefs,fullScreen}
\LuaClassUp{UserDefs}

Enable fullscreen rendering.

\code
bool UserDefs.fullScreen
\endcode

\endpage




<!-- ======================================== -->
\LuaMethod{UserDefs,blenderMat}
\LuaClassUp{UserDefs}

Convert blender mesh objects basied on the assigned material index.
Opposed to the face texture / UV layers. Also uses the blender material for the mesh material. 

\code
bool UserDefs.blenderMat
\endcode

\endpage

<!-- ======================================== -->
\LuaMethod{UserDefs,grabInput}
\LuaClassUp{UserDefs}

Places the system mouse in the center of the render window, and hides it.

\code
bool UserDefs.grabInput
\endcode

\endpage

<!-- ======================================== -->
\LuaMethod{UserDefs,debugFps}
\LuaClassUp{UserDefs}

Enables frame rate / profiling print out on the render window.

\code
bool UserDefs.debugFps
\endcode

\endpage


<!-- ======================================== -->
\LuaMethod{UserDefs,debugPhysics}
\LuaClassUp{UserDefs}

Draw physics shapes and contact points.

\code
bool UserDefs.debugPhysics
\endcode

\endpage



<!-- ======================================== -->
\LuaMethod{UserDefs,debugPhysicsAABB}
\LuaClassUp{UserDefs}

Draw the axis aligned bounding box on physics enabled objects.

\code
bool UserDefs.debugPhysicsAABB
\endcode

\endpage



<!-- ======================================== -->
\LuaMethod{UserDefs,debugSounds}
\LuaClassUp{UserDefs}

Draw sound playback visualization.


\code
bool UserDefs.debugSounds
\endcode

\endpage


<!-- ======================================== -->
\LuaMethod{UserDefs,showDebugProps}
\LuaClassUp{UserDefs}

Show properties attached to game objects.

\code
bool UserDefs.showDebugProps
\endcode

\note The property in must have the debug flag enabled in the Blender UI.

\LuaSee{Property,makeDebug}
\endpage


<!-- ======================================== -->
\LuaMethod{UserDefs,enableShadows}
\LuaClassUp{UserDefs}

Enables Ogre shadow rendering.

\code
bool UserDefs.enableShadows
\endcode

\endpage


<!-- ======================================== -->
\LuaMethod{UserDefs,shadowTechnique}
\LuaClassUp{UserDefs}

Sets the shadow render technique.

\code
string UserDefs.shadowTechnique
\endcode

\see \LuaClassRef{ShadowType}

\endpage

<!-- ======================================== -->
\LuaMethod{UserDefs,colourShadowR}
\LuaClassUp{UserDefs}

Sets the shadows red component.

\code
number UserDefs.colourShadowR
\endcode

\endpage

<!-- ======================================== -->
\LuaMethod{UserDefs,colourShadowG}
\LuaClassUp{UserDefs}

Sets the shadows green component.

\code
number UserDefs.colourShadowG
\endcode

\endpage

<!-- ======================================== -->
\LuaMethod{UserDefs,colourShadowB}
\LuaClassUp{UserDefs}

Sets the shadows blue component.

\code
number UserDefs.colourShadowB
\endcode

\endpage

<!-- ======================================== -->
\LuaMethod{UserDefs,farDistanceShadow}
\LuaClassUp{UserDefs}

Sets the shadows distance.

\code
number UserDefs.farDistanceShadow
\endcode


\endpage





<!-- ======================================== -->
\LuaMethod{UserDefs,__index}
\LuaClassUp{UserDefs}


Index meta method.

\code
function UserDefs:__index(name)
\endcode

\param name lookup.
\returns string, bool, or number. Depending on the type of name.


\code
-- Is equal to
local val = UserDefs.someValue
\endcode

\endpage




<!-- ======================================== -->
\LuaMethod{UserDefs,__newindex}
\LuaClassUp{UserDefs}


New index meta method.

\code
function UserDefs:__newindex(name, bool)
\endcode

\param name lookup.
\param bool The boolean value to set.


\code
-- Is equal to
UserDefs.someValue = true
\endcode


<!-- ================= -->
\sectionseperator{Overload 1:}


\code
function UserDefs:__newindex(name, number)
\endcode

\param name lookup.
\param number The numerical value to set.


\code
-- Is equal to
UserDefs.someValue = 123
\endcode


<!-- ================= -->
\sectionseperator{Overload 2:}


\code
function UserDefs:__newindex(name, string)
\endcode

\param name lookup.
\param string The string value to set.


\code
-- Is equal to
UserDefs.someValue = "abc"
\endcode


\endpage


<!-- ============================================ Mouse ============================================ -->
\LuaClass{Mouse}
\LuaSectionUp{Engine}

\beginmenu{Methods}
\LuaMethodMenu{Mouse,constructor}
\LuaMethodMenu{Mouse,capture}
\LuaMethodMenu{Mouse,isButtonDown}
\endmenu



\beginmenu{Members}
\LuaMethodMenu{Mouse,xpos}
\LuaMethodMenu{Mouse,ypos}
\LuaMethodMenu{Mouse,xrel}
\LuaMethodMenu{Mouse,yrel}
\LuaMethodMenu{Mouse,winx}
\LuaMethodMenu{Mouse,winy}
\LuaMethodMenu{Mouse,wheel}
\LuaMethodMenu{Mouse,moved}
\endmenu

\endpage





<!-- ======================================== -->
\LuaMethod{Mouse,constructor}
\LuaClassUp{Mouse}

Creates a mouse to test mouse events.


\code
function Mouse:constructor()
\endcode

\endpage


<!-- ======================================== -->
\LuaMethod{Mouse,capture}
\LuaClassUp{Mouse}

Updates the current state of the device.

\code
function Mouse:capture()
\endcode

\endpage



<!-- ======================================== -->
\LuaMethod{Mouse,isButtonDown}
\LuaClassUp{Mouse}

Returns true if the specified code is active.

\code
function Mouse:isButtonDown(code)
\endcode


\param code \LuaClassRef{MouseButton}.
\returns bool.


\endpage


<!-- ======================================== -->
\LuaMethod{Mouse,xpos}
\LuaClassUp{Mouse}

Absolute X position of the mouse within the window.

\code
number Mouse.xpos
\endcode

\endpage

<!-- ======================================== -->
\LuaMethod{Mouse,ypos}
\LuaClassUp{Mouse}

Absolute Y position of the mouse within the window.

\code
number Mouse.ypos
\endcode

\endpage


<!-- ======================================== -->
\LuaMethod{Mouse,xrel}
\LuaClassUp{Mouse}

Relitve X direction of the mouse within the window.

\code
number Mouse.xrel
\endcode

\endpage



<!-- ======================================== -->
\LuaMethod{Mouse,yrel}
\LuaClassUp{Mouse}

Relitve Y direction of the mouse within the window.

\code
number Mouse.yrel
\endcode

\endpage



<!-- ======================================== -->
\LuaMethod{Mouse,winx}
\LuaClassUp{Mouse}

Window width.

\code
number Mouse.winx
\endcode

\endpage



<!-- ======================================== -->
\LuaMethod{Mouse,winy}
\LuaClassUp{Mouse}

Window height.

\code
number Mouse.winy
\endcode

\endpage

<!-- ======================================== -->
\LuaMethod{Mouse,wheel}
\LuaClassUp{Mouse}

Mouse wheel delta 

\code
number Mouse.wheel
\endcode

\endpage



<!-- ======================================== -->
\LuaMethod{Mouse,moved}
\LuaClassUp{Mouse}


True if the mouse has changed position.

\code
bool Mouse.moved
\endcode

\endpage




<!-- ============================================ ObjectList ============================================ -->
\LuaClass{ObjectList}
\LuaSectionUp{Engine}
ObjectList is an \LuaClassRef{Array} template implementation where T is equal to \LuaClassRef{GameObject}


\LuaGlobalRef{CArrayIter}

\endpage

<!-- ============================================ Keyboard ============================================ -->
\LuaClass{Keyboard}
\LuaSectionUp{Engine}

\beginmenu{Methods}
\LuaMethodMenu{Keyboard,constructor}
\LuaMethodMenu{Keyboard,isKeyDown}
\endmenu


\endpage


<!-- ======================================== -->
\LuaMethod{Keyboard,constructor}
\LuaClassUp{Keyboard}

Creates a keyboard to test key press events.


\code
function Keyboard:constructor()
\endcode


\endpage



<!-- ======================================== -->
\LuaMethod{Keyboard,isKeyDown}
\LuaClassUp{Keyboard}

Returns true if the specified scan code is active.

\code
function Keyboard:isKeyDown(code)
\endcode

\param code \LuaClassRef{ScanCode} key code.
\returns bool.


\endpage


<!-- ============================================ Engine ============================================ -->
\LuaClass{Engine}
\LuaSectionUp{Engine}

\beginmenu{Methods}
\LuaMethodMenu{Engine,constructor}
\LuaMethodMenu{Engine,initialize}
\LuaMethodMenu{Engine,run}
\LuaMethodMenu{Engine,requestExit}
\LuaMethodMenu{Engine,connect}
\LuaMethodMenu{Engine,loadBlendFile}
\LuaMethodMenu{Engine,getUserDefs}
\endmenu

\endpage



<!-- ======================================== -->
\LuaMethod{Engine,constructor}
\LuaClassUp{Engine}

Create or get an instance of the engine. 

\code
function Engine:constructor()
\endcode


\endpage

<!-- ======================================== -->
\LuaMethod{Engine,initialize}
\LuaClassUp{Engine}

Initialize the engine.

\code
function Engine:initialize()
\endcode


\endpage



<!-- ======================================== -->
\LuaMethod{Engine,run}
\LuaClassUp{Engine}

Run the engine loop.

\code
function Engine:run()
\endcode


\endpage


<!-- ======================================== -->
\LuaMethod{Engine,requestExit}
\LuaClassUp{Engine}

Sends an exit signal.


\code
function Engine:requestExit()
\endcode


\endpage


<!-- ======================================== -->
\LuaMethod{Engine,connect}
\LuaClassUp{Engine}

Connects an event listener.

\code
function Engine:connect(type, func)
\endcode


\param type \LuaClassRef{EngineEvents} as the event to listen to.
\param func Global callback function.



<!-- ================= -->
\sectionseperator{Overload:}


\code
function Engine:connect(type, table, func)
\endcode


\param type \LuaClassRef{EngineEvents} as the event to listen to.
\param table    Pointer to a Lua table object, ie; self 
\param function Pointer to a Lua function, method of self. 


\endpage




<!-- ======================================== -->
\LuaMethod{Engine,loadBlendFile}
\LuaClassUp{Engine}

Returns the active scene in the .blend

\code
function Engine:loadBlendFile(name)
\endcode


\param name Path to the blend file.
\returns \LuaClassRef{Scene}

\note The returned scene is \b not loaded.

\endpage


<!-- ======================================== -->
\LuaMethod{Engine,getUserDefs}
\LuaClassUp{Engine}

Returns the user defined options class. 

\code
function Engine:getUserDefs()
\endcode


\returns \LuaClassRef{UserDefs}

\endpage

<!-- ============================================ Loadable ============================================ -->
\LuaClass{Loadable}
\LuaSectionUp{Engine}



\sectionseperator{Loadable Inheritance:}

\dot
digraph Parent_Child_Loadable
{
    node [shape="box", fontname="Verdana",fontsize="10",labelfontname="Verdana",labelfontsize="10" height=".13"];


    Loadable        [style="filled", fillcolor="#ECEBE8", color="#93B300" fontcolor="#000000"];
    Scene           [URL="\ref OgreKit_Scene", color="#C0C0C0", fontcolor="#444A26"];
    GameObject      [URL="\ref OgreKit_GameObject", color="#C0C0C0", fontcolor="#444A26"];
    Camera          [URL="\ref OgreKit_Camera", color="#C0C0C0", fontcolor="#444A26"];
    Entity          [URL="\ref OgreKit_Entity", color="#C0C0C0" fontcolor="#444A26"];
    Light           [URL="\ref OgreKit_Light", color="#C0C0C0" fontcolor="#444A26"];
    Skeleton        [URL="\ref OgreKit_Skeleton", color="#C0C0C0" fontcolor="#444A26"];


    Loadable -> GameObject;
    Loadable -> Scene;
    GameObject -> Camera;
	GameObject -> Entity;
	GameObject -> Light;
	GameObject -> Skeleton;
} 
\enddot

\beginmenu{Methods}
\LuaMethodMenu{Loadable,load}
\LuaMethodMenu{Loadable,unload}
\LuaMethodMenu{Loadable,reload}
\LuaMethodMenu{Loadable,getName}
\endmenu


\endpage




<!-- ======================================== -->
\LuaMethod{Loadable,load}
\LuaClassUp{Loadable}

Adds this object.

\code
function Loadable:load()
\endcode

\endpage


<!-- ======================================== -->
\LuaMethod{Loadable,unload}
\LuaClassUp{Loadable}

Removes this object.

\code
function Loadable:unload()
\endcode

\endpage


<!-- ======================================== -->
\LuaMethod{Loadable,reload}
\LuaClassUp{Loadable}

Removes this object, then adds it back.

\code
function Loadable:reload()
\endcode

\endpage


<!-- ======================================== -->
\LuaMethod{Loadable,getName}
\LuaClassUp{Loadable}

Returns the string identifier of this object.

\code
function Loadable:getName()
\endcode

\returns string

\endpage

<!-- ============================================ Scene ============================================ -->
\LuaClass{Scene}
\LuaSectionUp{Engine}


\sectionseperator{Scene Inheritance:}

\dot
digraph Parent_Child_Scene
{
    node [shape="box", fontname="Verdana",fontsize="10",labelfontname="Verdana",labelfontsize="10" height=".13"];

    Loadable        [URL="\ref OgreKit_Loadable", color="#C0C0C0", fontcolor="#444A26"];
    Scene           [style="filled", fillcolor="#ECEBE8", color="#93B300" fontcolor="#000000"];

    Loadable -> Scene;
} 
\enddot


\beginmenu{Methods}
\LuaMethodMenu{Scene,createEmpty}
\LuaMethodMenu{Scene,getCamera}
\LuaMethodMenu{Scene,getEntity}
\LuaMethodMenu{Scene,getLight}
\LuaMethodMenu{Scene,getObject}
\LuaMethodMenu{Scene,getObjectList}
\LuaMethodMenu{Scene,getSkeleton}
\LuaMethodMenu{Scene,hasObject}

\endmenu


\endpage




<!-- ======================================== -->
\LuaMethod{Scene,hasObject}
\LuaClassUp{Scene}


Returns true if the scene contains the specified object.

\code
function Scene:hasObject(name)
\endcode

\param name Find by the object's identifier.
\return bool
\endpage

<!-- ======================================== -->
\LuaMethod{Scene,getObject}
\LuaClassUp{Scene}



Looks for an generic game object in the scene.

\code
function Scene:getObject(name)
\endcode

\param name Find by the object's identifier.
\returns \LuaClassRef{GameObject} or nil if the object is not found.

\endpage


<!-- ======================================== -->
\LuaMethod{Scene,getObjectList}
\LuaClassUp{Scene}

Returns a list of all loaded objects in the scene.

\code
function Scene:getObjectList()
\endcode

\returns \LuaClassRef{ObjectList}

\endpage


<!-- ======================================== -->
\LuaMethod{Scene,getEntity}
\LuaClassUp{Scene}



Looks for an entity object in the scene.

\code
function Scene:getEntity(name)
\endcode

\param name Find by the object's identifier.
\returns \LuaClassRef{Entity} or nil if the object is not found.

\endpage

<!-- ======================================== -->
\LuaMethod{Scene,getCamera}
\LuaClassUp{Scene}



Looks for a camera object in the scene.

\code
function Scene:getCamera(name)
\endcode

\param name Find by the object's identifier.
\returns \LuaClassRef{Camera} or nil if the object is not found.

\endpage

<!-- ======================================== -->
\LuaMethod{Scene,getLight}
\LuaClassUp{Scene}



Looks for a light object in the scene.

\code
function Scene:getLight(name)
\endcode

\param name Find by the object's identifier.
\returns \LuaClassRef{Light} or nil if the object is not found.

\endpage


<!-- ======================================== -->
\LuaMethod{Scene,getSkeleton}
\LuaClassUp{Scene}

Looks for a skeleton/armature object in the scene.

\code
function Scene:getSkeleton(name)
\endcode

\param name Find by the object's identifier.
\returns \LuaClassRef{Skeleton} or nil if the object is not found.

\endpage


<!-- ======================================== -->
\LuaMethod{Scene,createEmpty}
\LuaClassUp{Scene}

Creates an empty node.

\code
function Scene:createEmpty(name)
\endcode

\param name Unique name of the empty node.
\return \LuaClassRef{GameObject}

\endpage


<!-- ============================================ GameObject ============================================ -->
\LuaClass{GameObject}
\LuaSectionUp{Engine}


\sectionseperator{GameObject Inheritance:}


\dot
digraph Parent_Child_GameObject
{
    node [shape="box", fontname="Verdana",fontsize="10",labelfontname="Verdana",labelfontsize="10" height=".13"];
    Loadable        [URL="\ref OgreKit_Loadable", color="#C0C0C0", fontcolor="#444A26"];
    GameObject      [style="filled", fillcolor="#ECEBE8", color="#93B300" fontcolor="#000000"];
    Camera          [URL="\ref OgreKit_Camera", color="#C0C0C0", fontcolor="#444A26"];
    Entity          [URL="\ref OgreKit_Entity", color="#C0C0C0" fontcolor="#444A26"];
    Light           [URL="\ref OgreKit_Light", color="#C0C0C0" fontcolor="#444A26"];
    Skeleton        [URL="\ref OgreKit_Skeleton", color="#C0C0C0" fontcolor="#444A26"];

    Loadable -> GameObject;
    GameObject -> Camera;
	GameObject -> Entity;
	GameObject -> Light;
	GameObject -> Skeleton;
} 
\enddot

\beginmenu{Methods}
\LuaMethodMenu{GameObject,getPosition}
\LuaMethodMenu{GameObject,getRotation}
\LuaMethodMenu{GameObject,getOrientation}
\LuaMethodMenu{GameObject,getScale}
\LuaMethodMenu{GameObject,getWorldPosition}
\LuaMethodMenu{GameObject,getWorldRotation}
\LuaMethodMenu{GameObject,getWorldOrientation}
\LuaMethodMenu{GameObject,getLinearVelocity}
\LuaMethodMenu{GameObject,getAngularVelocity}
\LuaMethodMenu{GameObject,setLinearVelocity}
\LuaMethodMenu{GameObject,setAngularVelocity}
\LuaMethodMenu{GameObject,setPosition}
\LuaMethodMenu{GameObject,setRotation}
\LuaMethodMenu{GameObject,setOrientation}
\LuaMethodMenu{GameObject,getType}
\LuaMethodMenu{GameObject,rotate}
\LuaMethodMenu{GameObject,translate}
\LuaMethodMenu{GameObject,scale}
\LuaMethodMenu{GameObject,yaw}
\LuaMethodMenu{GameObject,pitch}
\LuaMethodMenu{GameObject,roll}
\LuaMethodMenu{GameObject,getState}
\LuaMethodMenu{GameObject,getEntity}
\LuaMethodMenu{GameObject,getCamera}
\LuaMethodMenu{GameObject,getSkeleton}
\LuaMethodMenu{GameObject,getLight}
\LuaMethodMenu{GameObject,hasParent}
\LuaMethodMenu{GameObject,setParent}
\LuaMethodMenu{GameObject,getParent}
\LuaMethodMenu{GameObject,getScene}
\endmenu


\endpage


<!-- ======================================== -->
\LuaMethod{GameObject,getPosition}
\LuaClassUp{GameObject}


Returns the local position if this object is part of a parent / child hierarchy.
otherwise returns the world position. 

\code
function GameObject:getPosition()
\endcode

\returns \LuaClassRef{Vector3}

\endpage




<!-- ======================================== -->
\LuaMethod{GameObject,getRotation}
\LuaClassUp{GameObject}


Returns the local rotation if this object is part of a parent / child hierarchy.
otherwise returns the world rotation. 

\code
function GameObject:getRotation()
\endcode

\returns \LuaClassRef{Vector3} as Euler, yaw, pitch, roll 

\endpage



<!-- ======================================== -->
\LuaMethod{GameObject,getOrientation}
\LuaClassUp{GameObject}


Returns the local orientation if this object is part of a parent / child hierarchy.
otherwise returns the world orientation. 

\code
function GameObject:getOrientation()
\endcode

\returns \LuaClassRef{Quaternion}

\endpage



<!-- ======================================== -->
\LuaMethod{GameObject,getScale}
\LuaClassUp{GameObject}


Returns the objects scale.


\code
function GameObject:getScale()
\endcode

\returns \LuaClassRef{Vector3}

\endpage


<!-- ======================================== -->
\LuaMethod{GameObject,getWorldPosition}
\LuaClassUp{GameObject}


Returns the combined position of the object within the parent / child hierarchy.


\code
function GameObject:getWorldPosition()
\endcode

\returns \LuaClassRef{Vector3}

\endpage







<!-- ======================================== -->
\LuaMethod{GameObject,getWorldRotation}
\LuaClassUp{GameObject}


Returns the combined rotation of the object within the parent / child hierarchy.

\code
function GameObject:getWorldRotation()
\endcode

\returns \LuaClassRef{Vector3} as Euler, yaw, pitch, roll 

\endpage


<!-- ======================================== -->
\LuaMethod{GameObject,getWorldOrientation}
\LuaClassUp{GameObject}


Returns the combined orientation of the object within the parent / child hierarchy.

\code
function GameObject:getWorldOrientation()
\endcode

\returns \LuaClassRef{Quaternion}

\endpage




<!-- ======================================== -->
\LuaMethod{GameObject,getLinearVelocity}
\LuaClassUp{GameObject}

Returns the linear velocity of the game object.

\code
function GameObject:getLinearVelocity()
\endcode

\returns \LuaClassRef{Vector3}

\endpage


<!-- ======================================== -->
\LuaMethod{GameObject,getAngularVelocity}
\LuaClassUp{GameObject}

Returns the angular velocity of the game object.

\code
function GameObject:getAngularVelocity()
\endcode

\returns \LuaClassRef{Vector3}

\endpage





<!-- ======================================== -->
\LuaMethod{GameObject,setLinearVelocity}
\LuaClassUp{GameObject}

Sets the linear velocity of the game object.

\code
function GameObject:setLinearVelocity(vec)
\endcode

\param vec \LuaClassRef{Vector3} as the new velocity.


<!-- ================= -->
\sectionseperator{Overload:}

\code
function GameObject:setLinearVelocity(x, y, z)
\endcode

\param x x component.
\param y y component.
\param z z component.

\endpage


<!-- ======================================== -->
\LuaMethod{GameObject,setAngularVelocity}
\LuaClassUp{GameObject}

Sets the angular velocity of the game object.

\code
function GameObject:setAngularVelocity(vec)
\endcode

\param vec \LuaClassRef{Vector3} as the new velocity.


<!-- ================= -->
\sectionseperator{Overload:}

\code
function GameObject:setAngularVelocity(x, y, z)
\endcode

\param x x component.
\param y y component.
\param z z component.

\endpage






<!-- ======================================== -->
\LuaMethod{GameObject,setPosition}
\LuaClassUp{GameObject}

Sets the position of the game object.


\code
function GameObject:setPosition(pos)
\endcode

\param pos \LuaClassRef{Vector3} as the new position.


<!-- ================= -->
\sectionseperator{Overload:}

\code
function GameObject:setPosition(x, y, z)
\endcode

\param x x component.
\param y y component.
\param z z component.

\endpage




<!-- ======================================== -->
\LuaMethod{GameObject,setRotation}
\LuaClassUp{GameObject}

Sets the rotation of the game object.


\code
function GameObject:setRotation(eul)
\endcode

\param eul \LuaClassRef{Vector3} as the new rotation in degrees.

<!-- ================= -->
\sectionseperator{Overload:}

\code
function GameObject:setRotation(yaw, pitch, roll)
\endcode

\param yaw x component in degrees.
\param pitch y component in degrees.
\param roll z component in degrees.


\endpage


<!-- ======================================== -->
\LuaMethod{GameObject,setOrientation}
\LuaClassUp{GameObject}

Sets the orientation of the game object.


\code
function GameObject:setOrientation(quat)
\endcode

\param quat \LuaClassRef{Quaternion} as the new rotation.

<!-- ================= -->
\sectionseperator{Overload:}

\code
function GameObject:setOrientation(w, x, y, z)
\endcode

\param w w component.
\param x x component.
\param y y component.
\param z z component.


\endpage

<!-- ======================================== -->
\LuaMethod{GameObject,rotate}
\LuaClassUp{GameObject}

Rotates the game object.

\code
function GameObject:rotate(yaw,pitch,roll, ts=OgreKit.TS_PARENT)
\endcode

\param yaw x rotation in degrees.
\param pitch y rotation in degrees.
\param roll z rotation in degrees.
\param ts \LuaClassRef{TransformSpace}



<!-- ================= -->
\sectionseperator{Overload 1:}


\code
function GameObject:rotate(eul, ts=OgreKit.TS_PARENT)
\endcode
\param eul \LuaClassRef{Vector3} as yaw, pitch, and roll in degrees.
\param ts \LuaClassRef{TransformSpace}


<!-- ================= -->
\sectionseperator{Overload 2:}

\code
function GameObject:rotate(quat, ts=OgreKit.TS_PARENT)
\endcode
\param quat \LuaClassRef{Quaternion} 
\param ts \LuaClassRef{TransformSpace}


\endpage

<!-- ======================================== -->
\LuaMethod{GameObject,translate}
\LuaClassUp{GameObject}

Translate the game object.

\code
function GameObject:translate(val, ts=OgreKit.TS_PARENT)
\endcode

\param val \LuaClassRef{Vector3} as the delta translation
\param ts \LuaClassRef{TransformSpace}

<!-- ================= -->
\sectionseperator{Overload:}


\code
function GameObject:translate(x, y, z, ts=OgreKit.TS_PARENT)
\endcode
\param x x-axis component.
\param y y-axis component.
\param z z-axis component.
\param ts \LuaClassRef{TransformSpace}

\endpage


<!-- ======================================== -->
\LuaMethod{GameObject,scale}
\LuaClassUp{GameObject}

Scale the game object.

\code
function GameObject:scale(val)
\endcode

\param val \LuaClassRef{Vector3} as scale


<!-- ================= -->
\sectionseperator{Overload:}


\code
function GameObject:scale(x, y, z)
\endcode

\param x x-axis component.
\param y y-axis component.
\param z z-axis component.

\endpage




<!-- ======================================== -->
\LuaMethod{GameObject,yaw}
\LuaClassUp{GameObject}

Rotate object around the x axis.

\code
function GameObject:yaw(val, ts=OgreKit.TS_PARENT)
\endcode

\param val Rotation in degrees.
\param ts \LuaClassRef{TransformSpace}

\endpage


<!-- ======================================== -->
\LuaMethod{GameObject,pitch}
\LuaClassUp{GameObject}

Rotate object around the y axis.

\code
function GameObject:pitch(val, ts=OgreKit.TS_PARENT)
\endcode

\param val Rotation in degrees.
\param ts \LuaClassRef{TransformSpace}


\endpage

<!-- ======================================== -->
\LuaMethod{GameObject,roll}
\LuaClassUp{GameObject}

Rotate object around the z axis.

\code
function GameObject:roll(val, ts=OgreKit.TS_PARENT)
\endcode

\param val Rotation in degrees.
\param ts \LuaClassRef{TransformSpace}


\endpage


<!-- ======================================== -->
\LuaMethod{GameObject,getState}
\LuaClassUp{GameObject}


Returns the logic brick state.

\code
function GameObject:getState()
\endcode

\returns number

\endpage


<!-- ======================================== -->
\LuaMethod{GameObject,getState}
\LuaClassUp{GameObject}


Returns the logic brick state.

\code
function GameObject:getState()
\endcode

\returns number

\endpage


<!-- ======================================== -->
\LuaMethod{GameObject,getEntity}
\LuaClassUp{GameObject}


Returns the entity data associated with this object.

\code
function GameObject:getEntity()
\endcode

\returns \LuaClassRef{Entity} or nil if this object is not an entity.

\endpage



<!-- ======================================== -->
\LuaMethod{GameObject,getCamera}
\LuaClassUp{GameObject}


Returns the camera data associated with this object.

\code
function GameObject:getCamera()
\endcode

\returns \LuaClassRef{Camera} or nil if this object is not a camera.

\endpage



<!-- ======================================== -->
\LuaMethod{GameObject,getSkeleton}
\LuaClassUp{GameObject}


Returns the skeleton data associated with this object.

\code
function GameObject:getSkeleton()
\endcode

\returns \LuaClassRef{Skeleton} or nil if this object is not a skeleton.

\endpage


<!-- ======================================== -->
\LuaMethod{GameObject,getLight}
\LuaClassUp{GameObject}


Returns the light data associated with this object.

\code
function GameObject:getLight()
\endcode

\returns \LuaClassRef{Light} or nil if this object is not a light.

\endpage


<!-- ======================================== -->
\LuaMethod{GameObject,hasParent}
\LuaClassUp{GameObject}

Returns true if the object is part of a parent/child hierarchy. 

\code
function GameObject:hasParent()
\endcode

\returns bool

\endpage


<!-- ======================================== -->
\LuaMethod{GameObject,setParent}
\LuaClassUp{GameObject}

Sets the parent game object.

\code
function GameObject:setParent(object)
\endcode

\param object \LuaClassRef{GameObject} as the new parent.

\endpage



<!-- ======================================== -->
\LuaMethod{GameObject,getParent}
\LuaClassUp{GameObject}

Returns the parent game object.


\code
function GameObject:getParent()
\endcode


\returns \LuaClassRef{GameObject} or nil if no parent.


\endpage


<!-- ======================================== -->
\LuaMethod{GameObject,getScene}
\LuaClassUp{GameObject}

Returns the owning scene

\code
function GameObject:getScene()
\endcode


\returns \LuaClassRef{Scene}

\endpage




<!-- ======================================== -->
\LuaMethod{GameObject,getType}
\LuaClassUp{GameObject}


Returns the sub class type.

\code
function GameObject:getType()
\endcode

\returns  \LuaClassRef{GameObjectTypes}

\endpage




<!-- ============================================ Camera ============================================ -->
\LuaClass{Camera}
\LuaSectionUp{Engine}


\sectionseperator{Camera Inheritance:}


\dot
digraph Parent_Child_Camera
{
    node [shape="box", fontname="Verdana",fontsize="10",labelfontname="Verdana",labelfontsize="10" height=".13"];
    Loadable        [URL="\ref OgreKit_Loadable", color="#C0C0C0", fontcolor="#444A26"];
    GameObject      [URL="\ref OgreKit_GameObject", color="#C0C0C0", fontcolor="#444A26"];
    Camera          [style="filled", fillcolor="#ECEBE8", color="#93B300" fontcolor="#000000"];

    Loadable -> GameObject -> Camera;
} 
\enddot


\beginmenu{Methods}
\LuaMethodMenu{Camera,setClipping}
\LuaMethodMenu{Camera,getClipStart}
\LuaMethodMenu{Camera,getClipEnd}
\LuaMethodMenu{Camera,setFov}
\LuaMethodMenu{Camera,getFov}
\LuaMethodMenu{Camera,makeCurrent}
\endmenu


\endpage


<!-- ======================================== -->
\LuaMethod{Camera,setClipping}
\LuaClassUp{Camera}

Sets the cameras near and far clip distances.


\code
function Camera:setClipping(start, end)
\endcode

\param start Near clip distance.
\param end Far clip distance.


\endpage


<!-- ======================================== -->
\LuaMethod{Camera,getClipStart}
\LuaClassUp{Camera}

Gets the cameras near distance.

\code
function Camera:getClipStart()
\endcode

\returns number

\endpage



<!-- ======================================== -->
\LuaMethod{Camera,getClipEnd}
\LuaClassUp{Camera}

Gets the cameras far distance.

\code
function Camera:getClipEnd()
\endcode

\returns number

\endpage

<!-- ======================================== -->
\LuaMethod{Camera,setFov}
\LuaClassUp{Camera}

Sets the field of view.

\code
function Camera:setFov(fov)
\endcode

\param fov number value expressed in degrees.

\endpage


<!-- ======================================== -->
\LuaMethod{Camera,getFov}
\LuaClassUp{Camera}

Gets the field of view.

\code
function Camera:getFov()
\endcode

\returns number

\endpage



<!-- ======================================== -->
\LuaMethod{Camera,makeCurrent}
\LuaClassUp{Camera}

Sets this camera as the viewing camera for the scene. 

\code
function Camera:makeCurrent()
\endcode


\endpage

<!-- ============================================ Entity ============================================ -->
\LuaClass{Entity}
\LuaSectionUp{Engine}

\sectionseperator{Entity Inheritance:}


\dot
digraph Parent_Child_Entity
{
    node [shape="box", fontname="Verdana",fontsize="10",labelfontname="Verdana",labelfontsize="10" height=".13"];
    Loadable        [URL="\ref OgreKit_Loadable", color="#C0C0C0", fontcolor="#444A26"];
    GameObject      [URL="\ref OgreKit_GameObject",color="#C0C0C0", fontcolor="#444A26"];
    Entity          [style="filled", fillcolor="#ECEBE8", color="#93B300" fontcolor="#000000"];

    Loadable -> GameObject -> Entity;
} 
\enddot


\beginmenu{Methods}
\LuaMethodMenu{Entity,playAction}
\endmenu



\endpage

<!-- ======================================== -->
\LuaMethod{Entity,playAction}
\LuaClassUp{Entity}

Plays an action object. 


\code
function Entity:playAction(name, blend)
\endcode

\param name Identifier of the action.
\param blend The number of blend-in frames, if a previous action is playing.


\endpage


<!-- ============================================ Light ============================================ -->
\LuaClass{Light}
\LuaSectionUp{Engine}

\sectionseperator{Light Inheritance:}


\dot
digraph Parent_Child_Light
{
    node [shape="box", fontname="Verdana",fontsize="10",labelfontname="Verdana",labelfontsize="10" height=".13"];

    Loadable        [URL="\ref OgreKit_Loadable", color="#C0C0C0", fontcolor="#444A26"];
    GameObject      [URL="\ref OgreKit_GameObject", color="#C0C0C0", fontcolor="#444A26"];
    Light           [style="filled", fillcolor="#ECEBE8", color="#93B300" fontcolor="#000000"];

    Loadable -> GameObject -> Light;
} 
\enddot


<b>Placeholder:</b> \n\n
Implement access.

\endpage

<!-- ============================================ Skeleton ============================================ -->
\LuaClass{Skeleton}
\LuaSectionUp{Engine}


\sectionseperator{Skeleton Inheritance:}


\dot
digraph Parent_Child_Skeleton
{
    node [shape="box", fontname="Verdana",fontsize="10",labelfontname="Verdana",labelfontsize="10" height=".13"];

    Loadable        [URL="\ref OgreKit_Loadable", color="#C0C0C0", fontcolor="#444A26"];
    GameObject      [URL="\ref OgreKit_GameObject", color="#C0C0C0", fontcolor="#444A26"];
    Skeleton        [style="filled", fillcolor="#ECEBE8", color="#93B300" fontcolor="#000000"];

    Loadable -> GameObject -> Skeleton;
} 
\enddot


<b>Placeholder:</b> \n\n
Implement access.

\endpage

<!-- ============================================ Debugger ============================================ -->
\LuaClass{Debugger}
\LuaSectionUp{Engine}



\beginmenu{Methods}
\LuaMethodMenu{Debugger,constructor}
\LuaMethodMenu{Debugger,drawLine}
\LuaMethodMenu{Debugger,drawObjectAxis}
\LuaMethodMenu{Debugger,clear}
\endmenu


\endpage

<!-- ======================================== -->
\LuaMethod{Debugger,constructor}
\LuaClassUp{Debugger}

Creates a debugger for the active scene.

\code
function Debugger:constructor(scene)
\endcode


\param scene The active \LuaClassRef{Scene} 


\endpage


<!-- ======================================== -->
\LuaMethod{Debugger,drawLine}
\LuaClassUp{Debugger}

Draws a line in the scene.

\code
function Debugger:drawLine(from, to, color)
\endcode


\param from \LuaClassRef{Vector3} as the starting position.
\param to \LuaClassRef{Vector3} as the ending position.
\param color \LuaClassRef{Vector3} as a color value ranging from [0..1]

\endpage

<!-- ======================================== -->
\LuaMethod{Debugger,drawObjectAxis}
\LuaClassUp{Debugger}

Draws the local axis of the specified game object.


\code
function Debugger:drawLine(object, size)
\endcode


\param from \LuaClassRef{GameObject} as the object in question.
\param size number representing the length of each axis.

\endpage



<!-- ======================================== -->
\LuaMethod{Debugger,clear}
\LuaClassUp{Debugger}

Removes all lines from the scene.

\code
function Debugger:clear()
\endcode

\endpage

<!-- ============================================ DebugPrint ============================================ -->
\LuaClass{DebugPrint}
\LuaSectionUp{Engine}

Prints message to the render window.

\code
function OgreKit.DebugPrint(string)
\endcode


\param string Debug message.

\endpage




<!-- ============================================ RenderSystem ============================================ -->
\LuaClass{RenderSystem}
\LuaSectionUp{Engine}

RenderSystem Options
\code
OgreKit.OGRE_RS_GL,     OpenGL RenderSystem
OgreKit.OGRE_RS_GLES,   OpenGL ES RenderSystem (TODO)
OgreKit.OGRE_RS_D3D9,   Direct3D 9 RenderSystem
OgreKit.OGRE_RS_D3D10,  Direct3D 10 RenderSystem
OgreKit.OGRE_RS_D3D11,  Direct3D 11 RenderSystem
\endcode


<!-- ============================================ ShadowType ============================================ -->
\LuaClass{ShadowType}
\LuaSectionUp{Engine}


Shadow technique identifiers.

\code
stencilmodulative
stenciladditive
texturemodulative
textureadditive
texturemodulativeintegrated
textureadditiveintegrated
\endcode


<!-- ============================================ PropertyType ============================================ -->
\LuaClass{PropertyType}
\LuaSectionUp{Engine}

Property identifiers.

\code
OgreKit.PROP_NULL
OgreKit.PROP_BOOL
OgreKit.PROP_NUMBER
OgreKit.PROP_STRING
\endcode

<!-- ============================================ TransformSpace ============================================ -->
\LuaClass{TransformSpace}
\LuaSectionUp{Engine}

GameObject transform space.

\code
OgreKit.TS_LOCAL
OgreKit.TS_PARENT
OgreKit.TS_WORLD
\endcode



<!-- ============================================ GameObjectTypes ============================================ -->
\LuaClass{GameObjectTypes}
\LuaSectionUp{Engine}

GameObject sub class enumerations.

\code
OgreKit.OB_CAMERA
OgreKit.OB_LIGHT
OgreKit.OB_ENTITY
OgreKit.OB_EMPTY
OgreKit.OB_SKELETON
\endcode



<!-- ============================================ EngineEvents ============================================ -->
\LuaClass{EngineEvents}
\LuaSectionUp{Engine}

Engine callback codes.

\code
OgreKit.EVT_TICK
\endcode

\LuaSee{Engine,connect}



<!-- ============================================ MouseButton ============================================ -->
\LuaClass{MouseButton}
\LuaSectionUp{Engine}
Mouse button codes.

\code
OgreKit.LEFT
OgreKit.RIGHT
OgreKit.MIDDLE
\endcode

<!-- ============================================ ScanCode ============================================ -->
\LuaClass{ScanCode}
\LuaSectionUp{Engine}

Keyboard scan codes.

\code
OgreKit.KC_AKEY
OgreKit.KC_BKEY
OgreKit.KC_CKEY
OgreKit.KC_DKEY
OgreKit.KC_EKEY
OgreKit.KC_FKEY
OgreKit.KC_GKEY
OgreKit.KC_HKEY
OgreKit.KC_IKEY
OgreKit.KC_JKEY
OgreKit.KC_KKEY
OgreKit.KC_LKEY
OgreKit.KC_MKEY
OgreKit.KC_NKEY
OgreKit.KC_OKEY
OgreKit.KC_PKEY
OgreKit.KC_QKEY
OgreKit.KC_RKEY
OgreKit.KC_SKEY
OgreKit.KC_TKEY
OgreKit.KC_UKEY
OgreKit.KC_VKEY
OgreKit.KC_WKEY
OgreKit.KC_XKEY
OgreKit.KC_YKEY
OgreKit.KC_ZKEY
OgreKit.KC_ZEROKEY
OgreKit.KC_ONEKEY
OgreKit.KC_TWOKEY
OgreKit.KC_THREEKEY
OgreKit.KC_FOURKEY
OgreKit.KC_FIVEKEY
OgreKit.KC_SIXKEY
OgreKit.KC_SEVENKEY
OgreKit.KC_EIGHTKEY
OgreKit.KC_NINEKEY
OgreKit.KC_CAPSLOCKKEY
OgreKit.KC_LEFTCTRLKEY
OgreKit.KC_LEFTALTKEY
OgreKit.KC_RIGHTALTKEY
OgreKit.KC_RIGHTCTRLKEY
OgreKit.KC_RIGHTSHIFTKEY
OgreKit.KC_LEFTSHIFTKEY
OgreKit.KC_ESCKEY
OgreKit.KC_TABKEY
OgreKit.KC_RETKEY
OgreKit.KC_SPACEKEY
OgreKit.KC_LINEFEEDKEY
OgreKit.KC_BACKSPACEKEY
OgreKit.KC_DELKEY
OgreKit.KC_SEMICOLONKEY
OgreKit.KC_PERIODKEY
OgreKit.KC_COMMAKEY
OgreKit.KC_QUOTEKEY
OgreKit.KC_ACCENTGRAVEKEY
OgreKit.KC_MINUSKEY
OgreKit.KC_SLASHKEY
OgreKit.KC_BACKSLASHKEY
OgreKit.KC_EQUALKEY
OgreKit.KC_LEFTBRACKETKEY
OgreKit.KC_RIGHTBRACKETKEY
OgreKit.KC_LEFTARROWKEY
OgreKit.KC_DOWNARROWKEY
OgreKit.KC_RIGHTARROWKEY
OgreKit.KC_UPARROWKEY
OgreKit.KC_PAD0
OgreKit.KC_PAD1
OgreKit.KC_PAD2
OgreKit.KC_PAD3
OgreKit.KC_PAD4
OgreKit.KC_PAD5
OgreKit.KC_PAD6
OgreKit.KC_PAD7
OgreKit.KC_PAD8
OgreKit.KC_PAD9
OgreKit.KC_PADEQUALS
OgreKit.KC_PADPERIOD
OgreKit.KC_PADSLASHKEY
OgreKit.KC_PADASTERKEY
OgreKit.KC_PADMINUS
OgreKit.KC_PADENTER
OgreKit.KC_PADPLUSKEY
OgreKit.KC_F1KEY
OgreKit.KC_F2KEY
OgreKit.KC_F3KEY
OgreKit.KC_F4KEY
OgreKit.KC_F5KEY
OgreKit.KC_F6KEY
OgreKit.KC_F7KEY
OgreKit.KC_F8KEY
OgreKit.KC_F9KEY
OgreKit.KC_F10KEY
OgreKit.KC_F11KEY
OgreKit.KC_F12KEY
OgreKit.KC_PAUSEKEY
OgreKit.KC_INSERTKEY
OgreKit.KC_HOMEKEY
OgreKit.KC_PAGEUPKEY
OgreKit.KC_PAGEDOWNKEY
OgreKit.KC_ENDKEY
OgreKit.KC_UNKNOWNKEY
OgreKit.KC_COMMANDKEY
OgreKit.KC_GRLESSKEY
\endcode

*/
