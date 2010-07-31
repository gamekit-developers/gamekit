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
#include "gkV8Utils.h"



void v8CollectionStep(int steps, int factor)
{
}


void v8Collect()
{
    int curAlloc =  v8::V8::AdjustAmountOfExternalAllocatedMemory(0);
    while (curAlloc != 0)
    {
        int nnr = v8::V8::AdjustAmountOfExternalAllocatedMemory(curAlloc);
        if (nnr == curAlloc)
        {
            // all collected
            v8::V8::AdjustAmountOfExternalAllocatedMemory(-curAlloc);
            break;
        }
        curAlloc = nnr;
    }
}


void v8HandleException(v8::TryCatch & caught)
{
    v8::HandleScope __scope__;
    v8::String::AsciiValue excpt(caught.Exception());
    const char *c_str = *excpt;
    printf("%s\n", c_str);
}

static v8::Handle<v8::Value> v8Print(const v8::Arguments &args)
{
    for (int i=0; i<args.Length(); ++i)
    {
        v8::HandleScope __scope__;
        v8::String::AsciiValue str(args[i]->ToString());
        const char *c_str =  *str; 

        if (c_str)
            printf("%s ", c_str);
    }

    printf("\n");
    return v8::Undefined();
}



void v8RegisterBuiltins(v8::Handle<v8::ObjectTemplate> &globals)
{
    globals->Set(v8::String::New("print"), v8::FunctionTemplate::New(v8Print));
}
