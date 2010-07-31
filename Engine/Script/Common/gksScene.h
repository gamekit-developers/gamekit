/*
-------------------------------------------------------------------------------
--                                                                           --
--  This file is part of GameKit.                                            --
--  http://gamekit.googlecode.com/                                           --
--                                                                           --
--                                                                           --
--  Released under the zlib license.                                         --
--  http://www.gzip.org/zlib/zlib_license.html                               --
--                                                                           --
--                                                                           --
-- Generic Script API Binding                                                --
-------------------------------------------------------------------------------
*/
#ifndef _gksScene_h_
#define _gksScene_h_


#include "gksObject.h"

class gksGameObject;


// ----------------------------------------------------------------------------
class gksScene : public gksObject
{
#ifndef SWIG

public:
	gksScene(gkObject *oth) : gksObject(oth) {}
#endif

public:
	gksScene() : gksObject() {}

	virtual ~gksScene() {}

	bool hasObject(const char *name);
	gksPointer<gksGameObject>   getObject(const char *name);
};

extern gksPointer<gksScene> getCurrentScene(void);

#endif//_gksScene_h_
