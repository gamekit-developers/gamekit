/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): Nestor Silveira.
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
#ifndef _gkLogicNode_h_
#define _gkLogicNode_h_

#include "gkLogicCommon.h"
#include "gkLogicSocket.h"
#include "gkString.h"
#include "gkMathUtils.h"

class gkGameObject;
class gkLogicTree;

class gkLogicNode
{
public:
	typedef utList<gkILogicSocket*>  Sockets;
	typedef utListIterator<Sockets> SocketIterator;
	static const int N_MAX_SOCKETS = 25;

public:
	gkLogicNode(gkLogicTree* parent, UTsize handle);

	virtual ~gkLogicNode();

	// do the update logic
	virtual void update(gkScalar tick) {}

	// see if an update is wanted
	virtual bool evaluate(gkScalar tick) {return true;}

	// do first run initialization
	virtual void initialize(void) {}

	gkILogicSocket*          getInputSocket(UTsize index);
	gkILogicSocket*          getOutputSocket(UTsize index);

	GK_INLINE void          attachObject(gkGameObject* ob)  {m_object = ob;}
	GK_INLINE gkGameObject* getAttachedObject(void)         {return m_other != 0 ? m_other : m_object; }

	GK_INLINE const UTsize  getHandle(void)         {return m_handle;}
	GK_INLINE bool          hasLinks(void)          {return m_hasLinks;}
	GK_INLINE void          setLinked(void)         {m_hasLinks = true;}
	GK_INLINE void          setPriority(int v)      {m_priority = v;}
	GK_INLINE int           getPriority(void)       {return m_priority;}


	GK_INLINE Sockets& getInputs(void)  {return m_inputs;}
	GK_INLINE Sockets& getOutputs(void) {return m_outputs;}

	template<typename T>
	void addISock(gkILogicSocket* dest, T defaultValue)
	{
		dest = new gkLogicSocket<T>(this, true, defaultValue);

		int idx = m_inputs.size() + m_outputs.size();

		GK_ASSERT(idx < N_MAX_SOCKETS);

		m_sockets[idx] = dest;

		m_inputs.push_back(dest);
	}

	template<typename T>
	void addOSock(gkILogicSocket* dest, T defaultValue)
	{
		dest = new gkLogicSocket<T>(this, false, defaultValue);

		int idx = m_inputs.size() + m_outputs.size();

		GK_ASSERT(idx < N_MAX_SOCKETS);

		m_sockets[idx] = dest;

		m_outputs.push_back(dest);
	}

	template<typename T>
	gkLogicSocket<T>* getSocket(int idx)
	{
		GK_ASSERT(idx < N_MAX_SOCKETS);
		return static_cast<gkLogicSocket<T>*>(m_sockets[idx]);
	}

	template<typename T>
	T getSocketValue(int idx)
	{
		GK_ASSERT(idx < N_MAX_SOCKETS);

		return static_cast<gkLogicSocket<T>*>(m_sockets[idx])->getValue();
	}

	template<typename T>
	T& getSocketRefValue(int idx)
	{
		GK_ASSERT(idx < N_MAX_SOCKETS);

		return static_cast<gkLogicSocket<T>*>(m_sockets[idx])->getRefValue();
	}


	template<typename T>
	void setSocketValue(int idx, T data)
	{
		GK_ASSERT(idx < N_MAX_SOCKETS);

		static_cast<gkLogicSocket<T>*>(m_sockets[idx])->setValue(data);
	}


protected:
	const UTsize    m_handle;
	gkGameObject*   m_object, *m_other;
	gkLogicTree*    m_parent;
	bool            m_hasLinks;
	Sockets         m_inputs;
	Sockets         m_outputs;
	int             m_priority;

	gkILogicSocket* m_sockets[N_MAX_SOCKETS];
};

#endif//_gkLogicNode_h_
