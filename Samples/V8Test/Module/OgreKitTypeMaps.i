

%typemap(in,checkfn="V8_IsFunction") CallbackObject
%{ $1 = v8::Persistent<v8::Function>::New(v8::Local<v8::Function>::Cast($input->ToObject())); %}


%typemap(in,checkfn="V8_IsString") gkString %{
    $1 = gkString((const char*)V8String($input));
%}

%typemap(in,checkfn="V8_IsString") gkString&, const gkString& (gkString temp) %{
    temp = gkString((const char*)V8String($input));
    $1 = &temp;
%}


%typemap(out) gkString, gkString&, const gkString& %{ 
    return v8::String::New($1.c_str());
%}

