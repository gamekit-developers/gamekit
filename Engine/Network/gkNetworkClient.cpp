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

#include "gkCommon.h"
#include "gkNetworkClient.h"
#include "gkEngine.h"
#include "gkUserDefs.h"
#include "gkDebugger.h"
#include "gkLogger.h"

gkNetworkClient::gkNetworkClient(const gkString& pName)
	:gkNetworkInstance(pName)
{
}  // gkNetworkClient::gkNetworkClient

gkNetworkClient::~gkNetworkClient(void)
{
}  // gkNetworkClient::~gkNetworkClient

bool gkNetworkClient::initialize(void)
{
	mHost = enet_host_create (NULL, // create a client host
		1, // only allow 1 outgoing connection
		2, // allow up 2 channels to be used, 0 and 1
		57600 / 8, // 56K modem with 56 Kbps downstream bandwidth
		14400 / 8); // 56K modem with 14 Kbps upstream bandwidth

	if (mHost == NULL)
	{
		gkLogger::write("An error occurred while trying to create an ENet client host.\n");
		return false;
	}  // if

	// Create event to hold connection information
	ENetEvent lEvent;

	// Connect to some.server.net:mPort
	enet_address_set_host (& mENetAddress, mAddress.c_str());
	mENetAddress.port = mPort;
	// Initiate the connection, allocating the two channels 0 and 1
	ENetPeer * lServer = enet_host_connect (mHost, & mENetAddress, 2, 0);    

	if (lServer == NULL)
	{
		gkLogger::write("Cannot find the server according to inputted inforamtion.\n");
		return false;
	}  // if

	// Wait up to 5 seconds for the connection attempt to succeed.
	// Either the 5 seconds are up or a disconnect event was
	// received. Reset the peer in the event the 5 seconds   
	// had run out without any significant event.     
	if (!((enet_host_service (mHost, & lEvent, 5000) > 0 &&
		lEvent.type == ENET_EVENT_TYPE_CONNECT)))
	{
		gkLogger::write("Connection failed.\n");
		enet_peer_reset (lServer);
		return false;
	}  // if

	return true;
}  // void gkNetworkClient::initialize
