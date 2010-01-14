cmake_minimum_required(VERSION 2.4)


project(GAMEKIT)

include_directories(
	${GAMEKIT_SERIALIZE_COMMON}
	${GAMEKIT_SERIALIZE_BLENDER}
	${GAMEKIT_SERIALIZE_BULLET}
	)

subdirs(Serialize/BlenderSerialize Serialize/CommonSerialize)
