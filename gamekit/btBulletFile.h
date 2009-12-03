/*
bParse
Copyright (c) 2006-2009 Charlie C & Erwin Coumans  http://gamekit.googlecode.com

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef BT_BULLET_FILE_H
#define BT_BULLET_FILE_H


#include "bFile.h"

namespace bParse {

	// ----------------------------------------------------- //
	class btBulletFile : public bFile
	{

	protected:
	
		
	public:

		btBulletFile();

		btBulletFile(const char* fileName);

		btBulletFile(char *memoryBuffer, int len);

		virtual ~btBulletFile();

		virtual	void	addDataBlock(char* dataBlock);
	

		// experimental
		virtual int		write(const char* fileName);

		virtual	void	parse(bool verboseDumpAllTypes);

		virtual	void parseData();

		virtual	void	writeDNA(FILE* fp);

		void	addStruct(char* structType,void* data, int len);

	};
};

#endif //BT_BULLET_FILE_H
