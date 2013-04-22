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

#include "gkEngine.h"

// createNetworkInstance
// Input: pType 0 indicates server instance
//              1 indicates client instance
//        pName The name of the network instance
//        pAddress The name of the server address if the instance is a client
//        pPort The port for the network instance to listen to
// Return:None
// Create network instance based on input
extern void createNetworkInstance(
	const int & pType, const gkString & pName, 
	const gkString & pAddress, const int & pPort);

// startNetworkInstance
// Input: None
// Return: None
// Start network instance if one exists
extern void startNetworkInstance(void);

// stopNetworkInstance
// Input: None
// Return: None
// Stop the network instance if one exists
extern void stopNetworkInstance(void);

// deleteNetworkInstance
// Input: None
// Return: None
// Remove the instance and free its resouces if there is one
extern void deleteNetworkInstance(void);

// sendMessage
// Input: pSender The name of the sender object
//        pReceiver The name of the receiver object
//        pSubject The subject of the message
//        pBody The body of the message
// Return: None
// Send Message through network to every connection
extern void sendNetworkInstanceMessage(
	const gkString & pSender, const gkString & pReceiver, 
	const gkString & pSubject, const gkString & pBody);

// isNetworkInstanceRunning
// Input:None
// Return: bool To indicate whether the instance is running
// To get bool to indicate whether the instance is running
extern bool isNetworkInstanceRunning(void);

// isNetworkInstanceExists
// Input: None
// Return: bool To indicate whether there is a network instance
// To get bool to indicate whether there is a network instance
extern bool isNetworkInstanceExists(void);
