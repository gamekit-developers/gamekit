/*
-------------------------------------------------------------------------------
    This file is part of FBT (File Binary Tables).
    http://gamekit.googlecode.com/

    Copyright (c) 2010 Charlie C & Erwin Coumans.

-------------------------------------------------------------------------------
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
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
#include "fbtBlend.h"
#include "Blender.h"
using namespace Blender;


int main(int argc, char** argv)
{
	if (argc < 2)
		return 1;


	//btBulletFile fp;
	fbtBlend fp;

	if (fp.parse(argv[argc-1], fbtFile::PM_READTOMEMORY) != fbtFile::FS_OK)
	{
		return 1;
	}

	Blender::FileGlobal* fg = fp.m_fg;

	for (Blender::Text* tx = (Blender::Text*)fp.m_text.first; tx; tx = (Blender::Text*)tx->id.next)
	{
		for (Blender::TextLine* tl = (Blender::TextLine*)tx->lines.first; tl; tl = tl->next)
		{
			fbtPrintf("%s\n", tl->line);

		}
	}



	for (Blender::bScreen* bs = (Blender::bScreen*)fp.m_screen.first; bs; bs = (Blender::bScreen*)bs->id.next)
	{
		fbtPrintf("%s\n", bs->id.name);

		for (Blender::ScrEdge* edge = (Blender::ScrEdge*)bs->edgebase.first; edge; edge = edge->next)
		{
			Blender::ScrVert* v1 = edge->v1;
			Blender::ScrVert* v2 = edge->v2;

			if (!v1)
			{
				fbtPrintf("%s Failed\n", bs->id.name);
			}
		}


	}


	fbtList& objects = fp.m_object;
	for (Object* ob = (Object*)objects.first; ob; ob = (Object*)ob->id.next)
	{
		fbtPrintf("%s\n", ob->id.name);

		if (ob->data && ob->type == 1)
		{
			Mesh* me = (Mesh*)ob->data;
			fbtPrintf("\t%s\n", me->id.name);

			if (me->mat && *me->mat)
			{
				for (int i = 0; i < me->totcol; ++i)
				{
					Material* ma = me->mat[i];

					if (ma)
					{
						fbtPrintf("\t\t%s\n", ma->id.name);

						if (ma->mtex)
						{
							int i = 0;
							while (ma->mtex[i] != 0)
							{
								if (ma->mtex[i]->tex)
									fbtPrintf("\t\t\t%s\n", ma->mtex[i]->tex->id.name);
								++i;

							}
						}
					}
				}
			}
		}
	}

	//fp.reflect("Test.blend");
	return 0;
}
