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
#ifndef _nsMath_h_
#define _nsMath_h_

#include <math.h>
#include <float.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>


typedef unsigned char   NSubyte;
typedef unsigned int    NSuint;
typedef float           NSfloat;

#define NS_INLINE       inline
#define NS_EPSILON      FLT_EPSILON
#define NS_INFINITY     FLT_MAX

// ----------------------------------------------------------------------------
static const NSfloat NSPi           = NSfloat(3.141592653589793238);
static const NSfloat NSPi2          = NSfloat(6.283185307179586476);
static const NSfloat NSPih          = NSfloat(1.570796326794896619);
static const NSfloat NSRPD          = NSfloat(0.017453292519943295);
static const NSfloat NSDPR          = NSfloat(57.295779513082320876);


// ----------------------------------------------------------------------------
NS_INLINE NSfloat   NSMax(NSfloat lhs, NSfloat rhs)                 { return (lhs) < (rhs) ? (rhs) : (lhs); }
NS_INLINE NSfloat   NSMin(NSfloat lhs, NSfloat rhs)                 { return (rhs) < (lhs) ? (rhs) : (lhs); }
NS_INLINE NSfloat   NSClamp(NSfloat x, NSfloat mi, NSfloat ma)      { return (x) <  (mi) ?  (mi)  : (x) > (ma) ? (ma) : (x); }
NS_INLINE NSfloat   NSAbs(NSfloat x)                                { return (x)  < ( 0.f ) ? -(x) : (x); }
NS_INLINE NSfloat   NSDegrees(NSfloat x)                            { return NSfloat(x) * NSDPR; }
NS_INLINE NSfloat   NSRadians(NSfloat x)                            { return NSfloat(x) * NSRPD; }
NS_INLINE bool      NSFuzzy(NSfloat x)                              { return NSAbs( x ) < NS_EPSILON; }
NS_INLINE bool      NSFuzzyT(NSfloat x, NSfloat tol)                { return NSAbs( x ) < tol; }
NS_INLINE bool      NSFuzzyEq(NSfloat x, NSfloat y)                 { return NSFuzzy(x - y); }
NS_INLINE bool      NSFuzzyEqT(NSfloat x, NSfloat y,  NSfloat tol)  { return NSFuzzyT(x - y, tol); }
NS_INLINE bool      NSIsInf(NSfloat x)                              { return NSAbs( x ) >= NS_INFINITY; }
NS_INLINE bool      NSIsNan(NSfloat x)                              { return (x != x);}
NS_INLINE bool      NSIsNumber(NSfloat x)                           { return !NSIsNan(x) || !NSIsInf(x); }
NS_INLINE NSfloat   NScolorf(NSfloat x)                             { return NSfloat(x) * NSfloat( 0.003921568627450980); }
NS_INLINE NSubyte   NScolorb(NSfloat x)                             { return (NSubyte)(x * NSfloat( 255.0 )); }
NS_INLINE NSfloat   NSColorClamp(NSfloat x)                         { return NSClamp(x, 0.f, 1.f); }
NS_INLINE NSfloat   NSATan2(NSfloat y, NSfloat x)                   { return NSfloat(atan2(double(y), double(x))); }
NS_INLINE NSfloat   NSFloor(NSfloat x)                              { return NSfloat(floor((double)x)); }
NS_INLINE NSfloat   NSCeil(NSfloat x)                               { return NSfloat(ceil((double)x));  }
NS_INLINE NSfloat   NSSqrt(NSfloat x)                               { return NSfloat(sqrt(double(x)));  }
NS_INLINE NSfloat   NSFmod(NSfloat x, NSfloat y)                    { return NSfloat(fmod(double(x), double(y)));  }
NS_INLINE NSfloat   NSRSqrt(NSfloat x)                              { return NSfloat(1.0) / NSfloat(sqrt(double(x)));}
NS_INLINE NSfloat   NSCos(NSfloat x)                                { return NSfloat(cos(double(x))); }
NS_INLINE NSfloat   NSSin(NSfloat x)                                { return NSfloat(sin(double(x)));}
NS_INLINE void      NSSinCos(NSfloat a,  NSfloat &s, NSfloat &c)    { s= NSSin(a); c= NSCos(a); }
NS_INLINE int       NSPow2(int n)                                   { --n; n |= n >> 16; n |= n >> 8; n |= n >> 4; n |= n >> 2; n |= n >> 1; ++n; return n; }


// ----------------------------------------------------------------------------
NS_INLINE size_t NSFloatCopy(NSfloat *dest, const NSfloat *src, size_t nr)
{
    size_t i= 0;
    for (; i < nr; i++) dest[i]= src[i];
    return i;
}


// ----------------------------------------------------------------------------
class NSvec2
{
public:

    NS_INLINE NSvec2() {}
    NS_INLINE NSvec2( NSfloat nx, NSfloat ny ) : x( nx ), y( ny ) {}
    NS_INLINE NSvec2( NSfloat *p ) : x( p[0] ), y( p[1] ) {}
    NS_INLINE NSvec2( const NSvec2 &v ) : x( v.x ), y( v.y ) {}

    NS_INLINE NSfloat *ptr()
    { return &x; }

    NS_INLINE const NSfloat *ptr() const
    { return &x; }

    NS_INLINE NSvec2 &operator = ( const NSvec2 &v )
    {x = v.x; y = v.y; return *this;}

    NS_INLINE NSvec2 operator / ( NSfloat v ) const
    {assert( v != 0.0 ); return NSvec2( x / v, y / v );}

    NS_INLINE NSvec2 operator / ( const NSvec2 &v ) const
    {return NSvec2( x / v.x, y / v.y );}

    NS_INLINE NSvec2 operator * ( NSfloat v ) const
    { return NSvec2( x*v, y*v ); }

    NS_INLINE NSvec2 operator * ( const NSvec2 &v ) const
    {return NSvec2( x*v.x, y*v.y );}

    NS_INLINE NSvec2 operator - ( const NSvec2 &v ) const
    {return NSvec2( x - v.x, y - v.y );}

    NS_INLINE NSvec2 operator - () const
    {return NSvec2( -x, -y );}

    NS_INLINE NSvec2 operator + ( const NSvec2 &v ) const
    {return NSvec2( x + v.x, y + v.y );}

    NS_INLINE NSfloat length() const
    {return NSfloat( NSSqrt( length2() ) );}

    NS_INLINE NSfloat length2() const
    {return x*x + y*y;}

    NS_INLINE NSvec2 abs() const
    {return NSvec2( NSAbs( x ), NSAbs( y ) );}

    NS_INLINE NSfloat distance( const NSvec2 &v ) const
    {return NSvec2( x - v.x, y - v.y ).length();}

    NS_INLINE NSfloat dot( const NSvec2 &v ) const
    {return x*v.x + y*v.y;}

    NS_INLINE void normalize()
    {
        NSfloat len = NSRSqrt( x * x + y * y );
        if ( NSIsNumber( len ) )
        {
            x *= len;
            y *= len;
        }
    }

    NS_INLINE bool valid() const
    { return NSIsNumber(x) && NSIsNumber(y); }

    NS_INLINE NSvec2 perpendicular() const
    { return NSvec2( -y, x ); }

    NS_INLINE NSvec2 slope( const NSvec2 &o ) const
    { return NSvec2( o.x - x, o.y - y ); }

    NS_INLINE bool isZero()
    {return NSFuzzy( x ) && NSFuzzy( y ); }


    NS_INLINE NSvec2 normalizeCpy() const
    { NSvec2 v( x, y ); v.normalize(); return v; }

    NS_INLINE NSvec2 &operator += ( const NSvec2 &v )
    { x += v.x; y += v.y; return *this; }

    NS_INLINE NSvec2 &operator += ( const NSfloat &v )
    { x += v; y += v; return *this; }

    NS_INLINE NSvec2 &operator -= ( const NSvec2 &v )
    { x -= v.x; y -= v.y; return *this; }

    NS_INLINE NSvec2 &operator -= ( const NSfloat &v )
    { x -= v; y -= v; return *this; }

    NS_INLINE NSvec2 &operator /= ( const NSfloat &v )
    { assert( v != 0 ); x /= v; y /= v; return *this; }

    NS_INLINE bool operator == ( const NSvec2 &v ) const
    { return x == v.x && y == v.y; }

    NS_INLINE bool operator != ( const NSvec2 &v ) const
    { return x != v.x || y != v.y; }

    friend NS_INLINE NSvec2 operator * ( NSfloat s, const NSvec2 &v )
    { return NSvec2( v.x*s, v.y*s ); }


    NSfloat x, y;
};


class NSvec3
{
public:

    NSvec3() {}
    NSvec3(NSfloat nx, NSfloat ny, NSfloat nz) : x(nx), y(ny), z(nz) {}
    NSvec3(NSfloat *p) : x(p[0]), y(p[1]), z(p[2]) {}
    NSvec3(const NSvec3 &v) : x(v.x), y(v.y), z(v.z) {}

    NS_INLINE NSfloat *ptr()
    { return &x; }

    NS_INLINE const NSfloat *ptr() const
    { return &x; }

    NS_INLINE bool operator == ( const NSvec3 &v ) const
    { return x == v.x && y == v.y && z == v.z; }

    NSfloat x, y, z;
};

class NSvec4
{
public:

    NSvec4() {}
    NSvec4(NSfloat nx, NSfloat ny, NSfloat nz, NSfloat nw) : x(nx), y(ny), z(nz), w(nw) {}
    NSvec4(NSfloat *p) : x(p[0]), y(p[1]), z(p[2]), w(p[3]) {}
    NSvec4(const NSvec4 &v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

    NS_INLINE NSfloat *ptr()
    { return &x; }

    NS_INLINE const NSfloat *ptr() const
    { return &x; }
    NS_INLINE bool operator == ( const NSvec4 &v ) const
    { return x == v.x && y == v.y && z == v.z && w == v.w; }

    NSfloat x, y, z, w;
};



class NSquat
{
public:

    NSquat() {}
    NSquat(NSfloat nw, NSfloat nx, NSfloat ny, NSfloat nz) : w(nw), x(nx), y(ny), z(nz) {}
    NSquat(NSfloat *p) : x(p[0]), y(p[1]), z(p[2]), w(p[3]) {}
    NSquat(const NSvec4 &v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

    NS_INLINE NSfloat *ptr()
    { return &x; }

    NS_INLINE const NSfloat *ptr() const
    { return &x; }

    NS_INLINE bool operator == ( const NSquat &v ) const
    { return  w == v.w && x == v.x && y == v.y && z == v.z; }

    NSfloat w, x, y, z;
};


class NSrect
{
public:
    NS_INLINE NSrect() {}

    NS_INLINE NSrect(NSfloat xv, NSfloat yv, NSfloat wv, NSfloat hv) :
        x(xv), y(yv), width(wv), height(hv)
    {
    }

    NS_INLINE NSrect &operator= (const NSrect &o)
    { x= o.x; y= o.y; width= o.width; height= o.height; return *this; }

    NS_INLINE NSvec2 getSize() const
    { return NSvec2(width, height); }

    NS_INLINE NSvec2 getPosition() const
    { return NSvec2(x, y); }

    NS_INLINE NSfloat getWidth() const
    { return width; }

    NS_INLINE NSfloat getHeight() const
    { return height; }

    NS_INLINE NSfloat getLeft() const
    { return x; }

    NS_INLINE NSfloat getRight() const
    { return x + width; }

    NS_INLINE NSfloat getTop() const
    { return y; }

    NS_INLINE NSfloat getBottom() const
    { return y + height; }

    NS_INLINE void setSize(const NSvec2 &s)
    { width= s.x; height= s.y; }

    NS_INLINE void setSize(NSfloat sx, NSfloat sy)
    { width= sx; height= sy; }

    NS_INLINE void setPosition(const NSvec2 &p)
    { x= p.x; y= p.y; }

    NS_INLINE void setPosition(NSfloat px, NSfloat py)
    { x= px; y= py; }

    NS_INLINE NSvec2 getLeftTop() const
    { return NSvec2(x, y); }

    NS_INLINE NSvec2 getRightTop() const
    { return NSvec2(x + width, y); }

    NS_INLINE NSvec2 getLeftBottom() const
    { return NSvec2(x, y + height); }

    NS_INLINE NSvec2 getRightBottom() const
    { return NSvec2(x + width, y + height); }

    NS_INLINE void getCorners(NSvec2 &lt, NSvec2 &rt, NSvec2 &lb, NSvec2 &rb) const
    {
        lt= getLeftTop();      rt= getRightTop();
        lb= getLeftBottom();   rb= getRightBottom();
    }


    NS_INLINE void offset(const NSvec2 &v)
    { x += v.x; y += v.y; }

    NS_INLINE void offset(NSfloat vx, NSfloat vy)
    { x += vx; y += vy; }

    NS_INLINE void expand(NSfloat vx, NSfloat vy)
    {
        NSfloat r= x + width;
        NSfloat b= y + height;

        x -= vx; y -= vy;
        r += vx; b += vy;

        width= r - x;
        height= b - y;
    }

    NS_INLINE void expand(const NSvec2 &v)
    {
        expand(v.x, v.y);
    }

    NS_INLINE void contract(const NSvec2 &v)
    {
        expand(-v.x, -v.y);
    }

    NS_INLINE void contract(NSfloat vx, NSfloat vy)
    {
        expand(-vx, -vy);
    }

    NS_INLINE bool contains(NSfloat vx, NSfloat vy) const
    { return (vx >= x && vy >= y && ((vx - x) <= width) && ((vy - y) <= height)); }


    NS_INLINE bool containsX(NSfloat vx) const
    { return (vx >= x && ((vx - x) <= width)); }

    NS_INLINE bool containsY(NSfloat vy) const
    { return (vy >= y && ((vy - y) <= height)); }

    NS_INLINE bool contains(const NSvec2 &v) const
    { return contains(v.x, v.y); }

    NS_INLINE bool contains(const NSrect &rect) const
    {
        if (contains(rect.getLeftTop()))
            return true;
        if (contains(rect.getRightTop()))
            return true;
        if (contains(rect.getRightBottom()))
            return true;
        if (contains(rect.getLeftBottom()))
            return true;
        if (contains(rect.center()))
            return true;
        return false;
    }

    NS_INLINE bool clipped(NSfloat tx, NSfloat ty, NSfloat tw, NSfloat th) const
    {
        if (tx > (x + width))
            return true;
        if (ty > (y + height))
            return true;
        if ((tx + tw) < x)
            return true;
        if ((ty + th) < y)
            return true;
        return false;
    }

    NS_INLINE bool clipped(NSfloat tx, NSfloat ty, NSfloat tw, NSfloat th, const int dir) const
    {
        switch (dir)
        {
        case 0:     // left
            if ((tx + tw) < x)
                return true;
            break;
        case 1:     // bottom
            if (ty > (y + height))
                return true;
            break;
        case 2:     // right
            if (tx > (x + width))
                return true;
            break;
        case 3:     // top
            if ((ty + th) < y)
                return true;
            break;

        }
        return false;
    }

    NS_INLINE bool clipped(const NSrect &rct) const
    { return clipped(rct.x, rct.y, rct.width, rct.height); }

    NS_INLINE void getBounds(NSfloat &xmin, NSfloat &ymin, NSfloat &xmax, NSfloat &ymax) const
    {
        xmin= (x);
        ymin= (y);
        xmax= (x + width);
        ymax= (y + height);
    }

    NS_INLINE void getAbsBounds(NSfloat &xmin, NSfloat &ymin, NSfloat &xmax, NSfloat &ymax) const
    {
        xmin= NSAbs(x);
        ymin= NSAbs(y);
        xmax= xmin + width;
        ymax= ymin + height;
    }


    NS_INLINE NSfloat getAspect() const
    { return width / height; }

    NS_INLINE NSvec2 center() const
    { return NSvec2(x + (width * NSfloat(0.5)), y + (height * NSfloat(0.5))); }

    NS_INLINE void makeMinMax()
    {
        NSfloat xmin, ymin, xmax, ymax;

        xmin= getLeft();
        xmax= getRight();
        ymin= getTop();
        ymax= getBottom();

        x= NSMin(xmin, xmax);
        y= NSMin(ymin, ymax);

        width=  NSMax(xmin, xmax) - NSMin(xmin, xmax);
        height= NSMax(ymin, ymax) - NSMin(ymin, ymax);
    }


    NS_INLINE NSrect makeMinMaxCopy() const
    {
        NSrect r= *this;
        r.makeMinMax();
        return r;
    }

    NSfloat x, y;
    NSfloat width, height;
};


class NSmat3
{
    /// 3x3 transformation matrix

public:
    NS_INLINE NSmat3() {}

    NS_INLINE NSmat3(  NSfloat _xx, NSfloat _xy, NSfloat _tx,
                       NSfloat _yx, NSfloat _yy, NSfloat _ty,
                       NSfloat _zx, NSfloat _zy, NSfloat _zz )
    {
        m[0][0] = _xx; m[0][1] = _xy; m[0][2] = _tx;
        m[1][0] = _yx; m[1][1] = _yy; m[1][2] = _ty;
        m[2][0] = _zx; m[2][1] = _zy; m[2][2] = _zz;
    }

    NS_INLINE NSmat3( NSfloat *p )
    { NSFloatCopy(ptr, p, 9); }

    NS_INLINE NSmat3( const NSmat3 &v )
    { NSFloatCopy(ptr, v.ptr, 9); }

    NS_INLINE void setTranslation( const NSvec2 &v )        {m[0][2] = v.x;  m[1][2] = v.y;}
    NS_INLINE void setTranslation( NSfloat x, NSfloat y )   {m[0][2] = x;    m[1][2] = y;}
    NS_INLINE void setScale( const NSvec2 &s )              {m[0][0] = s.x;  m[1][1] = s.y;}
    NS_INLINE void setScale( NSfloat x, NSfloat y )         {m[0][0] = x;    m[1][1] = y;}

    NS_INLINE void setTransform( const NSrect &v )
    {
        m[0][0] = v.width;   m[0][1] = 0.f;         m[0][2] = v.x;
        m[1][0] = 0.f;       m[1][1] = v.height;    m[1][2] = v.y;
        m[2][0] = 0.f;       m[2][1] = 0.f;         m[2][2] = 1.f;
    }

    NS_INLINE NSmat3 operator * ( const NSmat3 &B ) const
    {
        return NSmat3(
                   m[0][0] * B.m[0][0] + m[0][1] * B.m[1][0] + m[0][2] * B.m[2][0],
                   m[0][0] * B.m[0][1] + m[0][1] * B.m[1][1] + m[0][2] * B.m[2][1],
                   m[0][0] * B.m[0][2] + m[0][1] * B.m[1][2] + m[0][2] * B.m[2][2],

                   m[1][0] * B.m[0][0] + m[1][1] * B.m[1][0] + m[1][2] * B.m[2][0],
                   m[1][0] * B.m[0][1] + m[1][1] * B.m[1][1] + m[1][2] * B.m[2][1],
                   m[1][0] * B.m[0][2] + m[1][1] * B.m[1][2] + m[1][2] * B.m[2][2],

                   m[2][0] * B.m[0][0] + m[2][1] * B.m[1][0] + m[2][2] * B.m[2][0],
                   m[2][0] * B.m[0][1] + m[2][1] * B.m[1][1] + m[2][2] * B.m[2][1],
                   m[2][0] * B.m[0][2] + m[2][1] * B.m[1][2] + m[2][2] * B.m[2][2]
               );
    }

    NS_INLINE NSvec2 operator * ( const NSvec2 &v ) const
    { return NSvec2(  m[0][0] * v.x + m[0][1] * v.y + m[0][2], m[1][0] * v.x + m[1][1] * v.y + m[1][2] ); }

    NS_INLINE bool operator == ( const NSmat3 &v ) const
    { return !memcmp(&ptr[0], &v.ptr[0], 9); }

    union
    {
        NSfloat m[3][3];
        NSfloat ptr[9];
    };

    static const NSmat3 IDENTITY;
};


class NSmat4
{
public:
    NS_INLINE NSmat4() {}
    NS_INLINE NSmat4(NSfloat m00, NSfloat m01, NSfloat m02, NSfloat m03,
                     NSfloat m10, NSfloat m11, NSfloat m12, NSfloat m13,
                     NSfloat m20, NSfloat m21, NSfloat m22, NSfloat m23,
                     NSfloat m30, NSfloat m31, NSfloat m32, NSfloat m33
                    )
    {
        vec[0][0]= m00; vec[0][1]= m01; vec[0][2]= m02; vec[0][3]= m03;
        vec[1][0]= m10; vec[1][1]= m11; vec[1][2]= m12; vec[1][3]= m13;
        vec[2][0]= m20; vec[2][1]= m21; vec[2][2]= m22; vec[2][3]= m23;
        vec[3][0]= m30; vec[3][1]= m31; vec[3][2]= m32; vec[3][3]= m33;
    }

    NS_INLINE NSmat4(const NSmat3 &v)
    {
        vec[0][0]= v.m[0][0]; vec[0][1]= v.m[0][1]; vec[0][2]= 0.f; vec[0][3]= v.m[0][2];
        vec[1][0]= v.m[1][0]; vec[1][1]= v.m[1][1]; vec[1][2]= 0.f; vec[1][3]= v.m[1][2];
        vec[2][0]= 0.f;       vec[2][1]= 0.f;       vec[2][2]= 1.f; vec[2][3]= 0.f;
        vec[3][0]= 0.f;       vec[3][1]= 0.f;       vec[3][2]= 0.f; vec[3][3]= 1.f;
    }

    NS_INLINE NSmat4(const NSmat4 &v)
    {
        *this= v;
    }

    NS_INLINE NSmat4(const NSfloat *v)
    {
        vec[0][0]= *v++; vec[0][1]= *v++;  vec[0][2]= *v++;  vec[0][3]= *v++;
        vec[1][0]= *v++; vec[1][1]= *v++;  vec[1][2]= *v++;  vec[1][3]= *v++;
        vec[2][0]= *v++; vec[2][1]= *v++;  vec[2][2]= *v++;  vec[2][3]= *v++;
        vec[3][0]= *v++; vec[3][1]= *v++;  vec[3][2]= *v++;  vec[3][3]= *v;
    }

    NS_INLINE NSmat4 &operator= (const NSmat4 &v)
    {
        vec[0][0]= v.vec[0][0]; vec[0][1]= v.vec[0][1]; vec[0][2]= v.vec[0][2]; vec[0][3]= v.vec[0][3];
        vec[1][0]= v.vec[1][0]; vec[1][1]= v.vec[1][1]; vec[1][2]= v.vec[1][2]; vec[1][3]= v.vec[1][3];
        vec[2][0]= v.vec[2][0]; vec[2][1]= v.vec[2][1]; vec[2][2]= v.vec[2][2]; vec[2][3]= v.vec[2][3];
        vec[3][0]= v.vec[3][0]; vec[3][1]= v.vec[3][1]; vec[3][2]= v.vec[3][2]; vec[3][3]= v.vec[3][3];
        return *this;
    }

    NS_INLINE NSmat4 transpose(void) const
    {
        return NSmat4(vec[0][0], vec[1][0], vec[2][0], vec[3][0],
                      vec[0][1], vec[1][1], vec[2][1], vec[3][1],
                      vec[0][2], vec[1][2], vec[2][2], vec[3][2],
                      vec[0][3], vec[1][3], vec[2][3], vec[3][3]);
    }
    NS_INLINE bool operator == ( const NSmat4 &v ) const
    { return !memcmp(&ptr[0], &v.ptr[0], 16); }

    union
    {
        NSfloat vec[4][4];
        NSfloat ptr[16];
    };

    static const NSmat4 IDENTITY;
};

class NSmath
{
public:

    static NS_INLINE void ortho2D(NSmat3 &dest, NSfloat l, NSfloat t, NSfloat r, NSfloat b)
    {
        NSfloat dx= (r - l), dy= (t - b);

        if (NSFuzzy(dx) || NSFuzzy(dy))
        {
            dest= NSmat3::IDENTITY;
            return;
        }

        dx= 1.f / dx;
        dy= 1.f / dy;
        dest= NSmat3(
                  2.f * dx,   0.f, (-(r + l) * dx),
                  0.f,        2.f  * dy, (-(t + b) * dy),
                  0.f,        0.f,        1.f
              );
    }

    static NS_INLINE void ortho2D(NSmat4 &dest, NSfloat l, NSfloat t, NSfloat r, NSfloat b)
    {
        NSfloat dx= (r - l), dy= (t - b);

        if (NSFuzzy(dx) || NSFuzzy(dy))
        {
            dest= NSmat4::IDENTITY;
            return;
        }

        dx= 1.f / dx;
        dy= 1.f / dy;
        dest= NSmat4(
                  2.f * dx,   0.f,        0.f,  -(r + l) * dx,
                  0.f,        2.f  * dy,  0.f,  -(t + b) * dy,
                  0.f,        0.f,       -1.f,  0.f,
                  0.f,        0.f,        0.f,  1.f
              );
    }
};



struct NSColorConvert
{
    static NS_INLINE void toFloat(NSuint v, NSfloat &r, NSfloat &g, NSfloat &b, NSfloat &a)
    {
        union
        {
            NSubyte pixel[4];
            NSuint  color;
        } color;

        color.color= v;
        r= NScolorf(color.pixel[3]);
        g= NScolorf(color.pixel[2]);
        b= NScolorf(color.pixel[1]);
        a= NScolorf(color.pixel[0]);
    }

    static NS_INLINE void toFloatRGB(NSuint v, NSfloat &r, NSfloat &g, NSfloat &b, NSfloat &a)
    {
        union
        {
            NSubyte pixel[3];
            NSuint  color;
        } color;

        color.color= v;
        r= NScolorf(color.pixel[2]);
        g= NScolorf(color.pixel[1]);
        b= NScolorf(color.pixel[0]);
        a= 1.f;
    }

    static NS_INLINE NSuint toInt(NSfloat r, NSfloat g, NSfloat b, NSfloat a)
    {
        union
        {
            NSubyte pixel[4];
            NSuint  color;
        } color;

        color.pixel[3]= NScolorb(r);
        color.pixel[2]= NScolorb(g);
        color.pixel[1]= NScolorb(b);
        color.pixel[0]= NScolorb(a);
        return color.color;
    }

};


class NScolor
{
public:
    NS_INLINE NScolor() : r(1.0), g(1.0), b(1.0), a(1.0) {}
    NS_INLINE NScolor(NSfloat rc, NSfloat gc, NSfloat bc, NSfloat ac= 1.0) :
        r(NSColorClamp(rc)), g(NSColorClamp(gc)), b(NSColorClamp(bc)), a(NSColorClamp(ac)) {}

    NS_INLINE NScolor(NSuint v)
    {
        NSColorConvert::toFloat(v, r, g, b, a);
    }

    NS_INLINE NScolor &operator=(NSuint v)
    {
        NSColorConvert::toFloat(v, r, g, b, a);
        return *this;
    }

    NS_INLINE NScolor &operator=(const NScolor &o)
    { r= o.r; g= o.g; b= o.b; a= o.a; return *this; }


    NS_INLINE operator NSuint() const
    { return NSColorConvert::toInt(r, g, b, a); }

    NS_INLINE NSuint RGBA() const
    { return NSColorConvert::toInt(r, g, b, a); }

    NS_INLINE NSuint BGRA() const
    { return NSColorConvert::toInt(b, g, r, a); }

    NS_INLINE NSuint ARGB() const
    { return NSColorConvert::toInt(a, r, g, b); }

    NS_INLINE NSuint ABGR() const
    { return NSColorConvert::toInt(a, b, g, r); }

    NS_INLINE NSuint LUMINANCE() const
    { return NSColorConvert::toInt(r, r, r, 1.f); }

    NS_INLINE NSuint LUMINANCE_ALPHA() const
    { return NSColorConvert::toInt(r, r, r, a); }

    NS_INLINE NScolor blend(const NScolor &bc, NSfloat t) const
    {
        NSfloat s= NSfloat(1.0) - t;

        return NScolor((NSfloat)s * r + t * bc.r,
                       (NSfloat)s * g + t * bc.g,
                       (NSfloat)s * b + t * bc.b,
                       (NSfloat)s * a + t * bc.a);
    }


    NS_INLINE void clamp(void)
    {
        r = NSColorClamp(r);
        g = NSColorClamp(g);
        b = NSColorClamp(b);
        a = NSColorClamp(a);
    }

    NS_INLINE NScolor operator *(NSfloat fac) const
    {
        NScolor v;
        v.r = r * fac;
        v.g = g * fac;
        v.b = b * fac;
        v.a = a * fac;
        return v;
    }
    NS_INLINE NScolor operator /(NSfloat fac) const
    {
        if (NSFuzzy(fac))
            fac = 1.f;
        NScolor v;
        v.r = r / fac;
        v.g = g / fac;
        v.b = b / fac;
        v.a = a / fac;
        return v;
    }

    NS_INLINE NScolor operator +(NSfloat fac) const
    {
        NScolor v;
        v.r = r + fac;
        v.g = g + fac;
        v.b = b + fac;
        v.a = a + fac;
        return v;
    }
    NS_INLINE NScolor operator -(NSfloat fac) const
    {
        NScolor v;
        v.r = r - fac;
        v.g = g - fac;
        v.b = b - fac;
        v.a = a - fac;
        return v;
    }

    NS_INLINE NScolor operator *(const NScolor &fac) const
    {
        NScolor v;
        v.r = r * fac.r;
        v.g = g * fac.g;
        v.b = b * fac.b;
        v.a = a * fac.a;
        return v;
    }
    NS_INLINE NScolor operator /(const NScolor &fac) const
    {
        NScolor v;
        v.r = r / fac.r;
        v.g = g / fac.g;
        v.b = b / fac.b;
        v.a = a / fac.a;
        return v;
    }
    NS_INLINE NScolor operator +(const NScolor &fac) const
    {
        NScolor v;
        v.r = r + fac.r;
        v.g = g + fac.g;
        v.b = b + fac.b;
        v.a = a + fac.a;
        return v;
    }
    NS_INLINE NScolor operator -(const NScolor &fac) const
    {
        NScolor v;
        v.r = r - fac.r;
        v.g = g - fac.g;
        v.b = b - fac.b;
        v.a = a - fac.a;
        return v;
    }

    NSfloat r, g, b, a;
};


// ----------------------------------------------------------------------------
class NSaabb
{
public:
    NSaabb() : m_min(NS_INFINITY, NS_INFINITY), m_max(-NS_INFINITY, -NS_INFINITY) {}

    NSaabb(NSfloat xmin, NSfloat ymin, NSfloat xmax, NSfloat ymax) :
        m_min(xmin, ymin), m_max(xmax, ymax)
    {
    }

    NSaabb(const NSrect &r)
    { r.getBounds(m_min.x, m_min.y, m_max.x, m_max.y); }

    NS_INLINE void clear()
    {
        m_min.x=  NS_INFINITY; m_min.y=  NS_INFINITY;
        m_max.x= -NS_INFINITY; m_max.y= -NS_INFINITY;
    }

    NS_INLINE NSrect getRect() const
    { return NSrect(m_min.x, m_min.y, m_max.x - m_min.x, m_max.y - m_min.y); }

    NS_INLINE void normalize()
    {
        NSrect r(m_min.x, m_min.y, m_max.x - m_min.x, m_max.y - m_min.y);
        r.makeMinMax();
        r.getBounds(m_min.x, m_min.y, m_max.x, m_max.y);
    }

    NS_INLINE bool valid()
    {
        return !(m_max - m_min).isZero();
    }

    NS_INLINE void compair(NSfloat x, NSfloat y)
    {
        if (x < m_min.x) m_min.x= x;
        if (x > m_max.x) m_max.x= x;
        if (y < m_min.y) m_min.y= y;
        if (y > m_max.y) m_max.y= y;
    }

    NSvec2 m_min;
    NSvec2 m_max;
};


#endif//_nsMath_h_
