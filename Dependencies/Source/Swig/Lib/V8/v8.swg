%include <v8typemaps.swg> 
%include <v8runtime.swg>
/* -----------------------------------------------------------------------------
 *                         No operator support in V8 ? 
 * ----------------------------------------------------------------------------- */
%ignore *::operator+;
%ignore *::operator-;
%ignore *::operator*;
%ignore *::operator/;
%ignore *::operator-();
%ignore *::operator-() const;
%ignore *::operator==;	
%ignore *::operator!=;  
%ignore *::operator<;
%ignore *::operator>;
%ignore *::operator<=;	
%ignore *::operator>=;
%ignore *::operator!;
%ignore *::operator();
%ignore *::operator&&;	%ignore operator&&;
%ignore *::operator||;	%ignore operator||;
%ignore *::operator+=;
%ignore *::operator-=;
%ignore *::operator*=;
%ignore *::operator/=;
%ignore *::operator%=;
%ignore *::operator++;	%ignore *::operator--;

%ignore *::operator=;
%ignore operator+;
%ignore operator-;
%ignore operator*;
%ignore operator/;
%ignore operator%;
%ignore operator[];
%ignore operator>;	%ignore operator>=;	
%ignore operator<;	%ignore operator<=;
%ignore operator==;	%ignore operator!=;


