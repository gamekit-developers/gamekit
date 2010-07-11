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
*/
#include "gkV8Utils.h"



void v8CollectionStep(int steps, int factor)
{
    //int curAlloc =  v8::V8::AdjustAmountOfExternalAllocatedMemory(0) + factor;
    //int i=0;
    //while (curAlloc != 0 && (i++ < steps))
    //{
    //    int nnr = v8::V8::AdjustAmountOfExternalAllocatedMemory(curAlloc);
    //    if (nnr == curAlloc)
    //    {
    //        // all collected
    //        v8::V8::AdjustAmountOfExternalAllocatedMemory(-curAlloc);
    //        break;
    //    }
    //    curAlloc = nnr;
    //}
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
