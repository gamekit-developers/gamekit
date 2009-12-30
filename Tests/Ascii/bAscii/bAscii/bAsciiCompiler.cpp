/*
-------------------------------------------------------------------------------
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C
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
#include <stdlib.h>
#include "utPrint.h"
#include "bAsciiCompiler.h"
#include "bAsciiScanner.h"
#include "bAsciiBlock.h"
#include "bAsciiArgument.h"
#include "bAscii.h"



// ----------------------------------------------------------------------------
bAsciiCompiler::bAsciiCompiler(bAscii *parent, bool verbose) :
		m_parent(parent), m_verbose(verbose)
{
}

// ----------------------------------------------------------------------------
bAsciiCompiler::~bAsciiCompiler()
{
}

// ----------------------------------------------------------------------------
bool bAsciiCompiler::compileBlock(bAsciiScanner& scanner, struct bAsciiLocal* globals, struct bAsciiLocal* locals, bAsciiBlock *parent)
{
	// find extra arguments
	bAsciiToken tok;
	int op;
	op = scanner.scan(tok, m_verbose);
	if (m_verbose)
		utPrintf("\tScanOp ==> 0x%02X %s\n", ((unsigned char)op), tok.m_string.c_str());

	if (op != OP_SECTION)
	{
		utPrintf("%s(%i): expecting sub section ':', found '%s' \n", scanner.getSource(), tok.m_line, tok.m_string.c_str());
		return false;
	}

	// look for optional sub section name|number

	op = scanner.scan(tok, m_verbose);
	if (m_verbose)
		utPrintf("\tScanOp ==> 0x%02X %s\n", ((unsigned char)op), tok.m_string.c_str());


	if (locals && locals->ctor != OP_UNKNOWN)
	{
		if (op != locals->ctor)
		{
			const char *dbg = locals->ctor == OP_STRING ? "string" : "number";
			utPrintf("%s(%i): missing block constructor expected '%s', found '%s' \n", scanner.getSource(), tok.m_line, dbg, tok.m_string.c_str());
			return false;
		}
	}

	if (op == OP_STRING)
	{
		parent->setName(tok.m_string);
		m_parent->addBlock(tok.m_string, parent);
		op = scanner.scan(tok, m_verbose);
		if (m_verbose)
			utPrintf("\tScanOp ==> 0x%02X %s\n", ((unsigned char)op), tok.m_string.c_str());
	}
	else if (op == OP_NUMBER)
	{
		parent->setLength((bNumber)atof(tok.m_string.c_str()));
		op = scanner.scan(tok, m_verbose);

		if (m_verbose)
			utPrintf("\tScanOp ==> 0x%02X %s\n", ((unsigned char)op), tok.m_string.c_str());
	}


	// assert
	if (op != OP_BRACKET_OPEN)
	{
		utPrintf("%s(%i): expecting open bracket '{', found '%s' \n", scanner.getSource(), tok.m_line, tok.m_string.c_str());
		return false;
	}

	// scan recursivly
	do
	{

		op = scanner.scan(tok, m_verbose);
		if (m_verbose)
			utPrintf("\tScanOp ==> 0x%02X %s\n", ((unsigned char)op), tok.m_string.c_str());

		if (op == OP_BRACKET_CLOSE)
			break;

		if (op != OP_ALPHA)
		{
			utPrintf("%s(%i): expecting block or statement, found '%s' \n", scanner.getSource(), tok.m_line, tok.m_string.c_str());
			return false;
		}

		bAsciiLocal *local = 0;
		if (locals)
			local = bAscciFindLocal(locals, tok.m_string.hash());
		else
			local = bAscciFindLocal(globals, tok.m_string.hash());

		if (!local)
		{
			utPrintf("%s(%i): invalid keyword '%s' \n", scanner.getSource(), tok.m_line, tok.m_string.c_str());
			return false;
		}

		if (local->types)
		{
			// it's a sub block
			bAsciiBlock *block = new bAsciiBlock(tok.m_string, local->op);
			block->setParent(parent);
			m_parent->addBlock(block);

			if (!compileBlock(scanner, globals, local, block))
				return false;
		}
		else
		{

			bStringF cur_tok = tok.m_string;
			op = scanner.scan(tok, m_verbose);
			if (m_verbose)
				utPrintf("\tScanOp ==> 0x%02X %s\n", ((unsigned char)op), tok.m_string.c_str());

			// its and assignment

			if (tok.m_tok != OP_ASSIGNMENT)
			{
				utPrintf("%s(%i): expecting assignment, found '%s' \n", scanner.getSource(), tok.m_line, tok.m_string.c_str());
				return false;
			}

			op = scanner.scan(tok, m_verbose);
			if (m_verbose)
				utPrintf("\tScanOp ==> 0x%02X %s\n", ((unsigned char)op), tok.m_string.c_str());


			if (op == OP_TERM)
			{
				utPrintf("%s(%i): empty assignment statement.\n", scanner.getSource(), tok.m_line);
				return false;
			}
			else if (op == OP_END)
			{
				utPrintf("%s(%i): end of file found while scanning assignment statement.\n", scanner.getSource(), tok.m_line);
				return false;
			}

			bAsciiArgument *arg = new bAsciiArgument(local->op, local->type, local->nrarg);
			parent->addArgument(arg);



			int arg_nr = 0;
			int args_to_check = local->nrarg;
			if (args_to_check != -1 && local->type > OP_TYPE_ARRAY_BEGIN && local->type < OP_TYPE_ARRAY_END)
			{
				// nrarg then becomes the stride
				int plen = (int)parent->getLength();
				if (plen != -1) args_to_check *= plen;
				else args_to_check = -1;
			}

			// scan till term
			do
			{
				if (args_to_check != -1 && op != OP_TERM && op !=OP_SEPERATOR )
				{
					if (arg_nr > args_to_check)
					{
						const char *dbg = local->arg == OP_STRING ? "string(s)" : "number(s)";

						utPrintf("%s(%i): incorrect number of arguments to '%s', expected '%i' %s.\n",
							scanner.getSource(),
							tok.m_line,
							cur_tok.c_str(),
							args_to_check,
							dbg);
						return false;
					}
				}

				if ((op == OP_STRING || op == OP_ALPHA))
				{
					if (local->arg != OP_STRING && local->type != OP_TYPE_MIXED)
					{
						utPrintf("%s(%i): invalid string argument to '%s', expected %i number(s).\n",
							scanner.getSource(),
							cur_tok.c_str(),
							args_to_check,
							tok.m_line);
						return false;
					}
				}

				if (op == OP_NUMBER)
				{
					if (local->arg != OP_NUMBER && local->type != OP_TYPE_MIXED)
					{
						utPrintf("%s(%i): invalid string argument to '%s', expected %i string(s).\n",
							scanner.getSource(),
							tok.m_line,
							cur_tok.c_str(),
							args_to_check);
						return false;
					}
				}



				if (op == OP_STRING)
				{
					arg->m_alphas.push_back(tok.m_string);
					arg_nr++;
				}
				else if (op == OP_ALPHA)
				{
					arg->m_alphas.push_back(tok.m_string);
					arg_nr++;
				}
				else if (op == OP_NUMBER)
				{
					arg->m_numbers.push_back((bNumber)atof(tok.m_string.c_str()));
					arg_nr++;
				}
				else
				{
					if (op != OP_TERM && op != OP_SEPERATOR)
					{
						utPrintf("%s(%i): expecting ',' or ';', found '%s' \n", scanner.getSource(), tok.m_line, tok.m_string.c_str());
						return false;
					}
					else if (op == OP_END)
					{
						utPrintf("%s(%i): end of file found while scanning assignment statement.\n", scanner.getSource(), tok.m_line);
						return false;
					}
				}

				op = scanner.scan(tok, m_verbose);
				if (op == OP_END)
				{
					utPrintf("%s(%i): end of file found while scanning assignment statement.\n", scanner.getSource(), tok.m_line);
					return false;
				}


			}
			while (op != OP_TERM);

			// validate closing args
			if (local->maxarg != -1 && arg_nr > local->maxarg)
			{
				utPrintf("%s(%i): maximum arguments exceeded for %s, max is %i, found %i.\n",
					scanner.getSource(), tok.m_line, cur_tok.c_str(), local->maxarg, arg_nr);
				return false;
			}

			if (args_to_check != -1 && arg_nr != args_to_check)
			{
				utPrintf("%s(%i): required arguments for %s is %i, found %i.\n",
					scanner.getSource(), tok.m_line, cur_tok.c_str(), args_to_check, arg_nr);
				return false;
			}

		}

	}
	while (op != OP_BRACKET_CLOSE && op != OP_END);
	return true;
}

// ----------------------------------------------------------------------------
bool bAsciiCompiler::compile(bAsciiScanner &scanner)
{
	bAsciiToken tok;
	int op;

	do
	{
		op = scanner.scan(tok, m_verbose);
		if (m_verbose)
			utPrintf("\tScanOp ==> 0x%02X %s\n", ((unsigned char)op), tok.m_string.c_str());

		if (op == OP_END)
			break;
		if (op == OP_UNKNOWN)
		{
			utPrintf("%s(%i): unknown error, '%s' \n", scanner.getSource(), tok.m_line, tok.m_string.c_str());
			return false;
		}

		// looking for top level start block OP_ALPHA
		if (op != OP_ALPHA)
		{
			utPrintf("%s(%i): expecting start block, found '%s' \n", scanner.getSource(), tok.m_line, tok.m_string.c_str());
			return false;
		}

		bAsciiLocal *global = bAscciFindGlobal(tok.m_string.hash());

		if (!global || global->op == OP_UNKNOWN)
		{
			utPrintf("%s(%i): unknown block '%s' \n", scanner.getSource(), tok.m_line, tok.m_string.c_str());
			return false;
		}

		bAsciiBlock *block = new bAsciiBlock(tok.m_string, global->op);
		m_parent->addBlock(block);
		m_parent->addBlock(global->op, block);

		// build recursively
		if (!compileBlock(scanner, global, 0, block))
			return false;
	}
	while (op != OP_UNKNOWN && op != OP_END);
	return true;
}
