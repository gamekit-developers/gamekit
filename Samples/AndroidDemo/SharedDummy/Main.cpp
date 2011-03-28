/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 zcube(JiSeop Moon) & harkon.kr.

    Contributor(s): 
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
#include <jni.h>
#include <stdlib.h>
//#include "AndroidLogListener.h"
#include <android/log.h>
#include <string>
#include <assert.h>

#include "OgreKit.h"
#include "OIS.h"
#include "android/AndroidInputManager.h"

//#include "OgreKit.h"

//#include <Ogre.h>
//#include "OgreRTShaderSystem.h"

#define  LOG_TAG    "ogrekit"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOG_FOOT   LOGI("%s %d", __FUNCTION__, __LINE__)

typedef std::string gkString;

const gkString gkDefaultBlend   = "/sdcard/momo_ogre_i.blend";



#if 0
class OgreKit // : public gkCoreApplication
{
public:
	gkString    m_blend;
	gkScene*    m_scene;
public:
	OgreKit();
	virtual ~OgreKit() {}

	int init();

	//void keyReleased(const gkKeyboard& key, const gkScanCode& sc);
private:

	bool setup(void);
};



OgreKit::OgreKit()
	:   m_blend(gkDefaultBlend), m_scene(0)
{
}

int OgreKit::init(void)
{
	gkString cfgfname;

	// Parse command line
	m_blend = gkDefaultBlend;	

	m_prefs.winsize.x        = 800;
	m_prefs.winsize.y        = 480;
	m_prefs.wintitle         = gkString("OgreKit Demo (Press Escape to exit)[") + m_blend + gkString("]");

	gkPath path = cfgfname;

	// overide settings if found
	if (path.isFileInBundle())
		m_prefs.load(path.getPath());

	return 0;
}


bool OgreKit::setup(void)
{
	gkBlendFile* blend = gkBlendLoader::getSingleton().loadFile(gkUtils::getFile(m_blend), gkBlendLoader::LO_ALL_SCENES);
	if (!blend)
	{
		LOGI("File loading failed.\n");
		return false;
	}

	m_scene = blend->getMainScene();
	if (!m_scene)
	{
		LOGI("No usable scenes found in blend.\n");
		return false;
	}


	m_scene->createInstance();

	// add input hooks
	//gkWindowSystem::getSingleton().addListener(this);
	return true;
}


OgreKit okit;
#endif

//AndroidLogListener *g_ll = 0;
//Ogre::Log* gLog;


class EventListener : 
	public OIS::MultiTouchListener,
	public OIS::KeyListener
{
public:
	OIS::AndroidInputManager* m_input;
	OIS::Keyboard* m_ikeyboard;
	OIS::MultiTouch* m_itouch;
	

	EventListener() : m_input(0), m_ikeyboard(0), m_itouch(0)
	{
		init();
	}
	~EventListener()
	{
		m_input->destroyInputObject(m_ikeyboard);
		m_input->destroyInputObject(m_itouch);
		OIS::InputManager::destroyInputSystem(m_input);
	}
	
	void init()
	{
		LOGI("try init");
		

		try {
			//params.insert(std::make_pair("WINDOW", Ogre::StringConverter::toString(handle)));
			m_input = static_cast<OIS::AndroidInputManager*>(OIS::InputManager::createInputSystem(0));
			LOGI("try init2 %s", m_input->inputSystemName().c_str());
			m_input->enableAddOnFactory(OIS::InputManager::AddOn_All);

			LOGI("try init3");
#if 1
			m_ikeyboard = (OIS::Keyboard*)m_input->createInputObject(OIS::OISKeyboard, true);  
			if (m_ikeyboard) m_ikeyboard->setEventCallback(this);
			else LOGI("no keyboard");
#endif		
			LOGI("try init4");

			m_itouch = (OIS::MultiTouch*)m_input->createInputObject(OIS::OISMultiTouch, true);
			if (m_itouch) m_itouch->setEventCallback(this); 
			else LOGI("no touch");
		
		} catch (std::exception&e) {
			LOGI("OIS EXCEPTION:");
			LOGI("OIS EXCEPTION: %s", e.what());
		}
	}

	bool keyPressed(const OIS::KeyEvent& evt) 
	{
		LOGI("KeyDown: %d %c", evt.key, evt.text);
		return true;
	}
	bool keyReleased(const OIS::KeyEvent& evt) 
	{
		LOGI("KeyUp: %d %c", evt.key, evt.text);
		return true;
	}

	bool touchPressed(const OIS::MultiTouchEvent& arg)
	{
		return true;
	}
	bool touchReleased(const OIS::MultiTouchEvent& arg)
	{
		return true;
	}
	bool touchMoved(const OIS::MultiTouchEvent& arg)
	{
		LOGI("touchmove (%d %d) (%d %d)", arg.state.X.abs, arg.state.X.rel, arg.state.Y.abs, arg.state.Y.rel);
		return true;
	}
	bool touchCancelled(const OIS::MultiTouchEvent& arg)
	{		
		return true;
	}
} *gListener;

jboolean init(JNIEnv* env, jobject thiz, jstring arg)
{
	LOG_FOOT;
		
	
	gkString file = gkDefaultBlend;
	const char* str = env->GetStringUTFChars(arg, 0);
	if (str) 
	{
		file = str;
		env->ReleaseStringUTFChars(arg, str);	
	}

	LOGI("****** %s ******", file.c_str());

	gListener = new EventListener();

	LOG_FOOT;
	
//	okit.getPrefs().verbose = true;
	if (0) //okit.init() != 0)
	{
		LOG_FOOT;
		// error
		return JNI_FALSE;
	}

	//gkEngine::getSingleton().initialize();

	LOG_FOOT;

	//gkEngine::getSingleton().initializeStepLoop();


	LOG_FOOT;



	return JNI_TRUE;
}

jboolean render(JNIEnv* env, jobject thiz, jint drawWidth, jint drawHeight, jboolean forceRedraw)
{
	//LOG_FOOT;

	static bool first = true;
	if(first)
	{
		first = false;
		if (gListener && gListener->m_input)
			gListener->m_input->setWindowSize(drawWidth, drawHeight);
	}
	//gkEngine::getSingleton().stepOneFrame();
	return JNI_TRUE;
}

void cleanup(JNIEnv* env)
{
	LOG_FOOT;
	
	if (gListener)
	{
		delete gListener;
		gListener = 0;
	}

	//gkEngine::getSingleton().finalizeStepLoop();
}

jboolean inputEvent(JNIEnv* env, jobject thiz, jint action, jfloat mx, jfloat my)
{
	LOGI("%s %d %d (%f %f)", __FUNCTION__, __LINE__, action, mx, my);
	
	if (gListener && gListener->m_input)
		gListener->m_input->injectTouch(action, mx, my);

	return JNI_TRUE;
}

jboolean keyEvent(JNIEnv* env, jobject thiz, jint action, jint unicodeChar, jint keyCode, jobject keyEvent)
{
	LOGI("%s %d %d %d", __FUNCTION__, __LINE__, unicodeChar, keyCode);
	
	if (gListener && gListener->m_input)
		gListener->m_input->injectKey(action, unicodeChar, keyCode);

	return JNI_TRUE;  
}

void setOffsets(JNIEnv* env, jobject thiz, jint x, jint y)
{
	LOGI("%s %d %d", __FUNCTION__, x, y);
	if (gListener && gListener->m_input)
		gListener->m_input->setOffsets(x, y);
}

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv *env;

    LOGI("JNI_OnLoad called");
    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK)
    {
    	LOGE("Failed to get the environment using GetEnv()");
        return -1;
    }
    JNINativeMethod methods[] =
    {
		{
            "init",
            "(Ljava/lang/String;)Z",
            (void *) init
        },
        {
        	"render",
			"(IIZ)Z",
			(void *) render
        },
        {
			"inputEvent",
			"(IFFLandroid/view/MotionEvent;)Z",
			(void *) inputEvent

        },
        {
            "keyEvent",
            "(IIILandroid/view/KeyEvent;)Z",
            (void *) keyEvent
        },
        {
            "cleanup",
            "()V",
            (void *) cleanup
        },
		{
			"setOffsets",
			"(II)V",
			(void *) setOffsets
		},
    };
    jclass k;
    k = (env)->FindClass ("org/gamekit/jni/Main");
    (env)->RegisterNatives(k, methods, 6);

    return JNI_VERSION_1_4;
}