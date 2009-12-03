/*
-------------------------------------------------------------------------------
	This file is part of the Ogre GameKit port.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#include "gkPropertyTree.h"
#include "gkPropertyScript.h"
#include "gkPropertyScanner.h"
#include "gkPath.h"
#include "gkLogger.h"


using namespace Ogre;



#define INITIAL_STACK_SIZE 100
// ----------------------------------------------------------------------------
typedef gkStack<gkPropertyNode*, INITIAL_STACK_SIZE> NodeStack;


// ----------------------------------------------------------------------------
gkPropertyScript::gkPropertyScript() : m_tree(0)
{
}

// ----------------------------------------------------------------------------
gkPropertyScript::~gkPropertyScript()
{
	if (m_tree)
		delete m_tree;
	m_tree= 0;

}

// ----------------------------------------------------------------------------
void gkPropertyScript::scan(gkPropertyScanner *scnr)
{
	gkPropertyNode *node= 0;
	gkPropertyToken *c=0;

	/// current stack
	NodeStack stack;

	for (;;)
	{
		c= scnr->scan();
		switch (c->token)
		{
		case TOK_NAME:
			{
				gkPropertyToken *n= scnr->scan();

				gkDebugNode dn= {c->debug.line, c->debug.source};


				switch (n->token)
				{
				case TOK_NAME:
					{
						// name name
						// possible1: name name {
						// possible2: name name section name {

						gkPropertyToken *l2= scnr->scan();

						if (l2->token == TOK_OPEN_BRACE)
						{
							node= new gkPropertyNode();
							node->setName(n->string);
							node->setType(c->string);
							m_tree->addNode(node);
						}
						else if (l2->token == TOK_SECTION)
						{
							gkPropertyToken *l3= scnr->scan();

							if (l3->token != TOK_NAME)
							{
								gkPrintSLM(l3->debug.source, l3->debug.line, "expecting parent name.");
								goto error;
							}

							node= new gkPropertyNode();
							node->setName(n->string);
							node->setType(c->string);
							node->setBase(l3->string);
							m_tree->addNode(node);

							scnr->doneToken(l3);


							l3= scnr->scan();
							if (l3->token != TOK_OPEN_BRACE)
							{
								gkPrintSLM(l3->debug.source, l3->debug.line, "expecting open brace.");
								goto error;
							}
							scnr->doneToken(l3);
						}
						else
						{
							gkPrintSLM(l2->debug.source, l2->debug.line, "expecting open brace, or base class declaration.");
							goto error;
						}

						scnr->doneToken(l2);
					}
					break;

				case TOK_ASSIGN:
					{
						// name= assign
						gkPropertyAttribute *attr= new gkPropertyAttribute(c->string, n->string);
						m_tree->addAttribute(attr);

						gkDebugNode dn= {c->debug.line, c->debug.source};
						if (node)
						{
							node->addAttribute(attr);
							attr->setDebugNode(dn);
						}


					}
					break;

				case TOK_OPEN_BRACE:
					{
						node= new gkPropertyNode();
						node->setType(c->string);
						node->setName(String("__unnamed__") + StringConverter::toString((size_t)node));
						m_tree->addNode(node);
					}
					break;
				case TOK_ERROR:
				case TOK_END:
				case TOK_CLOSE_BRACE:
				case TOK_SECTION:
					gkPrintSLM(c->debug.source, c->debug.line, "invalid token.");
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
				stack.pop();
				if (!stack.empty())
					node= stack.top();
				else
					node= 0;

				scnr->doneToken(c);
			}
			break;
		case TOK_INCLUDE:
			{
				if (!c->string.empty())
					m_tree->addInclude(c->string);
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
	/// exit
	return;
}


// ----------------------------------------------------------------------------
void gkPropertyScript::parseBuffer(const String& name, const String& membuf)
{
	/// parser storage
	m_tree= new gkPropertyTree(name);
	if (membuf.empty())
		return;

	gkPropertyScanner scnr(name.c_str(), membuf, membuf.size());
	this->scan(&scnr);
}

// ----------------------------------------------------------------------------
void gkPropertyScript::parseFile(const gkPath &pth)
{
	const String& name= pth.getAbsPath();
	String buf= pth.getAsString();
	parseBuffer(name, buf);
}
