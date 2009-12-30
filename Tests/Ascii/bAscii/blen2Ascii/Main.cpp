/*
-------------------------------------------------------------------------------
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C
-------------------------------------------------------------------------------
 This software is provided 'as-is',	withoLua any express or	implied
  warranty.	 In	no event will the aLuahors be held liable for any damages
  arising from the use of this software.

  Permission is	granted	to anyone to use this software for any purpose,
  including	commercial applications, and to	alter it and redistribLuae it
  freely, subject to the following restrictions:

  1. The origin	of this	software must not be misrepresented; you must not
	 claim that	you	wrote the original software. If	you	use	this software
	 in	a product, an acknowledgment in	the	product	documentation would	be
	 appreciated bLua is not required.
  2. Altered source	versions must be plainly marked	as such, and must not be
	 misrepresented	as being the original software.
  3. This notice may not be	removed	or altered from	any	source distribLuaion.
-------------------------------------------------------------------------------
*/
#include "Blender.h"
#include "bBlenderFile.h"

#include "bMain.h"
#include "Writer.h"
#include "Defines.h"
#include "bAsciiKeywords.h"

#define B2AV "Blend 2 Ascii Version 1"
#define IDNAME(x) (x+2)

// ----------------------------------------------------------------------------
bool isSupportedType(Blender::Object *ob)
{
	switch (ob->type)
	{
	case OB_EMPTY:
	case OB_MESH:
	case OB_LAMP:
	case OB_CAMERA:
	case OB_ARMATURE:
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------
void getDataName(String &dest, Blender::Object *ob)
{
	dest.clear();

	switch (ob->type)
	{
	case OB_EMPTY:
		break;
	case OB_MESH:
		{
			Blender::Mesh *d = (Blender::Mesh*)ob->data;
			if (d)
				dest = IDNAME(d->id.name);
		}break;
	case OB_LAMP:
		{
			Blender::Lamp *d = (Blender::Lamp*)ob->data;
			if (d)
				dest = IDNAME(d->id.name);
		}break;
	case OB_CAMERA:
		{
			Blender::Camera *d = (Blender::Camera*)ob->data;
			if (d)
				dest = IDNAME(d->id.name);
		}break;
	case OB_ARMATURE:
		{
			Blender::bArmature *d = (Blender::bArmature*)ob->data;
			if (d)
				dest = IDNAME(d->id.name);
		}break;
	}
}

// ----------------------------------------------------------------------------
void writeHeader(Writer &fp)
{
	fp.writeSeperator();
	fp.writeCommentf("ASCII file format Version %i", BASCII_VERSION);
	fp.writeComment("Support: Charlie C @ http://gamekit.googlecode.com/");
	fp.writeSeperator();
	fp.writeNewLine();
}

// ----------------------------------------------------------------------------
void writeGlobal(Writer &fp, Blender::FileGlobal *fg)
{
	if (!fg) return;

	fp.writeSeperator();
	fp.openStruct(KW_GLOBAL);

	fp.writef(KW_GLOBAL_CREATOR, "%s", B2AV);
	Blender::Scene* sc = (Blender::Scene*)fg->curscene;
	if (sc)
		fp.writeString(KW_GLOBAL_SCENE, IDNAME(sc->id.name));
	fp.closeStruct();
}

// ----------------------------------------------------------------------------
void writeObjectLibrary(Writer &fp, bParse::bListBasePtr &objects)
{
	if (objects.size() == 0)
		return;

	fp.writeSeperator();
	fp.openStruct(KW_OBJECTLIBRARY);

	for (int i=0; i<objects.size(); i++)
	{
		Blender::Object *ob = (Blender::Object*)objects[i];

		fp.openStruct(KW_OBJECT, IDNAME(ob->id.name));

		String dn;
		getDataName(dn, ob);
		if (!dn.empty())
			fp.writeString(KW_OBJECT_DATA, dn.c_str());

		fp.writeFloat16(KW_OBJECT_MATRIX, (float*)ob->obmat);
		fp.writeFloat3(KW_OBJECT_LOC, ob->loc);
		fp.writeFloat3(KW_OBJECT_ROT, ob->rot);
		fp.writeFloat3(KW_OBJECT_SIZE, ob->size);

		if (ob->parent)
			fp.writeString(KW_OBJECT_PARENT, ob->parent->id.name);


		fp.closeStruct();
	}

	fp.closeStruct();
}

// ----------------------------------------------------------------------------
void writeCameraLibrary(Writer &fp, bParse::bListBasePtr &objects)
{
	if (objects.size() == 0)
		return;

	fp.writeSeperator();
	fp.openStruct(KW_CAMERALIBRARY);

	for (int i=0; i<objects.size(); i++)
	{
		Blender::Camera *ob = (Blender::Camera*)objects[i];
		fp.openStruct(KW_CAMERA, IDNAME(ob->id.name));

		float clip[] = {ob->clipsta, ob->clipend};
		fp.writeFloat2(KW_CAMERA_CLIP, clip);

		float ang = (float)(360.0 * atan(16.0 / ob->lens) / 3.1415926535897932384626433832795);

		fp.writeFloat(KW_CAMERA_FOV, ang);
		fp.closeStruct();
	}

	fp.closeStruct();
}

// ----------------------------------------------------------------------------
void writeLampLibrary(Writer &fp, bParse::bListBasePtr &objects)
{
	if (objects.size() == 0)
		return;

	fp.writeSeperator();
	fp.openStruct(KW_LIGHTLIBRARY);

	for (int i=0; i<objects.size(); i++)
	{
		Blender::Lamp *ob = (Blender::Lamp*)objects[i];
		fp.openStruct(KW_LIGHT, IDNAME(ob->id.name));
		fp.closeStruct();
	}

	fp.closeStruct();
}

// ----------------------------------------------------------------------------
void writeSceneLibrary(Writer &fp, bParse::bListBasePtr &objects)
{
	if (objects.size() == 0)
		return;

	fp.writeSeperator();
	fp.openStruct(KW_SCENELIBRARY);

	for (int i=0; i<objects.size(); i++)
	{
		Blender::Scene *ob = (Blender::Scene*)objects[i];
		fp.openStruct(KW_SCENE, IDNAME(ob->id.name));

		CharArray baseob;
		for (Blender::Base *base = (Blender::Base*)ob->base.first; base; base = base->next)
		{
			if (base->object)
			{
				if (isSupportedType(base->object))
					baseob.push_back(IDNAME(base->object->id.name));
			}
		}

		if (!baseob.empty())
		{
			fp.writeStringArray(KW_SCENE_OBJECTS, baseob, 6);
		}


		fp.closeStruct();
	}

	fp.closeStruct();
}


// ----------------------------------------------------------------------------
void writeMeshLibrary(Writer &fp, bParse::bListBasePtr &objects)
{
	if (objects.size() == 0)
		return;

	fp.writeNewLine();
	fp.writeSeperator();

	fp.openStruct(KW_MESHLIBRARY);

	for (int i=0; i<objects.size(); i++)
	{
		Blender::Mesh *me = (Blender::Mesh*)objects[i];


		fp.openStruct(KW_MESH, IDNAME(me->id.name));

		if (me->mat)
		{
			
			CharArray matptr;

			for (int i=0; i<me->totcol; i++)
			{
				Blender::Material *mat = me->mat[i];
				if (mat)
					matptr.push_back(IDNAME(mat->id.name));
			}

			if (!matptr.empty())
			{
				fp.writeStringArray(KW_MESH_MATERIALS, matptr, 4);
			}

		}



		Blender::MVert* mvert = me->mvert;


		int f;


		FloatArray co, weight, no;
		IntArray matnrs, flag;

		if (me->totvert)
		{
			co.reserve(me->totvert * 3);
			no.reserve(me->totvert * 3);
			flag.reserve(me->totvert);

			for (f=0; f<me->totvert; f++, ++mvert)
			{
				co.push_back(mvert->co[0]);
				co.push_back(mvert->co[1]);
				co.push_back(mvert->co[2]);
				
				no.push_back((float)mvert->no[0]/32767.f);
				no.push_back((float)mvert->no[1]/32767.f);
				no.push_back((float)mvert->no[2]/32767.f);
			}

			fp.openStruct(KW_MESH_VBUFFER, me->totvert);
			fp.writeFloatArray(KW_MESH_VERTS, co, 12);
			fp.writeFloatArray(KW_MESH_NORMALS, no, 12);
			fp.closeStruct();
		}

		if (me->totface)
		{
			Blender::MFace *mface = me->mface;

			IntArray index, matnrs;
			flag.reserve(me->totface);
			index.reserve(me->totface * 4);
			matnrs.reserve(me->totface);

			bool write_flags = false;

			for (f=0; f<me->totface; f++, ++mface)
			{
				index.push_back(mface->v1);
				index.push_back(mface->v2);
				index.push_back(mface->v3);
				index.push_back(mface->v4);

				if (mface->flag & ME_SMOOTH)
				{
					write_flags = true;
					flag.push_back(1);
				}
				else
					flag.push_back(0);

				if (me->mat)
					matnrs.push_back(mface->mat_nr);
			}
			fp.openStruct(KW_MESH_IBUFFER, me->totface);

			fp.writeIntArray(KW_MESH_INDICES, index, 24);
			if (me->mat)		fp.writeIntArray(KW_MESH_MATNRS, matnrs, 24);
			if (write_flags)	fp.writeIntArray(KW_MESH_FLAGS, flag, 12);
			fp.closeStruct();
		}

		fp.closeStruct();
	}

	fp.closeStruct();
}

// ----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	if (argc != 3)
	{
		printf("Usage: %s file.blend file.ascii\n", argv[0]);
		return 1;
	}


	bParse::bBlenderFile file(argv[1]);
	if (!file.ok())
		return 1;


	file.parse(false);

	Writer fp(argv[2], false);
	if (!fp.isOpen())
	{
		printf("failed to open .ascii file %s, for writing\n", argv[2]);
		return 1;
	}

	writeHeader(fp);
	writeGlobal(fp, (Blender::FileGlobal*)file.getFileGlobal());
	writeSceneLibrary(fp, *file.getMain()->getScene());
	writeObjectLibrary(fp, *file.getMain()->getObject());
	writeCameraLibrary(fp, *file.getMain()->getCamera());
	writeLampLibrary(fp, *file.getMain()->getLamp());
	writeMeshLibrary(fp, *file.getMain()->getMesh());
	return 0;
}
