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



//#define DUMP_BLOCK_NAMES 1


#include "BulletBlendReader.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "abs-file.h"

#include "readblend.h"
#include "blendtype.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.h"
#include "BulletCollision/Gimpact/btGImpactShape.h"
#include <stdio.h>

BulletBlendReader::BulletBlendReader(btDynamicsWorld* destinationWorld)
:m_bf(0),
m_destinationWorld(destinationWorld)
{

}

int	BulletBlendReader::readFile(FILE* posixFile, int verboseDumpAllTypes)
{
	MY_FILETYPE* buffer;

	buffer = MY_FILE_OPEN_FOR_READ(posixFile);

	m_bf =  blend_read(buffer);

	if (!m_bf)
	{
		fprintf(stderr, "couldn't read blender file. :(\n");
		MY_CLOSE(buffer);
		return 0;
	}
	
	if (verboseDumpAllTypes)
	{
		blend_dump_typedefs(m_bf);
	}
	
	MY_CLOSE(buffer);

	return 1;
}



BulletBlendReader::~BulletBlendReader()
{
	///@todo: delete/close file/data structures
	
}

static int
name_is_pointer(char* name) {
	int len = strlen(name);
	/*fprintf(stderr,"[%s]",name);*/
	if (len >= 1) {
		if (name[0] == '*')
			return 1;
	}
	if (len >= 2) {
		if (name[1] == '*')
			return 1;
	}
	return 0;
}

static int
name_is_array(char* name, int* dim1, int* dim2) {
	int len = strlen(name);
	/*fprintf(stderr,"[%s]",name);*/
	/*if (len >= 1) {
	if (name[len-1] != ']')
	return 1;
	}
	return 0;*/
	char *bp;
	int num;
	if (dim1) {
		*dim1 = 1;
	}
	if (dim2) {
		*dim2 = 1;
	}
	bp = strchr(name, '[');
	if (!bp) {
		return 0;
	}
	num = 0;
	while (++bp < name+len-1) {
		const char c = *bp;
		if (c == ']') {
			break;
		}
		if (c <= '9' && c >= '0') {
			num *= 10;
			num += (c - '0');
		} else {
			printf("array parse error.\n");
			return 0;
		}
	}
	if (dim2) {
		*dim2 = num;
	}

	/* find second dim, if any. */
	bp = strchr(bp, '[');
	if (!bp) {
		return 1; /* at least we got the first dim. */
	}
	num = 0;
	while (++bp < name+len-1) {
		const char c = *bp;
		if (c == ']') {
			break;
		}
		if (c <= '9' && c >= '0') {
			num *= 10;
			num += (c - '0');
		} else {
			printf("array2 parse error.\n");
			return 1;
		}
	}
	if (dim1) {
		if (dim2) {
			*dim1 = *dim2;
			*dim2 = num;
		} else {
			*dim1 = num;
		}
	}

	return 1;
}



#if 0
	if (!strcmp(m_bf->types[fieldType].name,"char") && (dim1>1 || dim2>1))
			{
				
				index=0;



				for (k=0;k<dim1;k++)
				{
					for (l=0;l<dim2;l++)
					{
						ptrptr = &block->array_entries->field_bytes[block->array_entries->field_offsets[field_index+dim2*k+ l]];
									
						objType = typestring_to_blendobj_type(m_bf, m_bf->types[fieldType].name) ;
						if (ptrptr && index<1023)
						{
							char val = *(char*)ptrptr;
							namebuf[index++]=val;
						}
					}
				}

				namebuf[index]=0;
				printf("\"%s\"",namebuf);
			}
			else
			{
				for (k=0;k<dim1;k++)
				{
					if (dim1>1 || dim2>1)
					{
						printf("[");
					}
					for (l=0;l<dim2;l++)
					{
						ptrptr = &block->array_entries->field_bytes[block->array_entries->field_offsets[field_index+dim2*k+ l]];
									
						objType = typestring_to_blendobj_type(m_bf, m_bf->types[fieldType].name) ;
						if (ptrptr)
						{
							switch (objType)
							{
							case BLEND_OBJ_NULL:
								{
									printf("NULL ");
									break;
								}
							case BLEND_OBJ_OPAQUE:
								{
									printf("OPAQUE? ");
									break;
								}
							case BLEND_OBJ_UCHAR8:
								{
									unsigned char val = *(unsigned char*)ptrptr;
									printf("%d ",val);
									break;
								}
							case BLEND_OBJ_CHAR8:
								{
									char val = *(char*)ptrptr;
									printf("%d ",val);
									break;
								}
							case BLEND_OBJ_USHORT16:
								{
									unsigned short int val = *(unsigned short int*)ptrptr;
									printf("%d ",val);
									break;
								}
							case BLEND_OBJ_SHORT16:
								{
									short int val = *(short int*)ptrptr;
									printf("%d ",val);
									break;
								}
							case BLEND_OBJ_ULONG32:
								{
									unsigned int val = *(unsigned int*)ptrptr;
									printf("%d ",val);
									break;
								}
							case BLEND_OBJ_LONG32:
								{
									int val = *(int*)ptrptr;
									printf("%d ",val);
									break;
								}
							case BLEND_OBJ_FLOAT:
								{
									float val = *(float *)ptrptr;
									printf("%f ",val);
									break;
								}
							case BLEND_OBJ_DOUBLE:
								{
									double  val = *(double *)ptrptr;
									printf("%f ",val);
									break;
								}
							case BLEND_OBJ_POINTER:
								{
									void* val = *ptrptr;
									printf("%x ",val);
									break;
								}
							case BLEND_OBJ_STRUCT:
								{
									char	dest[1024];
									int isListBase = 0;
									int max_chars = 1023;

									
									if (!strcmp(m_bf->types[m_bf->types[obj.type].fieldtypes[i]].name,"ListBase"))
									{
										isListBase  =1;
									}
									if (name_is_pointer(m_bf->names[m_bf->types[obj.type].fieldnames[i]]) || isListBase)
									{

										BlendBlockPointer ptr = *ptrptr;
										if (ptr)
										{
											BlendObject idstruc_obj;
											BlendBlock* block;
											BlendObject name_obj;
											

											
											BlendBlockPointer curveblockptr = blend_block_from_blendpointer(m_bf, ptr);
											if (curveblockptr)
											{
												idstruc_obj = blend_block_get_object(m_bf, curveblockptr, 0);
												block = (BlendBlock*)idstruc_obj.block;
												
												if (BLEND_OBJ_STRUCT == blend_object_type(m_bf, idstruc_obj) &&
													blend_object_structure_getfield(m_bf, &name_obj,
													idstruc_obj, "name")) {
														/* great, get the string from the 'name' field. */
														if (blend_object_getstring(m_bf, name_obj,
															dest, max_chars)) {
																printf("\"%s\" ",dest);
																
														} else {
															printf("%x ",block->blender_pointer);
														}
												} else {
													printf("%x ",block->blender_pointer);

												}
												
											} else
											{
												printf("NULL ");
											}

										} else
										{
											printf("NULL ");
										}
									} else
									{
										if (!strcmp(m_bf->names[m_bf->types[obj.type].fieldnames[i]],"id"))
										{
											if (blend_object_get_IDname(m_bf, obj, dest, max_chars)) 
											{
												printf("\"%s\" ",dest);
												break;
											}
										} 
										printf("Embedded struct\n");

									}
									break;
								}
							default:
								{
									printf("unknown value");
								}
							}
						}
					}
					if (dim1>1 || dim2>1)
					{
						printf("]");
					}
				}
			}
			
			printf("\n");
			
			{
				int fos;
				BlendObject qo = obj;
				qo.type = m_bf->types[obj.type].fieldtypes[i];
				qo.name = m_bf->types[obj.type].fieldnames[i];
				qo.field_index = field_index;
				fos = get_num_type_segments(m_bf, qo);
				field_index += fos;
			}
		}
#endif



		/* recursively count the number of fields and array items in this
structure, for the purposes of skipping in the field offset array */
static long
get_num_type_segments(BlendFile* blend_file,
					  BlendObject obj)
{
	int i;
	long rtn = 0;
	int dim1,dim2;

	name_is_array(blend_file->names[obj.name],
		&dim1, &dim2);

	if (name_is_pointer(blend_file->names[obj.name]) ||
		!blend_file->types[obj.type].is_struct) {
			return (1 * dim1 * dim2);
	}

	/* fprintf(stderr, "STRUCTYAYYY ");*/

	for (i=0; i<blend_file->types[obj.type].fieldnames_count; ++i) {
		BlendObject qo = obj;
		qo.type = blend_file->types[obj.type].fieldtypes[i];
		qo.name = blend_file->types[obj.type].fieldnames[i];
		qo.field_index = i;
		rtn += get_num_type_segments(blend_file, qo) * dim1 * dim2;
	}

	return (rtn);
}


static int
blend_type_basename_compare(const char *fancy, const char *raw) {
	const int flen = strlen(fancy);
	const int rlen = strlen(raw);
	int i, strcmp_result = 123;

	i = 0;
	while (i < flen && (fancy[i]=='*' || fancy[i]=='(')) {
		++i;
	}

	strcmp_result = strncmp(&fancy[i], raw, rlen);

	if (strcmp_result == 0 && flen > rlen+i) {
		i = rlen + i;
		if (fancy[i] != ')' && fancy[i] != '(' && fancy[i] != '[') {
			strcmp_result = -1;
		}
	}

	return strcmp_result;
}


static BlendObjType
typestring_to_blendobj_type(BlendFile* blend_file,
							const char* type_name) 
{
	if (blend_type_basename_compare(type_name, "char") == 0) {
		return BLEND_OBJ_CHAR8;
	} else if (blend_type_basename_compare(type_name, "uchar") == 0) {
		return BLEND_OBJ_UCHAR8;
	} else if (blend_type_basename_compare(type_name, "short") == 0) {
		return BLEND_OBJ_SHORT16;
	} else if (blend_type_basename_compare(type_name, "ushort") == 0) {
		return BLEND_OBJ_USHORT16;
	} else if (blend_type_basename_compare(type_name, "int") == 0) {
		return BLEND_OBJ_LONG32;
	} else if (blend_type_basename_compare(type_name, "long") == 0) {
		return BLEND_OBJ_LONG32;
	} else if (blend_type_basename_compare(type_name, "ulong") == 0) {
		return BLEND_OBJ_ULONG32;
	} else if (blend_type_basename_compare(type_name, "float") == 0) {
		return BLEND_OBJ_FLOAT;
	} else if (blend_type_basename_compare(type_name, "double") == 0) {
		return BLEND_OBJ_DOUBLE;
	} else if (blend_type_basename_compare(type_name, "void") == 0) {
		return BLEND_OBJ_OPAQUE;
	} else {
		return BLEND_OBJ_STRUCT; /* structure */
	}
}


static BlendBlockPointer
blendBlockFromBlendpointer(BlendFile *blend_file,
							  uint32_t blendpointer)
{
	int i;

	/* fprintf(stderr, "%04x: ", blendpointer);*/

	if (blendpointer != 0) {
		for (i=0; i<blend_file->blocks_count; ++i) {
			/*fprintf(stderr, "%04x? ", blend_file->blocks[i].blender_pointer); */
			if (blend_file->blocks[i].blender_pointer == blendpointer) {
				return &blend_file->blocks[i];
			}
		}
	}

	return NULL;
}

btDataObject* BulletBlendReader::extractSingleObject(BlendObject* objPtr)
{
	btAssert(objPtr);

	BlendObject& obj = *objPtr;
	btDataObject* dob = new btDataObject();
	int sz = sizeof(btDataValue);
	BlendBlock* block = (BlendBlock*)obj.block;
	BlendObjType objType;

	int fieldType,fieldName;

	if (m_bf->types[obj.type].is_struct) 
	{
		int i;
		int field_index = 0;
		for (i=0; i<m_bf->types[obj.type].fieldnames_count; ++i) {
			//printf("filename = %s\n",m_bf->names[m_bf->types[obj.type].fieldnames[i]]);
			fieldType = m_bf->types[obj.type].fieldtypes[i];
			fieldName =  m_bf->types[obj.type].fieldnames[i];
			printf("	%s %s = ",m_bf->types[fieldType].name, m_bf->names[fieldName]);


			int dim1=1;
			int dim2=1;
			name_is_array(m_bf->names[fieldName], &dim1, &dim2);

			int k,l;
			void** ptrptr;

			btDataValue* val = new btDataValue();
			val->m_name = fieldName;
			val->m_type = fieldType;
			
			dob->m_dataMap.insert(m_bf->names[fieldName],val);


			for (k=0;k<dim1;k++)
			{
				if (dim1>1 || dim2>1)
				{
					printf("[");
				}
				for (l=0;l<dim2;l++)
				{
					ptrptr = (void**)&block->array_entries->field_bytes[block->array_entries->field_offsets[field_index+dim2*k+ l]];
								
					objType = typestring_to_blendobj_type(m_bf, m_bf->types[fieldType].name) ;
					if (ptrptr)
					{
						switch (objType)
						{
						case BLEND_OBJ_NULL:
							{
								printf("NULL ");
								break;
							}
						
						case BLEND_OBJ_UCHAR8:
							{
								val->m_iValue = *(unsigned char*)ptrptr;
								printf("%d ",val->m_iValue);
								break;
							}
						case BLEND_OBJ_CHAR8:
							{
								val->m_iValue = *(char*)ptrptr;
								printf("%d ",val->m_iValue);
								break;
							}
						case BLEND_OBJ_USHORT16:
							{
								val->m_iValue = *(unsigned short int*)ptrptr;
								printf("%d ",val->m_iValue);
								break;
							}
						case BLEND_OBJ_SHORT16:
							{
								val->m_iValue = *(short int*)ptrptr;
								printf("%d ",val);
								break;
							}
						case BLEND_OBJ_ULONG32:
							{
								val->m_uiValue = *(unsigned int*)ptrptr;
								printf("%d ",val->m_uiValue);
								break;
							}
						case BLEND_OBJ_LONG32:
							{
								val->m_iValue = *(int*)ptrptr;
								printf("%d ",val->m_iValue);
								break;
							}
						case BLEND_OBJ_FLOAT:
							{
								val->m_fValue = *(float *)ptrptr;
								printf("%f ",val->m_fValue);
								break;
							}
						case BLEND_OBJ_DOUBLE:
							{
								double  val = *(double *)ptrptr;
								printf("%f ",val);
								break;
							}
						case BLEND_OBJ_POINTER:
							{
								void* val = *ptrptr;
								printf("%x ",val);
								break;
							}
						case BLEND_OBJ_OPAQUE:
						case BLEND_OBJ_STRUCT:
							{
								char	dest[1024];
								int isListBase = 0;
								int max_chars = 1023;

								
								if (!strcmp(m_bf->types[m_bf->types[obj.type].fieldtypes[i]].name,"ListBase"))
								{
									isListBase  =1;
								}
								if (name_is_pointer(m_bf->names[m_bf->types[obj.type].fieldnames[i]]) || isListBase)
								{

									uint32_t ptr = (uint32_t)*ptrptr;
									if (ptr)
									{
										BlendObject idstruc_obj;
										BlendBlock* block;
										BlendObject name_obj;
										

										
										BlendBlockPointer curveblockptr = blendBlockFromBlendpointer(m_bf, ptr);
										if (curveblockptr)
										{
											idstruc_obj = blend_block_get_object(m_bf, curveblockptr, 0);
											block = (BlendBlock*)idstruc_obj.block;
											val->m_uiValue =  block->blender_pointer;
											
											if (BLEND_OBJ_STRUCT == blend_object_type(m_bf, idstruc_obj) &&
												blend_object_structure_getfield(m_bf, &name_obj,
												idstruc_obj, "name")) {
													/* great, get the string from the 'name' field. */
													if (blend_object_getstring(m_bf, name_obj,
														dest, max_chars)) {
															printf("\"%s\" ",dest);
															
													} else {
														printf("%x ",block->blender_pointer);
													}
											} else {
												printf("%x ",block->blender_pointer);
											}
										} else
										{
											printf("NULL ");
										}

									} else
									{
										printf("NULL ");
									}
								} else
								{
									if (!strcmp(m_bf->names[m_bf->types[obj.type].fieldnames[i]],"id"))
									{
										if (blend_object_get_IDname(m_bf, obj, dest, max_chars)) 
										{
											printf("\"%s\" ",dest);
											break;
										}
									} 
									printf("Embedded struct\n");

								}
								break;
							}
						default:
							{
								printf("unknown value");
							}
						}
					}
				}
				if (dim1>1 || dim2>1)
				{
					printf("]");
				}
			}



			printf("\n");
			
			{
				int fos;
				BlendObject qo = obj;
				qo.type = m_bf->types[obj.type].fieldtypes[i];
				qo.name = m_bf->types[obj.type].fieldnames[i];
				qo.field_index = field_index;
				fos = get_num_type_segments(m_bf, qo);
				field_index += fos;
			}
		}
		printf("bla\n");
	}

	dob->m_blenderPointer = block->blender_pointer;
	m_dataObjects.insert(block->blender_pointer,dob);

	return dob;
}


void	BulletBlendReader::convertAllObjects(int verboseDumpAllBlocks)
{

	if (!m_bf)
	{
		printf("ERROR: no file loaded, use openFile first\n");
		return;
	}

	if (verboseDumpAllBlocks)
	{
		blend_dump_blocks(m_bf);
	}

	int sceneVisibleLayer = 0;
	btDataObject* fileGlobalObject =0;


	int j;
	for (j=0; j<m_bf->blocks_count; ++j) 
	{
		{
			int entry_count = blend_block_get_entry_count(m_bf, &m_bf->blocks[j]);
			for (int i=0; i<entry_count; ++i) 
			{
				BlendObject obj = blend_block_get_object(m_bf, &m_bf->blocks[j], i);
				


				
				BlendObject data_obj;
				BlendObject data_obj2;

				BlendBlock* tmpBlock=0;

				{
					const char* type_name = m_bf->types[obj.type].name;

#if DUMP_BLOCK_NAMES
					printf("type_name=%s. ",type_name);
					printf("block blenderptr = %lx\n",m_bf->blocks[j].blender_pointer);
#endif //DUMP_BLOCK_NAMES

					if (strcmp(type_name,"FileGlobal")==0)
					{
						
						btDataObject* dob = extractSingleObject(&obj);
						printf("Found FileGlobal\n");
						short minversion = dob->getIntValue("minversion",123);
						short manversion = dob->getIntValue("manversion",123);
						int curScene = dob->getIntValue("*curscene",0);
						printf("minversion = %d\n",minversion);
						BlendBlock* block = (BlendBlock*)obj.block;
						fileGlobalObject = dob;
						//blend_object_dump_field(m_bf,	obj);
					
					}
					
					if (strcmp(type_name,"Scene")==0)
					{
						printf("Found a scene\n");
						btDataObject* dob = extractSingleObject(&obj);
						
						if (fileGlobalObject && fileGlobalObject->getIntValue("*curscene",0)==dob->m_blenderPointer)
						{
							sceneVisibleLayer = dob->getIntValue("lay",0);
						}
						

					}

					if (strcmp(type_name,"bRigidBodyJointConstraint")==0)
					{
						btDataObject* dob = extractSingleObject(&obj);
						printf("Found a bRigidBodyJointConstraint\n");
					}

					if (strcmp(type_name,"bConstraint")==0)
					{
						btDataObject* dob = extractSingleObject(&obj);
						printf("Found a bConstraint\n");
					}

					if (strcmp(type_name,"Object")==0)
					{
						bObj tmpObj;
						blend_acquire_obj_from_obj(m_bf,&obj,&tmpObj,0);
						btDataObject* dob = extractSingleObject(&obj);
						int obLayer = dob->getIntValue("lay",0);

						///only convert objects that are in a visible layer
						if (obLayer & sceneVisibleLayer)
						{
							btCollisionObject* colObj = convertSingleObject(&tmpObj);
							if (colObj)
							{
								dob->m_userPointer = colObj;
							}
							m_visibleGameObjects.push_back(dob);
						}

					}

					if (strcmp(type_name,"Mesh")==0)
					{
						//printf("object type_name = %s\n",type_name);
						bMesh tmpMesh;
						blend_acquire_mesh_from_obj(m_bf, &obj, &tmpMesh);
						convertSingleMesh(&tmpMesh);
					}
				}
			}
		}
	}

	///now fix up some constraint and logic bricks

#define CONSTRAINT_TYPE_RIGIDBODYJOINT 17
#define CONSTRAINT_RB_BALL		1
#define CONSTRAINT_RB_HINGE		2
#define CONSTRAINT_RB_CONETWIST 4
#define CONSTRAINT_RB_VEHICLE	11
#define CONSTRAINT_RB_GENERIC6DOF 12




	int i;
	for (i=0;i<m_visibleGameObjects.size();i++)
	{
		btDataObject* ob = m_visibleGameObjects[i];
		unsigned int cPtr = ob->getIntValue("constraints");
		if (cPtr)
		{
			btDataObject** constraintPtr = m_dataObjects[cPtr];
			if (constraintPtr && *constraintPtr)
			{
				btDataObject* constraint = *constraintPtr;
				int constraintType = constraint->getIntValue("type",0);
				if (constraintType==CONSTRAINT_TYPE_RIGIDBODYJOINT)
				{
					unsigned int dataPtr = constraint->getIntValue("*data",0);
					if (dataPtr)
					{

						btDataObject** rbConstraintPtr = m_dataObjects[dataPtr];
						if (rbConstraintPtr && *rbConstraintPtr)
						{
							btDataObject* rbConstraint = *rbConstraintPtr;

							unsigned int rbConstraintType = rbConstraint->getIntValue("type",0);

///some debugging, spit out all the fields
#if 0
							for (int k=0;k<rbConstraint->m_dataMap.size();k++)
							{
								const btHashString& structName = rbConstraint->m_dataMap.getKey(k);
								const btDataValue* valPtr = *rbConstraint->m_dataMap.find(structName);
								printf("hello\n");
							}
#endif
							

							/* important: these defines need to match up with PHY_DynamicTypes headerfile */
							switch (rbConstraintType)
							{

							case CONSTRAINT_RB_BALL:
								{
									btVector3 pivotInA(rbConstraint->getFloatValue("pivX"),rbConstraint->getFloatValue("pivY"),rbConstraint->getFloatValue("pivZ"));
																		
									btCollisionObject* colObj = (btCollisionObject*) ob->m_userPointer;
									btRigidBody* rbA = btRigidBody::upcast(colObj);
									if (rbA)
									{
										btCollisionObject* colObjB = 0;
										unsigned int objectBPtr = rbConstraint->getIntValue("*tar");
										if (objectBPtr)
										{
											btDataObject** dataBPtr = m_dataObjects[objectBPtr];
											if (dataBPtr && *dataBPtr)
											{
												btDataObject* dataB = *dataBPtr;
												colObjB = (btCollisionObject*)dataB->m_userPointer;
											}
										}
										btPoint2PointConstraint* p2p = 0;

										if (colObjB && btRigidBody::upcast(colObjB))
										{
											btRigidBody* rbB = btRigidBody::upcast(colObjB);

											btVector3 pivotInB = rbB->getCenterOfMassTransform().inverse()(rbA->getCenterOfMassTransform()(pivotInA));
											p2p = new btPoint2PointConstraint(*rbA,*rbB,pivotInA,pivotInB);

										} else
										{
											p2p = new btPoint2PointConstraint(*rbA,pivotInA);
										}
										m_destinationWorld->addConstraint(p2p);
									}


									break;
								}
							case CONSTRAINT_RB_HINGE:
								{
									break;
								}
							case CONSTRAINT_RB_CONETWIST:
								{
									break;
								}
							case CONSTRAINT_RB_VEHICLE:
								{
									break;
								}
							case CONSTRAINT_RB_GENERIC6DOF:
								{
									break;
								}
							
							default:
								{
									printf("unsupported rigid body constraint type\n");
								}
							}
						}
					}

				}
			}
		}

	}

}



///for each Blender Object, this method will be called to convert/retrieve data from the bObj
btCollisionObject* BulletBlendReader::convertSingleObject(_bObj* object)
{

	switch (object->type)
	{
	case BOBJ_TYPE_MESH:
		break;

	case BOBJ_TYPE_CAMERA:
		addCamera(object);
		return 0;
	case BOBJ_TYPE_LAMP:
		addLight(object);
		return 0;
	default:
		{
		}

	};

	//let's try to create some static collision shapes from the triangle meshes
	if (object->data.mesh)
	{
		btTriangleMesh* meshInterface = new btTriangleMesh();
		
		btVector3 minVert(1e30f,1e3f,1e30f);
		btVector3 maxVert(-1e30f,-1e30f,-1e30f);
		for (int t=0;t<object->data.mesh->face_count;t++)
		{
			
			float* vt0 = object->data.mesh->vert[object->data.mesh->face[t].v[0]].xyz;
			btVector3 vtx0(vt0[0],vt0[1],vt0[2]);
			minVert.setMin(vtx0); maxVert.setMax(vtx0);
			float* vt1 = object->data.mesh->vert[object->data.mesh->face[t].v[1]].xyz;
			btVector3 vtx1(vt1[0],vt1[1],vt1[2]);
			minVert.setMin(vtx1); maxVert.setMax(vtx1);
			float* vt2 = object->data.mesh->vert[object->data.mesh->face[t].v[2]].xyz;
			btVector3 vtx2(vt2[0],vt2[1],vt2[2]);
			minVert.setMin(vtx2); maxVert.setMax(vtx2);
			meshInterface ->addTriangle(vtx0,vtx1,vtx2);
			
			if (object->data.mesh->face[t].v[3])
			{
				float* vt3 = object->data.mesh->vert[object->data.mesh->face[t].v[3]].xyz;
				btVector3 vtx3(vt3[0],vt3[1],vt3[2]);
				minVert.setMin(vtx3); maxVert.setMax(vtx3);
				meshInterface ->addTriangle(vtx0,vtx3,vtx2);//?
			}
		}
		if (!meshInterface->getNumTriangles())
			return 0;

/* boundtype */
#define OB_BOUND_BOX		0
#define OB_BOUND_SPHERE		1
#define OB_BOUND_CYLINDER	2
#define OB_BOUND_CONE		3
#define OB_BOUND_POLYH		4
#define OB_BOUND_POLYT		5
#define OB_BOUND_DYN_MESH   6

/* ob->gameflag */
#define OB_DYNAMIC		1
//#define OB_CHILD		2
//#define OB_ACTOR		4
//#define OB_INERTIA_LOCK_X	8
//#define OB_INERTIA_LOCK_Y	16
//#define OB_INERTIA_LOCK_Z	32
//#define OB_DO_FH			64
//#define OB_ROT_FH			128
//#define OB_ANISOTROPIC_FRICTION 256
//#define OB_GHOST			512
#define OB_RIGID_BODY		1024
//#define OB_BOUNDS		2048

//#define OB_COLLISION_RESPONSE	4096//??
//#define OB_COLLISION	65536
//#define OB_SOFT_BODY	0x20000

		btVector3 localPos = (minVert+maxVert)*0.5f;
		btVector3 localSize= (maxVert-minVert)*0.5f;

		
		btTransform	worldTrans;
		worldTrans.setIdentity();
		worldTrans.setOrigin(btVector3(object->location[0],object->location[1],object->location[2]));
		//		blenderobject->loc[0]+blenderobject->dloc[0]//??
		
		//btVector3 eulerXYZ(object->rotphr[0],object->rotphr[1],object->rotphr[2]);
		worldTrans.getBasis().setEulerZYX(object->rotphr[0],object->rotphr[1],object->rotphr[2]);
		btVector3 scale(object->scaling[0],object->scaling[1],object->scaling[2]);

		if ( (object->gameflag & OB_RIGID_BODY) || (object->gameflag & OB_DYNAMIC))
		{
			//m_destinationWorld->addRigidBody(
			btCollisionShape* colShape = 0;

			switch (object->boundtype)
			{
			case OB_BOUND_SPHERE:
				{
					btScalar radius = localSize[localSize.maxAxis()];
					colShape = new btSphereShape(radius);
					break;
				};
			case OB_BOUND_BOX:
				{
					colShape = new btBoxShape(localSize);
					break;
				}
			case OB_BOUND_CYLINDER:
				{
					colShape = new btCylinderShapeZ(localSize);
					break;
				}
			case OB_BOUND_CONE:
				{
					btScalar radius = btMax(localSize[0], localSize[1]);
					btScalar height = 2.f*localSize[2];
					colShape = new btConeShapeZ(radius,height);
					break;
				}
			case OB_BOUND_POLYT:
			
				{
					//better to approximate it, using btShapeHull
					colShape = new btConvexTriangleMeshShape(meshInterface);
					break;
				}
			case OB_BOUND_POLYH:
			case OB_BOUND_DYN_MESH:
				{
					btGImpactMeshShape* gimpact = new btGImpactMeshShape(meshInterface);
					gimpact->postUpdate();
					colShape = gimpact;
					break;
				}

			default:
				{

				}
			};
			
			if (colShape)
			{
				colShape->setLocalScaling(scale);
				btVector3 inertia;
				colShape->calculateLocalInertia(object->mass,inertia);
				btRigidBody* body = new btRigidBody(object->mass,0,colShape,inertia);
				if (!(object->gameflag & OB_RIGID_BODY))
				{
					body->setAngularFactor(0.f);
				}
				body->setWorldTransform(worldTrans);
				m_destinationWorld->addRigidBody(body);
				//body->setActivationState(DISABLE_DEACTIVATION);

				createGraphicsObject(object,body);
				return body;
			}

		} else
		{
			btCollisionObject* colObj = new btCollisionObject();
			colObj->setWorldTransform(worldTrans);

			btCollisionShape* colShape =0;
			if (meshInterface->getNumTriangles()>0)
			{
				btBvhTriangleMeshShape* childShape = new btBvhTriangleMeshShape(meshInterface,true);
				
				if (scale[0]!=1. || scale[1]!=1. || scale[2]!=1.)
				{
					colShape = new btScaledBvhTriangleMeshShape(childShape,scale);
				} else
				{
					colShape = childShape;
				}
				colObj->setCollisionShape(colShape);

				m_destinationWorld->addCollisionObject(colObj);
			
				createGraphicsObject(object,colObj);
				return colObj;

			}
			
		}
	}
	return 0;
}

void BulletBlendReader::convertSingleMesh(_bMesh* mesh)
{

}
