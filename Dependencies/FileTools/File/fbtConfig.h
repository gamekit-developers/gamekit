/*
-------------------------------------------------------------------------------
    This file is part of FBT (File Binary Tables).
    http://gamekit.googlecode.com/

    Copyright (c) 2010 Charlie C & Erwin Coumans.

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
#ifndef _fbtConfig_h_
#define _fbtConfig_h_


/** \addtogroup FBT
*  @{
*/

// global config settings

#define fbtDEBUG        1           // Traceback detail
#define fbtMaxTable     5000        // Maximum number of elements in a table
#define fbtMaxID        64          // Maximum character array length
#define fbtMaxMember    256         // Maximum number of members in a struct or class.
#define fbtDefaultAlloc 2048        // Table default allocation size
#define FBT_TYPE_LEN_VALIDATE   1   // Write a validation file (use MakeFBT.cmake->ADD_FBT_VALIDATOR to add a self validating build)
#define FBT_ARRAY_SLOTS         2   // Maximum dimensional array, eg: (int m_member[..][..] -> [FBT_ARRAY_SLOTS])

/** @}*/

#endif//_fbtConfig_h_
