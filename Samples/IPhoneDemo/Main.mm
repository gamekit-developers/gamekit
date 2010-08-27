/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 harkon.kr.

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

#import <UIKit/UIKit.h>
#include "OgreKit.h"



#define USE_CADISPLAYLINK 1 //iOS version of 3.1 or greater is required

const gkString gkDefaultBlend   = "momo_ogre_i.blend";
const gkString gkDefaultConfig  = "OgreKitStartup.cfg";



// ----------------------------------------------------------------------------
class OgreKit : public gkCoreApplication, public gkWindowSystem::Listener
{
public:
    gkString    m_blend;
    gkScene    *m_scene;
	
public:
    OgreKit();
    virtual ~OgreKit() {}

    bool init();

    bool setup(void);
    void keyReleased(const gkKeyboard &key, const gkScanCode &sc);
	
	
	bool initializeStepLoop(void)
	{
		if (!initialize())
			return false;
		
		if (!m_engine->initializeStepLoop())
			return false;
		
		return true;
	}
	bool stepOneFrame(void)
	{
		return m_engine->stepOneFrame();
	}
	void finalizeStepLoop(void)
	{
		m_engine->finalizeStepLoop();
	}
	
};


// ----------------------------------------------------------------------------
OgreKit::OgreKit()
    :   m_blend(gkDefaultBlend), m_scene(0)
{
}


// ----------------------------------------------------------------------------
bool OgreKit::init()
{
    gkString cfgfname;

	m_blend = gkDefaultBlend;
	cfgfname = gkDefaultConfig;

	m_prefs.winsize.x        = 480;
    m_prefs.winsize.y        = 320;
	
    m_prefs.wintitle         = gkString("OgreKit Demo (Press Escape to exit)[") + m_blend + gkString("]");

    gkPath path = cfgfname;

	// overide settings if found
    if (path.isFileInBundle()) 
        m_prefs.load(path.getPath());

	return true;
}



// ----------------------------------------------------------------------------
bool OgreKit::setup(void)
{
    //gkBlendFile *blend = m_engine->loadBlendFile(gkUtils::getFile(m_blend));
	gkBlendFile *blend = gkBlendLoader::getSingleton().loadFile(gkUtils::getFile(m_blend), gkBlendLoader::LO_ALL_SCENES);

    if (!blend)
    {
        gkPrintf("File loading failed. %s\n", gkUtils::getFile(m_blend).c_str());
        return false;
    }

	m_scene = blend->getMainScene();
	if (!m_scene)
	{
		gkPrintf("No usable scenes found in blend.\n");
		return false;
	}
	
	m_scene->createInstance();
	

    // add input hooks
    gkWindowSystem::getSingleton().addListener(this);
    return true;
}


// ----------------------------------------------------------------------------
void OgreKit::keyReleased(const gkKeyboard &key, const gkScanCode &sc)
{
    if (sc == KC_ESCKEY) 
        m_engine->requestExit();
}


int main(int argc, char **argv)
{
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	int retVal = UIApplicationMain(argc, argv, @"UIApplication", @"AppDelegate");
	[pool release];
	return retVal;
}

//copy from ogre3d samplebrowser
@interface AppDelegate : NSObject <UIApplicationDelegate>
{
    NSTimer *mTimer;
    OgreKit m_okit;

    // Use of the CADisplayLink class is the preferred method for controlling your animation timing.
    // CADisplayLink will link to the main display and fire every vsync when added to a given run-loop.
    // The NSTimer class is used only as fallback when running on a pre 3.1 device where CADisplayLink
    // isn't available.
    id m_displayLink;
    NSDate* m_date;
    NSTimeInterval m_lastFrameTime;
    bool m_displayLinkSupported;

}

- (void)go;

@property (retain) NSTimer *m_timer;
@property (nonatomic) NSTimeInterval m_lastFrameTime;

@end

@implementation AppDelegate


@synthesize m_timer;
@dynamic m_lastFrameTime;

- (NSTimeInterval)mLastFrameTime
{
    return m_lastFrameTime;
}

- (void)setLastFrameTime:(NSTimeInterval)frameInterval
{
    // Frame interval defines how many display frames must pass between each time the
    // display link fires. The display link will only fire 30 times a second when the
    // frame internal is two on a display that refreshes 60 times a second. The default
    // frame interval setting of one will fire 60 times a second when the display refreshes
    // at 60 times a second. A frame interval setting of less than one results in undefined
    // behavior.
    if (frameInterval >= 1)
    {
        m_lastFrameTime = frameInterval;
    }
}

- (void)go 
{
	
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
	
    try {
		m_okit.initializeStepLoop();
    } catch( Ogre::Exception& e ) {
        gkPrintf("An exception has occurred: %s", e.getFullDescription().c_str());
	}
		
	if (m_displayLinkSupported)
	{
		// CADisplayLink is API new to iPhone SDK 3.1. Compiling against earlier versions will result in a warning, but can be dismissed
		// if the system version runtime check for CADisplayLink exists in -initWithCoder:. The runtime check ensures this code will
		// not be called in system versions earlier than 3.1.
		m_date = [[NSDate alloc] init];
		m_lastFrameTime = -[m_date timeIntervalSinceNow];
		
		m_displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(stepOneFrame:)];
		[m_displayLink setFrameInterval:m_lastFrameTime];
		[m_displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
	}
	else
	{
		m_timer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)(1.0f / 60.0f) * m_lastFrameTime
													target:self
													selector:@selector(stepOneFrame:)
													userInfo:nil
													repeats:YES];
	}
	
	[pool release];
}

		
- (void)applicationDidFinishLaunching:(UIApplication *)application 
{
	// Hide the status bar
	[[UIApplication sharedApplication] setStatusBarHidden:YES];
	
	m_displayLinkSupported = false;
	m_lastFrameTime = 1;
	m_displayLink = nil;
	m_timer = nil;
	
	m_okit.init();
	
	// A system version of 3.1 or greater is required to use CADisplayLink. The NSTimer
	// class is used as fallback when it isn't available.
#if USE_CADISPLAYLINK
	NSString *reqSysVer = @"3.1";
	NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
	if ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending)
		m_displayLinkSupported = TRUE;
#endif
	
	[self go];
}

- (void)stepOneFrame:(id)sender
{
	if (m_displayLinkSupported)
	{
		// NSTimerInterval is a simple typedef for double
		NSTimeInterval currentFrameTime = -[m_date timeIntervalSinceNow];
		NSTimeInterval differenceInSeconds = currentFrameTime - m_lastFrameTime;
		m_lastFrameTime = currentFrameTime;
		
		m_okit.stepOneFrame();
	}
	else
	{	
		float t = (float)[m_timer timeInterval];
		m_okit.stepOneFrame();
	}
}

- (void)applicationWillTerminate:(UIApplication *)application 
{
	m_okit.finalizeStepLoop();
}

- (void)dealloc 
{
	if (m_displayLinkSupported)
	{
		[m_date release];
		m_date = nil;
		
		[m_displayLink invalidate];
		m_displayLink = nil;
	}
	else
	{
		[m_timer invalidate];
		m_timer = nil;
	}
	
	[super dealloc];
}
		

@end

