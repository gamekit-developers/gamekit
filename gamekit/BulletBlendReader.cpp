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


int gTotalExpands=0;
char	gTmpFieldBuffer[4*1024*1024];


static char* gBuffer = 0;
static int gCurrentFilePtr = 0;
static int gFileSize = 0;


long MY_FILELENGTH(FILE *fp) {
  long currentpos = ftell(fp); /* save current cursor position */
  long newpos;
  fseek(fp, 0, SEEK_END); /* seek to end */
  newpos = ftell(fp); /* find position of end -- this is the length */
  fseek(fp, currentpos, SEEK_SET); /* restore previous cursor position */
  return newpos;
}


MY_FILETYPE* MY_FILE_OPEN_FOR_READ(FILE* fp)
{
		long currentpos = ftell(fp); /* save current cursor position */
		long newpos;
		int bytesRead;

		fseek(fp, 0, SEEK_END); /* seek to end */
		newpos = ftell(fp); /* find position of end -- this is the length */
		fseek(fp, currentpos, SEEK_SET); /* restore previous cursor position */
		gFileSize = newpos;
		gBuffer = (char*)malloc(gFileSize);
		bytesRead = fread(gBuffer,gFileSize,1,fp);
		return gBuffer;
}


MY_FILETYPE* MY_OPEN_FOR_READ(const char *const filename)
{
	FILE* gFile = fopen(filename,"rb");
	if (gFile)
	{
		MY_FILE_OPEN_FOR_READ (gFile);
		gCurrentFilePtr = 0;
		fclose(gFile);
		gFile = 0;
		return gBuffer;
	}
	return 0;
}

int MY_GETC(MY_FILETYPE *const fp)
{
	return gBuffer[gCurrentFilePtr++];
}

void MY_SEEK(MY_FILETYPE* fp,int pos)
{
	gCurrentFilePtr = pos;
}

#define MY_REWIND(fp) MY_SEEK(fp,0)
int MY_READ(unsigned char* dest,int size,int num,MY_FILETYPE* fp)
{
	int n;
	memcpy(dest,&gBuffer[gCurrentFilePtr],size*num);
	gCurrentFilePtr += size*num;
	return num;
}

unsigned char* MY_FAKE_READ(int size,int num,MY_FILETYPE* fp)
{
	unsigned char* ptr = (unsigned char*) &gBuffer[gCurrentFilePtr];
	gCurrentFilePtr += size*num;
	return ptr;
}


void	MY_CLOSE(MY_FILETYPE* fp)
{
	free(gBuffer);
	gBuffer = 0;
	gCurrentFilePtr = 0;
	gFileSize = 0;

}

int	MY_TELL(MY_FILETYPE* fp)
{
	return gCurrentFilePtr;
}

int	MY_ATEOF(MY_FILETYPE* fp)
{
	return gCurrentFilePtr<gFileSize? 0 : 1;
}


int readData(char* file, BlendFile* bf);

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

	readData(buffer,m_bf);

	if (verboseDumpAllTypes)
	{
		//blend_dump_typedefs(m_bf);
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
		for (i=0; i<blend_file->m_blocks.size(); ++i) {
			/*fprintf(stderr, "%04x? ", blend_file->blocks[i].blender_pointer); */
			if (blend_file->m_blocks[i]->blender_pointer == blendpointer) {
				return blend_file->m_blocks[i];
			}
		}
	}

	return NULL;
}

#define NUM_KEYWORDS 11
char* keywords[NUM_KEYWORDS] = {"FileGlobal", "Scene", "Object", "bSensor", "bKeyboardSensor", "bAlwaysSensor", "bController","bActuator","bIpoActuator","bRigidBodyJointConstraint", "bConstraint"};

bool	BulletBlendReader::needsExtraction(const char* type_name)
{
	bool found = false;

	for (int i=0;i<NUM_KEYWORDS;i++)
	{
		char* keyword = keywords[i];

		if (strcmp(type_name,keyword)==0)
			return true;
	}
	return false;
}


int
blend_object_getstring(BlendFile* blend_file,
					   BlendObject obj,
					   char *dest, int max_chars)
{
	int strpos = 0;
	int dim1, dim2;
	int rtn = 1;
	BlendObjType bo_type;

	name_is_array(blend_file->names[obj.name],
		&dim1, &dim2);

	if (dim2 < max_chars) {
		max_chars = dim2;
	}

	bo_type = blend_object_type(blend_file, obj);
	if (! (bo_type==BLEND_OBJ_CHAR8 || bo_type==BLEND_OBJ_UCHAR8)) {
		printf("tried to get string from an object that's not of type uchar/char. (is type %d)\n", bo_type);
		rtn = 0;
		goto done;
	}

	for (strpos=0; strpos<max_chars-1; ++strpos) {
		char c;
		if (!blend_object_array_getdata(blend_file, &c, obj, 0, strpos)) {
			printf("unexpected error accessing array. :(\n");
			rtn = 0;
			goto done;
		}
		if (c == '\0') {
			goto done;
		}
		dest[strpos] = c;
	}

done:
	dest[strpos] = '\0';
	return rtn;
}


int
blend_object_get_IDname(BlendFile* blend_file,
						BlendObject obj,
						char *dest, int max_chars) 
{
	if (BLEND_OBJ_STRUCT == blend_object_type(blend_file, obj)) {
		BlendObject idstruc_obj;
		/* See if this structure has an 'id' field */
		if (blend_object_structure_getfield(blend_file, &idstruc_obj,
			obj, "id")) {
				/* we have an 'id' field -- look for a 'name' field within it */
				BlendObject name_obj;
				/* An ID field should have a 'name' field, but we'll make sure... */
				if (BLEND_OBJ_STRUCT == blend_object_type(blend_file, idstruc_obj) &&
					blend_object_structure_getfield(blend_file, &name_obj,
					idstruc_obj, "name")) {
						/* great, get the string from the 'name' field. */
						if (blend_object_getstring(blend_file, name_obj,
							dest, max_chars)) {
								return 1;
						} else {
							printf("oh no, couldn't extract string.\n");
						}
				} else {
					/* next entry please. */
				}
		} else {
			/* this block's type structure doesn't have an ID field at all */
		}
	} else {
		/* this block's type isn't a structure */
		printf("non-objectstruct type of type %d\n",		blend_object_type(blend_file, obj));
	}

	return 0; /* failure */
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
//			printf("	%s %s = ",m_bf->types[fieldType].name, m_bf->names[fieldName]);


			int dim1=1;
			int dim2=1;
			name_is_array(m_bf->names[fieldName], &dim1, &dim2);

			int k,l;
			void** ptrptr;

			btDataValue* val = new btDataValue();
			val->m_name = fieldName;
			val->m_type = fieldType;

			dob->m_dataMap.insert(m_bf->names[fieldName],val);


			///for arrays, use an aligned object array. This is very bloated.

			btDataValue* arrayVal =  0;



			///this access is a bloated, will probably make special cases for common arrays
			if (dim1>1 || dim2>1)
			{
				arrayVal = val;
				arrayVal->m_valueArray.resize(dim1);
				for (int i=0;i<dim1;i++)
				{
					arrayVal->m_valueArray[i].m_name = fieldName;
					arrayVal->m_valueArray[i].m_type = fieldType;
					arrayVal->m_valueArray[i].m_valueArray.resize(dim2);

					for (int j=0;j<dim2;j++)
					{
						arrayVal->m_valueArray[i].m_valueArray[j].m_name = fieldName;
						arrayVal->m_valueArray[i].m_valueArray[j].m_type = fieldType;
					}
				}
			}


			for (k=0;k<dim1;k++)
			{
				for (l=0;l<dim2;l++)
				{
					if (arrayVal)
						val = &arrayVal->m_valueArray[k].m_valueArray[l];

					ptrptr = (void**)&block->m_array_entries_[0]->field_bytes[block->m_array_entries_[0]->m_field_offsets[field_index+dim2*k+ l]];

					objType = typestring_to_blendobj_type(m_bf, m_bf->types[fieldType].name) ;
					if (ptrptr)
					{
						switch (objType)
						{
						case BLEND_OBJ_NULL:
							{
								break;
							}

						case BLEND_OBJ_UCHAR8:
							{
								val->m_iValue = *(unsigned char*)ptrptr;
//								printf("%d ",val->m_iValue);
								break;
							}
						case BLEND_OBJ_CHAR8:
							{
								val->m_iValue = *(char*)ptrptr;
//								printf("%d ",val->m_iValue);
								break;
							}
						case BLEND_OBJ_USHORT16:
							{
								val->m_iValue = *(unsigned short int*)ptrptr;
//								printf("%d ",val->m_iValue);
								break;
							}
						case BLEND_OBJ_SHORT16:
							{
								val->m_iValue = *(short int*)ptrptr;
//								printf("%d ",val);
								break;
							}
						case BLEND_OBJ_ULONG32:
							{
								val->m_uiValue = *(unsigned int*)ptrptr;
//								printf("%d ",val->m_uiValue);
								break;
							}
						case BLEND_OBJ_LONG32:
							{
								val->m_iValue = *(int*)ptrptr;
//								printf("%d ",val->m_iValue);
								break;
							}
						case BLEND_OBJ_FLOAT:
							{
								val->m_fValue = *(float *)ptrptr;
//								printf("%f ",val->m_fValue);
								break;
							}
						case BLEND_OBJ_DOUBLE:
							{
								double  val = *(double *)ptrptr;
//								printf("%f ",val);
								break;
							}
						case BLEND_OBJ_POINTER:
							{
								void* val = *ptrptr;
//								printf("%x ",val);
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

									uint32_t ptr = *(uint32_t*)ptrptr;
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
//															printf("\"%s\" ",dest);

													} else {
//														printf("%x ",block->blender_pointer);
													}
											} else {
//												printf("%x ",block->blender_pointer);
											}
										} else
										{
//											printf("NULL ");
										}

									} else
									{
//										printf("NULL ");
									}
								} else
								{
									if (!strcmp(m_bf->names[m_bf->types[obj.type].fieldnames[i]],"id"))
									{
										if (blend_object_get_IDname(m_bf, obj, dest, max_chars))
										{
//											printf("\"%s\" ",dest);
											break;
										}
									}
//									printf("Embedded struct\n");

								}
								break;
							}
						default:
							{
//								printf("unknown value");
							}
						}
					}
				}
			}



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
	}

	dob->m_blenderPointer = block->blender_pointer;
	m_dataObjects.insert(block->blender_pointer,dob);

	return dob;
}




#define CONSTRAINT_TYPE_RIGIDBODYJOINT 17
#define CONSTRAINT_RB_BALL		1
#define CONSTRAINT_RB_HINGE		2
#define CONSTRAINT_RB_CONETWIST 4
#define CONSTRAINT_RB_VEHICLE	11
#define CONSTRAINT_RB_GENERIC6DOF 12

///flags
#define CONSTRAINT_DISABLE_LINKED_COLLISION 0x80



void	BulletBlendReader::convertConstraints()
{

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
							}
#endif

							btVector3 pivotInA(rbConstraint->getFloatValue("pivX"),rbConstraint->getFloatValue("pivY"),rbConstraint->getFloatValue("pivZ"));
							btVector3 pivotInB(0,0,0);
							btCollisionObject* colObj = (btCollisionObject*) ob->m_userPointer;
							btRigidBody* rbA = btRigidBody::upcast(colObj);
							btRigidBody* rbB = 0;

							int flag = rbConstraint->getIntValue("flag");
							bool disableCollisionBetweenLinkedBodies = (flag & CONSTRAINT_DISABLE_LINKED_COLLISION) != 0;

							if (rbA)
							{
								unsigned int objectBPtr = rbConstraint->getIntValue("*tar");
								if (objectBPtr)
								{
									btDataObject** dataBPtr = m_dataObjects[objectBPtr];
									if (dataBPtr && *dataBPtr)
									{
										btDataObject* dataB = *dataBPtr;
										btCollisionObject* colObjB = (btCollisionObject*)dataB->m_userPointer;
										rbB = btRigidBody::upcast(colObjB);
										pivotInB = rbB->getCenterOfMassTransform().inverse()(rbA->getCenterOfMassTransform()(pivotInA));
									}
								}

								float radsPerDeg = 6.283185307179586232f / 360.f;

								//localConstraintFrameBasis
								btMatrix3x3 localCFrame;
								localCFrame.setEulerZYX(radsPerDeg*rbConstraint->getFloatValue("axX"),radsPerDeg*rbConstraint->getFloatValue("axY"),radsPerDeg*rbConstraint->getFloatValue("axZ"));
								btVector3 axisInA = localCFrame.getColumn(0);
								btVector3 axis1 = localCFrame.getColumn(1);
								btVector3 axis2 = localCFrame.getColumn(2);
								bool angularOnly = false;


								/* important: these defines need to match up with PHY_DynamicTypes headerfile */
								switch (rbConstraintType)
								{

								case CONSTRAINT_RB_BALL:
									{
										btPoint2PointConstraint* p2p = 0;

										if (rbB)
										{
											p2p = new btPoint2PointConstraint(*rbA,*rbB,pivotInA,pivotInB);
										} else
										{
											p2p = new btPoint2PointConstraint(*rbA,pivotInA);
										}

										m_destinationWorld->addConstraint(p2p);

										break;
									}
								case CONSTRAINT_RB_HINGE:
									{
										btHingeConstraint* hinge = 0;
										if (rbB)
										{
											btVector3 axisInB = rbB ?
											(rbB->getCenterOfMassTransform().getBasis().inverse()*(rbA->getCenterOfMassTransform().getBasis() * axisInA)) :
											rbA->getCenterOfMassTransform().getBasis() * axisInA;

											hinge = new btHingeConstraint(*rbA,*rbB,pivotInA,pivotInB,axisInA,axisInB);


										} else
										{
											hinge = new btHingeConstraint(*rbA,	pivotInA,axisInA);

										}
										hinge->setAngularOnly(angularOnly);

										//m_constraints.push_back(hinge);
										m_destinationWorld->addConstraint(hinge,disableCollisionBetweenLinkedBodies);
										//hinge->setUserConstraintId(gConstraintUid++);
										//hinge->setUserConstraintType(type);
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

										btTransform frameInA;
										btTransform frameInB;
										if (axis1.length() == 0.0)
										{
											btPlaneSpace1( axisInA, axis1, axis2 );
										}
										frameInA.getBasis().setValue( axisInA.x(), axis1.x(), axis2.x(),
																	  axisInA.y(), axis1.y(), axis2.y(),
																	  axisInA.z(), axis1.z(), axis2.z() );
										frameInA.setOrigin( pivotInA );
										btTransform inv = rbB->getCenterOfMassTransform().inverse();
										btTransform globalFrameA = rbA->getCenterOfMassTransform() * frameInA;
										frameInB = inv  * globalFrameA;
										bool useReferenceFrameA = true;
										btGeneric6DofSpringConstraint* genericConstraint = new btGeneric6DofSpringConstraint(*rbA,*rbB,	frameInA,frameInB,useReferenceFrameA);
										m_destinationWorld->addConstraint(genericConstraint,disableCollisionBetweenLinkedBodies);

										//if it is a generic 6DOF constraint, set all the limits accordingly
										int dof;
										int dofbit=1;
										for (dof=0;dof<6;dof++)
										{
											if (flag & dofbit)
											{
												///this access is a bloated, will probably make special cases for common arrays
												btScalar minLimit = (*rbConstraint->m_dataMap.find("minLimit[6]"))->m_valueArray[0].m_valueArray[dof].m_fValue;
												btScalar maxLimit = (*rbConstraint->m_dataMap.find("maxLimit[6]"))->m_valueArray[0].m_valueArray[dof].m_fValue;
												genericConstraint->setLimit(dof,minLimit,maxLimit);
											} else
											{
												//minLimit > maxLimit means free(disabled limit) for this degree of freedom
												genericConstraint->setLimit(dof,1,-1);
											}
											dofbit<<=1;
										}
										break;
									}

								default:
									{
										printf("unsupported rigid body constraint type\n");
									}
								}
							} //if (rbA)
						}
					}

				}
			}
		}

	}
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
//		blend_dump_blocks(m_bf);
	}

	int sceneVisibleLayer = 0;
	btDataObject* fileGlobalObject =0;


	int j;
	for (j=0; j<m_bf->m_blocks.size(); ++j)
	{
		{
			int entry_count = blend_block_get_entry_count(m_bf, m_bf->m_blocks[j]);
			for (int i=0; i<entry_count; ++i)
			{
				BlendObject obj = blend_block_get_object(m_bf, m_bf->m_blocks[j], i);




				BlendObject data_obj;
				BlendObject data_obj2;

				BlendBlock* tmpBlock=0;

				{
					const char* type_name = m_bf->types[obj.type].name;

#if DUMP_BLOCK_NAMES
					printf("type_name=%s. ",type_name);
					printf("block blenderptr = %lx\n",m_bf->m_blocks[j]->blender_pointer);
#endif //DUMP_BLOCK_NAMES

					btDataObject* dob = 0;

					if (needsExtraction(type_name))
					{
						dob = extractSingleObject(&obj);
					}

					if (strcmp(type_name,"FileGlobal")==0)
					{
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

						if (fileGlobalObject && fileGlobalObject->getIntValue("*curscene",0)==dob->m_blenderPointer)
						{
							sceneVisibleLayer = dob->getIntValue("lay",0);
						}


					}


					if (strcmp(type_name,"Object")==0)
					{
						bObj tmpObj;
						blend_acquire_obj_from_obj(m_bf,&obj,&tmpObj,0);
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

	createParentChildHierarchy();

	///now fix up some constraint and logic bricks
	
	convertConstraints();

	convertLogicBricks();

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

				void* gfxObject = createGraphicsObject(object,body);
				body->setUserPointer(gfxObject);

				return body;
			}

		} else
		{

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

				btVector3 inertia(0,0,0);
				btRigidBody* colObj = new btRigidBody(0.f,0,colShape,inertia);
				colObj->setWorldTransform(worldTrans);
				colObj->setCollisionShape(colShape);

				m_destinationWorld->addRigidBody(colObj);

				void* gfxObject = createGraphicsObject(object,colObj);
				colObj->setUserPointer(gfxObject);

				return colObj;

			}

		}
	}
	return 0;
}

void BulletBlendReader::convertSingleMesh(_bMesh* mesh)
{

}








extern int MY_READ(unsigned char* dest,int size,int num,MY_FILETYPE* fp);



static unsigned long
read_ulong(MY_FILETYPE* file) {
	unsigned char c[4];
	if (MY_READ(c, 4, 1, file) == 1) {
		return c[0] | (c[1]<<8) | (c[2]<<16) | (c[3]<<24);
	} else {
		return 0xFFFFFFFF;
	}
}
#define EXPANDO1(ARRAY,ADDITION) \
	do { \
	(ARRAY) = (BlendField*)realloc((ARRAY), sizeof((ARRAY)[0]) * (1 + (ARRAY##_count))); \
	(ARRAY)[ARRAY##_count] = (ADDITION); \
	++(ARRAY##_count); \
	} while(0)

#define EXPANDO2(ARRAY,ADDITION) \
	do { \
	(ARRAY) = (BlendBlock*)realloc((ARRAY), sizeof((ARRAY)[0]) * (1 + (ARRAY##_count))); \
	(ARRAY)[ARRAY##_count] = (ADDITION); \
	++(ARRAY##_count); \
	} while(0)


#define EXPANDO3(ARRAY,ADDITION) \
	do { \
	(ARRAY) = (int*)realloc((ARRAY), sizeof((ARRAY)[0]) * (1 + (ARRAY##_count))); \
	(ARRAY)[ARRAY##_count] = (ADDITION); \
	++(ARRAY##_count); \
	} while(0)

#define EXPANDO4(ARRAY,ADDITION) \
	do { \
	(ARRAY) = (char*)realloc((ARRAY), sizeof((ARRAY)[0]) * (1 + (ARRAY##_count))); \
	(ARRAY)[ARRAY##_count] = (ADDITION); \
	++(ARRAY##_count); \
	} while(0)


#define EXPANDO6(ARRAY,ADDITION) \
	do { \
	(ARRAY) = (BlendType*)realloc((ARRAY), sizeof((ARRAY)[0]) * (1 + (ARRAY##_count))); \
	(ARRAY)[ARRAY##_count] = (ADDITION); \
	++(ARRAY##_count); \
	} while(0)

#define EXPANDO5(ARRAY,ADDITION) \
	do { \
	(ARRAY) = (char**)realloc((ARRAY), sizeof((ARRAY)[0]) * (1 + (ARRAY##_count))); \
	(ARRAY)[ARRAY##_count] = (ADDITION); \
	++(ARRAY##_count); \
	} while(0)


#define EXPANDO_MULTIPLE(ARRAY,ADDITIONPTR,NUMBER) \
	do { \
	(ARRAY) = (char*)realloc((ARRAY), sizeof((ARRAY)[0]) * (NUMBER +(ARRAY##_count))); \
	memcpy(&(ARRAY)[ARRAY##_count], ADDITIONPTR, sizeof((ARRAY)[0]) * NUMBER); \
	(ARRAY##_count) += NUMBER; \
	} while(0)

#define SIZE_ROUNDUP(SIZE) (((SIZE) + sizeof(int) - 1) & ~(sizeof(int) - 1))


static void
recursively_read_type(MY_FILETYPE* file, BlendFile* bf,
					  unsigned long section_type,
					  BlendField* field)
{
	char *new_data = NULL;
	int new_data_size = 0;
	int dim1, dim2;

	if (bf->types[section_type].is_struct) {
		int i;
		/*fprintf(stderr, "type%d(%s)is_struct(%d) ", section_type, bf->types[section_type].name, bf->types[section_type].size);*/
		for (i=0; i<bf->types[section_type].fieldtypes_count; ++i) {
			/*fprintf(stderr, "@%d ", i);*/
			int j,k;
#if 0
			if (name_is_array(bf->names[bf->types[section_type].fieldnames[i]],
				&j,&k) || 1) {
					dprintf(stderr, " %s/%s=[%d][%d] ",
						bf->types[bf->types[section_type].fieldtypes[i]].name,
						bf->names[bf->types[section_type].fieldnames[i]], j, k);
			}
#endif 
			if (name_is_pointer(bf->names[bf->types[section_type].fieldnames[i]])) {
				/*fprintf(stderr, "*(4) ");*/
				name_is_array(bf->names[bf->types[section_type].fieldnames[i]],
					&dim1,&dim2);
				new_data_size = SIZE_ROUNDUP(4);
				new_data = (char*)malloc(new_data_size);
				/*fprintf(stderr, " ");*/
				for (j=0; j<dim1; ++j) {
					for (k=0; k<dim2; ++k) {
						/*fprintf(stderr, "*");*/
						new_data[0] = MY_GETC(file);
						new_data[1] = MY_GETC(file);
						new_data[2] = MY_GETC(file);
						new_data[3] = MY_GETC(file);
						//EXPANDO(field->field_offsets, field->field_bytes_count);

						//field->field_offsets = (int*)realloc((field->field_offsets), sizeof((field->field_offsets)[0]) * (1 + (field->field_offsets_count)));
						//field->field_offsets[field->field_offsets_count] = (field->field_bytes_count);
						field->m_field_offsets.push_back(field->field_bytes_count);
						++field->field_offsets_count;


						//	EXPANDO_MULTIPLE(field->field_bytes, new_data, new_data_size);
						{
							//field->field_bytes = (char*)realloc((field->field_bytes), sizeof((field->field_bytes)[0]) * (new_data_size +(field->field_bytes_count))); 
							//memcpy(&(field->field_bytes)[field->field_bytes_count], new_data, sizeof((field->field_bytes)[0]) * new_data_size); 

							memcpy( &gTmpFieldBuffer[field->field_bytes_count],new_data,new_data_size);
							field->field_bytes_count += new_data_size; 
							gTotalExpands++;
						}

						/*fprintf(stderr, "N*(%d) ", new_data_size);*/
					}
				}
				free(new_data);
			} else {
				name_is_array(bf->names[bf->types[section_type].fieldnames[i]],
					&dim1,&dim2);
				/*fprintf(stderr, " ");*/
				for (j=0; j<dim1; ++j) {
					for (k=0; k<dim2; ++k) {
						/*fprintf(stderr, "R");*/
						recursively_read_type(file, bf,
							bf->types[section_type].fieldtypes[i],
							field);
					}
				}
			}
		}
	} else {
		/*fprintf(stderr, "type%d(%s)plain(%d) ", section_type, bf->types[section_type].name, bf->types[section_type].size);     */
		new_data_size = SIZE_ROUNDUP(bf->types[section_type].size);
		/*fprintf(stderr, "%d... ", bf->types[section_type].size);
		if (bf->types[section_type].size > 4) {
		fprintf(stderr, "%d ", field->field_bytes_count);
		}*/
		if (new_data_size) {

#ifdef REAL_READ
			new_data = (char*)malloc(new_data_size);
			MY_READ((unsigned char*)new_data, 1, bf->types[section_type].size, file);
			EXPANDO3(field->field_offsets, field->field_bytes_count);
			EXPANDO_MULTIPLE(field->field_bytes, new_data, new_data_size);
			/*fprintf(stderr, "ND(%d) ", new_data_size); */
			free(new_data);
#else
			new_data =  &gTmpFieldBuffer[field->field_bytes_count];//(char*)malloc(new_data_size);
			MY_READ((unsigned char*)new_data, 1, bf->types[section_type].size, file);

			//new_data = (char*)MY_FAKE_READ(1, bf->types[section_type].size, file);
			//EXPANDO3(field->field_offsets, field->field_bytes_count);
			field->m_field_offsets.push_back(field->field_bytes_count);
			field->field_offsets_count++;

		

			{
			//EXPANDO_MULTIPLE(field->field_bytes, new_data, new_data_size);
				//field->field_bytes = (char*)realloc((field->field_bytes), sizeof((field->field_bytes)[0]) * (new_data_size +(field->field_bytes_count))); 
				//memcpy(&(field->field_bytes)[field->field_bytes_count], new_data, sizeof((field->field_bytes)[0]) * new_data_size); 
				//memcpy( &gTmpFieldBuffer[field->field_bytes_count],new_data,new_data_size);
				field->field_bytes_count += new_data_size; 
			}
				//free(new_data);
			gTotalExpands++;
			/*fprintf(stderr, "ND(%d) ", new_data_size); */

#endif//REAL_READ
		} else {
			//dprintf(stderr, " <NUL[%d]> ",			bf->types[section_type].size);
			printf(" <NUL[%d]> ",			bf->types[section_type].size);
		}
	}

}


void  BulletBlendReader::read_type(char* file, BlendFile* bf, unsigned long section_type,BlendField& rtn)
{
	rtn.field_bytes = NULL;
	rtn.field_bytes_count = 0;
	//rtn.field_offsets = NULL;
	rtn.field_offsets_count = 0;

//	memset(gTmpFieldBuffer,0,1024*1024);
	recursively_read_type(file, bf, section_type, &rtn);
	rtn.field_bytes = (char*)malloc(rtn.field_bytes_count);
	memcpy(rtn.field_bytes,gTmpFieldBuffer,rtn.field_bytes_count);
}

int BulletBlendReader::readData(char* file, BlendFile* bf)
{
//blend_read_data(MY_FILETYPE* file, BlendFile* bf)
	long next_block_start = 12;
	int finished_extracting = 0;
	char section_name[5] = {0,0, 0,0, 0};
	int blockiter = 0;


	/* slurp up the whole file block by block! */

	do {
		
		unsigned long section_size;
		unsigned long section_pointer;
		unsigned long section_type;
		unsigned long section_ents;
		MY_SEEK(file, next_block_start);

		blockiter++;
		MY_READ((unsigned char*)section_name, 4, 1, file);


		if (strcmp(section_name, "DNA1") != 0) {


			int i;
			BlendBlock* blockPtr = new BlendBlock;
			BlendBlock& block = *blockPtr;


			if (strcmp(section_name, "IM") == 0) {
				//printf("image\n");
			}




			if (strcmp(section_name, "IP") == 0) {
				//printf("ipo\n");
			}

			section_size    = read_ulong(file);
			section_pointer = read_ulong(file);
			section_type    = read_ulong(file);
			section_type    = bf->strc_indices[section_type];
			section_ents    = read_ulong(file);

//			if (dumpNames)
			{
//				printf("section_name = %s, section_type = %d / %s\n",section_name,section_type,bf->types[section_type]);

			}

			memcpy(block.tag, section_name, 4);
			block.tag[4] = '\0';
			block.type_index = section_type;
			block.blender_pointer = section_pointer;
			/*block.fixed_pointer = NULL;*/
			//block.array_entries = NULL;

			//block.array_entries_count = 0;
			block.customData = 0;
			block.customDataSize = 0;

			//      dprintf(stderr, "\nsizeof(%s)=%ld: %s[%ld]\n", section_name, section_size, bf->types[section_type].name, section_ents);

			if (strcmp("Link",bf->types[section_type].name)==0)
			{
				if (section_size>0)
				{
					//read customData
					block.customData= (char*)malloc(section_size);
					block.customDataSize = section_size;
					MY_READ((unsigned char*)block.customData,1,section_size,file);
				}
			} else
			{
				for (i=0; i<section_ents; ++i) {
					BlendField* field = new BlendField;
					read_type(file, bf, section_type,*field);
					block.m_array_entries_.push_back(field);
				}
			}

			bf->m_blocks.push_back(blockPtr);

			next_block_start += 4+4+4+4+4 + section_size;

#ifdef B_DEBUG
			if (MY_TELL(file) > next_block_start) {
				//dprintf(stderr, " **OVER-READ(%ld,%ld)** ",	MY_TELL(file), next_block_start);
				if (strcmp(bf->types[section_type].name, "Link") == 0 &&
					MY_TELL(file) - next_block_start == 4) {
						//dprintf(stderr, "<- don't panic, known Link struct weirdness.");
				} else {
					dprintf(stderr, "<- okay, PANIC!");
				}
				//dprintf(stderr, "\n");
			} else if (MY_TELL(file) < next_block_start) {
				/*dprintf(stderr, " **under-READ(%ld,%ld)** ",
				MY_TELL(file), next_block_start);*/
			} else {
				/*dprintf(stderr, " :) ");*/
			}
#endif

		} else {
			finished_extracting = 1;
		}

	} while (!finished_extracting);

	return 1;

}



static unsigned short
read_ushort(MY_FILETYPE* file) {
	unsigned char c[2];
	if (MY_READ(c, 2, 1, file) == 1) 
	{
		return c[0] | (c[1]<<8);
	} else {
		return 0xFFFF;
	}
}


static long
seek_past_string(MY_FILETYPE* file, const char *str) {
	const int match_max = strlen(str);
	int match_now = 0;

	do {
		const char c = MY_GETC(file);
		if (c == str[match_now]) {
			++match_now;
		} else {
			match_now = 0;
		}
	} while(match_now < match_max &&
		!MY_ATEOF(file));

	if (MY_ATEOF(file)) {
		return -1;
	}

	return MY_TELL(file) - match_max;
}



static long
read_long(MY_FILETYPE* file) {
	unsigned char c[4];
	if (MY_READ(c, 4, 1, file) == 1) {
		return ((unsigned long)c[0] | (c[1]<<8) | (c[2]<<16) | (c[3]<<24));
	} else {
		return 0xFFFFFFFF;
	}
}

BlendFile* blend_read(char* file)
{
	char blender_mark[10] = {0,0,0,0, 0,0,0,0,0,0};
	BlendFile *bf;
	long sdnaname_offs, type_offs, tlen_offs, strc_offs, endb_offs;
	long sdnaname_size, type_size, tlen_size, strc_size;
	long sdnaname_ents, type_ents, tlen_ents, strc_ents;

	MY_REWIND(file);

	/* Check file signature */

	MY_READ((unsigned char*)blender_mark, 1, 9, file);
	if (strcmp(blender_mark, "BLENDER_v") != 0) {
		printf("Not a valid Blender file (.blend file needs to be written on a 32bit little-endian machine)\n");
		return NULL;
	}

	/* Alloc a handle to return */

	bf = new BlendFile();
	
	bf->types = NULL;
	bf->types_count = 0;
	bf->names = NULL;
	bf->names_count = 0;
	bf->m_blocks.clear();// = NULL;
	//bf->blocks_count = 0;
	bf->strc_indices = NULL;
	bf->strc_indices_count = 0;
	bf->name_undef = -1;
	


	/* Scan the whole file (!) for file section markers */

	sdnaname_offs = seek_past_string(file, "SDNANAME");
	sdnaname_ents = read_long(file);
	type_offs     = seek_past_string(file, "TYPE");
	type_ents     = read_long(file);
	tlen_offs     = seek_past_string(file, "TLEN");
	tlen_ents     = type_ents;
	strc_offs     = seek_past_string(file, "STRC");
	strc_ents     = read_long(file);
	endb_offs     = seek_past_string(file, "ENDB");

	if (sdnaname_offs == -1 || type_offs == -1 || tlen_offs == -1 ||
		strc_offs == -1 || endb_offs == -1) 
	{
			printf("Couldn't find all necessary file markers. :(\n");
			return NULL;
	}

	/* Move marker offsets to point to the start of each one's block */

	sdnaname_offs += 8 + 4;
	sdnaname_size = type_offs - sdnaname_offs;

	type_offs += 4 + 4;
	type_size = tlen_offs - type_offs;

	tlen_offs += 4;
	tlen_size = strc_offs - tlen_offs;

	strc_offs += 4 + 4;
	strc_size = endb_offs - strc_offs;

	/* read the NAME table */

	MY_SEEK(file, sdnaname_offs);
	{
		long offs = 0;
		int i;
		char *top_block_name;

		for (i=0; i<sdnaname_ents; ++i) {
			int finished_name = 0;
			char *this_name_chars = NULL;
			int this_name_chars_count = 0;

			do {
				const char c = MY_GETC(file);
				++offs;
				if (offs == sdnaname_size ||
					c == '\0') {
						finished_name = 1;
				}

				EXPANDO4(this_name_chars, c);
			} while (!finished_name);

			/*fprintf(stderr, "%s ", this_name_chars); */

			EXPANDO5(bf->names, this_name_chars);
		}

		/* our top-block name */
#define BLEND_TOP_BLOCK_NAME "READBLEND_TOP_BLOCK"
		top_block_name = (char*)calloc(1, 1+strlen(BLEND_TOP_BLOCK_NAME));
		strcpy(top_block_name, BLEND_TOP_BLOCK_NAME);
		bf->name_undef = bf->names_count;
		EXPANDO5(bf->names, top_block_name);
	}

	/* read the TYPE table */

	MY_SEEK(file, type_offs);
	{
		long offs = 0;
		int i;

		for (i=0; i<type_ents; ++i) {
			int finished_type = 0;
			char *this_type_chars = NULL;
			int this_type_chars_count = 0;
			BlendType bt;

			do {
				const char c = MY_GETC(file);
				++offs;
				if (offs == type_size ||
					c == '\0') {
						finished_type = 1;
				}

				EXPANDO4(this_type_chars, c);
			} while (!finished_type);

			bt.name = this_type_chars;
			bt.size = -1;
			bt.is_struct = 0;
			bt.fieldtypes_count = 0;
			bt.fieldtypes = NULL;
			bt.fieldnames_count = 0;
			bt.fieldnames = NULL;

			/* fprintf(stderr, "(%d:%s)", bt.size, bt.name);*/

			EXPANDO6(bf->types, bt);
		}
	}

	/* read the TLEN table */

	MY_SEEK(file, tlen_offs);
	{
		int i;
		for (i=0; i<bf->types_count; ++i) {
			unsigned short len = read_ushort(file);
			bf->types[i].size = len;
			/*fprintf(stderr, "sizeof(%s)=%d ", bf->types[i].name, len); */
		}
	}

	/* Read the STRC table */

	MY_SEEK(file, strc_offs);
	{
		int i,j;
		for (i=0; i<strc_ents; ++i) {
			unsigned short struc_type_index, num_fields;
			struc_type_index = read_ushort(file);
			num_fields = read_ushort(file);
			bf->types[struc_type_index].is_struct = 1;
			EXPANDO3(bf->strc_indices, struc_type_index);
			/*dprintf(stderr, "\n%s: ", bf->types[struc_type_index].name); */
			for (j=0; j<num_fields; ++j) {
				unsigned short ftype = read_ushort(file);
				unsigned short fname = read_ushort(file);
				EXPANDO3(bf->types[struc_type_index].fieldtypes, ftype);
				EXPANDO3(bf->types[struc_type_index].fieldnames, fname);
				/*dprintf(stderr, "%s %s ,  ", bf->types[ftype].name, bf->names[fname]); */
			}
		}
	}

	//blend_read_data(file, bf);

	/* Return the new handle */

	return bf;
}

/* endianness conversion macros */
#define BGETLEUINT16(p) ((uint16_t) ( \
	( ((uint16_t)*( 1 + (uint8_t*)(p) ))<<8 ) | \
	( ((uint16_t)*( 0 + (uint8_t*)(p) ))<<0 ) \
	) )
#define BGETLEINT16(p) ((int16_t) BGETLEUINT16(p))
#define BGETLEUINT32(p) ((uint32_t) ( \
	( ((uint32_t)*( 3 + (uint8_t*)(p) ))<<24 ) | \
	( ((uint32_t)*( 2 + (uint8_t*)(p) ))<<16 ) | \
	( ((uint32_t)*( 1 + (uint8_t*)(p) ))<<8 ) | \
	( ((uint32_t)*( 0 + (uint8_t*)(p) ))<<0 ) \
	) )
#define BGETLEINT32(p) ((int32_t) BGETLEUINT32(p))
#define BGETLEUINT64(p) ((uint64_t) ( \
	( ((uint64_t)*( 7 + (uint8_t*)(p) ))<<56 ) | \
	( ((uint64_t)*( 6 + (uint8_t*)(p) ))<<48 ) | \
	( ((uint64_t)*( 5 + (uint8_t*)(p) ))<<40 ) | \
	( ((uint64_t)*( 4 + (uint8_t*)(p) ))<<32 ) | \
	( ((uint64_t)*( 3 + (uint8_t*)(p) ))<<24 ) | \
	( ((uint64_t)*( 2 + (uint8_t*)(p) ))<<16 ) | \
	( ((uint64_t)*( 1 + (uint8_t*)(p) ))<<8 ) | \
	( ((uint64_t)*( 0 + (uint8_t*)(p) ))<<0 ) \
	) )
#define BGETLEINT64(p) ((int64_t) BGETLEUINT64(p))
static float BGETLEFLOAT32(char *p) {
	union { /* use type-punning, or aliasing optimization will kick our arse */
		uint32_t u32;
		float f32;
	} punner;
	punner.u32 = BGETLEUINT32(p);
	return punner.f32;
}
static double BGETLEDOUBLE64(char *p) {
	union { /* use type-punning, or aliasing optimization will kick our arse */
		uint64_t u64;
		float f64;
	} punner;
	punner.u64 = BGETLEUINT64(p);
	return punner.f64;
}

int
blend_object_array_getdata(BlendFile* blend_file,
						   void* dest, BlendObject obj,
						   int dim_index_1, int dim_index_2)
{
	const char* type_name = blend_file->types[obj.type].name;
	const BlendBlock* bb = (const BlendBlock*)obj.block;
	
	void* data;
	int dim1, dim2;

	if (!bb->m_array_entries_.size())
		return 0;

	const BlendField *bf = bb->m_array_entries_[obj.entry_index];

	name_is_array(blend_file->names[obj.name], &dim1, &dim2);
	/*dprintf(stderr, "copying:'%s'[%d][%d] (of [%d][%d]) ", type_name, dim_index_1, dim_index_2, dim1, dim2);*/

	if (dim_index_1 >= dim1 ||
		dim_index_2 >= dim2) {
			printf("Array index (%d,%d) out of bounds for dimensionality [%d][%d]\n", dim_index_1, dim_index_2, dim1, dim2);
			return 0;
	}

	data = &bf->field_bytes[bf->m_field_offsets[obj.field_index +	dim2*dim_index_1 + dim_index_2]];
	/*dprintf(stderr, "fi[%d]byteo[%d]", obj.field_index,
	bf->field_offsets[obj.field_index +
	dim2*dim_index_1 + dim_index_2]);*/

	if (name_is_pointer(blend_file->names[obj.name])) {
		*(BlendBlockPointer*)dest =
			blend_block_from_blendpointer(blend_file,
			BGETLEUINT32(data));
		return 1;
	}

	/* FIXME: might be a good idea to do less-crappy word-size conversions
	here -- these might read beyond the end of malloc'd blocks if we
	ever change our field-padding policy.  There were endian problems
	too; these are believed fixed now. */
	/* The signed conversions look strange because they have to sign-expand
	negative results without relying on right-shifts which have undefined
	behaviour on negative data according to ANSI C. */
	if (blend_type_basename_compare(type_name, "char") == 0) {
		*(char*)dest = (*(char*)data) << (8*sizeof(char)-8) / (1<<(8*sizeof(char)-8));
	} else if (blend_type_basename_compare(type_name, "uchar") == 0) {
		*(unsigned char*)dest = *(unsigned char*)data;
	} else if (blend_type_basename_compare(type_name, "short") == 0) {
		*(int16_t*)dest = BGETLEINT16(data) << (8*sizeof(int16_t)-16) / (1<<(8*sizeof(int16_t)-16));
	} else if (blend_type_basename_compare(type_name, "ushort") == 0) {
		*(uint16_t*)dest = BGETLEUINT16(data);
	} else if (blend_type_basename_compare(type_name, "int") == 0) {
		*(int32_t*)dest = BGETLEINT32(data) << (8*sizeof(int32_t)-32) / (1<<(8*sizeof(int32_t)-32));
	} else if (blend_type_basename_compare(type_name, "long") == 0) {
		*(int32_t*)dest = BGETLEINT32(data) << (8*sizeof(int32_t)-32) / (1<<(8*sizeof(int32_t)-32));
	} else if (blend_type_basename_compare(type_name, "ulong") == 0) {
		*(uint32_t*)dest = BGETLEUINT32(data);
	} else if (blend_type_basename_compare(type_name, "float") == 0) {
		*(float*)dest = BGETLEFLOAT32((char*)data);
		/*fprintf(stderr, "GOT{%f'%f} ", *(float*)dest, *(float*)data);*/
	} else if (blend_type_basename_compare(type_name, "double") == 0) {
		*(double*)dest = BGETLEDOUBLE64((char*)data);
	} else if (blend_type_basename_compare(type_name, "void") == 0) {
		printf("Tried to fetch a void.\n");
		return 0;
	} else {
		printf("Tried to fetch a whole structure.\n");
		return 0;
	}
	return 1; /* success */
}

static BlendObjType
typelong_to_blendobj_type(BlendFile* blend_file,
						  long btype, long bname)
{
	if (name_is_pointer(blend_file->names[bname])) {
		return BLEND_OBJ_POINTER;
	} else if (blend_file->types[btype].is_struct) {
		return BLEND_OBJ_STRUCT;
	} else {
		return typestring_to_blendobj_type(blend_file,
			blend_file->types[btype].name);
	}
}



BlendObjType
blend_object_type(BlendFile* blend_file,
				  BlendObject obj) {
					  return typelong_to_blendobj_type(blend_file,
						  obj.type,
						  obj.name);
}


int
blend_object_structure_getfield(BlendFile* blend_file,
								BlendObject *result,
								BlendObject obj,
								const char* field_name)
{
	if (blend_file->types[obj.type].is_struct) {
		int i;
		int field_index = 0;
		for (i=0; i<blend_file->types[obj.type].fieldnames_count; ++i) {
			if (blend_type_basename_compare(
				blend_file->names[blend_file->types[obj.type].fieldnames[i]],
				field_name)
				== 0) {
					result->type = blend_file->types[obj.type].fieldtypes[i];
					result->name = blend_file->types[obj.type].fieldnames[i];
					result->block = obj.block;
					result->entry_index = obj.entry_index;
					result->field_index = field_index;
					return 1;
			}

			{
				BlendObject qo = obj;
				int fos;
				qo.type = blend_file->types[obj.type].fieldtypes[i];
				qo.name = blend_file->types[obj.type].fieldnames[i];
				qo.field_index = field_index;
				fos = get_num_type_segments(blend_file, qo);
				/*fprintf(stderr, ">>%s %s:%d ",
				blend_file->types[qo.type].name,
				blend_file->names[qo.name], fos);*/
				field_index += fos;
			}
		}
		return 0;
	} else {
		printf("Indexed object isn't a structure!\n");
		return 0;
	}
}

BlendObject
blend_block_get_object(BlendFile* blend_file,
					   BlendBlockPointer block,
					   int entry_index)
{
	BlendObject bo;
	const BlendBlock* bb = (const BlendBlock *)block;
	/*BlendField *bf = &bb->array_entries[entry_index]; */

	bo.type = bb->type_index;
	bo.name = blend_file->name_undef;
	bo.block = block;
	bo.entry_index = entry_index;
	bo.field_index = 0;

	return bo;
}

#define MAX_CACHED_IMAGES 1024
BlendBlockPointer tCachedTPage[MAX_CACHED_IMAGES];
bImage* tCachedImages[MAX_CACHED_IMAGES];
int gNumCachedImages=0;

#ifndef LEN3POW2
/* length squared */
#define LEN3POW2(xd,yd,zd) ((xd)*(xd) + (yd)*(yd) + (zd)*(zd))
#endif

#ifndef LEN3
/* length (expensive) */
#define LEN3(xd,yd,zd) (sqrt(LEN3POW2((xd),(yd),(zd))))
#endif

#ifndef NORMALIZE3
/* vector normalization (expensive) */
#define NORMALIZE3(xd,yd,zd) \
	do { \
	const double norm3_macro_len3 = LEN3((xd),(yd),(zd)); \
	if (norm3_macro_len3 != 0.0F) \
{ \
	(xd) = (xd) / norm3_macro_len3; \
	(yd) = (yd) / norm3_macro_len3; \
	(zd) = (zd) / norm3_macro_len3; \
} \
	} while (0)
#endif


void
blend_acquire_mesh_from_obj(BlendFile *bf, BlendObject *meobj, bMesh *mesh)
{
	{
		BlendObject obj;
		BlendBlockPointer vblock, fblock, cblock, ttblock,mtblock,dblock, matlink;
		int i;
		int32_t ldata = 123456;
		float fdata1, fdata2, fdata3;

		blend_init_mesh(mesh);

		if (!(blend_object_structure_getfield(bf, &obj, *meobj,
			"totvert") &&
			blend_object_getdata(bf, &ldata, obj))) 
		{
			printf("invalid mesh 0x1\n");
			meobj->type = BOBJ_TYPE_INVALID_MESH;
			return;
		}
		mesh->vert_count = ldata;

		if (!(blend_object_structure_getfield(bf, &obj, *meobj,
			"totface") &&
			blend_object_getdata(bf, &ldata, obj))) 
		{
				printf("invalid mesh 0x2\n");
				meobj->type = BOBJ_TYPE_INVALID_MESH;
				return;
		}
		mesh->face_count = ldata;

		//printf("%d verts, %d faces...\n",	mesh->vert_count, mesh->face_count);

		if (!(blend_object_structure_getfield(bf, &obj, *meobj,
			"mface") &&
			blend_object_getdata(bf, &fblock, obj))) 
		{
				printf("invalid mesh 0x3\n");
				meobj->type = BOBJ_TYPE_INVALID_MESH;
				return;
		}
		/* null fblock is okay */

		if (blend_object_structure_getfield(bf, &obj, *meobj,
			"mat") &&
			blend_object_getdata(bf, &matlink, obj) && matlink) {
				/* found an indirect material link, follow it */
				BlendObject matlinkobj = blend_block_get_object(bf, matlink, 0);
				BlendBlockPointer matblock;
				if (obj.block)
				{
					if (blend_object_structure_getfield(bf, &obj, matlinkobj,
						"next") &&
						blend_object_getdata(bf, &matblock, obj)) {
							if (matblock) {
								BlendObject matobj = blend_block_get_object(bf, matblock, 0);
								mesh->material = blend_alloc_material();
								blend_acquire_material_from_obj(bf, &matobj, mesh->material);
							} else {
								/* um, okay, link went nowhere, leave mesh->material NULL */
							}
					} else {
						//abort();//might fail?
					}
				}
		} else {
			/* no material -- mesh->material will remain NULL */
		}

		if (!(blend_object_structure_getfield(bf, &obj, *meobj,
			"mvert") &&
			blend_object_getdata(bf, &vblock, obj))) {
				abort();
		}
		/* null vblock is okay */

		if (!(blend_object_structure_getfield(bf, &obj, *meobj,
			"tface") &&
			blend_object_getdata(bf, &ttblock, obj))) {
				abort();
		}

		if (!(blend_object_structure_getfield(bf, &obj, *meobj,
			"mtface") &&
			blend_object_getdata(bf, &mtblock, obj))) {
				mtblock=0;
		}

		if (!(blend_object_structure_getfield(bf, &obj, *meobj,
			"mcol") &&
			blend_object_getdata(bf, &cblock, obj))) {
				abort();
		}

		if (!(blend_object_structure_getfield(bf, &obj, *meobj,
			"dvert") &&
			blend_object_getdata(bf, &dblock, obj))) {
				/* sometimes there isn't a dvert block... */
				dblock = NULL;
		}

		mesh->vert = (bVert*) malloc(sizeof(bVert) * mesh->vert_count);

		for (i=0; i<mesh->vert_count; ++i) {
			BlendObject obj = blend_block_get_object(bf, vblock, i);
			BlendObject aobj;
			float fdata1, fdata2, fdata3;
			int32_t sdata1, sdata2, sdata3;
			//char cdata;
			short int cdata;
			cdata=0;

			mesh->vert[i].xyz[0] =
				mesh->vert[i].xyz[1] =
				mesh->vert[i].xyz[2] = -12345;
			mesh->vert[i].cnormal[0] =
				mesh->vert[i].cnormal[1] =
				mesh->vert[i].cnormal[2] = -12345;
			mesh->vert[i].mat = -1;

			if (!(blend_object_structure_getfield(bf, &aobj, obj, "co") &&
				blend_object_array_getdata(bf, &fdata1, aobj, 0,0) &&
				blend_object_array_getdata(bf, &fdata2, aobj, 0,1) &&
				blend_object_array_getdata(bf, &fdata3, aobj, 0,2))) {
					abort();
			}
			mesh->vert[i].xyz[0] = fdata1;
			mesh->vert[i].xyz[1] = fdata2;
			mesh->vert[i].xyz[2] = fdata3;

			if (!(blend_object_structure_getfield(bf, &aobj, obj, "no") &&
				blend_object_array_getdata(bf, &sdata1, aobj, 0,0) &&
				blend_object_array_getdata(bf, &sdata2, aobj, 0,1) &&
				blend_object_array_getdata(bf, &sdata3, aobj, 0,2))) {
					abort();
			}
			mesh->vert[i].cnormal[0] = sdata1;
			mesh->vert[i].cnormal[1] = sdata2;
			mesh->vert[i].cnormal[2] = sdata3;
			/*fprintf(stderr, "%f ", LEN3(mesh->vert[i].normal[0],
			mesh->vert[i].normal[1],
			mesh->vert[i].normal[2]));*/
			if (sdata1 != 0 || sdata2 != 0 || sdata3 != 0) {
				NORMALIZE3(mesh->vert[i].cnormal[0],
					mesh->vert[i].cnormal[1],
					mesh->vert[i].cnormal[2]);
			}

			if (!(blend_object_structure_getfield(bf, &aobj, obj, "mat_nr") &&
				blend_object_getdata(bf, &cdata, aobj))) {
					abort();
			}
			mesh->vert[i].mat = cdata;
		}

		mesh->face = (bFace*)malloc(sizeof(bFace) * mesh->face_count);

		for (i=0; i<mesh->face_count; ++i) {
			int j,k;
			BlendObject obj = blend_block_get_object(bf, fblock, i);
			BlendObject aobj;
			//char cdata;
			int cdata;
			cdata=0;


			mesh->face[i].v[0] = mesh->face[i].v[1] =
				mesh->face[i].v[2] = mesh->face[i].v[3] = -1;
			mesh->face[i].mat = -1;
			mesh->face[i].flags = 0;

			for (j=0; j<4; ++j) {
				for (k=0; k<3; ++k) {
					mesh->face[i].rgba[j][k] = 1.0;
				}
				mesh->face[i].rgba[j][3] = 1.0f;
				mesh->face[i].uv[j][0] = 0.0f;
				mesh->face[i].uv[j][1] = 0.0f;
				mesh->face[i].m_image = NULL;
			}

			if (blend_object_structure_getfield(bf, &aobj, obj, "v1")) {
				if (0!=strcmp(bf->types[aobj.type].name,"int") &&
					0!=strcmp(bf->types[aobj.type].name,"ushort")) {
						printf("Expected vertex-index type to be 'ushort' or 'int', got '%s'\n", bf->types[aobj.type].name);
						abort();
				}

				if (0==strcmp(bf->types[aobj.type].name,"int")) {
					/* index type is a 32bit int, generated by newish Blenders */
					int32_t idata;
					if (!(blend_object_structure_getfield(bf, &aobj, obj, "v1") &&
						blend_object_getdata(bf, &idata, aobj))) {
							abort();
					}
					mesh->face[i].v[0] = idata;

					if (!(blend_object_structure_getfield(bf, &aobj, obj, "v2") &&
						blend_object_getdata(bf, &idata, aobj))) {
							abort();
					}
					mesh->face[i].v[1] = idata;

					if (!(blend_object_structure_getfield(bf, &aobj, obj, "v3") &&
						blend_object_getdata(bf, &idata, aobj))) {
							abort();
					}
					mesh->face[i].v[2] = idata;

					if (!(blend_object_structure_getfield(bf, &aobj, obj, "v4") &&
						blend_object_getdata(bf, &idata, aobj))) {
							abort();
					}
					mesh->face[i].v[3] = idata;
				} else {
					/* index type is a 16bit ushort, generated by old Blenders */
					uint16_t usdata;
					if (!(blend_object_structure_getfield(bf, &aobj, obj, "v1") &&
						blend_object_getdata(bf, &usdata, aobj))) {
							abort();
					}
					mesh->face[i].v[0] = usdata;

					if (!(blend_object_structure_getfield(bf, &aobj, obj, "v2") &&
						blend_object_getdata(bf, &usdata, aobj))) {
							abort();
					}
					mesh->face[i].v[1] = usdata;

					if (!(blend_object_structure_getfield(bf, &aobj, obj, "v3") &&
						blend_object_getdata(bf, &usdata, aobj))) {
							abort();
					}
					mesh->face[i].v[2] = usdata;

					if (!(blend_object_structure_getfield(bf, &aobj, obj, "v4") &&
						blend_object_getdata(bf, &usdata, aobj))) {
							abort();
					}
					mesh->face[i].v[3] = usdata;            
				}
			} else {
				abort();
			}

			if (!(blend_object_structure_getfield(bf, &aobj, obj, "mat_nr") &&
				blend_object_getdata(bf, &cdata, aobj))) {
					abort();
			}
			mesh->face[i].mat = cdata;

			if (!(blend_object_structure_getfield(bf, &aobj, obj, "flag") &&
				blend_object_getdata(bf, &cdata, aobj))) {
					abort();
			}
			mesh->face[i].flags = cdata;
		}

		if (cblock) {
			/* we have vertex colours */
			for (i=0; i<mesh->face_count; ++i) {
				int j;
				
				unsigned int cdata;
				//unsigned char cdata;
				BlendObject aobj;
				cdata=0;

				for (j=0; j<4; ++j) {
					BlendObject obj = blend_block_get_object(bf, cblock, i*4+j);
					if (!(blend_object_structure_getfield(bf, &aobj, obj, "b") &&
						blend_object_getdata(bf, &cdata, aobj))) {
							abort();
					}
					mesh->face[i].rgba[j][0] = cdata / 255.0f;

					if (!(blend_object_structure_getfield(bf, &aobj, obj, "g") &&
						blend_object_getdata(bf, &cdata, aobj))) {
							abort();
					}
					mesh->face[i].rgba[j][1] = cdata / 255.0f;

					if (!(blend_object_structure_getfield(bf, &aobj, obj, "r") &&
						blend_object_getdata(bf, &cdata, aobj))) {
							abort();
					}
					mesh->face[i].rgba[j][2] = cdata / 255.0f;

					/* alpha seems to be nonsense :( */
					/*
					if (!(blend_object_structure_getfield(bf, &aobj, obj, "a") &&
					blend_object_getdata(bf, &cdata, aobj))) {
					abort();
					}
					mesh->face[i].rgba[j][3] = cdata / 255.0f;
					*/
					mesh->face[i].rgba[j][3] = 1.0f;
				}
			}
		} else {
			/* !cblock (no vertex colours) */
			for (i=0; i<mesh->face_count; ++i) {
				int j;
				for (j=0; j<4; ++j) {
					mesh->face[i].rgba[j][0] = 1.0f;
					mesh->face[i].rgba[j][1] = 1.0f;
					mesh->face[i].rgba[j][2] = 1.0f;
					mesh->face[i].rgba[j][3] = 1.0f;
				}
			}
		}

		if (mtblock) 
		{
			/* we have tex co-ords */
			for (i=0; i<mesh->face_count; ++i) 
			{
				int j,k;
				void *pdata;
				BlendObject aobj;
				BlendObject obj = blend_block_get_object(bf, mtblock, i);
				BlendObject obj2,obj3; 
				unsigned char flag;
				unsigned short int mode;

				BlendBlockPointer tpageptr;

				if (blend_object_structure_getfield(bf, &obj3, obj, "flag"))
				{
					blend_object_getdata(bf, &flag, obj3);
					mesh->face[i].m_flag = flag;
				} else
				{
					mesh->face[i].m_flag = 0;
				}

				if (blend_object_structure_getfield(bf, &obj3, obj, "mode"))
				{
					blend_object_getdata(bf, &mode, obj3);
					mesh->face[i].m_mode = mode;
				} else
				{
					mesh->face[i].m_mode = 0;
				}

				if ((blend_object_structure_getfield(bf, &obj3, obj, "tpage")
					&& blend_object_getdata(bf, &tpageptr, obj3))) 
				{
					
					mesh->face[i].m_image = 0;

					for (j=0; j<4; ++j) 
					{
						uint32_t uldata;
						for (k=0; k<2; ++k) {
							float fdata;	      
							if (!(blend_object_structure_getfield(bf, &aobj, obj, "uv") &&
								blend_object_array_getdata(bf, &fdata, aobj, j,k))) {
									abort();
							}
							mesh->face[i].uv[j][k] = fdata;
						}
						mesh->face[i].uv[j][1] = 1.0f - mesh->face[i].uv[j][1];      
						/* texture face colour... not sure how this conceptually
						differs from the face vertex colours, but it does. */

						/* in its usual inconsistant style, blender packs this
						RGBA value into the bytes of an unsigned long... */
						
					}

					//printf("got tpage\n");
										
					{
						int k;
						for (k=0;k<gNumCachedImages;k++)
						{
							if (tCachedTPage[k]==tpageptr)
							{
								mesh->face[i].m_image=tCachedImages[k];
								break;
							}
						}
					}

					if (!mesh->face[i].m_image)
					{
						bImage* bimg= (bImage*)malloc (sizeof(bImage));
						tCachedImages[gNumCachedImages]=bimg;
						tCachedTPage[gNumCachedImages] = tpageptr;
						gNumCachedImages++;
						mesh->face[i].m_image = bimg;
						bimg->m_packedImagePtr = 0;
						bimg->m_sizePackedImage = 0;
					
						if (tpageptr)
						{
							BlendObject name_block;
							BlendObject tpage = blend_block_get_object(bf,tpageptr,0);

							BlendObject okblock;


							if ((blend_object_structure_getfield(bf, &okblock, tpage, "ok")))
							{
								short int okval;
								if (blend_object_getdata(bf,&okval,okblock))
								{
//									printf("ok=%d\n",okval);
									bimg->m_ok = okval;
								} else
								{
									bimg->m_ok=0;
								}

							}

							if ((blend_object_structure_getfield(bf, &okblock, tpage, "xrep")))
							{
								short int xrep;
								if (blend_object_getdata(bf,&xrep,okblock))
								{
//									printf("xrep=%d\n",xrep);
									bimg->m_xrep = xrep;
								} else
								{
									bimg->m_xrep = 0;
								}
							}

							if ((blend_object_structure_getfield(bf, &okblock, tpage, "yrep")))
							{
								short int yrep;
								if (blend_object_getdata(bf,&yrep,okblock))
								{
//									printf("yrep=%d\n",yrep);
									bimg->m_yrep = yrep;
								} else
								{
									bimg->m_yrep = 0;
								}
							}

							mesh->face[i].image_id = 0;
							{
								BlendBlockPointer packptr;

								if ((blend_object_structure_getfield(bf, &obj3, tpage, "packedfile")
									&& blend_object_getdata(bf, &packptr, obj3)))
								{
									if (packptr)
									{
										BlendObject packfile= blend_block_get_object(bf,packptr,0);
										BlendBlockPointer dataptr;
										if ((blend_object_structure_getfield(bf, &obj3, packfile, "data")
											&& blend_object_getdata(bf, &dataptr, obj3)))
										{
											/*BlendObject data= blend_block_get_object(bf,dataptr,0);
											char dest[1024];
											blend_object_getstring(bf,data,dest,1023);
											*/

//											printf("...\n");

											//if (blend_object_structure_getfield(bf, &obj2, ipo,"curve"))
											{
												BlendBlock* block = (BlendBlock*)obj3.block;


												void** ptrptr = (void**) &block->m_array_entries_[0]->field_bytes[block->m_array_entries_[0]->m_field_offsets[obj3.field_index]];
												BlendBlockPointer ptr = *ptrptr;
												if (ptr)
												{
													///todo: fix ptr issue
													BlendBlockPointer curveblockptr = blend_block_from_blendpointer(bf, (uint32_t)ptr);
													BlendObject curve = blend_block_get_object(bf, curveblockptr, 0);
													BlendBlock* bb = (BlendBlock* )curve.block;
													BlendBlock imgblock;
													mesh->face[i].image_id = (BlendBlockPointer)bb->blender_pointer;
													//imgblock = blend_block_get_object(bf, bb->blender_pointer, 0);
													bimg->m_packedImagePtr = bb->customData;
													bimg->m_sizePackedImage = bb->customDataSize;
												}

											}
										}


									}

								}
							}




						if ((blend_object_structure_getfield(bf, &name_block, tpage, "name")))
						{
							int max_chars=127;
							
							if (blend_object_getstring(bf,name_block,bimg->m_imagePathName, max_chars))
							{
//								printf("texname=%s\n",bimg->m_imagePathName);
							}

						}


					
					}

				}


				}




			}
		}
		else
		{
			if (ttblock) {
				/* we have tex co-ords */
				for (i=0; i<mesh->face_count; ++i) {
					int j,k;
					void *pdata;
					BlendObject aobj;
					BlendObject obj = blend_block_get_object(bf, ttblock, i);

					if (!(blend_object_structure_getfield(bf, &aobj, obj, "tpage") &&
						blend_object_getdata(bf, &pdata, aobj))) {
							abort();
					}
					mesh->face[i].image_id = pdata;

					for (j=0; j<4; ++j) {
						uint32_t uldata;
						for (k=0; k<2; ++k) {
							float fdata;	      
							if (!(blend_object_structure_getfield(bf, &aobj, obj, "uv") &&
								blend_object_array_getdata(bf, &fdata, aobj, j,k))) {
									abort();
							}
							mesh->face[i].uv[j][k] = fdata;
						}
						mesh->face[i].uv[j][1] = 1.0f - mesh->face[i].uv[j][1];      
						/* texture face colour... not sure how this conceptually
						differs from the face vertex colours, but it does. */
						if (!(blend_object_structure_getfield(bf, &aobj, obj, "col") &&
							blend_object_array_getdata(bf, &uldata, aobj, 0,j))) {
								abort();
						}
						/* in its usual inconsistant style, blender packs this
						RGBA value into the bytes of an unsigned long... */
						mesh->face[i].rgba2[j][0] = ((uldata >> 24) & 0xFF) / 255.0f;
						mesh->face[i].rgba2[j][1] = ((uldata >> 16) & 0xFF) / 255.0f;
						mesh->face[i].rgba2[j][2] = ((uldata >> 8) & 0xFF) / 255.0f;
						mesh->face[i].rgba2[j][3] = ((uldata >> 0) & 0xFF) / 255.0f;
					}
					/*mesh->face[i].uv[0][0]=0; mesh->face[i].uv[0][1]=0;
					mesh->face[i].uv[1][0]=1; mesh->face[i].uv[1][1]=0;
					mesh->face[i].uv[2][0]=1; mesh->face[i].uv[2][1]=1;
					mesh->face[i].uv[3][0]=0; mesh->face[i].uv[3][1]=1;*/

				}
			} else {
				/* !tblock (no texture co-ords, no face tex-colours) */
				for (i=0; i<mesh->face_count; ++i) {
					int j;
					for (j=0; j<4; ++j) {
						mesh->face[i].rgba2[j][0] = 1.0f;
						mesh->face[i].rgba2[j][1] = 1.0f;
						mesh->face[i].rgba2[j][2] = 1.0f;
						mesh->face[i].rgba2[j][3] = 1.0f;
					}
				}
			}
		}

		if (mtblock)
		{
			BlendObject obj = blend_block_get_object(bf, mtblock, 0);

			//6155
		}


		if (dblock) {
			/* we have vertex deformation weights */
			for (i=0; i<mesh->vert_count; ++i) {
				int j;
				int32_t ldata;
				float fdata;
				BlendBlockPointer pdata;
				BlendObject aobj;
				BlendObject obj = blend_block_get_object(bf, dblock, i);

				if (!(blend_object_structure_getfield(bf, &aobj, obj, "totweight") &&
					blend_object_getdata(bf, &ldata, aobj))) {
						abort();
				}
				mesh->vert[i].deform_weights_count = ldata;
				mesh->vert[i].deform_weights = (bDeformWeight*) malloc(ldata*sizeof(bDeformWeight));

				if (!(blend_object_structure_getfield(bf, &aobj, obj, "dw") &&
					blend_object_getdata(bf, &pdata, aobj))) {
						abort();
				}

				for (j=0; j<mesh->vert[i].deform_weights_count; ++j) {
					BlendObject dwobj = blend_block_get_object(bf, pdata, j);

					if (!(blend_object_structure_getfield(bf, &aobj, dwobj, "def_nr")
						&& blend_object_getdata(bf, &ldata, aobj))) {
							abort();
					}
					mesh->vert[i].deform_weights[j].bone_id = ldata;

					if (!(blend_object_structure_getfield(bf, &aobj, dwobj, "weight")
						&& blend_object_getdata(bf, &fdata, aobj))) {
							abort();
					}
					mesh->vert[i].deform_weights[j].weight = fdata;
				}	  
			}
		} else {
			/* !dblock (no vertex deformation weights) */
			for (i=0; i<mesh->vert_count; ++i) {
				mesh->vert[i].deform_weights = NULL;
				mesh->vert[i].deform_weights_count = 0;
			}
		}
	}
}

int
blend_block_get_entry_count(BlendFile* blend_file,
							BlendBlockPointer block)
{
	const BlendBlock* bb = (const BlendBlock*)block;
	return bb->m_array_entries_.size();//>array_entries_count;;
}

void
blend_init_mesh(bMesh *mesh)
{
	int i,j;
	mesh->vert = NULL;
	mesh->vert_count = 0;
	mesh->face = NULL;
	mesh->face_count = 0;
	mesh->material = NULL;
}



void extract_curve_bezier_triples(BlendFile *bf, BlendObject* curve)
{
	BlendObject newObj;
	BlendBlockPointer bezTriplePtr;
	int i;
	int totvert=0;
	

	


	if (!(blend_object_structure_getfield(bf, &newObj, *curve,"totvert") &&	blend_object_getdata(bf, &totvert, newObj))) 
	{
		return;
	}

	printf("Curve with %d bezier triples\n",totvert);

	if (!(blend_object_structure_getfield(bf, &newObj, *curve,"bezt") &&	blend_object_getdata(bf, &bezTriplePtr, newObj))) 
	{
		printf("invalid bezTriplePtr 0x1\n");
	}

	for ( i=0;i<totvert;i++)
	{
		
		
		
		if (bezTriplePtr)
		{
			BlendObject bezTriple;
			BlendBlockPointer vecPtr;
			bezTriple = blend_block_get_object(bf, bezTriplePtr, i);
			{
				BlendObject vecOb;
			
				if (blend_object_structure_getfield(bf, &vecOb, bezTriple, "vec"))
				{
					float v0;
					int i,j;

					for (i=0;i<3;i++)
					{
						for (j=0;j<3;j++)
						{
							if (blend_object_array_getdata(bf, &v0, vecOb, i,j))
							{
								//printf("IpoCurve.bezt.vec[%d][%d]=%f\n",i,j,v0);
							}
						}
					}
				} 

			}
		}
	}
}




void
blend_acquire_obj_from_obj(BlendFile *bf, BlendObject *objobj,
						   bObj *outobj, BlendObject **mallocdoutblendobject) 
{

	BlendObject obj, dataobj;
	BlendBlockPointer block,ipoblock,curveblock=0;
	short sdata = 12345;

#define B_IDNAME_MAX_SIZE 80
	char *idname = (char*)malloc(1 + B_IDNAME_MAX_SIZE);
	float fdata1, fdata2, fdata3;

	if (mallocdoutblendobject) *mallocdoutblendobject = NULL;

	blend_init_obj(outobj);

	if (!(blend_object_structure_getfield(bf, &obj, *objobj,
		"type") &&
		blend_object_getdata(bf, &sdata, obj))) 
	{
			abort(); /* couldn't get type */
	} else {
		switch (sdata) {
	case 1: /* mesh */
		outobj->type = BOBJ_TYPE_MESH; break;
	case 10: /* lamp */
		outobj->type = BOBJ_TYPE_LAMP; break;
	case 11: /* camera */
		outobj->type = BOBJ_TYPE_CAMERA; break;
	default:
		outobj->type = BOBJ_TYPE_UNKNOWN; break;
		}
	}

	if (blend_object_get_IDname(bf, *objobj, idname, B_IDNAME_MAX_SIZE)) {
		outobj->name = idname;
	} else {
		free(idname);
		abort(); /* couldn't get obj name */
	}

	/* now we override the mesh transform with the object's.  should
	we merge, instead???  - hm, dunno, don't think so. */


	if (blend_object_structure_getfield(bf, &obj, *objobj, "loc") &&
		blend_object_array_getdata(bf, &fdata1, obj, 0,0) &&
		blend_object_array_getdata(bf, &fdata2, obj, 0,1) &&
		blend_object_array_getdata(bf, &fdata3, obj, 0,2)) {
			outobj->location[0] = fdata1;
			outobj->location[1] = fdata2;
			outobj->location[2] = fdata3;
	} else {
		outobj->location[0] = 
			outobj->location[1] = 
			outobj->location[2] = 0.0f;
	}

	if (blend_object_structure_getfield(bf, &obj, *objobj, "size") &&
		blend_object_array_getdata(bf, &fdata1, obj, 0,0) &&
		blend_object_array_getdata(bf, &fdata2, obj, 0,1) &&
		blend_object_array_getdata(bf, &fdata3, obj, 0,2)) {
			outobj->scaling[0] = fdata1;
			outobj->scaling[1] = fdata2;
			outobj->scaling[2] = fdata3;
	} else {
		outobj->scaling[0] = 
			outobj->scaling[1] = 
			outobj->scaling[2] = 1.0f;
	}

	if (blend_object_structure_getfield(bf, &obj, *objobj, "rot") &&
		blend_object_array_getdata(bf, &fdata1, obj, 0,0) &&
		blend_object_array_getdata(bf, &fdata2, obj, 0,1) &&
		blend_object_array_getdata(bf, &fdata3, obj, 0,2)) {
			outobj->rotphr[0] = fdata1;
			outobj->rotphr[1] = fdata2;
			outobj->rotphr[2] = fdata3;
	} else {
		outobj->rotphr[0] = 
			outobj->rotphr[1] = 
			outobj->rotphr[2] = 0.0f;
	}

	if (blend_object_structure_getfield(bf, &obj, *objobj, "parentinv")) {
		int i,j;
		for (i=0; i<4; ++i) {
			for (j=0; j<4; ++j) {
				blend_object_array_getdata(bf, &fdata1, obj, i,j);
				outobj->parentimat[i][j] = fdata1;
			}
		}
	}
#if 0
	if (blend_object_structure_getfield(bf, &obj, *objobj, "transflag")) {
		char cdata;
		/* TODO: decode what these flags precisely mean. */
		/* top bit is 'powertrack' */
		if (blend_object_getdata(bf, &cdata, obj)) {
			outobj->transflags = (unsigned char)cdata;
		}
	}
#endif

	outobj->mass = 0.f;
	if (blend_object_structure_getfield(bf, &obj, *objobj, "mass")) 
	{
		float mass;
		if (blend_object_getdata(bf, &mass, obj))
		{
			outobj->mass = mass;
		}
	}




	if ((blend_object_structure_getfield(bf, &obj, *objobj,"ipo") && blend_object_getdata(bf, &ipoblock, obj)))
	{
		if (ipoblock)
		{
			BlendObject ipo = blend_block_get_object(bf, ipoblock, 0);
#define MAX_CHARS 31
			char iponame[MAX_CHARS];
			BlendObject obj2,obj3;

			blend_object_get_IDname(bf,ipo,iponame,MAX_CHARS-1);
//			printf("ipo.ID.name = %s\n",iponame);

			if (blend_object_structure_getfield(bf, &obj2, ipo,"curve"))
			{

				BlendBlock* block = (BlendBlock*)obj2.block;


						void** ptrptr = (void**)&block->m_array_entries_[0]->field_bytes[block->m_array_entries_[0]->m_field_offsets[obj2.field_index]];
				BlendBlockPointer ptrlast;
				BlendBlockPointer ptr = *ptrptr;
				ptrptr++;
				 ptrlast= *ptrptr;


				//ptrptr++; contains the 'last' pointer
				if (ptr)
				{
					BlendBlockPointer curveblockptr = blend_block_from_blendpointer(bf, (uint32_t)ptr);
					BlendObject curve;
					BlendBlock* block3;
					BlendBlock* block2;
					BlendObject newObj,nextOb;
					BlendBlockPointer nextPtr;
					
					printf("Found IpoCurve(s) (ListBase)\n");

					curve = blend_block_get_object(bf, curveblockptr, 0);
					block3 = (BlendBlock*)curve.block;
					
				
					block2 = (BlendBlock*)obj.block;
								
					

					do {
						extract_curve_bezier_triples(bf, &curve);

						//blend_acquire_obj_from_obj(bf,&obj,&tmpObj,0);
						if (!(blend_object_structure_getfield(bf, &nextOb, curve,"next") &&	blend_object_getdata(bf, &nextPtr, nextOb))) 
						{
							printf("error: no next?\n");
						} else
						{
							///this 'next' object would let you iterate over the linked list until next == 0
							if (nextPtr)
							{
								BlendBlock* block3;
								nextOb = blend_block_get_object(bf, nextPtr, 0);
								block3 = (BlendBlock*)nextOb.block;
								curve = nextOb;
							}
						}
						
						
					} while (nextPtr);

					

					


				}

			}


		}
	}




	outobj->boundtype = 0;
	if (blend_object_structure_getfield(bf, &obj, *objobj, "boundtype")) 
	{
		short int boundtype;
		if (blend_object_getdata(bf, &boundtype, obj))
		{
			outobj->boundtype= boundtype;
		}
	}

	outobj->gameflag = 0;
	if (blend_object_structure_getfield(bf, &obj, *objobj, "gameflag")) 
	{
		int gameflag;
		if (blend_object_getdata(bf, &gameflag, obj))
		{
			outobj->gameflag= gameflag;
		}
	}




	if (blend_object_structure_getfield(bf, &obj, *objobj, "obmat")) {
		int i,j;
		for (i=0; i<4; ++i) {
			for (j=0; j<4; ++j) {
				blend_object_array_getdata(bf, &fdata1, obj, i,j);
				outobj->transform[i][j] = fdata1;
//				printf("%0.3f ", fdata1);
			}
//			printf("\n");
		}
	}

	/* get actual mesh obj here */

	if (! (blend_object_structure_getfield(bf, &obj, *objobj, "data") &&
		blend_object_getdata(bf, &block, obj))) 
	{
		printf("no mesh\n");
		outobj->data.mesh = 0;
		return;
			//abort();
	}

	if (block == NULL) {
		outobj->type = BOBJ_TYPE_NULL;
	} else {
		dataobj = blend_block_get_object(bf, block, 0);
		if (mallocdoutblendobject) {
			*mallocdoutblendobject = (BlendObject*)malloc(sizeof(BlendObject));
			**mallocdoutblendobject = dataobj;
		}
	}


	switch (outobj->type) {
  case BOBJ_TYPE_MESH:
	  outobj->data.mesh = blend_alloc_mesh();
	  blend_acquire_mesh_from_obj(bf, &dataobj, outobj->data.mesh);
	  break;
  case BOBJ_TYPE_UNKNOWN:
  default:
  case BOBJ_TYPE_NULL:
	  outobj->data.dummy = NULL;
	  break;
	}

}




BlendBlockPointer
blend_block_from_blendpointer(BlendFile *blend_file,
							  uint32_t blendpointer)
{
	int i;

	/* fprintf(stderr, "%04x: ", blendpointer);*/

	if (blendpointer != 0) {
		for (i=0; i<blend_file->m_blocks.size(); ++i) {
			/*fprintf(stderr, "%04x? ", blend_file->blocks[i].blender_pointer); */
			if (blend_file->m_blocks[i]->blender_pointer == blendpointer) {
				return blend_file->m_blocks[i];
			}
		}
	}

	return NULL;
}


void
blend_acquire_material_from_obj(BlendFile *bf, BlendObject *matobj,
								bMaterial *mat) 
{
	BlendObject obj;
	int i;
	int32_t ldata = 123456;
	float fdata = 123456.0;

	blend_init_material(mat);

	if ((blend_object_structure_getfield(bf, &obj, *matobj, "r") &&
		blend_object_getdata(bf, &fdata, obj))) {
			mat->colour_rgba[0] = fdata;
	}
	if ((blend_object_structure_getfield(bf, &obj, *matobj, "g") &&
		blend_object_getdata(bf, &fdata, obj))) {
			mat->colour_rgba[1] = fdata;
	}
	if ((blend_object_structure_getfield(bf, &obj, *matobj, "b") &&
		blend_object_getdata(bf, &fdata, obj))) {
			mat->colour_rgba[2] = fdata;
	}
	if ((blend_object_structure_getfield(bf, &obj, *matobj, "alpha") &&
		blend_object_getdata(bf, &fdata, obj))) {
			mat->colour_rgba[3] = fdata;
	}
	if ((blend_object_structure_getfield(bf, &obj, *matobj, "emit") &&
		blend_object_getdata(bf, &fdata, obj))) {
			mat->emit = fdata;
	}

	if (!(blend_object_structure_getfield(bf, &obj, *matobj, "mode") &&
		blend_object_getdata(bf, &ldata, obj))) {
			abort();
	}
	if (ldata & 0x04) {
		mat->feature_mask |= BMAT_FEATURE_SHADELESS;
	}
	if (ldata & 0x08) {
		mat->feature_mask |= BMAT_FEATURE_WIRE;
	}
	if (ldata & 0x10) {
		mat->feature_mask |= BMAT_FEATURE_VCOLLIGHT;
	}
	if (ldata & 0x80) {
		mat->feature_mask |= BMAT_FEATURE_VCOLPAINT;
	}
	if (ldata & (1024 | 512 | 256)) { /* not sure about this, it's strange. */
		mat->feature_mask |= BMAT_FEATURE_TEXFACE;
	}

	for (i=0; i<BLENDER_MAX_TEX_LAYERS; ++i) {
		BlendBlockPointer texl_block;
		if (blend_object_structure_getfield(bf, &obj, *matobj, "mtex") &&
			blend_object_array_getdata(bf, &texl_block, obj, 0,i) && texl_block) {
				BlendObject tlobj = blend_block_get_object(bf, texl_block, 0);
				mat->tex_layer[i] = blend_alloc_texlayer();
				/*fprintf(stderr, "GETTING LAYER AT POS %d\n", i);*/
				blend_acquire_texlayer_from_obj(bf, &tlobj, mat->tex_layer[i]);
		} else {
			/*fprintf(stderr, "NOTHING FOUND AT POS %d\n", i);*/
		}
	}
}


bMaterial *
blend_alloc_material(void) {
	return (bMaterial *) malloc(sizeof(bMaterial));
}

int
blend_object_getdata(BlendFile* blend_file,
					 void* dest, BlendObject obj)
{
	int dim1, dim2;

	if (name_is_array(blend_file->names[obj.name], &dim1, &dim2)) {
		if (dim1 != 1 || dim2 != 1) {
			printf("Tried to fetch a whole array.\n");
			return 0;
		}
	}

	return (blend_object_array_getdata(blend_file, dest, obj, 0, 0));
}

bMesh* blend_alloc_mesh(void) {
	return (bMesh*)malloc(sizeof(bMesh));
}

static void bMatIdentity(bMatrix mat) {
	int i,j;
	for (i=0; i<4; ++i) {
		for (j=0; j<4; ++j) {
			mat[i][j] = 0.0f;
		}
		mat[i][i] = 1.0f;
	}
}



void
blend_init_obj(bObj *obj) {
	obj->type = BOBJ_TYPE_UNKNOWN;
	obj->name = NULL;
	bMatIdentity(obj->transform);
	bMatIdentity(obj->parentimat);
	obj->location[0] =
		obj->location[1] =
		obj->location[2] = 0.0f;
	obj->scaling[0] =
		obj->scaling[1] =
		obj->scaling[2] = 1.0f;
	obj->rotphr[0] =
		obj->rotphr[1] =
		obj->rotphr[2] = 0.0f;
	obj->data.dummy = NULL;
	obj->transflags = 0;
}


void /* MTex */
blend_acquire_texlayer_from_obj(BlendFile *bf, BlendObject *tlobj,
								bTexLayer *tl) 
{
	BlendObject obj;
	BlendBlockPointer tex_block;
	short sdata = 12345;

	blend_init_texlayer(tl);

	if (!(blend_object_structure_getfield(bf, &obj, *tlobj,
		"mapto") &&
		blend_object_getdata(bf, &sdata, obj))) {
			abort();
	}
	if (sdata & 0x01) {
		tl->affects_mask |= BTEX_AFFECT_COLOUR;
	}
	if (sdata & 0x40) {
		tl->affects_mask |= BTEX_AFFECT_EMIT;
	}
	if (sdata & 0x80) {
		tl->affects_mask |= BTEX_AFFECT_ALPHA;
	}
	/* note: mapto not fully decoded. */

	if (!(blend_object_structure_getfield(bf, &obj, *tlobj,
		"texflag") &&
		blend_object_getdata(bf, &sdata, obj))) {
			abort();
	}
	if (sdata & 0x02) {
		tl->affects_mask |= BTEX_AFFECT_STENCIL;
	}

	if (!(blend_object_structure_getfield(bf, &obj, *tlobj,
		"texco") &&
		blend_object_getdata(bf, &sdata, obj))) {
			abort();
	}
	switch (sdata) {
  case 1:
  case 2:
	  tl->coords_type = BTEX_COORDS_REFLECT;
	  break;
  case 16:
	  tl->coords_type = BTEX_COORDS_UV;
	  break;
  default:
	  /* I haven't seen this happen, but it probably does... */
	  tl->coords_type = BTEX_COORDS_NONE;
	  break;
	}

	if (!(blend_object_structure_getfield(bf, &obj, *tlobj,
		"blendtype") &&
		blend_object_getdata(bf, &sdata, obj))) {
			abort();
	}
	tl->blend_mode = sdata; /* not decoded yet :( */

	if (blend_object_structure_getfield(bf, &obj, *tlobj, "tex") &&
		blend_object_getdata(bf, &tex_block, obj) && tex_block) {
			BlendObject tobj = blend_block_get_object(bf, tex_block, 0);
			BlendBlockPointer im_block;
			BlendObject obj;

			if (!(blend_object_structure_getfield(bf, &obj, tobj, "extend") &&
				blend_object_getdata(bf, &sdata, obj))) {
					abort();
			}
			tl->is_st_clamped = !(sdata & 2 /*'repeat'*/);
			/*fprintf(stderr, "CLAMP=%d (was %d)\n", tl->is_st_clamped, sdata);*/
			if (!(blend_object_structure_getfield(bf, &obj, tobj, "xrepeat") &&
				blend_object_getdata(bf, &sdata, obj))) {
					tl->xrepeat = 1;
			} else {
				tl->xrepeat = sdata;
			}
			if (!(blend_object_structure_getfield(bf, &obj, tobj, "yrepeat") &&
				blend_object_getdata(bf, &sdata, obj))) {
					tl->yrepeat = 1;
			} else {
				tl->yrepeat = sdata;
			}
			if (!(blend_object_structure_getfield(bf, &obj, tobj, "flag") &&
				blend_object_getdata(bf, &sdata, obj))) {
					abort();
			} else {
				tl->flags = sdata;
			}
			if (!(blend_object_structure_getfield(bf, &obj, tobj, "imaflag") &&
				blend_object_getdata(bf, &sdata, obj))) {
					abort();
			} else {
				if (sdata & 0x0001) {
					tl->flags |= BIMG_FLAG_INTERPOLATE;
				}
				if (sdata & 0x0004) {
					tl->flags |= BIMG_FLAG_MIPMAP;
				}
				if (sdata & 0x0100) {
					tl->flags |= BIMG_FLAG_ANTIALIAS;
				}
			}
			if (!(blend_object_structure_getfield(bf, &obj, tobj, "type") &&
				blend_object_getdata(bf, &sdata, obj))) {
					abort();
			} else {
				tl->Ntype = sdata;
			}
			if (blend_object_structure_getfield(bf, &obj, tobj, "ima") &&
				blend_object_getdata(bf, &im_block, obj) && im_block) {
					BlendObject imobj = blend_block_get_object(bf, im_block, 0);
#define BF_IMAGE_FILENAME_MAXSIZE 160
					tl->filename = (char*)malloc(BF_IMAGE_FILENAME_MAXSIZE);
					if (!(blend_object_structure_getfield(bf, &obj, imobj, "name") &&
						blend_object_getstring(bf, obj,
						tl->filename, BF_IMAGE_FILENAME_MAXSIZE))) {
							abort();
					}
			}
	} else {
		abort();
	}
}



bTexLayer *
blend_alloc_texlayer(void) {
	return (bTexLayer *)malloc(sizeof(bTexLayer));
}

void
blend_init_material(bMaterial *mat) {
	int i;
	for (i=0; i<BLENDER_MAX_TEX_LAYERS; ++i) {
		mat->tex_layer[i] = NULL;
	}
	mat->feature_mask = 0;
	for (i=0; i<4; ++i) {
		mat->colour_rgba[i] = 1.0f;
	}
	mat->emit = 0.0f;
}

void
blend_init_texlayer(bTexLayer *tl) {
	tl->filename = NULL;
	tl->affects_mask = 0;
	tl->blend_mode = BTEX_BLEND_NORMAL;
	tl->coords_type = BTEX_COORDS_NONE;
	tl->is_st_clamped = 0;
	tl->flags = 0;
	tl->Nflags = tl->Ntype = 0;
	tl->xrepeat = tl->yrepeat = 1;
}