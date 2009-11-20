
#ifndef B_BLENDER_FILE_H
#define B_BLENDER_FILE_H


#include "bFile.h"

namespace bParse {

	// ----------------------------------------------------- //
	class bBlenderFile : public bFile
	{

	protected:
		bMain*				mMain;

		bStructHandle*		m_glob;

		bPtrMap				mLibPointers;

		void resolvePointersMismatch();
	
		void resolvePointersStruct(bChunkInd& dataChunk);

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

		bStructHandle*		getFileGlobal()
		{
			return m_glob;
		}

		virtual	void parseData();

		virtual void resolvePointers();

	};
};

#endif //B_BLENDER_FILE_H
