#ifndef CUSTOM_H
#define CUSTOM_H

namespace Custom
{
struct cstmStruct1;
struct cstmStruct2;

struct cstmStruct2{
	float fvalue;
	float fvalue2;
	char* string;
};

struct cstmStruct1{
	float fvalue;
	short svalue;
	short svalue2;
	cstmStruct2 s2value;
	cstmStruct2* s2ptr;
};

}

#endif //CUSTOM_H
