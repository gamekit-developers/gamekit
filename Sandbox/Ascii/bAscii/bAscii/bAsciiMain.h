/*
-------------------------------------------------------------------------------
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C
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
#ifndef _bAsciiMain_h_
#define _bAsciiMain_h_

#include "bUtils/utArray.h"
#include "bApi/Blender.h"
#include "bAsciiCommon.h"

// ----------------------------------------------------------------------------
class bAsciiMain  : public utAllocObject
{
protected:

	bAscii *m_parent;

	void clearMain(void);
	void freeObject(void);
	void freeCamera(void);
	void freeMesh(void);
	void freeScene(void);


public:

	typedef utArray<Blender::Object*>	ObjectArray;
	typedef utArray<Blender::Camera*>	CameraArray;
	typedef utArray<Blender::Mesh*>		MeshArray;
	typedef utArray<Blender::Scene*>	SceneArray;

	ObjectArray		object;
	CameraArray		camera;
	MeshArray		mesh;
	SceneArray		scene;


	bAsciiMain(bAscii *parent) : m_parent(parent) {}
	~bAsciiMain() { clearMain(); }


	template<typename T>
	T find(const char *name, utArray<T> &array)
	{
		if (!array.empty())
		{
			T first = array.at(0);
			while (first)
			{
				if (utCharEq(name, (first->id.name+2)))
				{
					return first;
				}
				first = static_cast<T>(first->id.next);
			}
		}
		return 0;
	}
};


#endif//_bAsciiMain_h_
