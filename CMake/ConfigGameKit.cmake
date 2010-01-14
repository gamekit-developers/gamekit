
macro (configure_gamekit BASEPATH)

	set(GAMEKIT_SERIALIZE ${BASEPATH}/Serialize)
	set(GAMEKIT_SERIALIZE_COMMON  ${BASEPATH}/Serialize/CommonSerialize)
	set(GAMEKIT_SERIALIZE_BLENDER ${BASEPATH}/Serialize/BlenderSerialize)
	set(GAMEKIT_SERIALIZE_BULLET ${BASEPATH}/Serialize/BulletSerialize)

	set(GAMEKIT_SERIALIZE_COMMON_TARGET bParse)
	set(GAMEKIT_SERIALIZE_BLENDER_TARGET BlenderSerialize)
	set(GAMEKIT_SERIALIZE_BULLET_TARGET BulletSerialize)

endmacro(configure_gamekit)
