/*
GameKit
Copyright (c) 2009 Erwin Coumans  http://gamekit.googlecode.com

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef __BULLET_BLEND_NEW_H
#define __BULLET_BLEND_NEW_H


namespace Blender
{
	class Object;
	class Scene;
};
namespace bParse
{
	class bBlenderFile;
};

class btCollisionObject;


struct	BulletObjectContainer
{
	// derrive for IrrlichtMeshContainer ?
	void* m_userPointer;
};

///BulletBlendReaderNew is based on Charlie's 2006 .blend reader, supporting reading of 32bit/64bit, little and big endian .blend files
class	BulletBlendReaderNew
{

protected:

	bParse::bBlenderFile* m_blendFile;

	class btDynamicsWorld* m_destinationWorld;

//	btHashMap<btHashInt,btDataObject*>	m_dataObjects;

	///m_visibleGameObjects is a subset of m_dataObjects of only the visible game objects, handy for iterating, adding logic bricks/physics constraints etc
//	btAlignedObjectArray<btDataObject*>	m_visibleGameObjects;

	btCollisionObject* createBulletObject(Blender::Object* object);

	btCollisionObject* findCollisionObject(Blender::Object* ob);
	
public:

	BulletBlendReaderNew();
	BulletBlendReaderNew(class btDynamicsWorld* destinationWorld);

	virtual ~BulletBlendReaderNew();

	///if you already have a file pointer, call readFile
	virtual int		readFile(char* memoryBuffer, int len, int verboseDumpAllTypes = false);

	int				writeFile(const char* fileName);

	void			convertAllObjects(int verboseDumpAllBlocks=false);

	virtual	void	convertConstraints();

	virtual	void	beginScene(Blender::Scene *scene){}
	virtual	void	convertLogicBricks() = 0;

	virtual	void	createParentChildHierarchy()=0;
	
	//after each object is converter, including collision object, create a graphics object (and bind them)
	virtual void*	createGraphicsObject(Blender::Object* tmpObject, class btCollisionObject* bulletObject)=0;

	virtual	void	addCamera(Blender::Object* tmpObject)=0;

	virtual void	addArmature(Blender::Object* tmpObject) = 0;

	virtual void	addEmpty(Blender::Object* tmpObject) = 0;

	virtual	void	addLight(Blender::Object* tmpObject)=0;

};

#endif //__BULLET_BLEND_NEW_H
