
macro (configure_gamekit ROOT)

	set(GAMEKIT_SERIALIZE_BLENDER			${ROOT}/Blender25/Serialize/BlenderSerialize ${ROOT}/Blender25/Blender)
	set(GAMEKIT_SERIALIZE_BLENDER_HEADERS   ${ROOT}/Blender25/Serialize/BlenderSerialize)
	set(GAMEKIT_SERIALIZE_BULLET			${ROOT}/Blender25/Serialize/BulletFileLoader)

	set(GAMEKIT_FBT_INCLUDE
		${FBT_SOURCE_DIR}/File 
		${FBT_SOURCE_DIR}/FileFormats/Blend
		#${FBT_SOURCE_DIR}/FileFormats/Bullet
	)
	
	set(GAMEKIT_FBT_LIBS
		fbtFile
		bfBlend
		#bfBullet
	)

	#message(${GAMEKIT_FBT_INCLUDE})

	add_definitions(-DUT_USE_ZLIB)
	set(GAMEKIT_UTILS_TARGET				GameKitUtils)
	set(GAMEKIT_UTILS_PATH					${ROOT}/GameKit/Utils)

	set(GAMEKIT_ANIMKIT_TARGET				AnimKit)
	set(GAMEKIT_ANIMKIT_PATH				${ROOT}/GameKit/AnimKit)

	set(GAMEKIT_SERIALIZE_BLENDER_TARGET	BulletFileLoader BlenderSerialize)

endmacro(configure_gamekit)
