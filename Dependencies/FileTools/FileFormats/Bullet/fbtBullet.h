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
#ifndef _btBulletFile_h_
#define _btBulletFile_h_

#include "fbtFile.h"



class btBulletFile : public fbtFile
{
public:
	btBulletFile();
	virtual ~btBulletFile();


protected:

	virtual int notifyData(void* p, const Chunk& id);

	virtual int initializeTables(fbtBinTables* tables);
	virtual void*   getFBT(void);
	virtual FBTsize getFBTlength(void);

};

#endif//_btBulletFile_h_
