#pragma once

#ifdef WIN32
#include <windows.h>
// http://www.gamedev.net/community/forums/topic.asp?topic_id=382642
class CMemSpaceA16
{
public:
   // new. dual with ReleaseMemSpaceA16();
   static BYTE* GetMemSpaceA16(size_t size)
   {
      BYTE *p = ::new BYTE[size + 16];
      if (p)
      {
         BYTE offset = (BYTE)(16 - ((unsigned long long)p & 15));
         p += offset;
         p[-1] = offset;
      }
	return p;
   };
   // delete. dual with GetMemSpaceA16();
   static void ReleaseMemSpaceA16( BYTE* p )
   {
      if(p) {
         p -= p[-1];
         ::delete [] p;
      }
   };
};

//---------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////
//   I16Aligned	-	16 byte aligned classes
//   just public inherit from this and job done ;)
///////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------

class I16Aligned
{
public:
   void* operator new(size_t size)
   {
      return static_cast<void*>( CMemSpaceA16::GetMemSpaceA16(size) );
   };
   void* operator new[](size_t size)
   {
      return static_cast<void*>( CMemSpaceA16::GetMemSpaceA16(size) );
   };

   void operator delete(void* p)
   {
      BYTE* pb = static_cast<BYTE*>(p);
      CMemSpaceA16::ReleaseMemSpaceA16(pb);
   };
   void operator delete[](void* p)
   {
      BYTE* pb = static_cast<BYTE*>(p);
      CMemSpaceA16::ReleaseMemSpaceA16(pb);
   };

   /////////////////////////////////////////////////////////
   //	Placement version (fake).
   /////////////////////////////////////////////////////////

   void* operator new(size_t /*size*/, void* ptrPlacement)
   {	return ptrPlacement; };
   void operator delete(void* /*ptr*/, void* /*ptrPlacement*/)
   {	return; };

   void* operator new[](size_t /*size*/, void* ptrPlacement)
   {	return ptrPlacement; };
   void operator delete[](void* /*ptr*/, void* /*ptrPlacement*/)
   {	return; };
};

#else
class I16Aligned
{
};
#endif
