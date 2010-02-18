/*
-------------------------------------------------------------------------------
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
	 claim that you wrote the original software. If you use this software
	 in a product, an acknowledgment in the product documentation would be
	 appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
	 misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#include "Blender.h"
#include "bAscii.h"
#include "bAsciiBlendLinker.h"
#include "bAsciiCompiler.h"
#include "bAsciiMain.h"
#include "bAsciiBlock.h"
#include "bAsciiArgument.h"
#include "utPrint.h"



void BlenderPushBack(Blender::ListBase &lb, void* v)
{
    struct Link
    {
        void*   next;
        void*   prev;
    };

	Link *link= (Link*)(v);
    if (!link)
        return;

	link->prev= lb.last;
    if (lb.last)
        ((Link*)lb.last)->next= (Link*)link;
	if (!lb.first)
        lb.first= link;
    lb.last= link;
}


template<typename T>
T* BlendLinkIDType(bAsciiBlock *obj, const char *dataname, utArray<T*> &array)
{
	T *ob = utMemUtils::talloc<T>(true);

	bStringF name;
	name.format("%s%s", dataname, obj->getName().c_str());
	utStrcpy(ob->id.name, name.c_str(), name.size());

	ob->id.next = 0;
	ob->id.prev = 0;

	if (!array.empty())
	{
		ob->id.prev = array.back();
		static_cast<Blender::ID*>(ob->id.prev)->next = ob;
	}
	array.push_back(ob);
	return ob;
}

// ----------------------------------------------------------------------------
BlendLinker::BlendLinker() : m_parent(0), m_main(0), m_verbose(false)
{
}

// ----------------------------------------------------------------------------
BlendLinker::~BlendLinker()
{
}

// ----------------------------------------------------------------------------
void BlendLinker::linkCamera(bAsciiBlock *lib)
{
	bAsciiBlockIterator it = lib->getChildIterator();
	while (it.hasMoreElements())
	{
		bAsciiBlock* obj = it.getNext();

		if (obj->getType() != OP_BLK_CA)
		{
			utPrintf("Block mismatch\n");
			continue;
		}
		Blender::Camera *ob = BlendLinkIDType(obj, "CA", m_main->camera);
		if (m_verbose)
			utPrintf("\tLink Mesh ==> %s\n", ob->id.name);


		ob->clipsta = 0.1f;
		ob->clipend = 100.f;
		if (obj->hasArgument(OP_CAMERA_CLIP))
		{
			bAsciiArgument *arg = obj->getArgument(OP_CAMERA_CLIP);

			if (arg->m_numbers.size() >=2)
			{
				ob->clipsta = arg->m_numbers[0];
				ob->clipend = arg->m_numbers[1];
			}
		}

		if (obj->hasArgument(OP_CAMERA_FOV))
		{
			bAsciiArgument *arg = obj->getArgument(OP_CAMERA_FOV);
			if (arg->m_numbers.size() >=1)
				ob->angle = arg->m_numbers[0];
		}
	}
}

// ----------------------------------------------------------------------------
void BlendLinker::linkMesh(bAsciiBlock *lib)
{
	bAsciiBlockIterator it = lib->getChildIterator();
	while (it.hasMoreElements())
	{
		bAsciiBlock* obj = it.getNext();

		if (obj->getType() != OP_BLK_ME)
		{
			utPrintf("Block mismatch\n");
			continue;
		}

		Blender::Mesh *ob = BlendLinkIDType(obj, "ME", m_main->mesh);
		if (m_verbose)
			utPrintf("\tLink Mesh ==> %s\n", ob->id.name);

		// build buffers

		bAsciiBlockIterator ch = obj->getChildIterator();
		while (ch.hasMoreElements())
		{
			bAsciiBlock *bufobj = ch.getNext();

			if (bufobj->getType() == OP_MESH_VERTEX_BUFFER && !ob->mvert)
			{
				int len = (int)bufobj->getLength();
				if (len > 0)
				{
					bAsciiArgument* co	=  bufobj->getArgument(OP_MESH_VERTEX_VERTS);
					bAsciiArgument* no =  bufobj->getArgument(OP_MESH_VERTEX_NORMALS);

					if (co && no)
					{
						if (co->m_numbers.size() != (len * 3) && no->m_numbers.size() != (len *3))
						{
							utPrintf("Vertex buffers do not match specified size of %i verts.\n", len);
							continue;
						}

						ob->totvert = len;

						bNumberList::Pointer cp = co->m_numbers.ptr();
						bNumberList::Pointer np = no->m_numbers.ptr();

						ob->mvert = utMemUtils::tcalloc<Blender::MVert>(len, false);
						Blender::MVert *mvert = ob->mvert;

						int offs = 0;
						while (len--)
						{
							mvert->co[0] = (cp[offs+0]);
							mvert->co[1] = (cp[offs+1]);
							mvert->co[2] = (cp[offs+2]);

							mvert->no[0] = (short)((np[offs+0]) * 32767.f);
							mvert->no[1] = (short)((np[offs+1]) * 32767.f);
							mvert->no[2] = (short)((np[offs+2]) * 32767.f);

							// unused
							mvert->bweight = '\0';
							mvert->flag = 0;
							mvert->mat_nr = -1;
							mvert->pad1 = 0;

							offs += 3;
							++mvert;
						}
					}
				}
			}

			if (bufobj->getType() == OP_MESH_INDEX_BUFFER && !ob->mface)
			{
				int len = (int)bufobj->getLength();
				if (len > 0)
				{
					bAsciiArgument* in = bufobj->getArgument(OP_MESH_INDEX_INDICES);
					bAsciiArgument* fl = bufobj->getArgument(OP_MESH_INDEX_FLAGS);
					bAsciiArgument* mn = bufobj->getArgument(OP_MESH_INDEX_MATERIALS);

					if (in)
					{
						if (	in->m_numbers.size() != (len * 4) && 
								(fl && fl->m_numbers.size() != (len)) &&  
								(mn && mn->m_numbers.size() != (len *4)))
						{
							utPrintf("Index buffers do not match specified size of %i.\n", len);
							continue;
						}

						ob->totface = len;

						bNumberList::Pointer ip = in->m_numbers.ptr();
						bNumberList::Pointer fp = fl ? fl->m_numbers.ptr() : 0;
						bNumberList::Pointer mp = mn ? mn->m_numbers.ptr() : 0;


						ob->mface = utMemUtils::tcalloc<Blender::MFace>(len, false);
						Blender::MFace *mface = ob->mface;

						int offs = 0, foffs = 0;
						while (len--)
						{
							mface->v1 = (int)(ip[offs+0]);
							mface->v2 = (int)(ip[offs+1]);
							mface->v3 = (int)(ip[offs+2]);
							mface->v4 = (int)(ip[offs+3]);

							if (fp && fp[foffs] == 1)
								mface->flag = 1;
							else mface->flag = 0;

							if (mp)
								mface->mat_nr = (char)(int)mp[foffs];
							else mface->mat_nr = 0;

							// unused
							mface->edcode = '\0';

							offs += 4;
							foffs += 1;
							++mface;
						}
					}
				}
			}
		}
	}
}

// ----------------------------------------------------------------------------
void BlendLinker::linkObject(bAsciiBlock *lib)
{

	bAsciiBlockIterator it = lib->getChildIterator();
	while (it.hasMoreElements())
	{
		bAsciiBlock* obj = it.getNext();

		if (obj->getType() != OP_BLK_OB)
		{
			utPrintf("Block mismatch\n");
			continue;
		}

		Blender::Object *ob = BlendLinkIDType(obj, "OB", m_main->object);
		if (m_verbose)
			utPrintf("\tLink Object ==> %s\n", ob->id.name);
	}
}


// ----------------------------------------------------------------------------
void BlendLinker::linkScene(bAsciiBlock *lib)
{

	bAsciiBlockIterator it = lib->getChildIterator();
	while (it.hasMoreElements())
	{
		bAsciiBlock* obj = it.getNext();

		if (obj->getType() != OP_BLK_SC)
		{
			utPrintf("Block mismatch\n");
			continue;
		}

		Blender::Scene *ob = BlendLinkIDType(obj, "SC", m_main->scene);
		if (m_verbose)
			utPrintf("\tLink Scene ==> %s\n", ob->id.name);


		if (obj->hasArgument(OP_SCENE_OBJECTS))
		{
			bAsciiArgument *objects = obj->getArgument(OP_SCENE_OBJECTS);
			for (size_t i=0; i<objects->m_alphas.size(); ++i)
			{
				bStringF alpha = objects->m_alphas[i];
				Blender::Object *bob = m_main->find(alpha.c_str(), m_main->object);
				if (bob == 0)
				{
					utPrintf("missing object %s\n", alpha.c_str());
					continue;
				}

				Blender::Base* bse = utMemUtils::talloc<Blender::Base>(true);

				bse->object = bob;
				BlenderPushBack(ob->base, bse);

				if (m_verbose)
					utPrintf("\tLink Object ==> %s\n", bob->id.name);
			}

		}

	}
}


// ----------------------------------------------------------------------------
int BlendLinker::link(bAscii *parent, bAsciiMain *main, bool verbose)
{
	UT_ASSERT(parent && main);

	m_verbose = verbose;
	m_main = main;
	m_parent = parent;

	bAsciiBlock *lib;

	lib = m_parent->findBlock(OP_LIB_CA);
	if (lib)
	{
		if (verbose)
			utPrintf("Linking Camera Library ==> %p\n", lib);
		linkCamera(lib);
	}

	lib = m_parent->findBlock(OP_LIB_ME);
	if (lib)
	{
		if (verbose)
			utPrintf("Linking Mesh Library ==> %p\n", lib);
		linkMesh(lib);
	}

	lib = m_parent->findBlock(OP_LIB_OB);
	if (lib)
	{
		if (verbose)
			utPrintf("Linking Object Library ==> %p\n", lib);
		linkObject(lib);
	}

	lib = m_parent->findBlock(OP_LIB_SC);
	if (lib)
	{
		if (verbose)
			utPrintf("Linking Scene Library ==> %p\n", lib);
		linkScene(lib);
	}

	return 0;
}
