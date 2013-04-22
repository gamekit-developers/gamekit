/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Charlie C.

    Contributor(s): Kai-Ting (Danil) Ko
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

#include "gkCommon.h"
#include "gkNetworkManager.h"
#include "gkNetworkInstance.h"
#include "gkEngine.h"
#include "gkUserDefs.h"
#include "gkDebugger.h"
#include "gkLogger.h"

gkNetworkManager::gkNetworkManager()
{
	mInstance = NULL;

	initialize();
}  // gkNetworkManager::gkNetworkManager

gkNetworkManager::~gkNetworkManager()
{
	deinitialize();
}  // gkNetworkManager::~gkNetworkManager

void gkNetworkManager::initialize(void)
{
	if (enet_initialize() != 0)
    {
		gkLogger::write("An error occurred while initializing ENet.\n");
    }  // if
}  // bool gkNetworkManager::initialize

void gkNetworkManager::deinitialize(void)
{
	if(mInstance != NULL)
	{
		delete mInstance;
	}  // if

	mInstance = NULL;

	enet_deinitialize();
}  // void gkNetworkManager::initialize

void gkNetworkManager::createNetworkInstance(const int & pType, const gkString & pName, const gkString & pAddress, const int & pPort)
{
	// If there is already an instance, a new one cannot be created until the existing one is removed
	if(mInstance != NULL)
	{
		gkLogger::write("Network Instance cannot be created since there is already an existing instance\n");
		return;
	}  // if

	// Only create if there is a non - empty name and a non - empty address
	if(pName.length() == 0 || pAddress.length() == 0)
	{
		gkLogger::write("Network Instance cannot be created due to invalid name and/or address\n");
		return;
	}  // if

	// Setup new connection
	if(pType == 0)
	{
		mInstance = new gkNetworkServer(pName);
	}  // if
	else
	{
		mInstance = new gkNetworkClient(pName);
	}  // else

	mInstance -> setAddress(pAddress);
	mInstance -> setPort(pPort);
}  // void gkNetworkManager::startNetworkInstance

void gkNetworkManager::startNetworkInstance()
{
	if(mInstance != NULL)
	{
		mInstance -> start();
	}  // if
}  // void gkNetworkManager::startNetworkInstance

void gkNetworkManager::stopNetworkInstance()
{
	if(mInstance != NULL)
	{
		mInstance -> stop();
	}  // if
}  // void gkNetworkManager::stopNetworkInstance

bool gkNetworkManager::isNetworkInstanceRunning()
{
	if(mInstance != NULL)
	{
		return mInstance -> isRunning();
	}  // if

	return false;
}  // bool gkNetworkManager::isNetworkInstanceRunning

void gkNetworkManager::deleteNetworkInstance()
{
	deinitialize();
}  // gkNetworkManager::deleteNetworkInstance

void gkNetworkManager::sendMessage(
	const gkString & pSender, const gkString & pReceiver, 
	const gkString & pSubject, const gkString & pBody)
{
	if(mInstance != NULL)
	{
		mInstance ->sendMessage(pSender, pReceiver, pSubject, pBody);
	}  // if
}  // gkNetworkManager::sendMessage

bool gkNetworkManager::isNetworkInstanceExists(void)
{
	if(mInstance != NULL)
	{
		return true;
	}  // if

	return false;
}  // bool isNetworkInstanceExists

UT_IMPLEMENT_SINGLETON(gkNetworkManager);
