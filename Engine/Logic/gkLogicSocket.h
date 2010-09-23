/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C & Nestor Silveira

    Contributor(s): none yet
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
#ifndef _gkLogicSocket_h_
#define _gkLogicSocket_h_

#include "gkLogicCommon.h"

class gkILogicSocket
{
public:

	gkILogicSocket()
		: m_isInput(true), m_from(0), m_connected(false), m_parent(0)
	{
	}

	gkILogicSocket(gkLogicNode* par, bool isInput)
		: m_isInput(isInput), m_from(0), m_connected(false), m_parent(par)
	{
	}

	virtual ~gkILogicSocket() {};

	void link(gkILogicSocket* fsock);

	gkGameObject* getGameObject()const;

	// owner node
	GK_INLINE gkLogicNode* getParent() const
	{
		return m_parent;
	}

	GK_INLINE bool isLinked() const
	{
		return m_from != 0;
	}

	GK_INLINE bool isConnected() const
	{
		return m_connected;
	}

	GK_INLINE gkILogicSocket* getFrom() const
	{
		return m_from;
	}

protected:

	bool m_isInput;

	typedef utList<gkILogicSocket*> Sockets;

	typedef utListIterator<Sockets> SocketIterator;

	// from socket to 'this' (used to link an input socket with an output socket)
	// Only one makes sense
	gkILogicSocket* m_from;

	// from 'this' to sockets (used to link an output socket with one or more than one input socket)
	Sockets m_to;

private:

	bool m_connected;

	// owner node
	gkLogicNode* m_parent;
};

template<typename T>
class gkLogicSocket : public gkILogicSocket
{
public:
	gkLogicSocket()
		: gkILogicSocket()
	{
	}

	gkLogicSocket(gkLogicNode* par, bool isInput, T defaultValue)
		: gkILogicSocket(par, isInput), m_data(defaultValue)
	{
	}

	void setValue(const T& value)
	{
		if (!m_isInput)
		{
			SocketIterator sockit(m_to);

			while (sockit.hasMoreElements())
			{
				gkLogicSocket<T>* sock = dynamic_cast<gkLogicSocket<T>*>(sockit.getNext());

				GK_ASSERT(sock && "Types have to match");

				sock->setValue(value);
			}
		}

		m_data = value;
	}

	T getValue() const
	{
		if (m_from)
		{
			gkLogicSocket<T>* sock = dynamic_cast<gkLogicSocket<T>*>(m_from);

			GK_ASSERT(sock && "Types have to match");

			return sock->getValue();
		}

		return m_data;
	}

	T& getRefValue()
	{
		if (m_from)
		{
			gkLogicSocket<T>* sock = dynamic_cast<gkLogicSocket<T>*>(m_from);

			GK_ASSERT(sock && "Types have to match");

			return sock->getRefValue();
		}

		return m_data;
	}


private:

	T m_data;
};

template<typename T>
gkLogicSocket<T>* getSocket(gkILogicSocket* pSock)
{
	return static_cast<gkLogicSocket<T>*>(pSock);
}

#define ADD_ISOCK( name, value ){      \
    addISock < _SOCKET_TYPE_##name > ( m_sockets[ (name) ], value );    \
}

#define ADD_OSOCK(name, value){      \
    addOSock< _SOCKET_TYPE_##name > ( m_sockets[ (name) ], value );    \
}

#define SET_SOCKET_VALUE( name, value )    \
    setSocketValue< _SOCKET_TYPE_##name > ( (name), (value) )    \

#define GET_SOCKET_VALUE( name )      \
    getSocketValue< _SOCKET_TYPE_##name > ( (name) )    \

#define GET_SOCKET_REF_VALUE( name )      \
    getSocketRefValue< _SOCKET_TYPE_##name > ( (name) )    \

#define GET_SOCKET( name )      \
    getSocket< _SOCKET_TYPE_##name > ( (name) )    \

#define DECLARE_SOCKET_TYPE( name, type)    \
    typedef type _SOCKET_TYPE_##name;    \
    GK_INLINE gkLogicSocket< type >* get##name() { return getSocket< type > ( (name) ); } \


#define CHECK_RETV(cond) {                  \
    if ((cond)) return;                     \
}

#endif//_gkLogicSocket_h_
