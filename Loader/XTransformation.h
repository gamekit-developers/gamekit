/*
Maya Character Exporter
Copyright (c) 2009 Jun Murakawa

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. 
   If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#pragma once

#include <iostream>
#include "I16Aligned.h"
#ifdef WIN32
#include <vectormath/SSE/cpp/vectormath_aos.h>
#else
#include <vectormath/scalar/cpp/vectormath_aos.h>
#endif
using namespace Vectormath::Aos;

class XTransformation
	: public I16Aligned
{
public:

	/// stream extraction operator
	friend std::istream& operator>>(std::istream&,XTransformation &);

	/// \brief	This function linearly interpolates the transform, however it 
	///			performs a Sphereical interpolation on the rotation components.
	/// \param	out	-	the output transform
	/// \param	t	-	the interpolation value
	/// \param	t1	-	the first transform
	/// \param	t2	-	the second transform
	/// 
	friend XTransformation LERP(const float, const XTransformation&, const XTransformation&);

	const XTransformation& operator = (const XTransformation& T) 
	{
		m_Rotation = T.m_Rotation;
		m_Translation = T.m_Translation;
		m_Scale = T.m_Scale;
		m_Visibility = T.m_Visibility;
		return *this;
	}


	///-------------------------------------------------------------------------
	/// \brief	ctor
	/// 
	XTransformation() 
		: m_Rotation( Quat::identity() ), 
		m_Translation( Vector3(0.0f) ),
		m_Scale(1.0f,1.0f,1.0f),
		m_Visibility(true)
	{
	}

	///-------------------------------------------------------------------------
	/// \brief	A slightly more intersting ctor
	/// \param	r	-	the rotation quaternion for this transform
	/// \param	t	-	the translation for this transform
	/// \param	s	-	the scale for this transform
	/// 
	XTransformation( const Quat& r,
			    const Vector3& t,
			    const Vector3& s )
		: m_Rotation(r),
		  m_Translation(t),
		  m_Scale(s),
		  m_Visibility(true)
	{
	}

	///-------------------------------------------------------------------------
	/// \brief	The copy constructor for the transform types
	/// \param	rhs	-	the copied argument
	/// 
	XTransformation( const XTransformation& rhs)
		: m_Rotation(rhs.m_Rotation),
		  m_Translation(rhs.m_Translation),
		  m_Scale(rhs.m_Scale),
		  m_Visibility(rhs.m_Visibility)
	{
	}

	~XTransformation(void);

	inline Matrix4 asMatrix() const 
	{
		Matrix4 M = Matrix4( m_Rotation, m_Translation );
		return appendScale( M, m_Scale );
	}

	Quat		m_Rotation;
	Vector3		m_Translation;
	Vector3		m_Scale;

	bool		m_Visibility;

};

class XTimeInfo
{
public:

	/// the constructor
	XTimeInfo()
	{
		memset(this,0,sizeof(XTimeInfo));
	}

	///-------------------------------------------------------------------------
	/// \brief	this function is used to initialised the time control class.
	///	\param	start		-	the start time (in seconds) for the animation.
	/// \param	end			-	the end time (in seconds) for the animation
	/// \param	numframes	-	the number of frames in the cycle
	/// 
	void Initialise(const float start,const float end,const unsigned int numframes)
	{
		m_iNumFrames = numframes;
		m_fStart     = start;
		m_fEnd       = end;
		m_fLength    = end-start;
		m_fFrameTime = m_fLength/numframes;
		m_fCurrTime  = start;
	}

	///-------------------------------------------------------------------------
	/// \brief	This function updates the time by the specified delta
	/// \param	dt	-	difference in time
	/// 
	void Update(const float dt) 
	{		
		m_fCurrTime += dt;

		while(m_fCurrTime > m_fEnd) 
		{
			m_fCurrTime -= m_fLength;
		}
	}

	///-------------------------------------------------------------------------
	/// \brief	this function returns the current frame number
	/// \return	the frame index
	/// 
	inline unsigned int GetFrameNum() const
	{
		return (unsigned int)( (float)(m_fCurrTime - m_fStart)/m_fFrameTime );
	}

	///-------------------------------------------------------------------------
	/// \brief	this function returns the interpolation t for the current frame.
	///			ie, interpolate frame n and n+1 by frameT
	/// 
	inline const float GetFrameT() const
	{
		return  fmodf( m_fCurrTime - m_fStart, m_fFrameTime ) / m_fFrameTime;
	}

	
	///-------------------------------------------------------------------------
	/// \brief	This function returns the t value over the whole animation length
	/// 
	inline const float GetInterpolatingT() const
	{
		return  (m_fCurrTime-m_fStart)/m_fLength;
	}

	/// the number of frames in the cycle
	unsigned int				m_iNumFrames;

protected:

	/// the time in seconds between each frame in the cycle.
	float						m_fFrameTime;

	/// the start time of the animation. 
	float						m_fStart;

	/// the end time of the animation
	float						m_fEnd;

	/// the length (in seconds) of the animation
	float						m_fLength;

	/// the current time in seconds
	float						m_fCurrTime;

};
