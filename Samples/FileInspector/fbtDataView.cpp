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
#include "fbtDataView.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#if FBT_COMPILER == FBT_COMPILER_MSVC
# define fbt_snprintf _snprintf
#else
# define fbt_snprintf snprintf
#endif


#define fbtVALID_NODE(nd)			(nd && nd->m_file && nd->m_chunk && nd->m_struct)

#define fbtMakeChar(data)		((data))
#define fbtMakeCharInt(data)	(static_cast<int>(static_cast<unsigned char>((*data))))
#define fbtMakeShort(data)		(static_cast<int>(*reinterpret_cast<const short*>(data)))
#define fbtMakeInt(data)		((*reinterpret_cast<const int*>(data )))
#define fbtMakeLong(data)		((*reinterpret_cast<const long*>(data )))
#define fbtMakeFloat(data)		((*reinterpret_cast<const float*>(data )))
#define fbtMakeDouble(data)		((*reinterpret_cast<const float*>(data )))


class fbtChunkViewNode;
typedef fbtArray<fbtChunkViewNode*> fbtChunkPool;
typedef fbtChunkPool fbtChunkViewNodePtrArray;


class fbtChunkViewNode
{
public:


	fbtChunkViewNode() 
		:	m_parent(0), 
			m_tables(0), 
			m_chunk(0),
			m_poff(0)
	{
	}

	~fbtChunkViewNode()
	{
		m_children.clear();
	}


	void setParent(fbtChunkViewNode* par)
	{
		m_parent = par;
		par->m_children.push_back(this);
	}

	void clear(void)
	{
		m_parent = 0;
		m_tables = 0;
		m_chunk = 0;
		m_poff = 0;
		m_children.clear();
	}


	fbtChunkViewNode* getParent(void) { return m_parent; }

	wxString getTypeName(void)
	{
		if (m_tables)
		{
			wxString ptrs('*', m_tables->m_name[m_key.k16[1]].m_ptrCount);
			return wxString(m_tables->m_type[m_key.k16[0]].m_name) + ptrs;
		}
		return wxEmptyString;
	}

	wxString getMemberName(void)
	{
		if (m_tables)
		{
			if (m_isArray && !m_isContainer)
			{
				return wxString::Format("[%i]", m_arrIndex);
			}
			else
			{
				wxString str = wxString(m_tables->m_name[m_key.k16[1]].m_name);
				return str.AfterLast('*');
			}
		}
		return wxEmptyString;
	}


public:

	fbtChunkViewNode*        m_parent;
	fbtChunkViewNodePtrArray m_children;
	fbtFile::MemoryChunk*    m_chunk;
	fbtBinTables*            m_tables;

	fbtKey32        m_key;
	FBTint32        m_off, m_poff;
	FBTint32        m_len;
	FBTint32		m_arrIndex;
	bool			m_isContainer;
	bool			m_isArray;


};


class fbtChunkViewModel : public wxDataViewModel
{
public:

	fbtChunkViewModel();
	virtual ~fbtChunkViewModel();

	virtual bool IsContainer(const wxDataViewItem& item) const;
	virtual wxDataViewItem GetParent(const wxDataViewItem& item)  const;
	virtual unsigned int GetChildren(const wxDataViewItem& item,
	                                 wxDataViewItemArray& children) const;
	virtual unsigned int GetColumnCount(void) const;
	virtual wxString GetColumnType(unsigned int col) const;

	virtual bool HasContainerColumns(const wxDataViewItem& ) const {return true;}


	virtual void GetValue(wxVariant& variant,
	                      const wxDataViewItem& item, unsigned int col) const;
	virtual bool SetValue(const wxVariant& variant,
	                      const wxDataViewItem& item, unsigned int col);


	void NotifyChunk(fbtFile* fp, fbtFile::MemoryChunk* mch, int nrIndex);
	void Clear(void);



private:

	fbtChunkViewNode *Alloc(void);


	void Compile(FBTuint32 i);
	void Compile(FBTtype strcId, FBTtype arrSize, fbtChunkViewNode* parent, FBTuint32& cof);
	void CompileElement(FBTtype* strc, FBTint32 count, FBTuint32& cof, fbtChunkViewNode *parent = 0);


	fbtChunkPool             m_alloc;
	FBTtype                  m_firstStrc;
	fbtFile::MemoryChunk*    m_chunk;
	fbtBinTables*            m_tables;
	FBTint32                 m_chunkOff;
};



fbtChunkViewModel::fbtChunkViewModel()
	:    m_firstStrc(0), m_chunk(0), m_tables(0), m_chunkOff(0)
{
}



fbtChunkViewModel::~fbtChunkViewModel()
{
	Clear();
}


fbtChunkViewNode *fbtChunkViewModel::Alloc(void)
{
	fbtChunkViewNode *node = new fbtChunkViewNode();
	node->m_chunk = m_chunk;
	node->m_tables = m_tables;

	m_alloc.push_back(node);
	return node;
}


void fbtChunkViewModel::Clear(void)
{
	m_tables = 0;
	m_chunk = 0;


	Cleared();


	fbtChunkPool::Iterator it = m_alloc.iterator();
	while (it.hasMoreElements())
	{
		fbtChunkViewNode *nd = it.getNext();
		nd->clear();
		delete nd;
	}

	m_alloc.clear();
}


bool fbtChunkViewModel::IsContainer(const wxDataViewItem& item) const
{
	if (!item.IsOk())
		return true;

	if (m_chunk)
	{
		fbtChunkViewNode* nd = (fbtChunkViewNode*)item.GetID();
		return nd->m_isContainer;
	}
	return false;
}

wxDataViewItem fbtChunkViewModel::GetParent(const wxDataViewItem& item)  const
{
	if (m_chunk && item.IsOk())
	{
		fbtChunkViewNode* nd = (fbtChunkViewNode*)item.GetID();

		return wxDataViewItem((void*)nd->m_parent);
	}

	return wxDataViewItem(0);
}

unsigned int fbtChunkViewModel::GetChildren(const wxDataViewItem& item, wxDataViewItemArray& children) const
{
	if (m_chunk)
	{
		fbtChunkViewNode* nd = (fbtChunkViewNode*)item.GetID();

		if (nd)
		{
			fbtChunkViewNodePtrArray& arr = nd->m_children;

			FBTsizeType i = 0, s = arr.size();

			while (i < s)
				children.Add(arr[i++]);
			return i;
		}
	}
	return 0;
}




unsigned int fbtChunkViewModel::GetColumnCount(void) const
{
	// constant
	return 3;
}



wxString fbtChunkViewModel::GetColumnType(unsigned int col) const
{
	return "string";
}


void fbtChunkViewModel::GetValue(wxVariant& variant, const wxDataViewItem& item, unsigned int col) const
{
	if (item.IsOk())
	{
		fbtChunkViewNode* node = (fbtChunkViewNode*)item.GetID();

		switch (col)
		{
		case 0:
			{
				variant = node->getMemberName();
				break;
			}
		case 2:
			{
				variant = node->getTypeName();
				break;
			}
		case 1:
			{

				char *cur = static_cast<char*>(m_chunk->m_newBlock) + (node->m_len * m_chunkOff);
				char *data = (cur + node->m_off);

				const fbtName &name = m_tables->m_name[node->m_key.k16[1]];

				if (node->m_key.k16[0] <= 1)
				{
					if (name.m_ptrCount > 0)
					{
						FBTsize *ptrPtr = (FBTsize*)data;
						variant = wxString((char *)(*ptrPtr));
					}
					else
					{
						if (node->m_isArray && !node->m_isContainer)
						{
							char ch = (*data);
							int len = strlen(cur + node->m_poff);

							if (ch != 0 && node->m_arrIndex < len)
								variant = wxString(ch);
							else 
								variant = wxString(' ');
						}
						else
						{

							if (name.m_arraySize > 1)
								variant = wxString(data);
							else
							{
								variant = wxVariant(fbtMakeCharInt(data)).MakeString();
							}
						}
					}
				}
				else if (node->m_key.k16[0] <= 3)
				{
					variant = wxVariant(fbtMakeShort(data)).MakeString();
				}
				else if (node->m_key.k16[0] == 4)
				{
					variant = wxVariant(fbtMakeInt(data)).MakeString();
				}
				else if (node->m_key.k16[0] <= 6)
				{
					variant = wxVariant(fbtMakeLong(data)).MakeString();
				}
				else if (node->m_key.k16[0] == 7)
				{
					float fp = fbtMakeFloat(data);
					char buf[64] = {0};


					fbt_snprintf(buf, 64, "%f", fp);
					variant = wxString(buf);
				}
				else if (node->m_key.k16[0] == 8)
				{
					double fp = fbtMakeDouble(data);
					char buf[64] = {0};

					fbt_snprintf(buf, 64, "%f", fp);
					variant = wxString(buf);
				}
				else
				{
					FBTsize *ptrPtr = (FBTsize*)data;
					void *ptr = (void*)(*ptrPtr);

					union  Pointer {
						FBTuint32 m_id[2];
						void *m_ptr; 
					};

					Pointer sptr = {0};
					sptr.m_ptr = ptr;





					char buf[128] = {0};

#if FBT_ARCH == FBT_ARCH_64
					fbt_snprintf(buf,128,  "%p {%u, %u}",ptr, sptr.m_id[0], sptr.m_id[1]);
#else
					fbt_snprintf(buf,128,  "0x%08x {%u, %u}",ptr, sptr.m_id[0], sptr.m_id[1]);
#endif
					variant = wxString(buf);
				}
				break;
			}
		}
	}
}




bool fbtChunkViewModel::SetValue(const wxVariant& variant, const wxDataViewItem& item, unsigned int col)
{
	return false;
}


void fbtChunkViewModel::NotifyChunk(fbtFile* fp, fbtFile::MemoryChunk* mch, int nrIndex)
{


	fbtBinTables* bt = fp->getMemoryTable();
	if (mch->m_newTypeId > bt->m_strcNr)
	{
		// out of range error
		return;
	}
	Clear();

	m_tables = bt;
	m_chunk  = mch;
	m_chunkOff = nrIndex;
	m_firstStrc = m_tables->m_strc[0][0];

	m_alloc.reserve(m_tables->m_offs[mch->m_newTypeId]->m_members.size());

	Compile(mch->m_newTypeId);
}



void fbtChunkViewModel::Compile(FBTuint32 i)
{
	FBTuint32 cof = 0;
	FBTuint16 l;

	FBTtype* strc = m_tables->m_strc[i];
	l = strc[1];
	strc += 2;

	CompileElement(strc, l, cof);

}


void fbtChunkViewModel::Compile(FBTtype strcId, FBTtype arrSize, fbtChunkViewNode* parent, FBTuint32& cof)
{
	FBTuint32 a, l;
	for (a = 0; a < arrSize; ++a)
	{
		FBTtype* strc = m_tables->m_strc[strcId];

		l = strc[1];
		strc += 2;
		CompileElement(strc, l, cof, parent);
	}
}



void fbtChunkViewModel::CompileElement(FBTtype* strc, FBTint32 count, FBTuint32& cof, fbtChunkViewNode *parent)
{
	FBTint32 e, a;
	for (e = 0; e < count; ++e, strc += 2)
	{
		FBTint32 totArray = m_tables->m_name[strc[1]].m_arraySize;
		FBTuint32 len = (m_tables->m_name[strc[1]].m_ptrCount ? m_tables->m_ptr : m_tables->m_tlen[strc[0]]) * totArray;


		if (strc[0] >= m_firstStrc && m_tables->m_name[strc[1]].m_ptrCount == 0)
		{
			fbtChunkViewNode *node = Alloc();

			node->m_key.k16[0]  = strc[0];
			node->m_key.k16[1]  = strc[1];
			node->m_off         = cof;
			node->m_len         = len;
			node->m_isContainer = true;
			node->m_isArray		= totArray > 1;
			node->m_arrIndex    = 0;

			if (parent)
				node->setParent(parent);

			ItemAdded(parent, node);

			FBTint32 strcId = m_tables->m_type[strc[0]].m_strcId;
			Compile(strcId, totArray, node, cof);
		}
		else
		{
			if (totArray == 1)
			{
				fbtChunkViewNode *node = Alloc();

				node->m_key.k16[0]  = strc[0];
				node->m_key.k16[1]  = strc[1];
				node->m_off         = cof;
				node->m_len         = len;
				node->m_isContainer = false;
				node->m_isArray     = false;
				node->m_arrIndex    = 0;

				if (parent)
					node->setParent(parent);

				cof += node->m_len;
				ItemAdded(parent, node);
			}
			else
			{
				fbtName &name = m_tables->m_name[strc[1]];

				fbtChunkViewNode *node = Alloc();

				node->m_key.k16[0]  = strc[0];
				node->m_key.k16[1]  = strc[1];
				node->m_off         = cof;
				node->m_len         = len;
				node->m_isContainer = true;
				node->m_isArray     = true;
				node->m_arrIndex    = 0;

				if (parent)
					node->setParent(parent);

				ItemAdded(parent, node);

				FBTuint32 elementLength = node->m_len / totArray;

				for (a = 0; a < totArray; a ++)
				{
					fbtChunkViewNode *anode = Alloc();

					anode->m_key.k16[0]  = strc[0];
					anode->m_key.k16[1]  = strc[1];
					anode->m_off         = cof;
					anode->m_poff        = node->m_off;
					anode->m_len         = elementLength;
					anode->m_isContainer = false;
					anode->m_isArray     = true;
					anode->m_arrIndex    = a;
					cof += elementLength;

					anode->setParent(node);
					ItemAdded(node, anode);
				}
			}
		}

	}
}



fbtChunkCtrl::fbtChunkCtrl(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize& size, long style)
	:	wxDataViewCtrl(parent, id, pos, size, style), m_model(new fbtChunkViewModel())
{
	AssociateModel(m_model);
	m_model->DecRef();
	LoadColumns();
}

fbtChunkCtrl::~fbtChunkCtrl()
{
	DeleteAllItems();
}


void fbtChunkCtrl::DeleteAllItems(void)
{
	m_model->Clear();
}


void fbtChunkCtrl::LoadColumns(void)
{
	wxDataViewTextRenderer *txt;
	wxDataViewColumn *col;


	txt = new wxDataViewTextRenderer(wxT("string"), wxDATAVIEW_CELL_INERT, wxALIGN_LEFT);
	col = new wxDataViewColumn(wxT("Name"), txt, 0, 128, wxALIGN_LEFT);
	AppendColumn(col);

	txt = new wxDataViewTextRenderer(wxT("string"), wxDATAVIEW_CELL_INERT, wxALIGN_LEFT);
	col = new wxDataViewColumn(wxT("Value"), txt, 1, 128, wxALIGN_LEFT);
	AppendColumn(col);

	txt = new wxDataViewTextRenderer(wxT("string"), wxDATAVIEW_CELL_INERT, wxALIGN_LEFT);
	col = new wxDataViewColumn(wxT("Type"), txt, 2, 128, wxALIGN_LEFT);
	AppendColumn(col);
}

void fbtChunkCtrl::LoadChunk(fbtFile* fp, fbtFile::MemoryChunk* mch, int nrIndex)
{
	m_model->Clear();

	if (m_model && fp && mch)
		m_model->NotifyChunk(fp, mch, nrIndex);
}



void fbtChunkCtrl::FitParent(void)
{
	unsigned int bestFit = GetClientSize().GetWidth() / GetColumnCount();
	GetColumn(0)->SetWidth(bestFit);
	GetColumn(1)->SetWidth(bestFit);
	GetColumn(2)->SetWidth(bestFit);

}
