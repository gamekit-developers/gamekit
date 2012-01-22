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
#ifndef _fbtBlend_h_
#define _fbtBlend_h_

#include "fbtFile.h"
#include "Blender.h"



class fbtBlend : public fbtFile
{
public:
	fbtBlend();
	virtual ~fbtBlend();


	fbtList m_scene;
	fbtList m_library;
	fbtList m_object;
	fbtList m_mesh;
	fbtList m_curve;
	fbtList m_mball;
	fbtList m_mat;
	fbtList m_tex;
	fbtList m_image;
	fbtList m_latt;
	fbtList m_lamp;
	fbtList m_camera;
	fbtList m_ipo;
	fbtList m_key;
	fbtList m_world;
	fbtList m_screen;
	fbtList m_script;
	fbtList m_vfont;
	fbtList m_text;
	fbtList m_sound;
	fbtList m_group;
	fbtList m_armature;
	fbtList m_action;
	fbtList m_nodetree;
	fbtList m_brush;
	fbtList m_particle;
	fbtList m_wm;
	fbtList m_gpencil;

	Blender::FileGlobal* m_fg;

	int save(const char* path, const int mode = PM_UNCOMPRESSED);
	
	void setIgnoreList(FBTuint32 *stripList) {m_stripList = stripList;}

protected:
	virtual int notifyData(void* p, const Chunk& id);
	virtual int initializeTables(fbtBinTables* tables);
	virtual bool skip(const FBTuint32& id);
	virtual int writeData(fbtStream* stream);

	FBTuint32* m_stripList;

	virtual void*   getFBT(void);
	virtual FBTsize getFBTlength(void);
};


#endif//_fbtBlend_h_
