#ifndef CUSTOM_H
#define CUSTOM_H

#define privsec public

namespace Custom
{

struct cstmStruct{
	cstmStruct* m_next, *m_prev;
	float floatValue;
	int   intValue;
	short shortValue;
	char  charValue;
	
	// 7 bytes padding
	// length should be multiple of 8
	char pad0;
	int pad1;
};

class cstmStructList{
privsec:
	cstmStructList() : m_first(0), m_last(0) {}
	
	void remove(cstmStruct* link)
	{
		if (!link)
			return;
		if (link->m_next)
			link->m_next->m_prev = link->m_prev;
		if (link->m_prev)
			link->m_prev->m_next = link->m_next;
		if (m_last == link)
			m_last = link->m_prev;
		if (m_first == link)
			m_first = link->m_next;
	}
	
	void push_back(cstmStruct* link)
	{
		if (!link)
			return;
		
		link->m_prev = m_last;
		if (m_last) m_last->m_next = link;
		if (!m_first) m_first = link;
		
		m_last = link;
	}
	
public:
	cstmStruct* m_first, *m_last;
};

struct cstmMain{
	float floatValue;
	int   intValue;
	short shortValue;
	char  charValue;
	
	// 1 byte padding for the arrays that follows
	// arrays offset should be multiple of 4 bytes
	char pad0;
	
	float floatArray[4];
	int   intArray[4];
	short shortArray[4];
	char charArray[32];

	// Arrays with more than 2 dimensions are not supported
	float float2DArray[4][4];
	
	// 4 bytes padding 
	// embeded struct offset should be multiple of 8 bytes
	int pad1;
	
	cstmStruct structValue;
	cstmStruct structArray[2];
	
	// no padding needed but
	// pointers offset should be multiple of 8 bytes
	cstmStruct* structPtr;
	char* data;
	
	cstmStructList structList;
};

struct cstmGlobal{

	unsigned int versionmajor, versionminor;
	cstmMain main;
};

}

#endif //CUSTOM_H
