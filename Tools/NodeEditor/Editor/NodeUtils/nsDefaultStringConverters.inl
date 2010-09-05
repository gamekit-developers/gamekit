/*
-------------------------------------------------------------------------------
    General Purpose Utility Library, should be kept dependency free.
    Unless the dependency can be compiled along with this library.

    Copyright (c) 2009-2010 Charlie C.
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


// ----------------------------------------------------------------------------
UT_INLINE void nsFromString(const nsString& s, bool &v)
{ 
    v = (s == "true") ? true : nsStringConverter::toInt(s) != 0; 
}


// ----------------------------------------------------------------------------
UT_INLINE void nsFromString(const nsString& s, int &v)
{ 
    v = nsStringConverter::fromString<int>(s); 
}

// ----------------------------------------------------------------------------
UT_INLINE void nsFromString(const nsString& s, NSfloat &v)   
{ 
    v = nsStringConverter::fromString<NSfloat>(s); 
}

// ----------------------------------------------------------------------------
UT_INLINE void nsFromString(const nsString& s, nsString &v)
{ 
    v = s; 
}

// ----------------------------------------------------------------------------
UT_INLINE void nsFromString(const nsString& s, NSvec2 &v)
{ 
    v = nsStringConverter::toVec2(s); 
}

// ----------------------------------------------------------------------------
UT_INLINE void nsFromString(const nsString& s, NSvec3 &v)
{ 
    v = nsStringConverter::toVec3(s); 
}
// ----------------------------------------------------------------------------
UT_INLINE void nsFromString(const nsString& s, NSvec4 &v)
{ 
    v = nsStringConverter::toVec4(s); 
}

// ----------------------------------------------------------------------------
UT_INLINE void nsFromString(const nsString& s, NSquat &v)
{ 
    v = nsStringConverter::toQuat(s); 
}

// ----------------------------------------------------------------------------
UT_INLINE void nsFromString(const nsString& s, NSrect &v)
{ 
    v = nsStringConverter::toRect(s); 
}
// ----------------------------------------------------------------------------
UT_INLINE void nsFromString(const nsString& s, void *v)
{ 
    v = nsStringConverter::fromString<void*>(s); 
}
// ----------------------------------------------------------------------------
UT_INLINE nsString nsToString(bool v)
{ 
    return v ? "true" : "false"; 
}

// ----------------------------------------------------------------------------
UT_INLINE nsString nsToString(int v)
{
    return nsStringConverter::toString<int>(v); 
}
// ----------------------------------------------------------------------------
UT_INLINE nsString nsToString(NSfloat v)
{ 
    return nsStringConverter::toString<NSfloat>(v); 
}
// ----------------------------------------------------------------------------
UT_INLINE nsString nsToString(const nsString &v)
{ 
    return v;
}

// ----------------------------------------------------------------------------
UT_INLINE nsString nsToString(const NSvec2& v, char split = nsStringConverter::nsStringSplitter)    
{ 
    return nsStringConverter::toStringArray<NSfloat, 2>(v.ptr(), split); 
}
// ----------------------------------------------------------------------------
UT_INLINE nsString nsToString(const NSvec3& v, char split = nsStringConverter::nsStringSplitter)
{ 
    return nsStringConverter::toStringArray<NSfloat, 3>(v.ptr(), split); 
}
// ----------------------------------------------------------------------------
UT_INLINE nsString nsToString(const NSvec4& v, char split = nsStringConverter::nsStringSplitter)
{ 
    return nsStringConverter::toStringArray<NSfloat, 4>(v.ptr(), split); 
}
// ----------------------------------------------------------------------------
UT_INLINE nsString nsToString(const NSquat& v, char split = nsStringConverter::nsStringSplitter)
{ 
    return nsStringConverter::toStringArray<NSfloat, 4>(v.ptr(), split); 
}
// ----------------------------------------------------------------------------
UT_INLINE nsString nsToString(const NSrect& v, char split = nsStringConverter::nsStringSplitter)
{ 
    return nsStringConverter::toStringArray<NSfloat, 4>(&v.x, split); 
}
