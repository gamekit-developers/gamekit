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



typedef struct _BlendField BlendField;
struct _BlendField {
  char* field_bytes;
  int field_bytes_count;

  /* the offset into field_bytes at which each field of a
     structure begins.  this is so we can keep the data aligned
     correctly for various architectures.  a non-structure type
     is equivalent to a structure with a single field.
  */
  btAlignedObjectArray<int> m_field_offsets;
  int field_offsets_count;
};
struct BlendBlock {
  char tag[5];
  uint32_t blender_pointer1;
  /*void* fixed_pointer;*/

  int type_index;
  /* a block is simply an array of its type as defined by type_index.
     array_entries is an array of pointers to each entry in the block's
     array.     
   */
  btAlignedObjectArray<BlendField*> m_array_entries_;
  
  void* customData; /* for Link blocks, with custom data such as .jpg pictures etc */
  int customDataSize;
};




/* the types extracted from the Blender file */
typedef struct _BlendType BlendType;
struct _BlendType {
	char* name;
	int m_size;

	int is_struct;

	/* if is_struct... this defines the types of each of the structure fields */
	int fieldtypes_count;
	int* fieldtypes; /* type indices */
	int fieldnames_count;
	int* fieldnames; /* name indices */
};






/* the opaque BlendFile structure */
struct BlendFile{
	BlendType* types;
	int types_count;

	char* *names;
	int names_count;

	int* strc_indices;
	int strc_indices_count;

	btAlignedObjectArray<BlendBlock*>	m_blocks;
	//int blocks_count;

	int name_undef; /* index of the name we add specially for top blocks */

};


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


struct btTypedLogicBrick
{
	int	m_type;
	btDataObject* m_ownerObject;

	btTypedLogicBrick(btDataObject* ownerObject, int type)
		:m_type(type),
		m_ownerObject(ownerObject)
	{
	}

};

//quick prototyping of some logic brick converting and processing system
struct	btSensor : public btTypedLogicBrick
{
	btSensor(btDataObject* ownerObject,int type)
		:btTypedLogicBrick(ownerObject,type)
	{
	}

};

struct	btController  : public btTypedLogicBrick
{
	btController(btDataObject* ownerObject,int type)
	:btTypedLogicBrick(ownerObject,type)
	{
	}
};

struct	btActuator : public btTypedLogicBrick
{
	btActuator(btDataObject* ownerObject,int type)
		:btTypedLogicBrick(ownerObject,type)
	{
	}
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
//			printf("processing a sensor of type %d\n",m_sensors[i]->m_type);
		}

		//process all controllers
		for (i=0;i<m_controllers.size();i++)
		{
//			printf("processing a controller of type %d\n",m_controllers[i]->m_type);
		}

		//process all actuators
		for (i=0;i<m_actuators.size();i++)
		{
//			printf("processing an actuator of type %d\n",m_actuators[i]->m_type);
		}

	}
	
};


char* btReadBuffer(FILE* fp, int* fileSize=0);


class	BulletBlendReader
{

	struct BlendFile* m_bf;

protected:

	class btDynamicsWorld* m_destinationWorld;

	btHashMap<btHashInt,btDataObject*>	m_dataObjects;
	

	///m_visibleGameObjects is a subset of m_dataObjects of only the visible game objects, handy for iterating, adding logic bricks/physics constraints etc
	btAlignedObjectArray<btDataObject*>	m_visibleGameObjects;

	void	read_type(char* file, BlendFile* bf, unsigned long section_type, BlendField & rtn);
	int readData(char* file, BlendFile* bf);

public:

	BulletBlendReader(class btDynamicsWorld* destinationWorld);

	virtual ~BulletBlendReader();

	///if you already have a file pointer, call readFile
	int		readFile( FILE* file, int verboseDumpAllTypes = false);

	///if you only have a fileName, call openFile
	//int		openFile(const char* fileName);

	bool	needsExtraction(const char* type_name);
	
	btDataObject* extractSingleObject(BlendObject*, bool verboseDumpAllBlocks);

	void	convertAllObjects(int verboseDumpAllBlocks=false);

	///for each Blender Object, this method will be called to convert/retrieve data from the bObj
	virtual	btCollisionObject* convertSingleObject(struct _bObj* object);

	virtual	void convertSingleMesh(struct _bMesh* mesh);

	virtual	void	convertConstraints();

	virtual	void	convertLogicBricks() = 0;

	virtual	void	createParentChildHierarchy()=0;
	
	//after each object is converter, including collision object, create a graphics object (and bind them)
	virtual void* createGraphicsObject(_bObj* tmpObject, class btCollisionObject* bulletObject)=0;

	virtual	void	addCamera(_bObj* tmpObject)=0;

	virtual	void	addLight(_bObj* tmpObject)=0;

};


#endif //BULLET_BLEND_READER_H