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




#include "OgreKit.h"
#include "Ogre.h"
#include "android/AndroidInputManager.h"
#include <stdlib.h>
#include <pthread.h>



//extern static void onJavaDetach(void* arg);
#include <jni.h>
#include <stdlib.h>
//#include "AndroidLogListener.h"
//#include "OgreGLES2Plugin.h"
#include <android/sensor.h>

#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/api-level.h>
#include <android/native_window_jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <EGL/egl.h>
#include "Android/OgreAndroidEGLWindow.h"
#include "Android/OgreAPKFileSystemArchive.h"
#include "Android/OgreAPKZipArchive.h"

#define LOG_TAG    "OgreKit"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#undef LOG_FOOT
#define LOG_FOOT   LOGI("%s %s %d", __FILE__, __FUNCTION__, __LINE__)

#define USE_APK_ARCHIVE 1

//const gkString gkDefaultBlend   = "/sdcard/gamekit/gk_android.blend";
//const gkString gkDefaultBlend   = "gk_android.blend";
// #if USE_APK_ARCHIVE
const gkString gkDefaultBlend   = "momo_ogre_i.blend";
// #else
// const gkString gkDefaultBlend   = "/sdcard/gamekit/momo_ogre_i.blend";
// #endif
//const gkString gkDefaultBlend   = "multitouch_test.blend";
const gkString gkDefaultConfig  = ""; //"/sdcard/OgreKitStartup.cfg";


class OgreKit : public gkCoreApplication, public gkMessageManager::GenericMessageListener
{
public:
	gkString    m_blend;
	gkScene*    m_scene;
	OIS::AndroidInputManager* m_input;
	bool m_inited;

	android_app* state;
    ASensorManager* sensorManager;
    const ASensor* accelerometerSensor;
    ASensorEventQueue* sensorEventQueue;
	AAssetManager* assetMgr;

	float accel_threshold;
	float input_frame;
	float inputFrameTimer;
	
	float gravity[3];
	float linear_acceleration[3];
	

public:
	OgreKit() 
		: m_blend(gkDefaultBlend)
		, m_scene(0)
		, m_input(0) 
		, m_inited(false)

		, state(0)
		, assetMgr(0)
		, sensorManager(0)
		, accelerometerSensor(0)
		, sensorEventQueue(0)
		, accel_threshold(0.5f)
		, input_frame(0.05f)		
	{ 
		inputFrameTimer = input_frame;
		for (int i = 0; i < 3; i++)
			linear_acceleration[i] = gravity[i] = 0;			
	}
	virtual ~OgreKit()   {}

	bool init(const gkString& blend);
	bool isInited() { return m_inited; }

	void keyReleased(const gkKeyboard& key, const gkScanCode& sc);

	void injectKey(int action, int uniChar, int keyCode) { if (m_input) m_input->injectKey(action, uniChar, keyCode); }
	void injectTouch(int action, float x, float y) { if (m_input) m_input->injectTouch(action, x, y); }
	void injectAcceleration(float x,float y, float z) { if (m_input) m_input->injectAcceleration(x,y,z);}
	void setOffsets(int x, int y) { if (m_input) m_input->setOffsets(x,y); }
	void setWindowSize(int w, int h) { if (m_input) m_input->setWindowSize(w,h); }
	void handleMessage(gkMessageManager::Message* message);
	OIS::AndroidInputManager* getInput() { return m_input; }

	float* linearAccel(float x,float y,float z) 
	{
        const float alpha = 0.8f;

        gravity[0] = alpha * gravity[0] + (1 - alpha) * x;
        gravity[1] = alpha * gravity[1] + (1 - alpha) * y;
        gravity[2] = alpha * gravity[2] + (1 - alpha) * z;
        
        linear_acceleration[0] = x - gravity[0];
        linear_acceleration[1] = y - gravity[1];
        linear_acceleration[2] = z - gravity[2];

        return linear_acceleration;
	}

private:
	bool setup(void);
	jmethodID mFireString;	

};



void OgreKit::handleMessage(gkMessageManager::Message* message)
{
#if 0
	LOGI("HANDLE MSG %s ",message->m_subject.c_str());

	//    jstring from = this->JNU_NewStringNative(env,message->m_subject.c_str());
	jstring from = env->NewStringUTF(message->m_from.c_str());
	jstring to = env->NewStringUTF(message->m_to.c_str());
	jstring subject = env->NewStringUTF(message->m_subject.c_str());
	jstring body = env->NewStringUTF(message->m_body.c_str());
#endif
}


bool OgreKit::init(const gkString& blend)
{
	gkPrintf("----------- OgreKit Android Demo init -----------------");
	LOG_FOOT;

	gkString cfgfname;	
	// Parse command line
	m_blend = gkDefaultBlend;
	if (!blend.empty()) m_blend = blend;

	getPrefs().debugFps = true;
	getPrefs().wintitle = gkString("OgreKit Demo (Press Escape to exit)[") + m_blend + gkString("]");
	getPrefs().blendermat=true;
	//getPrefs().shaderCachePath="/sdcard/gamekit";
	getPrefs().enableshadows=false;
	getPrefs().viewportOrientation="portrait";
	//	m_prefs.disableSound=false;
	gkPath path = cfgfname;	

	LOG_FOOT;

	// overide settings if found
	if (path.isFileInBundle())
		getPrefs().load(path.getPath());

	LOG_FOOT;

	m_inited = initialize();
	LOG_FOOT;
	//gkMessageManager::getSingleton().addListener(this);
	return m_inited;
}


bool OgreKit::setup(void)
{
	LOG_FOOT;
	  
	if (assetMgr)
	{
#if USE_APK_ARCHIVE
		Ogre::ArchiveManager::getSingleton().addArchiveFactory(new Ogre::APKFileSystemArchiveFactory(assetMgr));
		Ogre::ArchiveManager::getSingleton().addArchiveFactory(new Ogre::APKZipArchiveFactory(assetMgr));

		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("/", "APKFileSystem",  Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
#endif
	}

#if USE_APK_ARCHIVE
	Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(m_blend, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	if (stream.isNull())
	{
		gkPrintf("Error: Can't open file %s.", m_blend.c_str());
		return false;
	}

	size_t size = stream->size();
	char* buf = new char[size];
	stream->read(buf, size);

	gkBlendFile* blend = gkBlendLoader::getSingleton().loadFromMemory(buf,size,gkBlendLoader::LO_ALL_SCENES);	
#else
	gkBlendFile* blend = gkBlendLoader::getSingleton().loadFile(m_blend,gkBlendLoader::LO_ALL_SCENES); //, "", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);	
#endif
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
	
	m_scene->getMainCamera()->getCamera()->setAutoAspectRatio(true);

	// add input hooks
	gkWindow* win = gkWindowSystem::getSingleton().getMainWindow();
	m_input = static_cast<OIS::AndroidInputManager*>(win->getInputManager());

	LOG_FOOT;


	return true;
}




OgreKit okit;


#if 0



//JavaVM* vmPointer;

void cleanup(JNIEnv* env)
{
	LOG_FOOT;

	gkEngine::getSingleton().finalizeStepLoop();
}

jboolean inputEvent(JNIEnv* env, jobject thiz, jint action, jfloat mx, jfloat my)
{
	//	LOG_FOOT;

	okit.injectTouch(action, mx, my);

	return JNI_TRUE;
}

jboolean keyEvent(JNIEnv* env, jobject thiz, jint action, jint unicodeChar, jint keyCode, jobject keyEvent)
{
	//	LOG_FOOT;

	okit.injectKey(action, unicodeChar, keyCode);

	return JNI_TRUE;
}

void setOffsets(JNIEnv* env, jobject thiz, jint x, jint y)
{
	//	LOGI("%s %d %d", __FUNCTION__, x, y);

	okit.setOffsets(x,y);
}

void sendSensor(JNIEnv *env, jclass thiz, jint type, jfloat x, jfloat y, jfloat z)
{
	//	LOGI("%d %f %f %f", __FUNCTION__, type,x, y,z);
	okit.injectAcceleration(x,y,z);
}

/*
* Class:     org_gamekit_jni_GameKitJNI
* Method:    sendMessage
* Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V
*/
void sendMessage(JNIEnv *env, jclass thiz, jstring jfrom, jstring jto, jstring jsubject, jstring jbody)
{
	const char* str = env->GetStringUTFChars(jfrom, 0);
	gkString from = str;
	env->ReleaseStringUTFChars(jfrom,str);

	const char* str2 = env->GetStringUTFChars(jto, 0);
	gkString to = str2;
	env->ReleaseStringUTFChars(jto,str2);

	const char* str3 = env->GetStringUTFChars(jsubject, 0);
	gkString subject = str3;
	env->ReleaseStringUTFChars(jsubject,str3);

	const char* str4 = env->GetStringUTFChars(jbody, 0);
	gkString body = str4;
	env->ReleaseStringUTFChars(jbody,str4);

	if (gkMessageManager::getSingletonPtr())
	{
		gkMessageManager::getSingletonPtr()->sendMessage(from,to,subject,body);
	}
	//	LOGI("%s %s %s %s", from.c_str(),to.c_str(),subject.c_str(),body.c_str());
}

#endif

class OgreKitAndroidGlue
{
public: 
	static void init(struct android_app* state)
	{
		okit.state = state;
		//   gkLogger::write("steponeframe");
	    state->onAppCmd = &handleCmd;
	    state->onInputEvent = &handleInput;
	}

	static void shutdown()
	{
	}

	static int32_t handleInput(struct android_app* app, AInputEvent* event) 
	{
		if (okit.getInput())
		{
	        if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) 
            {
                int action = (int)(AMOTION_EVENT_ACTION_MASK & AMotionEvent_getAction(event));
                    
				
                if (action == 0)
					okit.injectTouch(2, AMotionEvent_getRawX(event, 0), AMotionEvent_getRawY(event, 0));
                    
                okit.injectTouch(action, AMotionEvent_getRawX(event, 0), AMotionEvent_getRawY(event, 0));
            }
            else 
            {
				int action = AKeyEvent_getAction(event);
				int unicodeChar = 0;
				okit.injectKey(action, unicodeChar, AKeyEvent_getKeyCode(event));
                //mInputInjector->injectKeyEvent(AKeyEvent_getAction(event), AKeyEvent_getKeyCode(event));
            }

            return 1;
        }
        return 0;
	}

	static void handleCmd(struct android_app* app, int32_t cmd)
	{
		switch (cmd) 
		{
		case APP_CMD_SAVE_STATE:
			break;

		case APP_CMD_INIT_WINDOW:
			if (app->window) // && mRoot)
			{
				AConfiguration* config = AConfiguration_new();
				AConfiguration_fromAssetManager(config, app->activity->assetManager);

				if (!okit.isInited())
				{					
					LOG_FOOT;

					//okit.getPrefs().winsize.x = drawWidth;
					//okit.getPrefs().winsize.y = drawHeight;
					
					okit.assetMgr = app->activity->assetManager;

					okit.getPrefs().extWinhandle  = Ogre::StringConverter::toString((int)app->window);
					okit.getPrefs().androidConfig = Ogre::StringConverter::toString((int)config);
					okit.getPrefs().verbose = true;

					//gkLogger::enable("OgreKitDemo.log", true);
					//Ogre::LogManager::getSingleton().getDefaultLog()->addListener(&gLogListener);

					
					if (okit.init(""))
					{
						LOG_FOOT;
						m_window = gkWindowSystem::getSingleton().getMainWindow();
						LOG_FOOT;
						okit.m_input = static_cast<OIS::AndroidInputManager*>(m_window->getInputManager());

						okit.sensorManager = ASensorManager_getInstance();
						okit.accelerometerSensor = ASensorManager_getDefaultSensor(okit.sensorManager,
								ASENSOR_TYPE_ACCELEROMETER);
						okit.sensorEventQueue = ASensorManager_createEventQueue(okit.sensorManager,
								okit.state->looper, LOOPER_ID_USER, NULL, NULL);

						gkEngine::getSingleton().initializeStepLoop();
						
						LOGI("inited");

						//okit.setWindowSize(drawWidth, drawHeight);			
					}
				}
				else
				{
					LOG_FOOT;
					Ogre::RenderWindow* rwnd = gkWindowSystem::getSingleton().getMainRenderWindow();
					if (rwnd)
						static_cast<Ogre::AndroidEGLWindow*>(rwnd)->_createInternalResources(app->window, config);
				}

				LOG_FOOT;

				AConfiguration_delete(config);
			}				
			break;

		case APP_CMD_TERM_WINDOW:
			if (okit.isInited())
			{
				Ogre::RenderWindow* rwnd = gkWindowSystem::getSingleton().getMainRenderWindow();
				if (rwnd)
					static_cast<Ogre::AndroidEGLWindow*>(rwnd)->_destroyInternalResources();
			}
			LOGI("terminiate.");
			break;

		case APP_CMD_GAINED_FOCUS:
			// When our app gains focus, we start monitoring the accelerometer.
            if (okit.accelerometerSensor != NULL) 
			{
                ASensorEventQueue_enableSensor(okit.sensorEventQueue,
                        okit.accelerometerSensor);
                // We'd like to get 60 events per second (in us).
                ASensorEventQueue_setEventRate(okit.sensorEventQueue,
                        okit.accelerometerSensor, (1000L/60)*1000);
            }
			break;

		case APP_CMD_LOST_FOCUS:
			// When our app loses focus, we stop monitoring the accelerometer.
            // This is to avoid consuming battery while not being used.
            if (okit.accelerometerSensor != NULL) 
			{
                ASensorEventQueue_disableSensor(okit.sensorEventQueue,
                        okit.accelerometerSensor);
            }
			break;

		case APP_CMD_CONFIG_CHANGED:
			break;
		}
	}

	
	static void go(struct android_app* state)
	{

		int ident, events;
        struct android_poll_source* source;
            
        while (true)
        {
            while ((ident = ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0)
            {
                if (source != NULL)
                    source->process(state, source);

//
				// If a sensor has data, process it now.
				if (ident == LOOPER_ID_USER) 
				{
					if (okit.accelerometerSensor != NULL) 
					{
						ASensorEvent event;
						while (ASensorEventQueue_getEvents(okit.sensorEventQueue, &event, 1) > 0) 
						{
							//LOGI("accelerometer: x=%f y=%f z=%f",event.acceleration.x, event.acceleration.y,event.acceleration.z);

							okit.injectAcceleration(event.acceleration.x, event.acceleration.y, event.acceleration.z);
						}
					}
				}
                    
                if (state->destroyRequested != 0)
				{
					LOGI("exit........");
                    return;
				}
            }	

            if(m_window != NULL && m_window->isActive())
            {
				// Ogre::RenderWindow* rwin = m_window->getRenderWindow();
				// Ogre::Viewport* vp = m_window->getViewport(0)->getViewport();
				// Ogre::Camera* camera = vp->getCamera();
                m_window->getRenderWindow()->windowMovedOrResized();
				//camera->setAspectRatio((float)vp->getActualWidth() / (float)vp->getActualHeight());

                gkEngine::getSingleton().stepOneFrame();
            }
        }		
	}

	static gkWindow* m_window;
};

gkWindow* OgreKitAndroidGlue::m_window = NULL;



void android_main(android_app* state) 
{
	app_dummy();

	OgreKitAndroidGlue::init(state);
	OgreKitAndroidGlue::go(state);
}
