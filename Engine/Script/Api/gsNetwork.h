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

/** \addtogroup Network
	@{
*/

/** 
	Create network instance based on input

	\code
	function createNetworkInstance(pType, pName, pAddress, pPort)
	\endcode

	\param pType 0: indicates server instance 1: indicates client instance
	\param pName The name of the network instance
	\param pAddress The name of the server address if the instance is a client
	\param pPort The port for the network instance to listen to	
*/
extern void createNetworkInstance(
	const int & pType, const gkString & pName, 
	const gkString & pAddress, const int & pPort);

/** 
	Start network instance if one exists

	\code
	function startNetworkInstance()
	\endcode
*/ 
extern void startNetworkInstance(void);

/** 
	Stop the network instance if one exists

	\code
	function stopNetworkInstance()
	\endcode
*/
extern void stopNetworkInstance(void);

/** 
	\code
	function deleteNetworkInstance()
	\endcode

	Remove the instance and free its resouces if there is one
*/
extern void deleteNetworkInstance(void);

/** 
	Send Message through network to every connection

	\code
	function sendNetworkInstanceMessage(pSender, pReceiver, pSubject, pBody)
	\endcode

	\param pSender string The name of the sender object
	\param pReceiver The name of the receiver object
	\param pSubject The subject of the message
	\param pBody The body of the message
*/
extern void sendNetworkInstanceMessage(
	const gkString & pSender, const gkString & pReceiver, 
	const gkString & pSubject, const gkString & pBody);

/** 
	To get bool to indicate whether the instance is running

	\code
	function isNetworkInstanceRunning()
	\endcode
	
	\return bool To indicate whether the instance is running
*/
extern bool isNetworkInstanceRunning(void);


/**
	To get bool to indicate whether there is a network instance

	\code
	function isNetworkInstanceExists()
	\endcode
 
	\return: bool To indicate whether there is a network instance
*/ 
extern bool isNetworkInstanceExists(void);

/** @} */