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

#include "gkNetworkInstance.h"
#include "gkEngine.h"
#include "gkMessageManager.h"
#include "gkLogger.h"

gkNetworkInstance::gkNetworkInstance(const gkString & pName)
	:   mName(pName),
	mThread(0),
	mStop(true)
{
}  // gkNetworkInstance::gkNetworkInstance

gkNetworkInstance::~gkNetworkInstance()
{
	exit();
}  //  gkNetworkInstance::~gkNetworkInstance

void gkNetworkInstance::exit(void)
{
	// exit thread
	if (mThread)
	{
		stop();
		// Wait for current task to be complete
		mSync.wait();

		mThread->join();

		delete mThread;
		mThread = 0;
	}  // if

	deinitialize();
}  // void gkNetworkInstance::exit

void gkNetworkInstance::stop(void)
{
	gkCriticalSection::Lock lock(mCriticalSelection);
	if (!mStop)
	{
		mStop = true;
	}  // if
}  // void gkNetworkInstance::stop

void gkNetworkInstance::start(void)
{
	gkCriticalSection::Lock lock(mCriticalSelection);
	if (mStop)
	{
		if (!mThread)
		{
			mStop = !initialize();
			mThread = new gkThread(this);
		}  // if
		else
		{
			mStop = false;
		}  // else
	}  // if
}  // void gkNetworkInstance::start

const bool gkNetworkInstance::isRunning(void)
{
	return !mStop;
}  // bool gkNetworkInstance::isRunning

const gkString gkNetworkInstance::getAddress(void)
{
	return mAddress;
}  // gkString gkNetworkInstance::getAddress

void gkNetworkInstance::setAddress(const gkString & pAddress)
{
	mAddress = pAddress;
}  // gkNetworkInstance::setAddressServerName

const int gkNetworkInstance::getPort(void)
{
	return mPort;
}  // const int gkNetworkInstance::getPort

void gkNetworkInstance::setPort(const int & pPort)
{
	mPort = pPort;
}  // void gkNetworkInstance::setPort

void gkNetworkInstance::receiveMessage(const ENetPacket * pPacket)
{
	gkString lTempString;

	size_t lLength = pPacket -> dataLength;
	size_t lIndex = 0;

	int lCurrentStringType = 0;

	gkString lFrom;
	gkString lTo;
	gkString lSubject;
	gkString lBody;

	// Loop through the entire string, and for every ; symbol save it into one part of message
	for(lIndex = 0; lIndex < lLength; lIndex++)
	{
		char lTempChar = (char)pPacket -> data[lIndex];
		if(lTempChar != ';')
		{
			lTempString = lTempString + lTempChar;
		}  // if
		else
		{
			switch(lCurrentStringType)
			{
			case 0:
				lFrom = lTempString;
				break;
			case 1:
				lTo = lTempString;
				break;
			case 2:
				lSubject = lTempString;
				break;
			case 3:
				lBody = lTempString;
				break;
			}   // switch

			if(lCurrentStringType == 3)
			{
				break;
			}  // if

			lCurrentStringType++;
			lTempString = "";
		}  // else
	}  // for

	gkMessageManager::getSingletonPtr() ->sendMessage(lFrom, lTo, lSubject, lBody);
}  // gkNetworkInstace::receiveMessage

void gkNetworkInstance::sendMessage(const gkString & pSender, const gkString & pReceiver, const gkString & pSubject, const gkString & pBody)
{
	// Concating messages into a single string
	gkString lPacketMessage = "";
	lPacketMessage.append(pSender);
	lPacketMessage.append(";");
	lPacketMessage.append(pReceiver);
	lPacketMessage.append(";");
	lPacketMessage.append(pSubject);
	lPacketMessage.append(";");
	lPacketMessage.append("pBody");
	lPacketMessage.append(";");

	// Create the packet according to the message size
	ENetPacket * lPacket = enet_packet_create (lPacketMessage.c_str(), 
		lPacketMessage.length(), 
		ENET_PACKET_FLAG_RELIABLE);

	enet_host_broadcast (mHost, 0, lPacket);

	// Flush out packet
	enet_host_flush (mHost);
}  // void gkNetworkInstance::sendMessage

void gkNetworkInstance::run(void)
{	
	/// Main network workload.
	/// Note: This is designed to be a long running background thread.

	while (isRunning())
	{
		// catch any exceptions
		try
		{
			ENetEvent lEvent;
			// Wait up to 1000 milliseconds for an event
			while (enet_host_service (mHost, & lEvent, 1000) > 0)
			{
				switch (lEvent.type)
				{
				case ENET_EVENT_TYPE_CONNECT:

					break;

				case ENET_EVENT_TYPE_RECEIVE:
					receiveMessage(lEvent.packet);

					// Clean up the packet now that we're done using it.
					enet_packet_destroy (lEvent.packet);

					break;

				case ENET_EVENT_TYPE_DISCONNECT:
					lEvent.peer -> data = NULL;
				}  // switch
			}  // while

			if(!mStop)
			{
				mSync.signal();
			}  // if
		}  // try
		catch (...)
		{
			printf("%s: unknown error!\n", mName.c_str());
		}  // catch
	}  // while

	mSync.signal();
}  // gkNetworkInstance::run

void gkNetworkInstance::deinitialize(void)
{
	if(mHost != NULL)
	{
		enet_host_destroy(mHost);
	} // if
}  // void gkNetworkInstance::deinitialize
