/*
-------------------------------------------------------------------------------
	This file is part of the Ogre GameKit port.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#include "gkMathUtils.h"
#include "OgreStringConverter.h"
#include "gkVariable.h"



// ----------------------------------------------------------------------------
class gkLogicSocket
{
public:

	typedef enum SocketType
	{
		ST_NULL= 0,
		ST_BOOL,
		ST_INT,
		ST_REAL,
		ST_VECTOR,
		ST_EULER,
		ST_QUAT,
		ST_STRING,
		ST_VARIABLE,
	}SocketType;

public:
	gkLogicSocket();
	gkLogicSocket(gkLogicNode* par, SocketType stype);

	void			link(gkLogicSocket *fsock);
	const		   gkLogicSocket& getLink();

	gkLogicNode*		  getParent() const;
	gkLogicSocket*		getFrom() const;
	const SocketType	getType() const;
	bool				isLinked() const;
	bool				isConnected() const;
	bool				isBlocked() const;

	void setValue(bool v);
	void setValue(int v);
	void setValue(Ogre::Real v);
	void setValue(const Ogre::Vector3& v);
	void setValue(const Ogre::Quaternion& v);
	void setValue(const Ogre::String& str);
	void setValue(const gkVariable& prop);
	void setValue(gkVariable* prop);

	bool		getValueBool() const;
	int		 getValueInt() const;
	Ogre::Real		getValueReal() const;
	Ogre::Vector3	 getValueVector3() const;
	Ogre::Quaternion  getValueQuaternion() const;
	Ogre::String getValueString() const;
	gkVariable getValueVariable() const;

	gkGameObject* getGameObject()const;
	void setConnected();

	void setName(const Ogre::String& v);
	Ogre::String getName();
	Ogre::String getRealName();

	void copy(const gkLogicSocket &o);

	void block(bool truth);
	void push_link(gkLogicNode *link);
	bool has_link(gkLogicNode *link);

	gkNodeBaseIterator getNodeIterator();


private:
	bool					mConnected, mBlocked;
	Ogre::Vector4		   mData;	  /* in w,x,y,z order */
	Ogre::String			mStrData;   /* string pointer */
	gkVariable			  mVar;	   /* gkVariable data */
	SocketType			  mType;	  /* base type*/
	gkLogicSocket*		  mFrom;	  /* fron socket to 'this' */
	gkLogicNode*			mParent;	/* owner node */
	Ogre::String			mName;	  /* Blender name */
	gkLogicNodeBase		 mNodes;	 /* Subtree for io blocking */

};

// ----------------------------------------------------------------------------
GK_INLINE gkLogicSocket::gkLogicSocket() :
		mConnected(false), mBlocked(false), mData(Ogre::Vector4::ZERO), mStrData(""),
		mVar(), mType(ST_NULL), mFrom(0), mParent(0), mName("")
{
}

// ----------------------------------------------------------------------------
GK_INLINE gkLogicSocket::gkLogicSocket(gkLogicNode* par, SocketType stype) :
		mConnected(false), mBlocked(false), mData(Ogre::Vector4::ZERO), mStrData(""),
		mVar(), mType(stype), mFrom(0), mParent(par),  mName("")
{
}

// ----------------------------------------------------------------------------
GK_INLINE const gkLogicSocket::SocketType gkLogicSocket::getType() const
{
	return mType;
}

// ----------------------------------------------------------------------------
GK_INLINE gkLogicNode* gkLogicSocket::getParent() const
{
	return mParent;
}

// ----------------------------------------------------------------------------
GK_INLINE bool gkLogicSocket::isLinked() const
{
	return mFrom != 0;
}

// ----------------------------------------------------------------------------
GK_INLINE bool gkLogicSocket::isConnected() const
{
	return mConnected;
}

// ----------------------------------------------------------------------------
GK_INLINE gkLogicSocket* gkLogicSocket::getFrom() const
{
	return mFrom;
}

// ----------------------------------------------------------------------------
GK_INLINE const gkLogicSocket& gkLogicSocket::getLink()
{
	/* const reference to linked mData */
	if (mFrom)
		return *mFrom;
	/* self */
	return *this;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkLogicSocket::setValue(bool v)
{
	mData.w= v ? 1.0 : 0.0;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkLogicSocket::setValue(int v)
{
	mData.w= v;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkLogicSocket::setValue(Ogre::Real v)
{
	mData.w= v;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkLogicSocket::setValue(const Ogre::Vector3& v)
{
	mData.w= v.x;
	mData.x= v.y;
	mData.y= v.z;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkLogicSocket::setValue(const Ogre::Quaternion& v)
{
	mData.w= v.w;
	mData.x= v.x;
	mData.y= v.y;
	mData.z= v.z;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkLogicSocket::setValue(const gkVariable &prop)
{
	mVar.setValue(prop);
}

// ----------------------------------------------------------------------------
GK_INLINE void gkLogicSocket::setValue(gkVariable *prop)
{
	if (prop)
		mVar.setValue(*prop);
}

// ----------------------------------------------------------------------------
GK_INLINE void gkLogicSocket::setValue(const Ogre::String& str)
{
	mStrData= str;
}

// ----------------------------------------------------------------------------
GK_INLINE bool gkLogicSocket::getValueBool() const
{
	/* recursion should be OK, cannot have mFrom->mFrom .... */
	if (mFrom)
		return mFrom->getValueBool();

	if (mType == ST_VARIABLE)
		return mVar.getValueBool();
	if (mType == ST_STRING)
		return Ogre::StringConverter::parseBool(mStrData);
	return mData.w != 0;
}

// ----------------------------------------------------------------------------
GK_INLINE int gkLogicSocket::getValueInt() const
{
	if (mFrom)
		return mFrom->getValueInt();

	if (mType == ST_VARIABLE)
		return mVar.getValueInt();

	if (mType == ST_STRING)
		return Ogre::StringConverter::parseInt(mStrData);
	return (int)mData.w;
}

// ----------------------------------------------------------------------------
GK_INLINE Ogre::Real gkLogicSocket::getValueReal() const
{
	if (mFrom)
		return mFrom->getValueReal();

	if (mType == ST_VARIABLE)
		return mVar.getValueReal();
	if (mType == ST_STRING)
		return Ogre::StringConverter::parseReal(mStrData);

	return mData.w;
}

// ----------------------------------------------------------------------------
GK_INLINE Ogre::Vector3 gkLogicSocket::getValueVector3() const
{
	if (mFrom)
		return mFrom->getValueVector3();


	if (mType == ST_VARIABLE)
		return mVar.getValueVector3();
	if (mType == ST_STRING)
		return Ogre::StringConverter::parseVector3(mStrData);
	return Ogre::Vector3(mData.w, mData.x, mData.y);
}

// ----------------------------------------------------------------------------
GK_INLINE Ogre::Quaternion gkLogicSocket::getValueQuaternion() const
{
	if (mFrom)
		return mFrom->getValueQuaternion();

	if (mType == ST_VARIABLE)
		return mVar.getValueQuaternion();
	if (mType == ST_STRING)
		return Ogre::StringConverter::parseQuaternion(mStrData);
	if (mType == ST_EULER)
		return gkMathUtils::getQuatFromEuler(Ogre::Vector3(mData.w, mData.x, mData.y), true);
	return Ogre::Quaternion(mData.w, mData.x, mData.y, mData.z);
}

// ----------------------------------------------------------------------------
GK_INLINE Ogre::String gkLogicSocket::getValueString() const
{
	if (mFrom)
		return mFrom->getValueString();

	if (mType == ST_VARIABLE)
		return mVar.getValueString();

	if (mType != ST_STRING)
	{
		if (mType == ST_BOOL)
			return Ogre::StringConverter::toString(mData.w != 0);
		if (mType == ST_INT)
			return Ogre::StringConverter::toString((int)mData.w);
		if (mType == ST_REAL)
			return Ogre::StringConverter::toString(mData.w);
		if (mType == ST_VECTOR || mType == ST_EULER)
			return Ogre::StringConverter::toString(Ogre::Vector3(mData.w, mData.x, mData.y));
		if (mType == ST_QUAT)
			return Ogre::StringConverter::toString(Ogre::Quaternion(mData.w, mData.x, mData.y, mData.z));
	}
	return mStrData;
}

// ----------------------------------------------------------------------------
GK_INLINE gkVariable gkLogicSocket::getValueVariable() const
{
	if (mFrom)
		return mFrom->getValueVariable();

	if (mType == ST_VARIABLE)
		return mVar;
	if (mType == ST_BOOL)
	{
		gkVariable prop;
		prop.setValue(mData.w != 0);
		return prop;
	}
	if (mType == ST_INT)
	{
		gkVariable prop;
		prop.setValue((int)mData.w);
		return prop;
	}
	if (mType == ST_REAL)
	{
		gkVariable prop;
		prop.setValue(mData.w);
		return prop;
	}
	if (mType == ST_VECTOR || mType == ST_EULER)
	{
		gkVariable prop;
		prop.setValue(Ogre::Vector3(mData.w, mData.x, mData.y));
		return prop;
	}
	if (mType == ST_QUAT)
	{
		gkVariable prop;
		prop.setValue(Ogre::Quaternion(mData.w, mData.x, mData.y, mData.z));
		return prop;
	}

	/*Ogre::String */
	gkVariable prop;
	prop.setValue(mStrData);
	return prop;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkLogicSocket::copy(const gkLogicSocket &o)
{
	mConnected= o.mConnected;
	mData= o.mData;
	mStrData= o.mStrData;
	mVar= o.mVar;
	mType= o.mType;
	mFrom= o.mFrom;
	mParent= o.mParent;
}

// ----------------------------------------------------------------------------
GK_INLINE bool gkLogicSocket::isBlocked() const
{
	return mBlocked;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkLogicSocket::setConnected()
{
	mConnected= true;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkLogicSocket::setName(const Ogre::String& v)
{
	mName= v;
}

// ----------------------------------------------------------------------------
GK_INLINE Ogre::String gkLogicSocket::getName()
{
	if (mFrom)
		return mFrom->mName;
	return mName;
}

// ----------------------------------------------------------------------------
GK_INLINE Ogre::String gkLogicSocket::getRealName()
{
	return mName;
}

// ----------------------------------------------------------------------------
GK_INLINE gkNodeBaseIterator gkLogicSocket::getNodeIterator()
{
	return gkNodeBaseIterator(mNodes.begin(), mNodes.end());
}




/* utility defines for nodes */
#define ADD_ISOCK(dest, parent, type){	  \
	dest= gkLogicSocket(parent, type);	   \
	mInputs.push_back(&dest);			   \
}

#define ADD_OSOCK(dest, parent, type){	  \
	dest= gkLogicSocket(parent, type);	   \
	mOutputs.push_back(&dest);			  \
}

#define CHECK_RETV(cond) {				   \
	if ((cond)) return;					 \
}

#endif//_gkLogicSocket_h_
