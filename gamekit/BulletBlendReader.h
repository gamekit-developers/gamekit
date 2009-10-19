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

#include <stdio.h>
#include "readblend.h"
#include "LinearMath/btAlignedObjectArray.h"
#include "LinearMath/btHashMap.h"
class btCollisionObject;

struct	btDataValue
{
	int	m_type;
	int	m_name;
	
	btDataValue()
		:m_ptr(0)
	{

	}
	btAlignedObjectArray<btDataValue> m_valueArray;
	union
	{
		char m_cValue;
		unsigned char m_ucValue;
		int		m_iValue;
		unsigned int m_uiValue;
		float	m_fValue;
		void*	m_ptr;
	};
};




struct	btDataObject
{
	btHashMap<btHashString,btDataValue*>	m_dataMap;
	unsigned int			m_blenderPointer;
	void*					m_userPointer;

	btDataObject()
		:m_blenderPointer(0),
		m_userPointer(0)
	{
	}

	int	getIntValue(const char* fieldName, int defaultValue=0)
	{
		btDataValue** valPtr = m_dataMap[fieldName];
		if (valPtr && *valPtr)
		{
			return (*valPtr)->m_iValue;
		}
		return defaultValue;
	}

	float getFloatValue(const char* fieldName, float defaultValue=0.)
	{
		btDataValue** valPtr = m_dataMap[fieldName];
		if (valPtr && *valPtr)
		{
			return (*valPtr)->m_fValue;
		}
		return defaultValue;
	}
};


//quick prototyping of some logic brick converting and processing system
struct	btSensor
{
};

struct	btController
{
};

struct	btActuator
{
};

struct	btLogicManager
{
	btAlignedObjectArray<btSensor*> m_sensors;
	btAlignedObjectArray<btController*> m_controllers;
	btAlignedObjectArray<btActuator*> m_actuators;

	void	processLogicBricks(btScalar	deltaTime)
	{
		int i;
		//process all sensors
		
		for (i=0;i<m_sensors.size();i++)
		{
		}

		//process all controllers
		for (i=0;i<m_controllers.size();i++)
		{
		}

		//process all actuators
		//process all controllers
		for (i=0;i<m_actuators.size();i++)
		{
		}

	}
	
};



typedef btHashKey<int> btHashInt;

class	BulletBlendReader
{
	struct _BlendFile* m_bf;

	class btDynamicsWorld* m_destinationWorld;

	btHashMap<btHashInt,btDataObject*>	m_dataObjects;
	
	///m_visibleGameObjects is a subset of m_dataObjects of only the visible game objects, handy for iterating, adding logic bricks/physics constraints etc
	btAlignedObjectArray<btDataObject*>	m_visibleGameObjects;

public:

	BulletBlendReader(class btDynamicsWorld* destinationWorld);

	virtual ~BulletBlendReader();

	///if you already have a file pointer, call readFile
	int		readFile( FILE* file, int verboseDumpAllTypes = false);

	///if you only have a fileName, call openFile
	//int		openFile(const char* fileName);

	bool	needsExtraction(const char* type_name);
	
	btDataObject* extractSingleObject(BlendObject*);

	void	convertAllObjects(int verboseDumpAllBlocks=false);

	///for each Blender Object, this method will be called to convert/retrieve data from the bObj
	virtual	btCollisionObject* convertSingleObject(struct _bObj* object);

	virtual	void convertSingleMesh(struct _bMesh* mesh);

	void	convertConstraints();

	void	convertLogicBricks();

	//after each object is converter, including collision object, create a graphics object (and bind them)
	virtual void createGraphicsObject(_bObj* tmpObject, class btCollisionObject* bulletObject)=0;

	virtual	void	addCamera(_bObj* tmpObject)=0;

	virtual	void	addLight(_bObj* tmpObject)=0;

};


#endif //BULLET_BLEND_READER_H