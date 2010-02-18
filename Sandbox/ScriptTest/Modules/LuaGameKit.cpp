/*
-------------------------------------------------------------------------------
	Lua 5.1 script tests.
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
#include "LuaEngine.h"
#include "LuaBindingUtils.h"
#include "gkEngine.h"
#include "gkUserDefs.h"
#include "gkSceneObject.h"
#include "gkCameraObject.h"
#include "gkGameObject.h"
#include "gkKeyCodes.h"

#include "Loaders/gkBlendFile.h"

#define ModuleName "GameKit"

struct LuaGameKit;
struct LuaBlendFile;
struct LuaScene;



///////////////////////////////////////////////////////////////////////////////

struct LuaGameKit
{
	LuaGameKit();
	~LuaGameKit();

	gkEngine *m_engine;

	// static members
	static int Load(LuaState *L);
	static int Run(LuaState *L);
	static int New(LuaState *L);
	static int Delete(LuaState *L);
	static int Set(LuaState *L);

	static LuaTypeDef Type;
	static LuaMethodDef Methods[];
	static const char DocString[];

};

struct LuaBlendFile
{
	LuaBlendFile(gkBlendFile *bf);
	~LuaBlendFile();

	static int Delete(LuaState *L);
	static int Create(LuaState *L, gkBlendFile *file);
	static int GetScene(LuaState *L);

	gkBlendFile *file;

	static LuaTypeDef Type;
	static LuaMethodDef Methods[];
	static const char DocString[];
};

struct LuaScene
{
	LuaScene(gkSceneObject *self);
	~LuaScene();

	static int Delete(LuaState *L);
	static int Create(LuaState *L, gkSceneObject *sc);
	static int Load(LuaState *L);
	static int Unload(LuaState *L);
	static int IsLoaded(LuaState *L);
	static int GetName(LuaState *L);
	static int GetObjectCount(LuaState *L);
	static int GetObjectAt(LuaState *L);
	static int CDLogic(LuaState *L);
	static int AddExitHook(LuaState *L);

	gkSceneObject *self;

	static LuaTypeDef Type;
	static LuaMethodDef Methods[];
	static const char DocString[];
};


struct LuaObject
{
	LuaObject(gkGameObject *self);
	~LuaObject();

	static int Delete(LuaState *L);
	static int Create(LuaState *L, gkGameObject *data);
	static int Load(LuaState *L);
	static int Unload(LuaState *L);
	static int IsLoaded(LuaState *L);
	static int GetName(LuaState *L);
	static int GetType(LuaState *L);

	gkGameObject *self;

	static LuaTypeDef Type;
	static LuaMethodDef Methods[];
	static const char DocString[];
};



///////////////////////////////////////////////////////////////////////////////
LuaScene::LuaScene(gkSceneObject *v) : self(v)
{
}


LuaScene::~LuaScene()
{
}


int LuaScene::Delete(LuaState *L)
{
	LuaScene *self= LuaBind::self<LuaScene>(L);
	if (self) LuaDelete(self, LuaScene);
	return 0;
}

int LuaScene::Create(LuaState *L, gkSceneObject *sc)
{
	LuaScene *ptr;
	LuaNew(L, ptr, Type, LuaScene, LuaScene(sc));
	return ptr ? 1 : 0;
}

int LuaScene::Load(LuaState *L)
{
	LuaScene *self= LuaBind::self<LuaScene>(L);

	if (!self)
		return LuaBind::seterror(L, "invalid self");

	self->self->load();
	return 0;
}


int LuaScene::Unload(LuaState *L)
{
	LuaScene *self= LuaBind::self<LuaScene>(L);

	if (!self)
		return LuaBind::seterror(L, "invalid self");

	self->self->unload();
	return 0;
}


int LuaScene::IsLoaded(LuaState *L)
{
	LuaScene *self= LuaBind::self<LuaScene>(L);

	if (!self)
		return LuaBind::seterror(L, "invalid self");

	return LuaBind::push(L, self->self->isLoaded());
}

int LuaScene::GetName(LuaState *L)
{
	LuaScene *self= LuaBind::self<LuaScene>(L);
	if (!self)
		return LuaBind::seterror(L, "invalid self");
	return LuaBind::push(L, self->self->getName().c_str());
}


int LuaScene::GetObjectCount(LuaState *L)
{
	LuaScene *self= LuaBind::self<LuaScene>(L);
	if (!self)
		return LuaBind::seterror(L, "invalid self");


	return LuaBind::push(L, (int)self->self->getLoadedObjects().size());
}

int LuaScene::GetObjectAt(LuaState *L)
{
	LuaScene *self= LuaBind::self<LuaScene>(L);
	if (!self)
		return LuaBind::seterror(L, "invalid self");

	int i=0;

	if (!LuaBind::getArg(L,0, &i))
		return LuaBind::seterror(L, "invalid index argument.");


	gkSceneObject *sc = self->self;
	gkGameObjectList &objs = sc->getLoadedObjects();

	if (i < 0 || i >= (int)objs.size())
		return LuaBind::seterror(L, "index out of bounds.");

	return LuaObject::Create(L, objs.at(i));
}




extern gkCameraObject* createMouseLook(gkSceneObject *sc, const Ogre::Vector3 &pos, const Ogre::Vector3& ori, 
								const float rotationScale = -0.3f,
								const float translationScale = 0.5f
								);

extern void createExitHook(gkSceneObject *sc, gkScanCode kc);


int LuaScene::CDLogic(LuaState *L)
{
	LuaScene *self= LuaBind::self<LuaScene>(L);
	if (!self)
		return LuaBind::seterror(L, "invalid self");

	float rs=-0.3f, ts=0.5f;
	if (!LuaBind::parse(L, "|ff", &rs, &ts))
		return 0;

	gkSceneObject *scene = self->self;
	if (!scene->isLoaded())
		return 0;

	if (!scene->hasObject("createMouseLook::cam"))
		createMouseLook(scene, Ogre::Vector3(0,-5,0), Ogre::Vector3(90, 0, 0), rs, ts);
	return 0;
}


int LuaScene::AddExitHook(LuaState *L)
{
	LuaScene *self= LuaBind::self<LuaScene>(L);
	if (!self)
		return LuaBind::seterror(L, "invalid self");

	int kc;
	if (!LuaBind::getArg(L, 0, &kc))
		return LuaBind::seterror(L, "invalid integer argument.");

	if (kc > KC_NONE && kc < KC_MAX)
		createExitHook(self->self, (gkScanCode)kc);
	return 0;
}



///////////////////////////////////////////////////////////////////////////////
LuaTypeDef LuaScene::Type=
{
	"Scene",
	"Scene Loader",
	0,
	LuaScene::Delete,
	0,
	0,
	LuaScene::Methods
};



LuaMethodDef LuaScene::Methods[]=
{
	{"load",			LuaScene::Load},
	{"unload",			LuaScene::Unload},
	{"isLoaded",		LuaScene::IsLoaded},
	{"getName",			LuaScene::GetName},
	{"getObjectCount",	LuaScene::GetObjectCount},
	{"getObjectAt",		LuaScene::GetObjectAt},
	{"addExitHook",		LuaScene::AddExitHook},
	{"createDemoLogic",	LuaScene::CDLogic},
	{0,0}
};


///////////////////////////////////////////////////////////////////////////////

LuaObject::LuaObject(gkGameObject *v) : self(v)
{
}


LuaObject::~LuaObject()
{
}


int LuaObject::Delete(LuaState *L)
{
	LuaObject *self= LuaBind::self<LuaObject>(L);
	if (self) LuaDelete(self, LuaObject);
	return 0;
}

int LuaObject::Create(LuaState *L, gkGameObject *data)
{
	LuaObject *ptr;
	LuaNew(L, ptr, Type, LuaObject, LuaObject(data));
	return ptr ? 1 : 0;
}

int LuaObject::GetName(LuaState *L)
{
	LuaObject *self= LuaBind::self<LuaObject>(L);
	if (!self)
		return LuaBind::seterror(L, "invalid self");
	return LuaBind::push(L, self->self->getName().c_str());
}

int LuaObject::GetType(LuaState *L)
{
	LuaObject *self= LuaBind::self<LuaObject>(L);
	if (!self)
		return LuaBind::seterror(L, "invalid self");
	return LuaBind::push(L, (int)self->self->getType());
}

///////////////////////////////////////////////////////////////////////////////
LuaTypeDef LuaObject::Type=
{
	"GameObject",
	"GameObject Instance",
	0,
	LuaObject::Delete,
	0,
	0,
	LuaObject::Methods
};


LuaMethodDef LuaObject::Methods[]=
{
	{"getName",		LuaObject::GetName},
	{"getType",		LuaObject::GetType},
	{0,0}
};


///////////////////////////////////////////////////////////////////////////////

LuaGameKit::LuaGameKit()
{
	m_engine= new gkEngine();
}

LuaGameKit::~LuaGameKit()
{
	if (m_engine)
		delete m_engine;
}

int LuaGameKit::Load(LuaState *L)
{
	LuaGameKit *gk= LuaBind::self<LuaGameKit>(L);
	if (!gk)
		return LuaBind::seterror(L, "invalid self");

	char *file;
	if (!LuaBind::getArg(L, 0, &file))
		return LuaBind::seterror(L, "expected file name argument");

	gkEngine &eng= *gk->m_engine;
	if (!eng.isInitialized())
		eng.initialize();

	gkBlendFile *bf= eng.loadBlendFile(file);
	if (!bf)
		return LuaBind::seterror(L, "failed to load blend file '%s'", file);

	return LuaBlendFile::Create(L, bf);
}

int LuaGameKit::Run(LuaState *L)
{
	LuaGameKit *self= LuaBind::self<LuaGameKit>(L);
	if (!self || !self->m_engine)
		return LuaBind::seterror(L, "invalid self");

	gkEngine *eng= self->m_engine;
	eng->run();
	return 0;
}


int LuaGameKit::New(LuaState *L)
{
	if (gkEngine::getSingletonPtr()==0)
	{
		LuaGameKit *gk;
		LuaNew(L, gk, Type, LuaGameKit, LuaGameKit());
		return gk ? 1 : 0;
	}
	return 0;
}

int LuaGameKit::Delete(LuaState *L)
{
	LuaGameKit *gk= LuaBind::self<LuaGameKit>(L);
	if (gk) LuaDelete(gk, LuaGameKit);
	return 0;
}

int LuaGameKit::Set(LuaState *L)
{
	LuaGameKit *gk= LuaBind::self<LuaGameKit>(L);
	if (!gk)
		return LuaBind::seterror(L, "invalid instance");

	gkUserDefs &defs= gk->m_engine->getUserDefs();


	char *var= 0;
	if (LuaBind::getArg(L, 0, &var) && var)
	{

		if (LuaCharEq(var, "rendersystem"))
		{
			int rs;
			if (!LuaBind::getArg(L, 1, &rs))
				return LuaBind::seterror(L, "invalid integer for %s", var);

			defs.rendersystem= gkFindRenderSystem((OgreRenderSystem)rs);
		}

		if (LuaCharEq(var, "title"))
		{
			char *title;
			if (!LuaBind::getArg(L, 1, &title))
				return LuaBind::seterror(L, "invalid string for %s", var);

			defs.wintitle= title;
		}
		else if (LuaCharEq(var, "blenmat"))
		{
			bool mat;
			if (!LuaBind::getArg(L, 1, &mat))
				return LuaBind::seterror(L, "invalid boolean for %s", var);

			defs.blendermat= mat;
		}
		else if (LuaCharEq(var, "fullscreen"))
		{
			bool v;
			if (!LuaBind::getArg(L, 1, &v))
				return LuaBind::seterror(L, "invalid boolean for %s", var);

			defs.fullscreen= v;
		}

		else if (LuaCharEq(var, "width"))
		{
			int v;
			if (!LuaBind::getArg(L, 1, &v))
				return LuaBind::seterror(L, "invalid integer for %s", var);

			defs.winsize.x= v;
		}
		else if (LuaCharEq(var, "height"))
		{
			int v;
			if (!LuaBind::getArg(L, 1, &v))
				return LuaBind::seterror(L, "invalid integer for %s", var);

			defs.winsize.y= v;
		}
		else if (LuaCharEq(var, "quiet"))
		{
			bool v;
			if (!LuaBind::getArg(L, 1, &v))
				return LuaBind::seterror(L, "invalid boolean for %s", var);
			defs.verbose= !v;
		}
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

LuaTypeDef LuaGameKit::Type=
{
	"Engine",
	"Engine Loader.",
	LuaGameKit::New,
	LuaGameKit::Delete,
	0,
	LuaGameKit::Set,
	LuaGameKit::Methods
};

LuaMethodDef LuaGameKit::Methods[]=
{
	{"loadBlend",	LuaGameKit::Load},
	{"run",			LuaGameKit::Run},
	{0,0}
};


///////////////////////////////////////////////////////////////////////////////

LuaBlendFile::LuaBlendFile(gkBlendFile *bf) :
		file(bf)
{
}

LuaBlendFile::~LuaBlendFile()
{
}

int LuaBlendFile::Delete(LuaState *L)
{
	LuaBlendFile *self= LuaBind::self<LuaBlendFile>(L);
	if (self) LuaDelete(self, LuaBlendFile);
	return 0;
}

int LuaBlendFile::Create(LuaState *L, gkBlendFile *file)
{
	LuaBlendFile *ptr;
	LuaNew(L, ptr, Type, LuaBlendFile, LuaBlendFile(file));
	return ptr ? 1 : 0;
}


int LuaBlendFile::GetScene(LuaState *L)
{
	LuaBlendFile *self= LuaBind::self<LuaBlendFile>(L);
	if (!self || !self->file)
		return LuaBind::seterror(L, "invalid self");

	gkBlendFile *f= self->file;

	gkSceneObject *sc = f->getSceneIterator().peekNext();
	if (!sc)
		return LuaBind::seterror(L, "missing default scene!");

	return LuaScene::Create(L, sc);;
}

///////////////////////////////////////////////////////////////////////////////

LuaTypeDef LuaBlendFile::Type=
{
	"BlendFile",
	"Blend file loader.",
	0,
	LuaBlendFile::Delete,
	0,
	0,
	LuaBlendFile::Methods
};

LuaMethodDef LuaBlendFile::Methods[]=
{
	{"getScene", LuaBlendFile::GetScene},
	{0,0}
};


///////////////////////////////////////////////////////////////////////////////

void LuaGameKit_initialize(LuaState *L)
{
	int tval= LuaBind::beginBinding(L);
	LuaBind::table(L, ModuleName);

	LuaBind::set(L, "OGRE_RS_GL",	(int)OGRE_RS_GL);
	LuaBind::set(L, "OGRE_RS_GLES", (int)OGRE_RS_GLES);
	LuaBind::set(L, "OGRE_RS_D3D9",	(int)OGRE_RS_D3D9);
	LuaBind::set(L, "OGRE_RS_D3D10",(int)OGRE_RS_D3D10);

	LuaBind::set(L, &LuaGameKit::Type);
	LuaBind::set(L, &LuaBlendFile::Type);
	LuaBind::set(L, &LuaScene::Type);
	LuaBind::set(L, &LuaObject::Type);

	LuaBind::table(L);
	LuaBind::endBinding(L, tval);
}
