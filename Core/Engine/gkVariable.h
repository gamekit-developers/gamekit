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
#ifndef _gkVariable_h_
#define _gkVariable_h_

#include "OgreString.h"
#include "OgreStringConverter.h"
#include "gkCommon.h"
#include "gkMathUtils.h"




// ----------------------------------------------------------------------------
class gkVariable
{
public:

	typedef enum PropertyTypes
	{
		VAR_NULL= 0,
		VAR_BOOL,
		VAR_REAL,
		VAR_INT,
		VAR_STRING,
		VAR_VECTOR,
		VAR_QUATERNION,
	}PropertyTypes;

	gkVariable();
	gkVariable(const Ogre::String &n, bool dbg);

	void setValue(float v);
	void setValue(double v);
	void setValue(bool v);
	void setValue(int v);
	void setValue(const Ogre::String& v);
	void setValue(const Ogre::Vector3& v);
	void setValue(const Ogre::Quaternion& v);
	void setValue(const gkVariable& v);
	void setValueRaw(const Ogre::Vector4& v, const Ogre::String &mString);

	bool getValueBool() const;
	Ogre::Real getValueReal() const;
	int getValueInt() const;
	const Ogre::String getValueString() const;
	Ogre::Vector3 getValueVector3() const;
	Ogre::Quaternion getValueQuaternion() const;

	bool isDebug();
	const Ogre::String& getName();

private:

	/* data types */
	mutable Ogre::Vector4 mNumeric;
	mutable Ogre::String  mString;
	mutable int	 mType;
	Ogre::String		  mName;
	bool			mDebug;
};

// ----------------------------------------------------------------------------
GK_INLINE gkVariable::gkVariable() :
		mNumeric(Ogre::Vector4::ZERO), mString(Ogre::StringUtil::BLANK),
		mType(VAR_NULL), mName(""), mDebug(false)
{
}


// ----------------------------------------------------------------------------
GK_INLINE gkVariable::gkVariable(const Ogre::String &n, bool dbg) :
		mNumeric(Ogre::Vector4::ZERO), mString(Ogre::StringUtil::BLANK),
		mType(VAR_NULL), mName(n), mDebug(dbg)
{
}

// ----------------------------------------------------------------------------
GK_INLINE bool gkVariable::isDebug()
{
	return mDebug;
}

// ----------------------------------------------------------------------------
GK_INLINE const Ogre::String& gkVariable::getName()
{
	return mName;
}


// ----------------------------------------------------------------------------
GK_INLINE void gkVariable::setValue(float v)
{
	mType= VAR_REAL;
	mNumeric.w= (Ogre::Real)v;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkVariable::setValue(double v)
{
	mType= VAR_REAL;
	mNumeric.w= (Ogre::Real)v;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkVariable::setValue(bool v)
{
	mType= VAR_BOOL;
	mNumeric.w= v ? 1.0 : 0.0;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkVariable::setValue(int v)
{
	mType= VAR_INT;
	mNumeric.w= (int)v;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkVariable::setValue(const Ogre::String& v)
{
	mType= VAR_STRING;
	mString= v;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkVariable::setValue(const Ogre::Vector3& v)
{
	mType= VAR_VECTOR;
	mNumeric.w= v.x;
	mNumeric.x= v.y;
	mNumeric.y= v.z;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkVariable::setValue(const Ogre::Quaternion& v)
{
	mType= VAR_QUATERNION;
	mNumeric.w= v.w;
	mNumeric.x= v.x;
	mNumeric.y= v.y;
	mNumeric.z= v.z;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkVariable::setValueRaw(const Ogre::Vector4& v, const Ogre::String &vs)
{
	mType= VAR_NULL;
	mNumeric.w= v.w;
	mNumeric.x= v.x;
	mNumeric.y= v.y;
	mNumeric.z= v.z;
	mString= vs;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkVariable::setValue(const gkVariable& v)
{
	mType= v.mType;
	mNumeric.w= v.mNumeric.w;
	mNumeric.x= v.mNumeric.x;
	mNumeric.y= v.mNumeric.y;
	mNumeric.z= v.mNumeric.z;
	mString= v.mString;
}

// ----------------------------------------------------------------------------
GK_INLINE bool gkVariable::getValueBool() const
{
	if (mType == VAR_STRING)
		return Ogre::StringConverter::parseBool(mString);
	return mNumeric.w != 0;
}

// ----------------------------------------------------------------------------
GK_INLINE Ogre::Real gkVariable::getValueReal() const
{
	if (mType == VAR_STRING)
		return Ogre::StringConverter::parseReal(mString);
	return mNumeric.w;
}

// ----------------------------------------------------------------------------
GK_INLINE int gkVariable::getValueInt() const
{
	if (mType == VAR_STRING)
		return Ogre::StringConverter::parseInt(mString);
	return (int)mNumeric.w;
}

// ----------------------------------------------------------------------------
GK_INLINE const Ogre::String gkVariable::getValueString() const
{
	if (mType != VAR_STRING)
	{
		if (mType == VAR_BOOL)
			return Ogre::StringConverter::toString(mNumeric.w != 0);
		if (mType == VAR_REAL)
			return Ogre::StringConverter::toString(mNumeric.w);
		if (mType == VAR_INT)
			return Ogre::StringConverter::toString((int)mNumeric.w);
		if (mType == VAR_VECTOR)
			return Ogre::StringConverter::toString(Ogre::Vector3(mNumeric.w, mNumeric.x, mNumeric.y));
		if (mType == VAR_QUATERNION)
			return Ogre::StringConverter::toString(Ogre::Quaternion(mNumeric.w, mNumeric.x, mNumeric.y, mNumeric.z));
		return Ogre::StringUtil::BLANK;
	}
	return mString;
}

// ----------------------------------------------------------------------------
GK_INLINE Ogre::Vector3 gkVariable::getValueVector3() const
{
	if (mType == VAR_STRING)
		return Ogre::StringConverter::parseVector3(mString);
	return Ogre::Vector3(mNumeric.w, mNumeric.x, mNumeric.y);
}

// ----------------------------------------------------------------------------
GK_INLINE Ogre::Quaternion gkVariable::getValueQuaternion() const
{
	if (mType == VAR_STRING)
		return Ogre::StringConverter::parseQuaternion(mString);
	return Ogre::Quaternion(mNumeric.w, mNumeric.x, mNumeric.y, mNumeric.z);
}





#endif//_gkVariable_h_
