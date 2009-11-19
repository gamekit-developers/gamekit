
#ifndef B_BLENDER_FILE_H
#define B_BLENDER_FILE_H


#include "bFile.h"

namespace bParse {

	// ----------------------------------------------------- //
	class bBlenderFile : public bFile
	{

	protected:
		bMain*				mMain;

		bPtrMap				mLibPointers;

	public:

		bBlenderFile(const char* fileName);

		bBlenderFile(char *memoryBuffer, int len);

		virtual ~bBlenderFile();

		bMain* getMain();

		virtual	void	addDataBlock(char* dataBlock);

		bPtrMap&		getLibPointers()
		{
			return mLibPointers;
		}

		virtual	void parseData();

	};
};

#endif //B_BLENDER_FILE_H
