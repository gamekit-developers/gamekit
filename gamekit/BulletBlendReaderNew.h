

#ifndef __BULLET_BLEND_NEW_H
#define __BULLET_BLEND_NEW_H

namespace Blender
{
	class Object;
};
namespace bParse
{
	class bFile;
};

class btCollisionObject;


///BulletBlendReaderNew is based on Charlie's 2006 .blend reader, supporting reading of 32bit/64bit, little and big endian .blend files
class	BulletBlendReaderNew
{

protected:

	bParse::bFile* m_blendFile;

	class btDynamicsWorld* m_destinationWorld;

//	btHashMap<btHashInt,btDataObject*>	m_dataObjects;

	///m_visibleGameObjects is a subset of m_dataObjects of only the visible game objects, handy for iterating, adding logic bricks/physics constraints etc
//	btAlignedObjectArray<btDataObject*>	m_visibleGameObjects;

public:

	BulletBlendReaderNew(class btDynamicsWorld* destinationWorld);

	virtual ~BulletBlendReaderNew();

	///if you already have a file pointer, call readFile
	int		readFile(char* memoryBuffer, int len, int verboseDumpAllTypes = false);

	void	convertAllObjects(int verboseDumpAllBlocks=false);

	virtual	void	convertConstraints();

	virtual	void	convertLogicBricks() = 0;

	virtual	void	createParentChildHierarchy()=0;
	
	//after each object is converter, including collision object, create a graphics object (and bind them)
	virtual void*	createGraphicsObject(Blender::Object* tmpObject, class btCollisionObject* bulletObject)=0;

	virtual	void	addCamera(Blender::Object* tmpObject)=0;

	virtual	void	addLight(Blender::Object* tmpObject)=0;

};

#endif //__BULLET_BLEND_NEW_H
