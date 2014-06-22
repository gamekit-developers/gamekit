/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 zcube(JiSeop Moon).

    Contributor(s): harkon.kr.
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
//#include <jni.h>
#include <stdlib.h>
#include "AndroidLogListener.h"
//#include <android/log.h>

#include "OgreKit.h"
#include "Ogre.h"
#include "android/AndroidInputManager.h"

#define LOG_TAG    "ogrekit"
#define LOGI(...)  printf(__VA_ARGS__)//__android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGE(...)  printf(__VA_ARGS__)//__android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define LOG_FOOT   printf("%s %d\n", __FUNCTION__, __LINE__)

typedef void JNIEnv;
typedef void JavaVM;
typedef bool jboolean;
typedef void* jobject;
typedef float jfloat;
typedef int jint;
typedef char* jstring;
#define JNI_FALSE false
#define JNI_TRUE true


const gkString gkDefaultBlend   = "momo_ogre_i.blend";
const gkString gkDefaultConfig  = "/sdcard/OgreKitStartup.cfg";


class OgreKit : public gkCoreApplication
{
public:
	gkString    m_blend;
	gkScene*    m_scene;
	OIS::AndroidInputManager* m_input;

public:
	OgreKit();
	virtual ~OgreKit() {}

	bool init(const gkString& blend);

	void keyReleased(const gkKeyboard& key, const gkScanCode& sc);
	void injectKey(int action, int uniChar, int keyCode) {}// if (m_input) m_input->injectKey(action, uniChar, keyCode); }
	void injectTouch(int action, float x, float y) {} // if (m_input) m_input->injectTouch(action, x, y); }
	void setOffsets(int x, int y) {} // if (m_input) m_input->setOffsets(x,y); }
	void setWindowSize(int w, int h) {} // if (m_input) m_input->setWindowSize(w,h); }
private:

	bool setup(void);
};



OgreKit::OgreKit()
	:   m_blend(gkDefaultBlend), m_scene(0), m_input(0)
{
}

bool OgreKit::init(const gkString& blend)
{
    LOG_FOOT;

	gkString cfgfname;

	// Parse command line
	m_blend = gkDefaultBlend;
	if (!blend.empty()) m_blend = blend;

	m_prefs.winsize.x        = 800;
	m_prefs.winsize.y        = 480;
	m_prefs.wintitle         = gkString("OgreKit Demo (Press Escape to exit)[") + m_blend + gkString("]");
	m_prefs.shaderCachePath = ".";

	gkPath path = cfgfname;

    LOG_FOOT;

	// overide settings if found
	if (path.isFileInBundle())
		m_prefs.load(path.getPath());

    LOG_FOOT;

	bool ok = initialize();

	LOG_FOOT;

	return ok;
}


bool OgreKit::setup(void)
{
    LOG_FOOT;

	gkBlendFile* blend = gkBlendLoader::getSingleton().loadFile(gkUtils::getFile(m_blend), gkBlendLoader::LO_ALL_SCENES);
	if (!blend)
	{
		LOGI("File loading failed.\n");
		return false;
	}

    LOG_FOOT;

	m_scene = blend->getMainScene();
	if (!m_scene)
	{
		LOGI("No usable scenes found in blend.\n");
		return false;
	}

    LOG_FOOT;

	m_scene->createInstance();

    LOG_FOOT;

	// add input hooks
	gkWindow* win = gkWindowSystem::getSingleton().getMainWindow();
	m_input = static_cast<OIS::AndroidInputManager*>(win->getInputManager());
	LOG_FOOT;


	return true;
}


OgreKit okit;
AndroidLogListener gLogListener;


jboolean init(JNIEnv* env, jobject thiz, jstring arg)
{	
	LOG_FOOT;
	gkLogger::enable("AndroidDemo.log", true);
	Ogre::LogManager::getSingleton().getDefaultLog()->addListener(&gLogListener);

	gkString file = gkDefaultBlend;
#if 0
	const char* str = env->GetStringUTFChars(arg, 0);
	if (str) 
	{
		file = str;
		env->ReleaseStringUTFChars(arg, str);	
	}
#endif

	LOGI("****** %s ******", file.c_str());


	gkPrintf("-----------******-------------------");
	Ogre::LogManager::getSingleton().stream() << "ffffffffffffffff";
 

	LOG_FOOT;
	
	okit.getPrefs().verbose = true;
	if (!okit.init(file))
	{
		LOG_FOOT;
		// error
		return JNI_FALSE;
	}

	LOG_FOOT;

	gkEngine::getSingleton().initializeStepLoop();


	LOG_FOOT;



	return JNI_TRUE;
}

jboolean render(JNIEnv* env, jobject thiz, jint drawWidth, jint drawHeight, jboolean forceRedraw)
{
	//LOG_FOOT;

	static bool first = true;
	if (first)
	{
		first = false;
		okit.setWindowSize(drawWidth, drawHeight);
	}
	
	if (gkEngine::getSingleton().stepOneFrame())
		return JNI_TRUE;
	else
		return JNI_FALSE;
}

void cleanup(JNIEnv* env)
{
	LOG_FOOT;

	gkEngine::getSingleton().finalizeStepLoop();
}

jboolean inputEvent(JNIEnv* env, jobject thiz, jint action, jfloat mx, jfloat my)
{
	LOG_FOOT;

	okit.injectTouch(action, mx, my);

	return JNI_TRUE;
}

jboolean keyEvent(JNIEnv* env, jobject thiz, jint action, jint unicodeChar, jint keyCode, jobject keyEvent)
{
	LOG_FOOT;

	okit.injectKey(action, unicodeChar, keyCode);

	return JNI_TRUE;  
}

void setOffsets(JNIEnv* env, jobject thiz, jint x, jint y)
{
	LOGI("%s %d %d", __FUNCTION__, x, y);

	okit.setOffsets(x,y);
}

#if 0
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

#else
void main()
{
	init(0,0,0);
	while(render(0,0,0,0,0))
	{		
	}
	cleanup(0);
}

#endif