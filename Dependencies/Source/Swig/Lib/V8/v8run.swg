/* --------------------------------------------------------------------------
 *
 * v8run.swg
 *
 * ------------------------------------------------------------------------- */
#include <v8.h>
#include <stdio.h>
#include <stdarg.h>
#include <map>

/* Method or global function */
typedef struct V8_Function
{
    const char *name;
    v8::InvocationCallback func;
} V8_Function;

/* Global or member get/set  */
typedef struct V8_GetSetter
{
    const char *name;
    v8::AccessorGetter getf;
    v8::AccessorSetter setf;
    v8::AccessControl accm;
    v8::PropertyAttribute prop;
} V8_GetSetter;

/* Class definition */
typedef struct V8_Class
{
    const char *name;
    swig_type_info **type;
    v8::InvocationCallback ctor;
    V8_Function *methods;
    V8_GetSetter *members;
    V8_Class **bases;
    const char **base_names;
    int size;
    v8::Persistent<v8::FunctionTemplate> tmpl;
} V8_Class;

#define V8_SCOPE_LOCK v8::HandleScope ___scope___;

namespace v8 {
template <typename T>
class GarbageCollector {
    // Collect pointers to prevent memory leaks.
    // When V8 runs a GC cycle, allocated objects  
    // will be removed with a call to __removeWeakRef__
    // At exit this class will remove any stragglers
public:
    typedef T *PointerObject;
    typedef std::map<PointerObject, int> ReferenceMap;
    typedef typename ReferenceMap::iterator Iterator;

private:
    ReferenceMap __ref;
    // destroy any remaining objects
    ~GarbageCollector() {
        if (!__ref.empty())
        {
            Iterator it = __ref.begin(), end = __ref.end();
            while (it != end)
            {
                if (it->first)
                    delete it->first;
                ++it;
            }
            __ref.clear();
        }
    }
    static GarbageCollector &get(void) {
        static GarbageCollector m_gc;
        return m_gc;
    }
    // Tell V8 the size of the allocation 
    // Plus any extra padding to speed up a collection cycle
    void __add__(void) {
        int __exsize = (int)sizeof(T);
        __exsize += SWIG_V8_GCPAD;
        V8::AdjustAmountOfExternalAllocatedMemory(__exsize);
    }
    void __sub__(void) {
        int __exsize = (int)sizeof(T);
        __exsize += SWIG_V8_GCPAD;
        V8::AdjustAmountOfExternalAllocatedMemory(-__exsize);
    }
    void __insert__(PointerObject obj) {
        if (obj != 0) {
            Iterator it = __ref.find(obj);
            if (it == __ref.end()) {
                __ref.insert(std::make_pair(obj, (int)1));
                __add__();
            }
        }
    }
    void __remove__(PointerObject obj) {
        if (obj != 0) {
            Iterator it = __ref.find(obj);
            if (it != __ref.end()) {
                delete obj;
                __ref.erase(it);
                __sub__();
            }
        }
    }
    static void __removeWeakRef__(Persistent<Value> obj, void* ptr) {
        obj.ClearWeak();
        if (ptr) get().__remove__((PointerObject)ptr);
    }
public:
    static Handle<Value> insert(PointerObject ptr, swig_type_info *type, int own) {
        V8_Class *cls = (V8_Class *)type->clientdata;
        if (!cls || cls->tmpl.IsEmpty()) {
            return v8::Undefined();
        }
        v8::Local<v8::ObjectTemplate> proto = cls->tmpl->PrototypeTemplate();
        v8::Persistent<v8::Object> obj = v8::Persistent<v8::Object>::New(proto->NewInstance());
        if (obj.IsEmpty())
            return v8::Undefined();
        obj->SetInternalField(0, v8::External::New(ptr));
        if (own) {
            get().__insert__(ptr);
            obj.MakeWeak(ptr, __removeWeakRef__);
        }
        return obj;
    }
};
template<typename T>
static Handle<Value> AddGarbageCollectedObject(T *ptr, swig_type_info *type, int own) {
    return GarbageCollector<T>::insert(ptr, type, own);
}
}
/* ----------------------------------------------------------------------- */
class V8String {
    // Temporary string access 
private:
    char *str;
    int len;
public:
    V8String() : str(0), len(0) {}
    V8String(const v8::Local<v8::Value> &arg)
    {
        v8::Local<v8::String> vstr = arg->ToString();
        v8::String::AsciiValue val(vstr);
        len = vstr->Length();
        str = new char[len + 1];
        memcpy(str, (*val), len);
        str[len] = 0;
    }
    V8String(const V8String &o) { *this = o; }
    V8String &operator =(const V8String &o)
    {
        len = o.len;
        str = new char[len+1];
        memcpy(str, o.str, len);
        str[len] = 0;
        return *this;
    }
    ~V8String() {delete []str;}
    operator char *() { return str; }
};
/* ----------------------------------------------------------------------- */
SWIGRUNTIME bool
V8_ValidSelf(const v8::AccessorInfo &inf)
{
    v8::Local<v8::Object> self = inf.Holder();
    if (!self.IsEmpty()) {
        v8::Local<v8::External> obj = v8::Local<v8::External>::Cast(self->GetInternalField(0));
        return !obj.IsEmpty() && obj->Value() != 0;
    }
    return false;
}
/* -----------------------------------------------------------------------
 * Error format util
 */
SWIGRUNTIME v8::Handle<v8::Value>
V8_Error(const char *fmt, ...)
{
#define V8_ErrMax 0xFFFF
    static char errbuf[V8_ErrMax + 1];
    va_list lst;
    va_start(lst, fmt);
    int size = vsnprintf(errbuf, V8_ErrMax, fmt, lst);
    va_end(lst);
    if (size < 0) { errbuf[V8_ErrMax] = 0; size = V8_ErrMax; }
    errbuf[size] = 0;
    return v8::ThrowException(v8::String::New(errbuf));
#undef V8_ErrMax
}


/* -----------------------------------------------------------------------
 * Extract 'self' pointer from accessor
 */
SWIGRUNTIME bool
V8_ConvertPtr(const v8::AccessorInfo &info, void **ptrptr)
{
    v8::Local<v8::Object> self = info.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
    if (ptrptr)
        (*ptrptr) = wrap->Value();
    return ptrptr && (*ptrptr) != 0;
}
/* -----------------------------------------------------------------------
 * Extract 'self' pointer from function arguments
 */
SWIGRUNTIME bool
V8_ConvertPtr(const v8::Arguments &self, void **ptrptr)
{
    v8::Local<v8::Object> obj = self.Holder();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(obj->GetInternalField(0));
    if (ptrptr)
        (*ptrptr) = wrap->Value();
    return ptrptr && (*ptrptr) != 0;
}
/* -----------------------------------------------------------------------
 * Extract 'self' pointer from specific argument
 */
SWIGRUNTIME bool
V8_ConvertPtr(const v8::Local<v8::Value> &self, void **ptrptr)
{
    if (!self->IsObject())
        return false;
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->ToObject()->GetInternalField(0));
    if (ptrptr)
        (*ptrptr) = wrap->Value();
    return ptrptr && (*ptrptr) != 0;
}

/* ------------------------------------------------------------------------- */
#define SWIG_ConvertPtr(I, O) V8_ConvertPtr(I, O)
#define SWIG_GetModule(clientdata) 0
#define SWIG_SetModule(clientdata, pointer)
/* ------------------------------------------------------------------------- */
#define SWIG_ArgCheck(name, exp) \
{\
  if (args.Length() != exp)\
    return V8_Error("Error in function '%s'.\n\tExpected '%d' arguments, found %d.", name,exp,args.Length()); \
}
#define SWIG_ArgCheckFail(name, exp, idx) \
{\
  return V8_Error("Error in function '%s'.\n\tExpected '%s', for parameter %d.", name, exp, idx); \
}
#define SWIG_PtrCheckFail(name) \
{\
  return V8_Error("Error in function '%s'.\n\tFound invalid pointer.", name); \
}
#define SWIG_PtrCheckFailV(name) \
{\
    V8_Error("Error in function '%s'.\n\tFound invalid pointer.", name); \
    return;\
}
#define SWIG_ArgCheckFailV(name, exp, idx) \
{\
  V8_Error("Error in function '%s'.\n\tExpected '%s', for parameter %d.", name, exp, idx);\
    return;\
}
/* -------------------------------------------------------------------------
 * Check functions
 */
SWIGRUNTIME bool
V8_IsNumber(const v8::Arguments &self, int idx) {
    return self[idx]->IsNumber();
}
SWIGRUNTIME bool
V8_IsString(const v8::Arguments &self, int idx) {
    return self[idx]->IsString();
}
SWIGRUNTIME bool
V8_IsBool(const v8::Arguments &self, int idx) {
    return self[idx]->IsBoolean();
}
SWIGRUNTIME bool
V8_IsObject(const v8::Arguments &self, int idx) {
    return self[idx]->IsObject();
}
SWIGRUNTIME bool
V8_IsFunction(const v8::Arguments &self, int idx) {
    if (V8_IsObject(self, idx))
        return self[idx]->ToObject()->IsFunction();
    return false;
}
SWIGRUNTIME bool
V8_IsNumber(const v8::Local<v8::Value> &val, int idx) {
    return val->IsNumber();
}
SWIGRUNTIME bool
V8_IsString(const v8::Local<v8::Value> &val, int idx) {
    return val->IsString();
}
SWIGRUNTIME bool
V8_IsBool(const v8::Local<v8::Value> &val, int idx) {
    return val->IsBoolean();
}
SWIGRUNTIME bool
V8_IsObject(const v8::Local<v8::Value> &val, int idx) {
    return val->IsObject();
}
SWIGRUNTIME bool
V8_IsFunction(const v8::Local<v8::Value> &val, int idx) {
    if (val->IsObject())
        return val->ToObject()->IsFunction();
    return false;
}