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

#ifndef BULLET_BLEND_READER_H
#define BULLET_BLEND_READER_H



class	BulletBlendReader
{
	struct _BlendFile* m_bf;

	class btDynamicsWorld* m_destinationWorld;

public:

	BulletBlendReader(class btDynamicsWorld* destinationWorld);

	virtual ~BulletBlendReader();

	int		openFile(const char* fileName);

	void	convertAllObjects();

	///for each Blender Object, this method will be called to convert/retrieve data from the bObj
	virtual	void convertSingleObject(struct _bObj* object);

	virtual	void convertSingleMesh(struct _bMesh* mesh);

	//after each object is converter, including collision object, create a graphics object (and bind them)
	virtual void createGraphicsObject(_bObj* tmpObject, class btCollisionObject* bulletObject)=0;

	virtual	void	addCamera(_bObj* tmpObject)=0;

	virtual	void	addLight(_bObj* tmpObject)=0;

};


#endif //BULLET_BLEND_READER_H