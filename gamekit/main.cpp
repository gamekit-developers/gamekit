/*
GameKit
Copyright (c) 2009 Erwin Coumans  http://gamekit.googlecode.com

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

///Note: this is based on Irrlicht Example 003 Custom SceneNode, so the copyright of the original parts belongs to Irrlicht

///you can disable custom node, to use irr::scene::SAnimatedMesh in Irrlicht 1.5
//#define USE_CUSTOM_NODE 1

///You can enable vertex buffer objects (VBO) for Irrlicht 1.5
#define USE_VBO 1

#include <irrlicht.h>
#include <iostream>
#include "IrrBlend.h"
#include "IrrBlendNew.h"
#include "btBulletDynamicsCommon.h"


///The TEST_ECHO_BLEND_READER define is to test reading .blend files using another 'readblend' implementation by Charlie C from 2006
///it has been written in C++ and is compatible with 32,64 bit .blend files, and both little and big endian

#define TEST_ECHO_BLEND_READER //use bParse (if disabled use readblend .blend parser)
//#define BATCH_UNIT_TESTING //run through an array of test .blend files


using namespace irr;
IrrlichtDevice* device=0;

bool gPause=false;
bool gWireframe = false;



class MyEventReceiver : public IEventReceiver
{
public:
	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event)
	{
		// Remember whether each key is down or up
		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
		{
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
			

			if (IsKeyDown(irr::KEY_KEY_Q))
			{
				exit(0);
			}
			if (IsKeyDown(irr::KEY_SPACE))
			{
				gPause = !gPause;
			}
			if (IsKeyDown(irr::KEY_KEY_W))
			{
				gWireframe = !gWireframe;
			}
		}

		return false;
	}

	// This is used to check whether a key is being held down
	virtual bool IsKeyDown(EKEY_CODE keyCode) const
	{
		return KeyIsDown[keyCode];
	}
	
	MyEventReceiver()
	{
		for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
			KeyIsDown[i] = false;
	}

private:
	// We use this array to store the current state of each key
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
};


#ifdef __APPLE__
#define MAXPATHLEN 512
char* AppleGetBundleDirectory(void) {
	CFURLRef bundleURL;
	CFStringRef pathStr;
	static char path[MAXPATHLEN];
	memset(path,MAXPATHLEN,0);
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	
	bundleURL = CFBundleCopyBundleURL(mainBundle);
	pathStr = CFURLCopyFileSystemPath(bundleURL, kCFURLPOSIXPathStyle);
	CFStringGetCString(pathStr, path, MAXPATHLEN, kCFStringEncodingASCII);
	CFRelease(pathStr);
	CFRelease(bundleURL);	
	return path;
}
#endif


char* fileNames[] = {
	"clubsilo_packed.blend", //blender 2.49b -> no conversion if using bParse
	"multimat.blend",//multi materials
	"light.blend",//lights
	"game_250_zipped.blend",//test zipped forward compatibility
	"momo_ogreSmallAnim.blend", //skeletal animation
	"cube_tex_4.blend", //64bit test
	"PhysicsAnimationBakingDemo.blend",//physics
	"BigEndian.blend",//big endian
	"cube_ipo_180.blend",//test an old Blender version
	""//terminator
};



int main(int argc,char** argv)
{

	int sz = sizeof(btCollisionObject);

	int numfiles = sizeof(fileNames);
	for (int i=0;strlen(fileNames[i]);i++)
	{
		const char* fileName = fileNames[i];
		
		if (argv[0])
			printf("argv[0]=%s\n",argv[0]);
		
		int verboseDumpAllTypes = false;//true;
		int verboseDumpAllBlocks = false;//false;//true;

		printf("Usage:\nGameKit [-verbose] [blendfile.blend]\n");

		if (argc>1 && argv[1])
		{
			if (!strcmp(argv[1],"-verbose"))
			{
				verboseDumpAllTypes = true;
				verboseDumpAllBlocks = true;
				printf("enable verbose output: verboseDumpAllTypes and verboseDumpAllBlocks\n");
			}
		}

	#if __APPLE__
		printf("Or double-click on the GameKit application. You can copy a file game.blend in the same directory as the GameKit application (not inside the bundle)\n");
	#endif
		
		FILE* file = fopen(fileName,"rb");
		if (argc>1 || !file)
		{
	#if __APPLE__

			char newName[1024];

			char* bundlePath = AppleGetBundleDirectory();
			//cut off the .app filename
			char* lastSlash=0;
			if( lastSlash = strrchr( bundlePath, '/' ) )
				*lastSlash = '\0';
			sprintf(newName,"%s/%s",bundlePath,"game.blend");
			file = fopen(newName,"rb");
		
			// how do you debug the Bundle execution, without a console?		
			FILE* dump = fopen ("/out.txt","wb");
			fwrite(newName,1,strlen(newName),dump);
			fclose(dump);
			
			if (!file)
			{
				sprintf(newName,"%s/%s/%s",AppleGetBundleDirectory(),"Contents/Resources",fileName);
				file=fopen(newName,"rb");
				if (!file)
					printf("cannot open file %s.\n",newName);
			}


			
	#else

			char newName[1024];
			newName[0] = 0;

			switch (argc)
			{
			case 2:
				{
					if (argv[1])
						fileName = argv[1];
					break;
				}
			case 3:
				{
					if (argv[2])
						fileName = argv[2];
				}

			default:
				{
				}
			}
			
			sprintf(newName,"%s",fileName);
			file = fopen(newName,"rb");
			//printf("cannot open file %s.\n",newName);

	#endif//__APPLE__
			if (!file)
			{
				printf("cannot open file %s.\n",argv[1]);
				exit(0);
			}
		}


		// let user select driver type

		video::E_DRIVER_TYPE driverType;
	#if 0
		printf("Please select the driver you want for this example:\n"\
			" (a) Direct3D 9.0c\n (b) Direct3D 8.1\n (c) OpenGL 1.5\n"\
			" (d) Software Renderer\n (e) Burning's Software Renderer\n"\
			" (f) NullDevice\n (otherKey) exit\n\n");

		char i;
		std::cin >> i;
		switch(i)
		{
			case 'a': driverType = video::EDT_DIRECT3D9;break;
			case 'b': driverType = video::EDT_DIRECT3D8;break;
			case 'c': driverType = video::EDT_OPENGL;   break;
			case 'd': driverType = video::EDT_SOFTWARE; break;
			case 'e': driverType = video::EDT_BURNINGSVIDEO;break;
			case 'f': driverType = video::EDT_NULL;     break;
			default: return 0;
		}
	#else
		//driverType = video::EDT_BURNINGSVIDEO;
		
		//driverType = video::EDT_DIRECT3D9;
		driverType = video::EDT_OPENGL;
		
	#endif
		// create device

		MyEventReceiver receiver;

		core::dimension2d<unsigned int>dim(640, 480);
		device = createDevice(driverType,		dim, 32, false,false,false,&receiver);
		//device = createDevice(driverType,		core::dimension2d<s32>(640, 480), 32, false,false,true,&receiver);
		//device = createDevice(driverType,		core::dimension2d<s32>(640, 480), 32, true,false,true,&receiver);
	//	device->setResizeAble(true);



		//device = createDevice(driverType,		core::dimension2d<s32>(1024, 768), 32, true);
			
		if (device == 0)
			return 1; // could not create selected driver.

		// create engine and camera

		device->setWindowCaption(L"Custom Scene Node - Irrlicht Engine Demo");


		video::IVideoDriver* driver = device->getVideoDriver();
		scene::ISceneManager* smgr = device->getSceneManager();

		irr::scene::ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS(0,20,0.01f,100);
		//irr::scene::ICameraSceneNode* cam = smgr->addCameraSceneNodeMaya(0, core::vector3df(0,-40,0), core::vector3df(0,0,0));
		//irr::scene::ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS(0,10,10);
		//irr::scene::ICameraSceneNode* cam = smgr->addCameraSceneNode(0, core::vector3df(0,50,30), core::vector3df(0,0,0));
		//cam->setUpVector(irr::core::vector3df(0,1,0));


		btCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
		btBroadphaseInterface* pairCache = new btDbvtBroadphase();
		btDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
		btConstraintSolver* constraintSolver = new btSequentialImpulseConstraintSolver();
		btDiscreteDynamicsWorld* physicsWorld = new btDiscreteDynamicsWorld(dispatcher,pairCache,constraintSolver,collisionConfiguration);

		btLogicManager* logicManager = new btLogicManager();

		
	//#ifdef SWAP_COORDINATE_SYSTEMS
		physicsWorld->setGravity(btVector3(0,0,-10));
	//#endif //SWAP_COORDINATE_SYSTEMS

	#ifdef TEST_ECHO_BLEND_READER

		int fileLen;
		char*memoryBuffer =  btReadBuffer(file,&fileLen);
		
		


		
	//	char* copyBuf = (char*) malloc (fileLen);
		
	//	for (int i=0;i<10;i++)
		{
	//		memcpy(copyBuf,memoryBuffer,fileLen);


			IrrBlendNew	bulletBlendReaderNew(device,smgr,physicsWorld,logicManager);
			if (!bulletBlendReaderNew.readFile(memoryBuffer,fileLen,verboseDumpAllTypes))
			{
				{
					bool ignoreCase=false;
					bool ignorePath=true;
					bool result = device->getFileSystem()->addFileArchive(fileName,ignoreCase,ignorePath);
					irr::io::IReadFile* zipfile = device->getFileSystem()->createAndOpenFile(fileName);
					if (zipfile)
					{
						char* uncompressedBuf = (char*)malloc (zipfile->getSize());
						int readbytes = zipfile->read(uncompressedBuf,zipfile->getSize());
						if (!bulletBlendReaderNew.readFile(uncompressedBuf,readbytes,verboseDumpAllTypes))
						{
							printf("Can't open compressed .blend file (%s), please submit an issue in http://gamekit.googlecode.com and attach .blend file\n",fileName);
							exit(0);
						}
					}
					else
					{
						printf("Can't open .blend file (%s) please submit an issue in http://gamekit.googlecode.com and attach .blend file\n",fileName);
						exit(0);
					}
			
				}
			}

			bulletBlendReaderNew.convertAllObjects(verboseDumpAllBlocks);

	//#define TEST_FILE_WRITING 1
	#if TEST_FILE_WRITING
			bulletBlendReaderNew.writeFile("test.blend");
			bulletBlendReaderNew.writeFile("test2.blend");



			FILE* fileTest = fopen("test2.blend","rb");
			int fileLenTest;
			char*memoryBufferTest =  btReadBuffer(fileTest,&fileLenTest);
			IrrBlendNew	bulletBlendReaderNewTest(device,smgr,physicsWorld,logicManager);
			if (!bulletBlendReaderNewTest.readFile(memoryBufferTest,fileLenTest,verboseDumpAllTypes))
			{
				printf("Failing reading back written test.blend\n");
			}
	#endif



			

		}

		

		
		fseek(file, 0, SEEK_SET); /* seek to start */


	#else

		IrrlichtBulletBlendReader	bulletBlendReader(device,smgr,physicsWorld,logicManager);
		if (!bulletBlendReader.readFile(file,verboseDumpAllTypes))
		{
			printf("cannot read Blender file %s.\n",argv[1]);
			fclose(file);
			exit(0);
		} else
		{
			bulletBlendReader.convertAllObjects(verboseDumpAllBlocks);
		}
	#endif //TEST_ECHO_BLEND_READER

		
		// add ambient lighting when converted scene contains zero lights
		if(smgr->getSceneNodeFromType(irr::scene::ESNT_LIGHT) == NULL)
		{
			scene::ILightSceneNode* light1 =
				smgr->addLightSceneNode(0, core::vector3df(0,0,0),
				video::SColorf(1.0f, 1.0f, 1.0f, 0.0f), 45.f);
			
			light1->setLightType(irr::video::ELT_DIRECTIONAL);
			light1->setRotation(irr::core::vector3df(90.0f, 0.0f, 0.0f));
			smgr->setAmbientLight(video::SColor(0,20,20,20));
		}
		
		/*
		Now draw everything and finish.
		*/
		u32 frames=0;
		
		//comment-out next line for batch testing

#ifndef BATCH_UNIT_TESTING
		while(device->run())
#endif //BATCH_UNIT_TESTING
		{

				static int ms = device->getTimer()->getTime();
			int newTime = device->getTimer()->getTime();
			int deltaTimeMs = newTime-ms;
			ms = newTime;
			btScalar deltaTime = deltaTimeMs*0.001f;

			
			if (!gPause)
				physicsWorld->stepSimulation(deltaTime);

			
			driver->beginScene(true, true, video::SColor(0,100,100,100));

			logicManager->processLogicBricks(deltaTime);

			smgr->drawAll();

			driver->endScene();
			if (++frames==100)
			{
				core::stringw str = L"GameKit http://gamekit.googlecode.com (Bullet+Irrlicht) [";
				str += driver->getName();
				str += L"] FPS: ";
				str += (s32)driver->getFPS();
				str += L" (press q to quit)";

				device->setWindowCaption(str.c_str());
				frames=0;
			}
			

		}

		
		device->drop();
	}
	return 0;
}

