/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 harkon.kr

    Contributor(s): none yet.
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

#ifndef _okApp_h_
#define _okApp_h_

#if 0
struct okSceneInfo 
{	
	gkScene* scene;
	gkBlendFile* blend;
	gkString group;

	okSceneInfo() : scene(0), blend(0) {}
};

typedef utArray<okSceneInfo> okSceneInfoArray;
#endif

class okApp : public gkCoreApplication, public gkWindowSystem::Listener, public utSingleton<okApp>
{
	utArray<gkScene*> m_scenes;

	gkString m_cfg;
	bool m_inited;
	int m_winCount;
//	int m_sceneCount;

	bool m_showPhysicsDebug; 

	bool hasScene() { return !m_scenes.empty(); }

	virtual bool setup();

	//UTsize findSceneInfo(gkScene* scene);
	//gkBlendFile* getSceneBlendFile(gkScene* scene);
protected:
	virtual void  tick (gkScalar rate);

public:
	okApp();
	virtual ~okApp();
	
	bool init(const gkString& cfg, const gkString& windowHandle, int winSizeX, int winSizeY);
	void uninit();

	GK_INLINE bool isInited() const { return m_inited; }

	gkWindow* createWindow(const gkString& windowHandle, int winSizeX, int winSizeY);
	gkWindow* getMainWindow();
	void destroyWindow(gkWindow* win);

	gkScene* loadScene(gkWindow* window, const gkString& blend, const gkString& sceneName="", bool ignoreCache=false);
	gkScene* mergeScene(gkScene* dstScene, const gkString& blend, const gkString& sceneName="", bool ignoreCache=false);

	void unloadScene(gkScene* scene);
	void unloadAllScenes();
	bool changeScene(gkScene* scene, const gkString& sceneName);

	gkScene* createEmptyScene();
	GK_INLINE gkScene* getFirstScene() { return m_scenes.empty() ? NULL : m_scenes[0]; }
	gkString getFirstSceneName();
	gkBlendFile* getFirstBlendFile();
	gkBlendFile* getBlendFile(const gkString& fname);
	bool step();
	

	GK_INLINE bool getShowPhysicsDebug() { return m_showPhysicsDebug; }
	void setShowPhysicsDebug(bool show);

	UT_DECLARE_SINGLETON(okApp)
};

#endif //_okApp_h_