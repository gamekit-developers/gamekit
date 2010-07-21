

include_directories(	../Serialize/BlenderSerialize ../Serialize/BulletFileLoader
			${BULLET_PHYSICS_SOURCE_DIR}/src/)

link_libraries(BlenderSerialize BulletFileLoader LinearMath)
add_executable(TestLoader Main.cpp)
