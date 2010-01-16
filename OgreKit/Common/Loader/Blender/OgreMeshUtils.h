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
#ifndef _OgreMeshUtils_h_
#define _OgreMeshUtils_h_


/// may not be nessary
/// but add some limit
#define LIMIT 1000000


#define VEC3SUB(a,b,d) {d[0]= a[0] - b[0]; d[1]= a[1] - b[1]; d[2]= a[2] - b[2];}
#define VEC2SUB(a,b,d) {d[0]= a[0] - b[0]; d[1]= a[1] - b[1];}
#define VEC3SUB2(a,b,d) {d[0]= a.x - b.x; d[1]= a.y - b.y; d[2]= a.z - b.z;}
#define VEC2SUB2(a,b,d) {d[0]= a.x - b.x; d[1]= a.y - b.y;}

#define VEC3DOT(a)	  (a[0] * a[0] + a[1] * a[1] + a[2] * a[2])
#define VEC2DOT(a)	  (a[0] * a[0] + a[1] * a[1])

#define VEC3ZRO(a)	  {a[0]= 0.f; a[1]=0.f; a[2]=0.f;}
#define VEC2ZRO(a)	  {a[0]= 0.f; a[1]= 0.f;}


// copy
#define VEC3CPY(a, b) {a[0]= b[0]; a[1]= b[1]; a[2]= b[2];}
#define VEC2CPY(a, b) {a[0]= b[0]; a[1]= b[1];}

#define VEC3CPY2(a, b) {a.x= b.x; a.y= b.y; a.z= b.z;}
#define VEC2CPY2(a, b) {a.x= b.x; a.y= b.y;}

// float normal from short
#define VEC3CPN(a, b) {a[0]= (b[0]/32767.f); a[1]= (b[1]/32767.f); a[2]= (b[2]/32767.f);}

/// equal tests
#define TESTBEQ(val) (val == b.val)
#define TESTNEQ(val) (val != b.val)
#define TESTBLE(val) (val < b.val)
#define SETTEST(val) (val= b.val)


#define MINTEST(a,b) (((a)<(b))?(a):(b))
#define MAXTEST(a,b) (((a)>(b))?(a):(b))

/// Test to enable only (real) materials
#define SORT_BY_MATERIAL 1

typedef size_t Index;
#define TOL 1e-10

#define VEC2F(v) ((float*)&(v))
#define VEC2D(v) ((double*)&(v))


#endif//_OgreMeshUtils_h_
