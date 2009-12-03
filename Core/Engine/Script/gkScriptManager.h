/*
-------------------------------------------------------------------------------
	This file is part of the Ogre GameKit port.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
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
#ifndef _gkScriptManager_h_
#define _gkScriptManager_h_

#include "gkCommon.h"
#include "OgreIteratorWrappers.h"
#include "OgreString.h"



// ----------------------------------------------------------------------------
class gkScriptExpression
{
public:
	enum ExpressionResult
	{
		EXPR_ERROR= -1,
		EXPR_FALSE,
		EXPR_TRUE
	};

	gkScriptExpression() {}
	virtual ~gkScriptExpression() {}

	virtual size_t getHandle()= 0;
	virtual bool isOk()= 0;
	virtual int run()= 0;
	virtual void addConstant(const Ogre::String &name, int v)= 0;
	virtual void addConstant(const Ogre::String &name, Ogre::Real v)= 0;
	virtual void addConstant(const Ogre::String &name, bool v)= 0;
	virtual void addConstant(const Ogre::String &name, Ogre::String v)= 0;

};

// ----------------------------------------------------------------------------
class gkScriptManager
{
public:

	typedef std::map<size_t, gkScriptExpression*> ExpressionMap;
	typedef Ogre::MapIterator<ExpressionMap> ExpressionMapIterator;

private:
	static gkScriptManager* mManager;

protected:
	gkScriptManager() {}
	virtual ~gkScriptManager() {}
	virtual void start()= 0;
	virtual void stop()= 0;

public:
	virtual bool execFile(const Ogre::String &file)= 0;
	virtual bool execBuffer(const Ogre::String& name, const Ogre::String &buf)= 0;

	virtual gkScriptExpression* createExpression(const Ogre::String &expr)= 0;
	virtual void clearExpressions()= 0;



	static gkScriptManager& getSingleton();
	static gkScriptManager* getSingletonPtr();

	static gkScriptManager* initialize();
	static void finalize();
};

#endif//_gkScriptManager_h_
