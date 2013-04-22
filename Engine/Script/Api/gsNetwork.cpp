/*
-------------------------------------------------------------------------------
This file is part of OgreKit.
http://gamekit.googlecode.com/

Copyright (c) 2006-2013 Charlie C.

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

#include "gsNetwork.h"

#ifdef OGREKIT_COMPILE_ENET
#include "Network/gkNetworkManager.h"
#endif

void createNetworkInstance(const int & pType, const gkString & pName, const gkString & pAddress, const int & pPort)
{
#ifdef OGREKIT_COMPILE_ENET
	if(gkNetworkManager::getSingletonPtr()!=NULL)
	{
		if(!gkNetworkManager::getSingletonPtr()->isNetworkInstanceExists())
		{
			gkNetworkManager::getSingletonPtr()->createNetworkInstance(pType, pName, pAddress, pPort);
		}  // if
	}  // if
#endif
}  // void createNetworkInstance

void startNetworkInstance(void)
{
#ifdef OGREKIT_COMPILE_ENET
	if(gkNetworkManager::getSingletonPtr()!=NULL)
	{
		if(gkNetworkManager::getSingletonPtr()->isNetworkInstanceExists())
		{
			gkNetworkManager::getSingletonPtr()->startNetworkInstance();
		}  // if
	}  // if
#endif
}  // void startNetworkInstance

void stopNetworkInstance(void)
{
#ifdef OGREKIT_COMPILE_ENET
	if(gkNetworkManager::getSingletonPtr()!=NULL)
	{
		if(gkNetworkManager::getSingletonPtr()->isNetworkInstanceExists())
		{
			gkNetworkManager::getSingletonPtr()->stopNetworkInstance();
		}  // if
	}  // if
#endif
}  // void stopNetworkInstance

void deleteNetworkInstance(void)
{
#ifdef OGREKIT_COMPILE_ENET
	if(gkNetworkManager::getSingletonPtr()!=NULL)
	{
		if(gkNetworkManager::getSingletonPtr()->isNetworkInstanceExists())
		{
			gkNetworkManager::getSingletonPtr()->deleteNetworkInstance();
		}  // if
	}  // if
#endif
}  // void deleteNetworkInstance

void sendNetworkInstanceMessage(
	const gkString & pSender, const gkString & pReceiver, 
	const gkString & pSubject, const gkString & pBody)
{
#ifdef OGREKIT_COMPILE_ENET
	if(gkNetworkManager::getSingletonPtr()!=NULL)
	{
		if(gkNetworkManager::getSingletonPtr()->isNetworkInstanceExists())
		{
			gkNetworkManager::getSingletonPtr()->sendMessage(
				pSender, pReceiver, 
				pSubject, pBody);
		}  // if
	}  // if
#endif
}  // void sendNetworkInstanceMessag

bool isNetworkInstanceRunning(void)
{
#ifdef OGREKIT_COMPILE_ENET
	if(gkNetworkManager::getSingletonPtr()!=NULL)
	{
		if(gkNetworkManager::getSingletonPtr()->isNetworkInstanceExists())
		{
			return gkNetworkManager::getSingletonPtr()->isNetworkInstanceRunning();
		}  // if
	} // if
#endif
	return false;
}  // bool isNetworkInstanceRunning

bool isNetworkInstanceExists(void)
{
#ifdef OGREKIT_COMPILE_ENET
	if(gkNetworkManager::getSingletonPtr()!=NULL)
	{
		return gkNetworkManager::getSingletonPtr()->isNetworkInstanceExists();
	}  // if
#endif
	return false;
}  // bool isNetworkInstanceExists

