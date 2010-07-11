// ----------------------------------------------------------------------------
// v8typemaps.swg
// ----------------------------------------------------------------------------


// numbers
%typemap(in,checkfn="V8_IsNumber") int, short, signed char 
%{$1 = ($type)$input->Int32Value();%}
%typemap(in,checkfn="V8_IsNumber") unsigned int, unsigned short, unsigned char
%{$1 = ($type)$input->Uint32Value();%}
%typemap(in,checkfn="V8_IsNumber") float, double
%{$1 = ($type)$input->NumberValue();%}




%typemap(out) int,short,long,
              unsigned int,unsigned short,unsigned long,
              signed char,unsigned char,
              float,double
%{return v8::Number::New($1);%}


// we must also provide typemaps for privitives by const reference:
// given a function:
//	int intbyref(const int& i);
// SWIG assumes that this code will need a pointer to int to be passed in
// (this might be ok for objects by const ref, but not for numeric primitives)
// therefore we add a set of typemaps to fix this (for both in & out)
%typemap(in,checkfn="V8_IsNumber") const int&($basetype temp)
%{ temp=($basetype)$input->Int32Value(); $1=&temp;%}
%typemap(in,checkfn="V8_IsNumber") const unsigned int&($basetype temp)
%{temp=($basetype)$input->Uint32Value(); $1=&temp;%}
%typemap(out) const int&, const unsigned int&, const float &, const double&
%{return v8::Number::New(*$1);%}


// for the other numbers we can just use an apply statement to cover them
%apply const int & {const short&,const long&,const signed char&,
             const float&,const double&};

%apply const unsigned int & {const unsigned short&,const unsigned long&,
             const unsigned char&};

// enums have to be handled slightly differently
// VC++ .net will not allow a cast from lua_Number(double) to enum directly.
%typemap(in, checkfn="V8_IsNumber") enum SWIGTYPE
%{$1 = ($type)(int)$input->Int32Value();%}

%typemap(out) enum SWIGTYPE
%{return v8::Integer::New($1);%}

// and const refs
%typemap(in, checkfn="V8_IsNumber") const enum SWIGTYPE &($basetype temp)
%{ temp=($basetype)(int)$input->Int32Value(); $1=&temp;%}
%typemap(out) const enum SWIGTYPE &
%{return v8::Integer::New(*$1);%}


%typemap(in, checkfn="V8_IsBool") bool
%{$1 =$input->BooleanValue();%}

%typemap(out) bool
%{return v8::Boolean::New($1);%}

%typemap(out) void "";


// for const bool&, SWIG treats this as a const bool* so we must dereference it
%typemap(in, checkfn="V8_IsBool") const bool& (bool temp)
%{temp= $input->BooleanValue(); $1=&temp;%}
%typemap(out) const bool&
%{return v8::Boolean::New(*$1);%}


// strings (char* and char[])
%typemap(in, checkfn="V8_IsString") char*, const char * (V8String temp)
%{
    temp = V8String($input);
    $1 = ($ltype)temp;
%}

%typemap(in, checkfn="V8_IsString") const char[ANY], char[ANY] (V8String temp)
%{
    temp = V8String($input);
    $1 = ($ltype)temp;
%}

%typemap(out) const char*, char*
%{return v8::String::New((const char *)$1);%}

%typemap(out) const char[ANY], char[ANY]
%{return v8::String::New((const char *)$1);%}

// char's
// currently treating chars as small strings, not as numbers
// (however signed & unsigned char's are numbers...)
%typemap(in, checkfn="V8_IsString") char
%{$1 = (*v8::String::AsciiValue($input))[0];%}

%typemap(out) char
%{return v8::Integer::New((int )$1);%}

// by const ref
%typemap(in, checkfn="V8_IsString") const char& (char temp)
%{temp =v8::String::AsciiValue($input)[0]; $1=&temp;%}

%typemap(out) const char&
%{return v8::Integer::New((int )$1);%}

%typemap(in, checkfn="SWIG_isptrtype") void*, const void*
%{if (!SWIG_ConvertPtr($accessor,(void**)&$1)) {$ptrfail;}%}






%typecheck(SWIG_TYPECHECK_INTEGER)
	 int, short, long,
 	 unsigned int, unsigned short, unsigned long,
	 signed char, unsigned char,
	 long long, unsigned long long, signed long long,
	 const int &, const short &, const long &,
 	 const unsigned int &, const unsigned short &, const unsigned long &,
	 const signed char&, const unsigned char&,
	 const long long &, const unsigned long long &,
	 enum SWIGTYPE,	const enum SWIGTYPE&,
	 float, double, const float &, const double&
%{
 $1 = V8_IsNumber($accessor, $input) ? 1 : 0;
%}

%typecheck(SWIG_TYPECHECK_BOOL)
    bool, const bool &
%{
  $1 = V8_IsBool($accessor, $input) ? 1 : 0;
%}

// special check for a char (string of length 1)
%typecheck(SWIG_TYPECHECK_CHAR) char, const char& 
%{
 $1 = V8_IsString($accessor, $input) ? 1 : 0;
%}


%typecheck(SWIG_TYPECHECK_STRING) char *, char[] 
%{
 $1 = V8_IsString($accessor, $input) ? 1 : 0;
%}


%typecheck(SWIG_TYPECHECK_POINTER) SWIGTYPE *, SWIGTYPE [] 
%{
 $1 = V8_IsObject($accessor, $input) ? 1 : 0; 
%}
%typecheck(SWIG_TYPECHECK_POINTER) SWIGTYPE & 
%{
 $1 = V8_IsObject($accessor, $input) ? 1 : 0; 
%}

%typecheck(SWIG_TYPECHECK_POINTER) SWIGTYPE
%{
 $1 = V8_IsObject($accessor, $input) ? 1 : 0; 
%}

%typecheck(SWIG_TYPECHECK_VOIDPTR) void * 
%{
 $1 = V8_IsObject($accessor, $input) ? 1 : 0; 
%}

// Also needed for object ptrs by const ref
// eg const A* ref_pointer(A* const& a);
// found in mixed_types.i
%typecheck(SWIG_TYPECHECK_POINTER) SWIGTYPE* const &
%{
 $1 = V8_IsObject($accessor, $input) ? 1 : 0; 
%}

%typemap(in) void*, const void*
%{if (!SWIG_ConvertPtr($accessor,(void**)&$1)) {$ptrfail;}%}


%typemap(in) SWIGTYPE*,SWIGTYPE[]
%{if (!SWIG_ConvertPtr($accessor,(void**)&$1)) {
    $ptrfail;
}%}

%typemap(out) SWIGTYPE*,SWIGTYPE[]
%{ 
    return v8::AddGarbageCollectedObject<$templatecode >($1, $1_descriptor, $owner);
%}

%typemap(in) SWIGTYPE&, const SWIGTYPE&
%{
    if (!SWIG_ConvertPtr($accessor,(void**)&$1)) {
        $ptrfail;
}%}


%typemap(out) SWIGTYPE
%{  
    $&1_ltype resultptr = new $1_ltype((const $1_ltype &) $1);
    return v8::AddGarbageCollectedObject<$templatecode >(resultptr, $&1_descriptor, 1);
%}

%typemap(out) SWIGTYPE&
%{  
    return v8::AddGarbageCollectedObject<$templatecode >($1, $descriptor, 0);
%}


// passing objects by value
// SWIG_ConvertPtr wants an object pointer (the $&ltype argp)
// then dereferences it to get the object
%typemap(in) SWIGTYPE ($&ltype argp)
%{
    if (!SWIG_ConvertPtr($accessor,(void**)&argp)){
        $ptrfail;
    }
    $1 = *argp;
%}

%typemap(out) SWIGTYPE *&
%{
    return v8::AddGarbageCollectedObject<$templatecode >(*$1, $*descriptor, $owner);
%}
