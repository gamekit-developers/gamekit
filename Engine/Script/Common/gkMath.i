%module Math
%{
#include "gksMath.h"
%}
%rename(Vector3)    gksVector3;
%rename(Quaternion) gksQuaternion;

%#ifdef SWIG_V8
%rename(__str__)    toString;
%#endif

%include "gksMath.h"
