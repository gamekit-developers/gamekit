
macro (configure_gamekit BASEPATH)

	set(GAMEKIT_SERIALIZE					${BLENDPATH}/Serialize)
	set(GAMEKIT_SERIALIZE_BLENDER			${BASEPATH}/Serialize/BlenderSerialize ${BASEPATH}/Blender)
	set(GAMEKIT_SERIALIZE_BLENDER_HEADERS   ${BASEPATH}/Blender)
	set(GAMEKIT_SERIALIZE_BULLET			${BASEPATH}/Serialize/BulletFileLoader ${BASEPATH}/Serialize/BulletWorldImporter)

	set(GAMEKIT_SERIALIZE_BLENDER_TARGET	BulletFileLoader BlenderSerialize)
	set(GAMEKIT_SERIALIZE_BULLET_TARGET		BulletFileLoader BulletWorldImporter)

endmacro(configure_gamekit)
