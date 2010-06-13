
macro (configure_gamekit BASEPATH)

	set(GAMEKIT_SERIALIZE_BLENDER			${BASEPATH}/Serialize/BlenderSerialize ${BASEPATH}/Blender)
	set(GAMEKIT_SERIALIZE_BLENDER_HEADERS   ${BASEPATH}/Serialize/BlenderSerialize)
	set(GAMEKIT_SERIALIZE_BULLET			${BASEPATH}/Serialize/BulletFileLoader)

	set(GAMEKIT_SERIALIZE_BLENDER_TARGET	BulletFileLoader BlenderSerialize)

endmacro(configure_gamekit)
