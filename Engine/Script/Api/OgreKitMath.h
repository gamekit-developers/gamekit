/*
-------------------------------------------------------------------------------
--                                                                           --
--  This file is part of GameKit.                                            --
--  http://gamekit.googlecode.com/                                           --
--                                                                           --
--                                                                           --
--  Released under the zlib license.                                         --
--  http://www.gzip.org/zlib/zlib_license.html                               --
--                                                                           --
--                                                                           --
-------------------------------------------------------------------------------
-- OgreKit Math Bindings
*/
#ifndef _OgreKitMath_h_
#define _OgreKitMath_h_

#include "OgreKitCommon.h"


namespace OgreKit
{


// ----------------------------------------------------------------------------
class Vector3 
#ifndef SWIG
    : public gkVector3
#endif
{
public:


#ifdef SWIG
    float x, y, z;
#else
    Vector3(const gkVector3& rhs) : gkVector3(rhs.x, rhs.y, rhs.z) {}
#endif
    Vector3() : gkVector3(0,0,0) {}
    Vector3(float _x, float _y, float _z) : gkVector3(_x,_y,_z) {}
    Vector3(const Vector3& rhs) : gkVector3(rhs.x, rhs.y, rhs.z) {}

    const char *__str__(void)
    {
        static char buffer[72];
        sprintf(buffer, "%f, %f, %f", x, y, z);
        return buffer;
    }

    float       normalize(void)                 { return normalise(); }
    float       dot(const Vector3& v)        { return dotProduct(v); }
    Vector3  cross(const Vector3& v)      { return Vector3(crossProduct(v));}
    float       length(void)                    { return gkVector3::length(); }
    float       length2(void)                   { return gkVector3::squaredLength(); }
    float       distance(const Vector3& v)   { return gkVector3::distance(v); }
    float       distance2(const Vector3& v)  { return gkVector3::squaredDistance(v); }

    Vector3 operator- (void)                 { return -(gkVector3)*this; }
    Vector3 operator+ (const Vector3& v)  { return ((const gkVector3&)*this).operator+(v); }
    Vector3 operator- (const Vector3& v)  { return ((const gkVector3&)*this).operator-(v); }
    Vector3 operator* (const Vector3& v)  { return ((const gkVector3&)*this).operator*(v); }
    Vector3 operator* (float v)              { return ((const gkVector3&)*this).operator*(v); }
    Vector3 operator/ (const Vector3& v)  { return ((const gkVector3&)*this).operator/(v); }
    Vector3 operator/ (float v)              { return ((const gkVector3&)*this).operator/(v); }
};

// ----------------------------------------------------------------------------
class Quaternion 
#ifndef SWIG
    : public gkQuaternion
#endif
{
public:

#ifdef SWIG
    float w, x, y, z;
#else
    Quaternion(const gkQuaternion& rhs) : gkQuaternion(rhs.w, rhs.x, rhs.y, rhs.z) {}
#endif
    Quaternion() : gkQuaternion(1, 0,0,0) {}
    Quaternion(float _w, float _x, float _y, float _z) : gkQuaternion(_w, _x,_y,_z) {}
    Quaternion(const Quaternion& rhs) : gkQuaternion(rhs.w, rhs.x, rhs.y, rhs.z) {}


    const char *__str__(void)
    {
        static char buffer[72];
        sprintf(buffer, "%f, %f, %f, %f", w, x, y, z);
        return buffer;
    }

    float       normalize(void)                 { return normalise(); }
    float       length2(void)                   { return Norm(); }
    float       dot(const Quaternion& v)        { return Dot(v); }
    Quaternion  inverse(void)                   { return Inverse(); }
    Quaternion operator- (void)                 { return -(gkQuaternion)*this; }
    Quaternion operator+ (const Quaternion& v)  { return ((const gkQuaternion&)*this).operator+(v); }
    Quaternion operator- (const Quaternion& v)  { return ((const gkQuaternion&)*this).operator-(v); }
    Quaternion operator* (const Quaternion& v)  { return ((const gkQuaternion&)*this).operator*(v); }
    Quaternion operator* (float v)              { return ((const gkQuaternion&)*this).operator*(v); }
    Vector3    operator* (const Vector3& v)     { return ((const gkQuaternion&)*this).operator*(v); }
};


class Ray
#ifndef SWIG
    : public Ogre::Ray
#endif
{
public:
    Ray() {}
    Ray(const Vector3& origin, const Vector3& direction) : Ogre::Ray(origin, direction) {}

    void setOrigin(const Vector3& origin)       {Ogre::Ray::setOrigin(origin);} 
    void setDirection(const Vector3& origin)    {Ogre::Ray::setDirection(origin);} 

    Vector3 getOrigin()       {return Ogre::Ray::getOrigin();} 
    Vector3 getDirection()    {return Ogre::Ray::getDirection();} 
};



// no operator support in V8
inline Vector3 Vec3Negate(const Vector3 &in)
{
    return -(gkVector3)in;
}
inline Vector3 Vec3AddVec3(const Vector3 &a, const Vector3 &b)
{
    return (gkVector3)a + (gkVector3)b;
}
inline Vector3 Vec3SubVec3(const Vector3 &a, const Vector3 &b)
{
    return (gkVector3)a - (gkVector3)b;
}
inline Vector3 Vec3MulVec3(const Vector3 &a, const Vector3 &b)
{
    return (gkVector3)a / (gkVector3)b;
}
inline Vector3 Vec3DivVec3(const Vector3 &a, const Vector3 &b)
{
    return (gkVector3)a / (gkVector3)b;
}
inline Vector3 Vec3Mulf(const Vector3 &a, const float &b)
{
    return (gkVector3)a / b;
}
inline Vector3 Vec3Divf(const Vector3 &a, const float &b)
{
    return (gkVector3)a / b;
}
inline Quaternion QuatNegate(const Quaternion& in)
{
    return -(gkQuaternion)in;
}
inline Quaternion QuatAddQuat(const Quaternion& a, const Quaternion& b)
{
    return (gkQuaternion)a + (gkQuaternion)b;
}
inline Quaternion QuatSubQuat(const Quaternion& a, const Quaternion& b)
{
    return (gkQuaternion)a - (gkQuaternion)b;
}
inline Quaternion QuatMulQuat(const Quaternion& a, const Quaternion& b)
{
    return (gkQuaternion)a * (gkQuaternion)b;
}
inline Quaternion QuatMulf(const Quaternion& a, const float& b)
{
    return (gkQuaternion)a * b;
}
inline Vector3 QuatMulVec3(const Quaternion& a, const Vector3& b)
{
    return (gkQuaternion)a * (gkVector3)b;
}


}


#endif//_OgreKitMath_h_
