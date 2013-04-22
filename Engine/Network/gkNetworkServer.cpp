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
#include "gkNetworkServer.h"
#include "gkEngine.h"
#include "gkUserDefs.h"
#include "gkDebugger.h"
#include "gkLogger.h"

gkNetworkServer::gkNetworkServer(const gkString& pName)
	:gkNetworkInstance(pName)
{
}  // gkNetworkServer::gkNetworkServer


gkNetworkServer::~gkNetworkServer(void)
{
}  // gkNetworkServer::~gkNetworkServer

bool gkNetworkServer::initialize(void)
{
	// Bind the server to the default localhost.     
	// A specific host address can be specified by   
	// enet_address_set_host (& address, "x.x.x.x")
	mENetAddress.host = ENET_HOST_ANY;
	/* Bind the server to port 1234. */
	mENetAddress.port = mPort;
	mHost = enet_host_create (& mENetAddress, // the address to bind the server host to 
		32,      // allow up to 32 clients and/or outgoing connections
		2,      // allow up to 2 channels to be used, 0 and 1
		0,      // assume any amount of incoming bandwidth
		0);      // assume any amount of outgoing bandwidth
	if (mHost == NULL)
	{
		gkLogger::write("An error occurred while trying to create an ENet server host.\n");
		return false;
	}  // if

	return true;
}  // void gkNetworkServer::initialize
