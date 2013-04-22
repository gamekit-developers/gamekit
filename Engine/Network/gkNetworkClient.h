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

#ifndef _gkNetworkClient_h_
#define _gkNetworkClient_h_

#include "gkCommon.h"
#include "gkHashedString.h"
#include "gkSerialize.h"
#include "gkNetworkInstance.h"
#include "gkNetworkClient.h"

// Network system server
class gkNetworkClient : public gkNetworkInstance
{
protected:
	// initialize
	// Input: None
	// Return: bool to indicate whether the instance inititailze successfully
	// To initialize a client to connect to the server with inputted configuration information
	bool initialize(void);

public:
	// Constructor
	// Input: pName The name for the instance
	// Return: None
	// Set name
	gkNetworkClient(const gkString& pName);
	// Destructor
	// Input: None
	// Return: None
	virtual ~gkNetworkClient();
};  // gkNetworkClient

#endif  // _gkNetworkClient_h_
