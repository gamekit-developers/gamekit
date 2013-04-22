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

#ifndef _gkNetworkManager_h_
#define _gkNetworkManager_h_

#include "gkCommon.h"
#include "gkHashedString.h"
#include "gkSerialize.h"
#include "utSingleton.h"
#include "gkNetworkInstance.h"
#include "gkNetworkServer.h"
#include "gkNetworkClient.h"

#include <enet/enet.h>

// Network system manager
class gkNetworkManager : public utSingleton<gkNetworkManager>
{
private:

	gkNetworkInstance * mInstance;

	// initialize
	// Input: None
	// Return: None
	// Initialize enent
	void initialize(void);
	
	// initialize
	// Input: None
	// Return: None
	// Deinitialize enet
	void deinitialize(void);

public:
	// Constructor
	// Input: None
	// Return: None
	// Initialize class
	gkNetworkManager();

	// Constructor
	// Input: None
	// Return: None
	// Deinitialize class
	virtual ~gkNetworkManager();

	// createNetworkInstance
	// Input: pType 0 indicates server instance
	//              1 indicates client instance
	//        pName The name of the network instance
	//        pAddress The name of the server address if the instance is a client
	//        pPort The port for the network instance to listen to
	// Return:None
	// Create network instance based on input
	void createNetworkInstance(
		const int & pType, const gkString & pName, 
		const gkString & pAddress, const int & pPort);

	// startNetworkInstance
	// Input: None
	// Return: None
	// Start network instance if one exists
	void startNetworkInstance(void);

	// stopNetworkInstance
	// Input: None
	// Return: None
	// Stop the network instance if one exists
	void stopNetworkInstance(void);

	// isNetworkInstanceRunning
	// Input:None
	// Return: bool To indicate whether the instance is running
	// To get bool to indicate whether the instance is running
	bool isNetworkInstanceRunning(void);

	// deleteNetworkInstance
	// Input: None
	// Return: None
	// Remove the instance and free its resouces if there is one
	void deleteNetworkInstance(void);

	// sendMessage
	// Input: pSender The name of the sender object
	//        pReceiver The name of the receiver object
	//        pSubject The subject of the message
	//        pBody The body of the message
	// Return: None
	// Send Message through network to every connection
	void sendMessage(
		const gkString & pSender, const gkString & pReceiver, 
		const gkString & pSubject, const gkString & pBody);

	// isNetworkInstanceExists
	// Input: None
	// Return: bool To indicate whether there is a network instance
	// To get bool to indicate whether there is a network instance
	bool isNetworkInstanceExists(void);

protected:

	UT_DECLARE_SINGLETON(gkNetworkManager);
};  // gkNetworkManager

#endif  // gkNetworkManager_h_
