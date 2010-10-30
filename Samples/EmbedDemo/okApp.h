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

class okApp : public gkCoreApplication, public gkWindowSystem::Listener
{
	gkString m_blend;
	gkString m_cfg;
	gkScene* m_scene;
	bool m_inited;
	gkBlendFile* m_blendFile;

	bool m_showPhysicsDebug; 

	bool setup();
protected:
	virtual void  tick (gkScalar rate);

public:
	okApp();
	virtual ~okApp();
	
	bool init(const gkString& blend, const gkString& cfg, const gkString& windowHandle, int winSizeX, int winSizeY);
	void uninit();

	bool load(const gkString& blend, const gkString& sceneName="");
	void unload();
	bool changeScene(const gkString& sceneName);

	bool createEmptyScene();

	bool step();

	gkScene* getActiveScene() { return m_scene; }
	gkString getActiveSceneName();
	gkBlendFile* getBlendFile() { return m_blendFile; }
	

	GK_INLINE bool getShowPhysicsDebug() { return m_showPhysicsDebug; }
	void setShowPhysicsDebug(bool show);
	
	GK_INLINE void setBlendFileName(const gkString& blend) { m_blend = blend; }
	GK_INLINE gkString getBlendFileName() const { return m_blend; }
};

#endif //_okApp_h_