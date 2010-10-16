/*
-------------------------------------------------------------------------------
    This file is part of FBT (File Binary Tables).
    http://gamekit.googlecode.com/

    Copyright (c) 2010 Charlie C & Erwin Coumans.

-------------------------------------------------------------------------------
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#ifndef _fbtDataApi_h_
#define _fbtDataApi_h_


#define INSP_VERSION_MAJOR      1
#define INSP_VERSION_MINOR      0
#define INSP_VERSION_SUB        0
#define INSP_VERSION_REV        9
#define INSPECTOR_VERSION_FILE  FBT_ID(INSP_VERSION_MAJOR, INSP_VERSION_MINOR, INSP_VERSION_SUB, INSP_VERSION_REV)
#define privsec public


typedef struct Link 
{
	// using Link as a data holder 
	void *m_data;
}Link;



class fbtText
{
privsec:
	fbtText();
	~fbtText();

	void clear(void);
	void copy(const char* buffer, int len);


	enum Flags
	{
		FILE_MODIFIED   = (1 << 0),
		FILE_IS_OPEN    = (1 << 1),
		FILE_OWN        = (1 << 2),
		FILE_IS_FBT     = (1 << 3),
		FILE_IS_GEN_FBT = (1 << 3),
		FILE_IS_NOTE    = (1 << 4),
	};

	bool isOpen(void)       const {return (m_flag & FILE_IS_OPEN) != 0;   }
	bool isModified(void)   const {return (m_flag & FILE_MODIFIED) != 0;  }
	bool isOwner(void)      const {return (m_flag & FILE_OWN) != 0;       }
	bool isFBT(void)        const {return (m_flag & FILE_IS_FBT) != 0;    }
	bool isGenHeader(void)  const {return (m_flag & FILE_IS_GEN_FBT) != 0;   }
	bool isNote(void)       const {return (m_flag & FILE_IS_NOTE) != 0;   }
	bool isSource(void)     const {return !isNote() && !isFBT() && !isGenHeader(); }


	void modify(bool v) {v ? (m_flag |= FILE_MODIFIED) : (m_flag &= ~FILE_MODIFIED); }
	void own(void)      {m_flag |= FILE_OWN; }

	void clearTypeBits(void)
	{
		m_flag &= ~(FILE_IS_FBT | FILE_IS_NOTE);
	}

	void makeNote(void)     {clearTypeBits(); m_flag |= FILE_IS_NOTE;}
	void makeFBT(void)      {clearTypeBits(); m_flag |= FILE_IS_FBT;}
	void makeSource(void)   {clearTypeBits();}
	void makeHeader(void)   {clearTypeBits(); m_flag |= FILE_IS_GEN_FBT;}


public:

	fbtText*   m_next, *m_prev;
	char    m_name[32];
	char*   m_data;
	int     m_size;
	int     m_flag;
	void*   m_textFile;
};


// Text edit file list
class fbtFileList
{
privsec:

	fbtFileList();
	void push_back(fbtText* fp);
	void remove(fbtText* fp);


public:
	fbtText* m_first, *m_last;
};




// Main project data
class fbtProjectFile
{
privsec:
	fbtProjectFile();
	~fbtProjectFile();

	void setLayout(const char* windowLayout, int len);
	void copy(const fbtProjectFile* rhs);
	void addFile(fbtText* text);
	void removeFile(fbtText* text);
	fbtText* getFile(const char* fname);


	bool hasFile(const char* fname);


	enum FileFlags
	{
		FF_SAVED    = (1 << 0),
		FF_DIRTY    = (1 << 1),
		FF_IS_BUILT = (1 << 2),
	};


	bool isBuilt(void) const {return (m_flags & FF_IS_BUILT) != 0;    }
	bool isDirty(void) const {return (m_flags & FF_DIRTY) != 0;       }
	bool isSaved(void) const {return (m_flags & FF_SAVED) != 0;       }

	void mark(bool v)   { v ? (m_flags |= FF_IS_BUILT) : (m_flags &= ~FF_IS_BUILT); }

	void dirty(bool v)
	{
		if (v)
		{
			m_flags |= FF_DIRTY;
			m_flags &= ~FF_SAVED;
		}
		else
		{
			m_flags &= ~FF_DIRTY;
			m_flags |= FF_SAVED;
		}
	}


public:

	char*       m_windowLayout;         // aui layout
	int         m_version;              // project version
	int         m_layoutSize;           // sizeof(m_windowLayout)
	int         m_flags;
	int			m_pad;
	fbtFileList m_projectFiles;
};

#endif//_fbtDataApi_h_
