/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Kai-Ting (Danil) Ko

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

#ifndef _gkNetworkInstance_h_
#define _gkNetworkInstance_h_

#include <enet/enet.h> 

#include "gkCommon.h"

#include "Thread/gkCriticalSection.h"
#include "Thread/gkThread.h"

// Network system instance
class gkNetworkInstance : public gkCall
{
protected:
	ENetAddress mENetAddress;
	gkString mAddress;
	int mPort;

	const gkString mName;

	ENetHost * mHost;

	gkCriticalSection   mCriticalSelection;
	gkThread*           mThread;
	gkSyncObj			mSync;
	bool                mStop;

	// initialize
	// Input: None
	// Return: bool to indicate whether the instance inititailze successfully
	// It will be implemented by child methods
	virtual bool initialize(void) = 0;

	// deinitialize
	// Input: None
	// Return: None
	// To free host resources if exist
	void deinitialize(void);

	// receiveMessage
	// Input: pPacket The packet pointer to packet object that contains 
	//                the information sent in from connections
	// Return: None
	// To analyze packet information and send message through MessageManager
	void receiveMessage(const ENetPacket * pPacket);

public:
	// Constructor
	// Input: pName The name for the instance
	// Return: None
	// Set the name for the instance
	gkNetworkInstance(const gkString& pName);

	// Destructor
	// Input: None
	// Return: None
	// Call exist to join thread and then deinitialize to free host resources
	virtual ~gkNetworkInstance();

	// run
	// Input: None
	// Return: None
	// Thread run method
	void run(void);

	// isRunning
	// Input: None
	// Return: bool To indicate whether the thread is running or not
	// Thread run method
	const bool isRunning();

	// stop
	// Input: None
	// Return: None
	// Stop the thread if it is currently running
	void stop(void);

	// start
	// Input: None
	// Return: None
	// Start the thread or create a new thread if one does not exist
	void start(void);

	// exist
	// Input: None
	// Return: None
	// Stop and merge the thread if possible and then deinitialize to free host resources
	void exit(void);

	// getAddress
	// Input: None
	// Return: gkString The address for the network instance
	// Retrive the address
	const gkString getAddress(void);
	// setAddress
	// Input: pAddress The address for the network instance
	// Return: None
	// Set the address
	void setAddress(const gkString & pAddress);

	// getPort
	// Input: None
	// Return: int The port for the network instance
	// Set the port
	const int getPort(void);
	// setPort
	// Input: pPort The port for the network instance
	// Return: None
	// Set the port
	void setPort(const int & pPort);

	// sendMessage
	// Input: pSender The name of the sender object
	//        pReceiver The name of the receiver object
	//        pSubject The subject of the message
	//        pBody The body of the message
	// Return: None
	// Send Message through network to every connection
	void sendMessage(
		const gkString & pSender, const gkString & pReceiver, 
		const gkString & pSubject, const gkString & pBody
		);
};  // gkNetworkInstancegk

#endif  // _gkNetworkInstance_h_