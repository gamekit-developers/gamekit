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
#include "fbtBullet.h"
#include "fbtTables.h"


static char btBulletFile_HeaderF[8] = {'B', 'U', 'L', 'L', 'E', 'T', 'f', '\0'};
static char btBulletFile_HeaderD[8] = {'B', 'U', 'L', 'L', 'E', 'T', 'd', '\0'};


extern unsigned char btBulletFBT[];
extern int btBulletLen;



btBulletFile::btBulletFile()
	:   fbtFile(btBulletFile_HeaderF)
{
}



btBulletFile::~btBulletFile()
{
}




int btBulletFile::notifyData(void* p, const Chunk& id)
{
	return FS_OK;
}


int btBulletFile::initializeTables(fbtBinTables* tables)
{
	return tables->read(btBulletFBT, btBulletLen, false) ? FS_OK : FS_FAILED;
}

void* btBulletFile::getFBT(void)
{
	return btBulletFBT;
}

FBTsize btBulletFile::getFBTlength(void)
{
	return btBulletLen;
}
