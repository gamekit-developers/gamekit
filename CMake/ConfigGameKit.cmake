
macro (configure_gamekit ROOT)

	set(GAMEKIT_SERIALIZE_BLENDER			${ROOT}/Blender25/Serialize/BlenderSerialize ${ROOT}/Blender25/Blender)
	set(GAMEKIT_SERIALIZE_BLENDER_HEADERS   ${ROOT}/Blender25/Serialize/BlenderSerialize)
	set(GAMEKIT_SERIALIZE_BULLET			${ROOT}/Blender25/Serialize/BulletFileLoader)


	add_definitions(-DUT_USE_ZLIB)
	set(GAMEKIT_UTILS_TARGET				GameKitUtils)
	set(GAMEKIT_UTILS_PATH					${ROOT}/GameKit/Utils)

	set(GAMEKIT_SERIALIZE_BLENDER_TARGET	BulletFileLoader BlenderSerialize)

endmacro(configure_gamekit)
