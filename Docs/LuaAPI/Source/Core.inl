/** \addtogroup Core
	@{
*/


/**
	<!-- ============================================ UserDefs ============================================ -->
		\LuaClass{UserDefs}

		\LuaGlobalRef{CDefs}

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

	<!-- ======================================== -->
		\LuaMethod{UserDefs,renderSystem}
		\LuaClassUp{UserDefs}

		The render system to use.

		\code
		number UserDefs.renderSystem
		\endcode

		\see \LuaClassRef{RenderSystem}.


	<!-- ======================================== -->
		\LuaMethod{UserDefs,verbose}
		\LuaClassUp{UserDefs}

		Enables full Ogre log messages.

		\code
		bool UserDefs.verbose
		\endcode


	<!-- ======================================== -->
		\LuaMethod{UserDefs,winx}
		\LuaClassUp{UserDefs}

		Window width.

		\code
		number UserDefs.winx
		\endcode


	<!-- ======================================== -->
		\LuaMethod{UserDefs,winy}
		\LuaClassUp{UserDefs}

		Window height.

		\code
		number UserDefs.winy
		\endcode

	<!-- ======================================== -->
		\LuaMethod{UserDefs,winTitle}
		\LuaClassUp{UserDefs}

		Window display text.

		\code
		string UserDefs.winTitle
		\endcode

	<!-- ======================================== -->
		\LuaMethod{UserDefs,fullScreen}
		\LuaClassUp{UserDefs}

		Enable fullscreen rendering.

		\code
		bool UserDefs.fullScreen
		\endcode

	<!-- ======================================== -->
		\LuaMethod{UserDefs,blenderMat}
		\LuaClassUp{UserDefs}

		Convert blender mesh objects basied on the assigned material index.
		Opposed to the face texture / UV layers. Also uses the blender material for the mesh material. 

		\code
		bool UserDefs.blenderMat
		\endcode

	<!-- ======================================== -->
		\LuaMethod{UserDefs,grabInput}
		\LuaClassUp{UserDefs}

		Places the system mouse in the center of the render window, and hides it.

		\code
		bool UserDefs.grabInput
		\endcode

	<!-- ======================================== -->
		\LuaMethod{UserDefs,debugFps}
		\LuaClassUp{UserDefs}

		Enables frame rate / profiling print out on the render window.

		\code
		bool UserDefs.debugFps
		\endcode

	<!-- ======================================== -->
		\LuaMethod{UserDefs,debugPhysics}
		\LuaClassUp{UserDefs}

		Draw physics shapes and contact points.

		\code
		bool UserDefs.debugPhysics
		\endcode

	<!-- ======================================== -->
		\LuaMethod{UserDefs,debugPhysicsAABB}
		\LuaClassUp{UserDefs}

		Draw the axis aligned bounding box on physics enabled objects.

		\code
		bool UserDefs.debugPhysicsAABB
		\endcode

	<!-- ======================================== -->
		\LuaMethod{UserDefs,debugSounds}
		\LuaClassUp{UserDefs}

		Draw sound playback visualization.

		\code
		bool UserDefs.debugSounds
		\endcode

	<!-- ======================================== -->
		\LuaMethod{UserDefs,showDebugProps}
		\LuaClassUp{UserDefs}

		Show properties attached to game objects.

		\code
		bool UserDefs.showDebugProps
		\endcode

		\note The property in must have the debug flag enabled in the Blender UI.

		\LuaSee{Property,makeDebug}

	<!-- ======================================== -->
		\LuaMethod{UserDefs,enableShadows}
		\LuaClassUp{UserDefs}

		Enables Ogre shadow rendering.

		\code
		bool UserDefs.enableShadows
		\endcode

	<!-- ======================================== -->
		\LuaMethod{UserDefs,shadowTechnique}
		\LuaClassUp{UserDefs}

		Sets the shadow render technique.

		\code
		string UserDefs.shadowTechnique
		\endcode

		\see \LuaClassRef{ShadowType}

	<!-- ======================================== -->
		\LuaMethod{UserDefs,colourShadowR}
		\LuaClassUp{UserDefs}

		Sets the shadows red component.

		\code
		number UserDefs.colourShadowR
		\endcode

	<!-- ======================================== -->
		\LuaMethod{UserDefs,colourShadowG}
		\LuaClassUp{UserDefs}

		Sets the shadows green component.

		\code
		number UserDefs.colourShadowG
		\endcode

	<!-- ======================================== -->
		\LuaMethod{UserDefs,colourShadowB}
		\LuaClassUp{UserDefs}

		Sets the shadows blue component.

		\code
		number UserDefs.colourShadowB
		\endcode

	<!-- ======================================== -->
		\LuaMethod{UserDefs,farDistanceShadow}
		\LuaClassUp{UserDefs}

		Sets the shadows distance.

		\code
		number UserDefs.farDistanceShadow
		\endcode
*/

/**
	<!-- ============================================ ShadowType ============================================ -->
		\LuaClass{ShadowType}

		Shadow technique identifiers.

		\code
		stencilmodulative
		stenciladditive
		texturemodulative
		textureadditive
		texturemodulativeintegrated
		textureadditiveintegrated
		\endcode



	<!-- ============================================ GameObjectTypes ============================================ -->
		\LuaClass{GameObjectTypes}


		GameObject sub class enumerations.

		\code
		OgreKit.OB_CAMERA
		OgreKit.OB_LIGHT
		OgreKit.OB_ENTITY
		OgreKit.OB_EMPTY
		OgreKit.OB_SKELETON
		\endcode



	<!-- ============================================ ScanCode ============================================ -->
		\LuaClass{ScanCode}


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

/** @} */
