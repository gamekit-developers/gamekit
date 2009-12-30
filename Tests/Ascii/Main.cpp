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
#include "bAscii.h"
#include "bAsciiMain.h"
#include "bUtils/utPrint.h"


char* files[] =
{
	"momo_ogreSmallAnim.bascii",
	"BigEndian.bascii",
	"MultiMat249.bascii",
	"MultiMat249.bascii",
	"UVLayers.bascii",
	"VertexColors.bascii",
	"CompileError.bascii",
	"InvalidSyntax.bascii",
	"InvalidKeyword.bascii",
	"InvalidArg.bascii",
	"InvalidArgCount.bascii",
	"InvalidStride.bascii",
	"MaxArg.bascii",
	"MissingConstructor.bascii",
	0
};


int main(int argc, char **argv)
{
	utPrintf("Batch testing ASCII file loading...\n");

	int expect= 8;
	int fail = 0;
	int i = 0;
	for (i=0; files[i]; ++i)
	{
		bAscii file;
		if (!file.loadBlender(files[i], true))
		{
			fail++;
			utPrintf("failed ==> %s\n", files[i]);
		}
		else
		{
			utPrintf("passed ==> %s\n", files[i]);
		}
	}

	utPrintf("%i failed, expected %i out of %i to fail\n", fail, expect, i);
	if (fail == expect)
		utPrintf("Success!\n");
	else
		utPrintf("Test failed!\n");
	return 0;
}
