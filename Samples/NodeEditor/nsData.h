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
Data types for special nodes & sockets
*/
#ifndef _nsData_h_
#define _nsData_h_

#include "nsCommon.h"
#include "nsMath.h"



// Abstract data for custom types attached to nodes.
class nsNodeData
{
protected:
	nsNodeDef* m_parent;

public:

	nsNodeData(nsNodeDef* parent) : m_parent(parent) {}
	virtual ~nsNodeData() {}

	virtual nsNodeData* clone(void) {return new nsNodeData(*this);}

	nsNodeDef* getParent(void) const {return m_parent;}
};



///Common button press data for MouseButton & Key Nodes
class nsButtonData : public nsNodeData
{
private:

	int m_enum;
	int m_delay;

public:

	nsButtonData(nsNodeDef* parent)
		:   nsNodeData(parent), m_enum(-1), m_delay(-1)
	{
	}


	virtual ~nsButtonData() {}
	virtual nsNodeData* clone(void) {return new nsButtonData(*this); }

	UT_INLINE int getValue(void) {return m_enum;}
	UT_INLINE int getDelay(void) {return m_delay;}

	UT_INLINE void setValue(int v) {m_enum = v;}
	UT_INLINE void setDelay(int v) {m_delay = v;}

};

class nsMotionData : public nsNodeData
{
protected:
	int         m_enum;
	int         m_flag;
	int         m_transform;
	int         m_cf; // 1 | 2 | 4
	NSvec2      m_cx, m_cy, m_cz;
	nsString    m_relObj;
	bool        m_keep;


public:

	nsMotionData(nsNodeDef* parent)
		:   nsNodeData(parent), m_enum(0), m_flag(-1), m_transform(1),
		    m_cf(0),
		    m_cx(0, 0),
		    m_cy(0, 0),
		    m_cz(0, 0),
		    m_relObj(""),
		    m_keep(false)
	{
	}

	virtual ~nsMotionData() {}

	virtual nsNodeData* clone(void) {return new nsMotionData(*this); }


	UT_INLINE int                   getEnum(void)                           {return m_enum;}
	UT_INLINE int                   getFlag(void)                           {return m_flag;}
	UT_INLINE int                   getTransform(void)                      {return m_transform;}
	UT_INLINE NSvec2                getClampX(void)                         {return m_cx;}
	UT_INLINE NSvec2                getClampY(void)                         {return m_cy;}
	UT_INLINE NSvec2                getClampZ(void)                         {return m_cz;}
	UT_INLINE const nsString&       getRelativeObject(void)                 {return m_relObj;}
	UT_INLINE bool                  getKeep(void)                           {return m_keep;}
	UT_INLINE int                   getClampFlag(void)                      {return m_cf;}

	UT_INLINE void                  setEnum(int v)                          {m_enum = v;}
	UT_INLINE void                  setFlag(int v)                          {m_flag = v;}
	UT_INLINE void                  setTransform(int v)                     {m_transform = v;}
	UT_INLINE void                  setClampX(const NSvec2& v)              {m_cx = v;}
	UT_INLINE void                  setClampY(const NSvec2& v)              {m_cy = v;}
	UT_INLINE void                  setClampZ(const NSvec2& v)              {m_cz = v;}
	UT_INLINE void                  setRelativeObject(const nsString& v)    {m_relObj = v;}
	UT_INLINE void                  setKeep(bool v)                         {m_keep = v;}
	UT_INLINE void                  setClampFlag(int v)                     {m_cf = v;}
};



class nsObjectSocketData
{
public:

	enum AccessType
	{
		OSD_DEFAULT,
		OSD_GET,
		OSD_SET,
		OSD_POSITION,
		OSD_ORIENTATION,
		OSD_ROTATION,
		OSD_LIV_VEL,
		OSD_ANG_VEL,
	};

protected:


	AccessType          m_access;
	nsString            m_object;


public:


	nsObjectSocketData(const nsString& v = "")
		:   m_access(OSD_DEFAULT), m_object(v)
	{
	}

	nsObjectSocketData(const nsObjectSocketData& rhs)
		:   m_access(rhs.m_access), m_object(rhs.m_object)
	{
	}

	~nsObjectSocketData() {}


	UT_INLINE const nsString&       getObject(void) const           {return m_object;}
	UT_INLINE const AccessType&     getAccess(void) const           {return m_access;}
	UT_INLINE void                  setObject(const nsString& v)    {m_object = v;}
	UT_INLINE void                  setAccess(const AccessType& v)  {m_access = v;}
};




UT_INLINE void nsFromString(const nsString& s, nsObjectSocketData& v)
{
	v.setObject(s);
}


UT_INLINE nsString nsToString(const nsObjectSocketData& v)
{
	return v.getObject();
}




// Clamped data type
template <typename T>
class nsTypeClamp
{
public:

	nsTypeClamp() {}

	nsTypeClamp(const T& v, double _min, double _max, bool clamp)
		:   m_data(v), m_min(_min), m_max(_max), m_clamp(clamp)
	{
	}


	T       m_data;
	double  m_min, m_max;
	bool    m_clamp;

	UT_INLINE bool operator == (const nsTypeClamp<T>& rhs) const
	{
		return m_data == rhs.m_data;
	}
};


typedef nsTypeClamp<NSvec2> NSClampedVec2;
typedef nsTypeClamp<NSvec3> NSClampedVec3;
typedef nsTypeClamp<NSvec4> NSClampedVec4;


#endif//_nsData_h_
