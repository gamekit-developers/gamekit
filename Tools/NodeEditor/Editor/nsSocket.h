/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

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
#ifndef _nsSocket_h_
#define _nsSocket_h_

#include "nsCommon.h"
#include "nsMath.h"
#include "nsVariable.h"
#include "nsNodeTypeInfo.h"





// Base socket definition
class nsSocketDef : public nsSocketListClass::Link
{
protected:

	nsDirection         m_dir;
	nsString            m_name;
	nsString            m_docStr;
	nsColorPalette      m_palette;

	// global socket id
	int                 m_id;

	// direction unique id
	int                 m_uid;


	double              m_min, m_max;
	NSrect              m_rect;
	const nsValue       m_default;

public:
	nsSocketDef(nsDirection dir, const nsString& name, int id, double min, double max, const nsValue& val);
	virtual ~nsSocketDef();


	UT_INLINE nsDirection           getDirection(void) const                {return m_dir;}
	UT_INLINE const nsString&        getName(void) const                    {return m_name;}
	UT_INLINE nsColorPalette&        getPalette(void)                       {return m_palette;}
	UT_INLINE const nsColorPalette&  getPalette(void) const                 {return m_palette;}
	UT_INLINE const nsString&        getDocString(void) const               {return m_docStr;}
	UT_INLINE int                   getId(void) const                       {return m_id;}
	UT_INLINE double                getMin(void)const                       {return m_min;}
	UT_INLINE double                getMax(void) const                      {return m_max;}
	UT_INLINE const NSrect&          getRect(void) const                    {return m_rect;}
	UT_INLINE int                   getUid(void) const                      {return m_uid;}


	UT_INLINE void                  setDirection(const nsDirection& v)      {m_dir = v;}
	UT_INLINE void                  setName(const nsString& v)              {m_name = v;}
	UT_INLINE void                  setPalette(const nsColorPalette& v)     {m_palette = v;}
	UT_INLINE void                  setDocString(const nsString& v)         {m_docStr = v;}
	UT_INLINE void                  setId(int id)                           {m_id = id;}
	UT_INLINE void                  setUid(int id)                          {m_uid = id;}
	UT_INLINE void                  setMin(const double& v)                 {m_min = v;}
	UT_INLINE void                  setMax(const double& v)                 {m_max = v;}
	UT_INLINE void                  setRect(const NSrect& v)                {m_rect = v;}



	// identity checks

	UT_INLINE bool isTypeOf(nsSocketDef* def) const
	{
		if (!def) return false;
		return m_default.isTypeOf(def->m_default);
	}


	UT_INLINE bool isOutput(void)   const { return m_dir == NS_SOCK_OUT;  }
	UT_INLINE bool isInput(void)    const { return m_dir == NS_SOCK_IN;   }


	nsSocket*                    create(nsNode* parent);

	UT_INLINE const nsValue& getValue(void) const {return m_default;}
};




class nsSocket : public nsSocketClass::Link
{
protected:

	NSrect          m_rect, m_derrivedRect;
	// instance type
	nsSocketDef*     m_type;
	// parent node instance
	nsNode*          m_parent;
	// from -> this
	nsSocket*        m_from;
	// this -> to
	nsSocketList    m_tosockets;

	// containing socket value
	nsValue         m_value;

public:
	nsSocket(nsNode* nd, nsSocketDef* st);
	~nsSocket();

	UT_INLINE bool                  isConnectedOut(void)            {return !m_tosockets.empty();}
	UT_INLINE bool                  isConnected(void)               {return m_from != 0;}
	UT_INLINE nsSocket*              getSocketLink(void)            {return m_from;}
	UT_INLINE nsNode*                getParent(void)                {return m_parent;}
	UT_INLINE nsSocketDef*           getType(void)                  {return m_type;}
	UT_INLINE bool                  isOutput(void)                  {return m_type->isOutput();}
	UT_INLINE bool                  isInput(void)                   {return m_type->isInput();}
	UT_INLINE NSrect&                getDerrivedRect(void)          {return m_derrivedRect;}
	UT_INLINE NSrect&                getRect(void)                  {return m_rect;}

	UT_INLINE const nsValue&         getValue(void) const           {return m_value;}
	UT_INLINE void                  setValue(const nsValue& v)      {m_value = v;}



	// identity checks

	UT_INLINE bool isTypeOf(nsSocket* def) const
	{
		if (!def) return false;
		return m_type->isTypeOf(def->m_type);
	}


	// Remove all references to this
	void unlink(void);

	// Add references to oth
	void connect(nsSocket* oth);

	// Update derrived rect
	void updateFromParent(void);


	// duplicate
	UT_INLINE nsSocket* clone(nsNode* newParent)
	{
		nsSocket* sock = new nsSocket(newParent, m_type);
		sock->m_value = m_value;
		return sock;
	}


	// Sockets are only allowed to connect to others of the same type
	// Int->Float, is wrong
	// Float->Float, is ok
	UT_INLINE bool canConnect(nsSocket* oth)
	{
		if (!oth) return false;
		return  m_parent != oth->m_parent &&  m_type->isTypeOf(oth->getType());
	}
};


#define NS_CREATE_SOCKET_T(pal, dir, name, id, min, max, val) {  \
    nsSocketDef *def = new nsSocketDef(dir, name,  id, min, max, val);        \
    def->setPalette(nsSystemPalette::getPalette(pal));              \
    dir == NS_SOCK_IN ? addInput(def) : addOutput(def);             \
}



// short hand
#define NS_CREATE_BOOL_SOCKET(dir, name, id, min, max, val)\
    NS_CREATE_SOCKET_T(NS_SPE_BOOL, dir, name, id, min, max, ((bool)val))

#define NS_CREATE_INT_SOCKET(dir, name, id, min, max, val)\
    NS_CREATE_SOCKET_T(NS_SPE_INT, dir, name, id, min, max, ((int)val))

#define NS_CREATE_FLOAT_SOCKET(dir, name, id, min, max, val)\
    NS_CREATE_SOCKET_T(NS_SPE_FLOAT, dir, name, id,  min, max, ((NSfloat)val))

#define NS_CREATE_STRING_SOCKET(dir, name, id, min, max, val)\
    NS_CREATE_SOCKET_T(NS_SPE_STRING, dir, name, id, min, max, (nsString(val)))

#define NS_CREATE_VEC2_SOCKET(dir, name, id, min, max, val)\
    NS_CREATE_SOCKET_T(NS_SPE_VEC2, dir, name, id,  min, max, val)

#define NS_CREATE_VEC3_SOCKET(dir, name, id, min, max, val)\
    NS_CREATE_SOCKET_T(NS_SPE_VEC3, dir, name, id,  min, max, val)

#define NS_CREATE_VEC4_SOCKET(dir, name, id, min, max, val)\
    NS_CREATE_SOCKET_T(NS_SPE_VEC4, dir, name, id,  min, max, val)

#define NS_CREATE_QUAT_SOCKET(dir, name, id, min, max, val)\
    NS_CREATE_SOCKET_T(NS_SPE_QUAT, dir, name, id,  min, max, val)

#define NS_CREATE_OBJECT_SOCKET(dir, name, id, min, max, val)\
    NS_CREATE_SOCKET_T(NS_SPE_OBJECT, dir, name, id, min, max, (nsObjectSocketData(val)))

#endif//_nsSocket_h_
