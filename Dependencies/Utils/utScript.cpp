/*
-------------------------------------------------------------------------------
    General Purpose Utility Library, should be kept dependency free.
    Unless the dependency can be compiled along with this library.

    Copyright (c) 2009-2010 Charlie C.
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
#include "utScript.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif


#define utPrintSLM(s,l,m) printf("%s(%i):%s\n", s, l, m)


utString ptrToString(void *p)
{
	char ptr[32];
	sprintf(ptr, "%p", p);
	return ptr;
}



struct utDebugInfo
{
	char       *source;
	UTsize      line;
};



typedef enum utTokenType
{
	TOK_ERROR= -1,
	TOK_END,
	TOK_NAME,
	TOK_ASSIGN,
	TOK_OPEN_BRACE,
	TOK_CLOSE_BRACE,
	TOK_SECTION,
	TOK_INCLUDE,
	TOK_USING,

} utTokenType;

typedef utString utTokenBuffer;


class utToken
{

public:

	utToken() :
		token(TOK_ERROR), string()
	{
		debug.line= 0;
		debug.source= 0;
	}


	void push(char c)
	{
		string.push_back(c);
	}

	utTokenType      token;
	utDebugInfo      debug;
	utTokenBuffer    string;
};


typedef utArray<utToken *> mtTokenArray;



class utScriptScanner
{
public:
	utScriptScanner(const char *srcname, const utString &data, UTsize len);
	virtual ~utScriptScanner();

	utToken *scan(void);

	void doneToken(utToken *CTOK);
protected:

	typedef utArray<utToken *> TokenAlloc;


	typedef char *CharPtr;
	void addCharacter(utToken *ptr, CharPtr &CTOK);
	utToken *makeToken(void);


	bool NL_TEST();

	const char         *source; 
	char               *buffer, *tok;
	UTsize             len, line, curTok; 
	int                m_state;
	TokenAlloc         m_tokens;
	bool first;
};



#define MATCH_ALPHA(c)      ((c >= 'a' && c <= 'z' || c >= 'A' && c <='Z') || c == '_')
#define MATCH_DIGIT(c)      ((c >= '0' && c <= '9'))
#define MATCH_ALPNU(c)      (MATCH_ALPHA(c) || MATCH_DIGIT(c))
#define MATCH_NL(c)         (c == '\n' || c == '\r')
#define MATCH_WS(c)         (c == ' ' || c == '\t')
#define MATCH_TERM(c)       (c == ';')

#define CTOK ((curTok < len && curTok >= 0) ? buffer[curTok] : TOK_END)
#define NTOK (((curTok+1) < len && curTok >= 0) ? buffer[curTok+1] : TOK_END)
#define PTOK ((curTok < len && curTok >= 0) ? &buffer[curTok] : 0)

#define ADVANCE (curTok += 1)
#define ADVANCEC(x) (curTok += x)
#define REWIND  (curTok -= 1)

#define TOKEN_PREALLOC      16


enum ScanStates
{
	SS_NONE=-1,
	SS_CLOSE,
	SS_TEXT,
};


utScriptScanner::utScriptScanner(const char *sname, const utString &sdata, UTsize slen) :
	source(sname),
	buffer(0),
	tok(0), len(slen), line(1), curTok(0),
	m_state(SS_NONE),
	first(true)
{
	if (slen > 0 && slen != UT_NPOS)
	{
		buffer= new char[slen + 1];
		memcpy(buffer, sdata.c_str(), slen);
		buffer[slen]=0;
		if (buffer != 0 && buffer[0] != TOK_END)
			tok= &buffer[0];
	}

	for (int i=0; i<TOKEN_PREALLOC; i++)
		m_tokens.push_back(new utToken());
}



utScriptScanner::~utScriptScanner()
{
	delete []buffer;

	for (UTsize i=0; i<m_tokens.capacity(); i++)
		delete m_tokens.at(i);
}


void utScriptScanner::doneToken(utToken *tok)
{
	if (tok)
		m_tokens.push_back(tok);
}


utToken *utScriptScanner::makeToken(void)
{
	utToken *dst= 0;

	if (m_tokens.empty())
		dst= new utToken();
	else
	{
		dst= m_tokens.back();
		m_tokens.pop_back();
	}

	UT_ASSERT(dst);

	dst->token= TOK_END;
	dst->debug.line= line;
	dst->debug.source= (char *)source;
	dst->string.clear();
	return dst;
}



void utScriptScanner::addCharacter(utToken *ptr, CharPtr &tok)
{
	if (*tok == TOK_END)
		return;
	ptr->push(*tok);
}



bool utScriptScanner::NL_TEST()
{
	if (CTOK == '\r' &&  NTOK!='\n')
	{
		//printf("CR\n");
		ADVANCE;
		line += 1;
		return true;
	}
	else if (CTOK == '\n')
	{
		//printf("LF\n");
		ADVANCE;
		line += 1;
		return true;
	}

	else if (CTOK == '\r' && NTOK=='\n')
	{
		//printf("CRLF\n");
		ADVANCE;
		ADVANCE;
		line += 1;
		return true;
	}

	return false;
}



utToken *utScriptScanner::scan(void)
{
	utToken *pt= makeToken();

	for (;;)
	{
		if (CTOK == TOK_END)
			goto finally;
		if (CTOK == '\n' || CTOK == '\r')
			NL_TEST();
		else if (CTOK == ' ' || CTOK == '\t')
			ADVANCE;
		else if (CTOK == '{')
		{
			ADVANCE;
			pt->string.push_back('{');
			pt->token = TOK_OPEN_BRACE;
			pt->debug.line = line;
			return pt;
		}
		else if (CTOK == '}')
		{
			ADVANCE;
			pt->string.push_back('}');
			pt->token = TOK_CLOSE_BRACE;
			pt->debug.line = line;
			return pt;
		}
		else if (CTOK == ':')
		{
			ADVANCE;
			pt->string.push_back(':');
			pt->debug.line= line;
			pt->token= TOK_SECTION;
			return pt;
		}
		else if (CTOK == '#' || CTOK == '/')
		{
			if (CTOK == '#')
			{
				if (strncmp(PTOK, "#include", 8)==0)
				{
					ADVANCEC(8);
					while (MATCH_WS(CTOK))
					{
						ADVANCE;
						if (CTOK == TOK_END)
							goto error;
					}
					if (CTOK != '"' && CTOK != '<')
						goto unknown;
					if (NTOK == '"' || NTOK == '<')
						goto empty;

					do
					{
						if (NL_TEST())
							break;

						ADVANCE;

						if (CTOK != '"' && CTOK != '<' && CTOK != '>')
							pt->string.push_back(CTOK);
						if (CTOK == TOK_END)
							goto error;
					}
					while (CTOK != '"' && CTOK != '<' && CTOK != '>');


					ADVANCE;

					pt->debug.line  = line;
					pt->token       = TOK_INCLUDE;
					return pt;
				}
				else
					goto error;
			}
			else if (CTOK == '/' && NTOK == '/')
			{
				ADVANCEC(2);
				while (!MATCH_NL(CTOK))
				{
					ADVANCE;
					if (CTOK == TOK_END)
						goto error;
				}
				// skip if crlf pair
				if (MATCH_NL(NTOK))
					ADVANCE;
				ADVANCE;
				line += 1;

			}
			else if (CTOK == '/' && NTOK == '*')
			{
				// c style
				for (;;)
				{
					if (CTOK == TOK_END)
						goto error;

					ADVANCE;
					NL_TEST();

					if (CTOK == '*' && NTOK == '/' )
					{
						ADVANCEC(2);
						break;
					}
				}

			}
		}
		else if (CTOK == '=')
		{
			ADVANCE;
			while (MATCH_WS(CTOK))
			{
				if (!MATCH_WS(NTOK)) break;
				ADVANCE;
				if (CTOK == TOK_END)
					goto error;
			}

			// scan till first term ';'
			if (MATCH_TERM((CTOK)))
				goto empty;

			pt->string.reserve(32);

			while (!MATCH_TERM(CTOK))
			{
				pt->string.push_back(CTOK);
				if (MATCH_TERM(NTOK))
				{
					ADVANCEC(2);
					break;
				}

				if (MATCH_NL(CTOK))
				{
					if (MATCH_NL(NTOK))
						ADVANCE;
					ADVANCE;
					line += 1;
				}

				ADVANCE;
				if (CTOK == TOK_END)
					goto error;
			}


			pt->debug.line= line;
			pt->token= TOK_ASSIGN;
			return pt;

		}
		else
		{
			if (0)//utCharNEq(PTOK, "using", 5))
			{
				ADVANCEC(5);
				while (MATCH_WS(CTOK))
				{
					if (!MATCH_WS(NTOK))
					{
						ADVANCE;
						break;
					}
					ADVANCE;
					if (CTOK == TOK_END)
						goto error;
				}

				// scan till first term ';'
				if (MATCH_TERM((CTOK)))
					goto empty;

				pt->string.reserve(32);
				while (!MATCH_TERM(CTOK))
				{
					pt->string.push_back(CTOK);
					if (MATCH_TERM(NTOK))
					{
						ADVANCEC(2);
						break;
					}

					if (MATCH_NL(CTOK))
					{
						if (MATCH_NL(NTOK))
							ADVANCE;
						ADVANCE;
						line += 1;
					}

					ADVANCE;
					if (CTOK == TOK_END)
						goto error;
				}

				pt->debug.line= line;
				pt->token= TOK_USING;
				return pt;


			}

			if (MATCH_ALPHA(CTOK))
			{
				pt->string.reserve(32);
				while (MATCH_ALPNU(CTOK))
				{
					pt->string.push_back(CTOK);
					ADVANCE;
					if (CTOK == TOK_END)
						goto error;
				}
				pt->debug.line= line;
				pt->token=  TOK_NAME;
				return pt;
			}

			goto unknown;
		}
	}

unknown:

	pt->debug.line= line;
	pt->token= TOK_ERROR;

	printf("%s(%i): syntax error: '%c' \n", source, line, CTOK);
	return pt;

empty:

	// empty assignment
	// x= ;

	pt->debug.line= line;
	pt->token= TOK_ERROR;

	printf("%s(%i): syntax error: empty statement. \n", source, line);
	return pt;

error:

	pt->debug.line= line;
	pt->token= TOK_ERROR;

	printf("%s(%i): error: premature end of file.\n\twhile scanning for '%c'\n", source, line, CTOK);
	return pt;

finally:

	// successful end of buffer
	pt->debug.line=  line;
	pt->token=  TOK_END;
	return pt;

}




utScriptAttribute::utScriptAttribute(const utString &name, const utString &val) :
	m_name(name), m_value(val)
{
	m_debug.line= 0;
	m_debug.source= "";
}


utScriptAttribute::~utScriptAttribute()
{
}





utScriptNode::utScriptNode(utScriptTree *tree) :
	m_name(), m_type(), m_base(), m_parent(0), m_root(tree)
{
	m_debug.line= 0;
	m_debug.source= "";
}


utScriptNode::~utScriptNode()
{
}


void utScriptNode::addChild(utScriptNode *strc)
{
	if (!strc)
		return;

	m_children.push_back(strc);
	strc->setParent(this);
}


void utScriptNode::addAttribute(utScriptAttribute *attr)
{
	if (!attr)
		return;

	m_attributes.push_back(attr);
	m_attrMap.insert(attr->getName(), attr);
}


utScriptAttribute *utScriptNode::getAttribute(const utHashedString &name, bool includeExtern)
{
	UTsize pos= m_attrMap.find(name);
	if (pos != UT_NPOS)
	{
		utScriptAttribute *attr=  m_attrMap.at(pos);

		if (includeExtern)
		{
			if (attr->getValue().find('.') != utString::npos)
			{
				utStringArray swiz;
				utStringUtils::split(swiz, attr->getValue(), ".");

				if (swiz.size() == 2)
				{
					utScriptNode *node = m_root->findNode(swiz[0]);
					if (node != 0 && node->hasAttribute(name))
						return node->getAttribute(name);
				}
			}
		}

		if (!m_base.empty())
		{
			utScriptNode *bn = m_root->findNode(m_base);
			if (bn != 0)
				return bn->getAttribute(name);	
		}
		return attr;

	}
	else
	{
		if (!m_base.empty())
		{
			utScriptNode *bn = m_root->findNode(m_base);
			if (bn != 0)
				return bn->getAttribute(name);	
		}
	}
	return 0;
}


bool utScriptNode::hasReferencedAttribute(const utHashedString &name)
{
	UTsize pos= m_attrMap.find(name);
	if (pos != UT_NPOS)
	{
		utScriptAttribute *attr=  m_attrMap.at(pos);

		if (attr->getValue().find('.') != utString::npos)
		{
			utStringArray swiz;
			utStringUtils::split(swiz, attr->getValue(), ".");

			if (swiz.size() == 2)
			{
				utScriptNode *node = m_root->findNode(swiz[0]);
				if (node != 0)
					return true;
			}
		}
	}
	return false;
}



void utScriptNode::getChildren(utScriptNode::Nodes &dest, const utString &type, bool fromBaseClass, bool fromBaseClassRecursive)
{
	UTsize i;
	for (i=0; i<m_children.size(); ++i)
	{
		if (m_children[i]->getType() == type)
			dest.push_back(m_children[i]);
	}


	if (fromBaseClass && !m_base.empty())
	{
		utScriptNode *bn = m_root->findNode(m_base);
		if (bn != 0)
			bn->getChildren(dest, type, fromBaseClassRecursive);
	}


}



utScriptTree::utScriptTree(const utString &fname) :
	m_file(fname)
{
}


utScriptTree::~utScriptTree()
{
	Attributes::Iterator it= m_attributes.iterator();
	while (it.hasMoreElements())
		delete it.getNext();


	Nodes::Iterator ni= m_nodes.iterator();
	while (ni.hasMoreElements())
		delete ni.getNext();
}



utScriptNode *utScriptTree::findNode(const utString &name)
{
	Nodes::Iterator it= m_nodes.iterator();
	while (it.hasMoreElements())
	{
		utScriptNode *node= it.getNext();
		if (node->getName() == name)
			return node;
	}
	return 0;
}


utScriptNode *utScriptTree::findFirstType(const utString &type)
{
	Nodes::Iterator it= m_nodes.iterator();
	while (it.hasMoreElements())
	{
		utScriptNode *node= it.getNext();
		if (node->getType() == type)
			return node;
	}
	return 0;
}


void utScriptTree::addNode(utScriptNode *nd)
{
	m_nodes.push_back(nd);
}


void utScriptTree::addAttribute(utScriptAttribute *at)
{
	m_attributes.push_back(at);
	m_attrMap.insert(at->getName(), at);
}



utScriptAttribute *utScriptTree::getAttribute(const utHashedString &name)
{
	UTsize pos = m_attrMap.find(name);
	if (pos != UT_NPOS)
		return m_attrMap.at(pos);
	return 0;
}

typedef utStack<utScriptNode *> NodeStack;


utScript::utScript() : m_tree(0)
{
}



utScript::~utScript()
{
	if (m_tree)
		delete m_tree;
	m_tree= 0;

}


void utScript::scan(utScriptScanner *scnr)
{
	utScriptNode *node= 0;
	utToken *c=0;

	NodeStack stack;

	for (;;)
	{
		c= scnr->scan();
		switch (c->token)
		{
		case TOK_NAME:
			{
				utToken *n= scnr->scan();

				utScriptDebug dn= {c->debug.line, c->debug.source};


				switch (n->token)
				{
				case TOK_NAME:
					{
						// possible1: name {
						// possible2: type name {
						// possible3: type name : extends {

						utToken *l2= scnr->scan();

						if (l2->token == TOK_OPEN_BRACE)
						{
							node= new utScriptNode(m_tree);
							node->setName(n->string);
							node->setType(c->string);
							m_tree->addNode(node);
						}
						else if (l2->token == TOK_SECTION)
						{
							utToken *l3= scnr->scan();

							if (l3->token != TOK_NAME)
							{
								utPrintSLM(l3->debug.source, l3->debug.line, "expecting parent name.");
								goto error;
							}

							node= new utScriptNode(m_tree);
							node->setName(n->string);
							node->setType(c->string);
							node->setBase(l3->string);
							m_tree->addNode(node);

							scnr->doneToken(l3);


							l3= scnr->scan();
							if (l3->token != TOK_OPEN_BRACE)
							{
								utPrintSLM(l3->debug.source, l3->debug.line, "expecting open brace.");
								goto error;
							}
							scnr->doneToken(l3);
						}
						else
						{
							utPrintSLM(l2->debug.source, l2->debug.line, "expecting open brace, or base class declaration.");
							goto error;
						}

						scnr->doneToken(l2);
					}
					break;

				case TOK_ASSIGN:
					{
						utString k = c->string;
						utString v = n->string;

						utStringUtils::trim(k);
						utStringUtils::trim(v);

						if (v[0] == '"')
							utStringUtils::trim(v, "\"");



						utScriptAttribute *attr= new utScriptAttribute(k, v);
						
						
						m_tree->addAttribute(attr);

						utScriptDebug dn= {c->debug.line, c->debug.source};
						if (node)
						{
							node->addAttribute(attr);
							attr->setDebugNode(dn);
						}


					}
					break;

				case TOK_OPEN_BRACE:
					{
						node= new utScriptNode(m_tree);
						node->setType(c->string);
						node->setName(utString("__unnamed__") + ptrToString(node));
						m_tree->addNode(node);
					}
					break;
				case TOK_ERROR:
				case TOK_END:
				case TOK_CLOSE_BRACE:
				case TOK_SECTION:
					utPrintSLM(c->debug.source, c->debug.line, "invalid token.");
					goto error;
				}


				if (node != 0 && n->token != TOK_ASSIGN)
				{
					if (stack.empty())
					{
						stack.push(node);
						m_tree->addRootNode(stack.top());
					}
					else
					{
						stack.top()->addChild(node);
						stack.push(node);
					}
				}

				if (node)
					node->setDebugNode(dn);


				scnr->doneToken(n);
				scnr->doneToken(c);
			}
			break;
		case TOK_CLOSE_BRACE:
			{
				if (!stack.empty())
					stack.pop();
				else
				{
					utPrintSLM(c->debug.source, c->debug.line, "Close bracket with no open bracket.");
					goto error;
				}

				if (!stack.empty())
					node= stack.top();
				else
					node= 0;

				scnr->doneToken(c);
			}
			break;
		case TOK_INCLUDE:
			{
				scnr->doneToken(c);
			}
			break;

		case TOK_USING:
			{
				scnr->doneToken(c);
			}
			break;
		case TOK_END:
			scnr->doneToken(c);
			return;
		case TOK_ASSIGN:
		case TOK_OPEN_BRACE:
		case TOK_SECTION:
		case TOK_ERROR:
			scnr->doneToken(c);
			goto error;
		}
	}

error:
	return;
}


void utScript::parseBuffer(const utString &name, const utString &membuf)
{
	m_tree= new utScriptTree(name);
	if (membuf.empty())
	{
		printf("failed to load %s\n", name.c_str());
		return;
	}

	utScriptScanner scnr(name.c_str(), membuf, membuf.size());
	scan(&scnr);
}
