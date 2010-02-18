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
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <string>
#include <string.h>
#include "Header.h"
#include "bBlenderFile.h"
#include "bMain.h"
#include "bDNA.h"

// ----------------------------------------------------------------------------
short *findStruct(const char *type, bParse::bDNA *dna)
{
	for (int i=0; i<dna->getNumStructs(); i++)
    {
        short *sp = dna->getStruct(i);
        char *st = dna->getType(sp[0]);

        if (strcmp(type, st)==0)
            return sp;
    }
    return 0;
}
// ----------------------------------------------------------------------------
void dumpClass(FILE *fp, bParse::bDNA *dna, short *strc)
{
    char *type = dna->getType(strc[0]);
    short len = strc[1];
    strc+=2;

	fprintf(fp, "class %s\n{\npublic:\n", type);

    int padding = 0;

    for (short el = 0; el <len; el++, strc+=2)
    {
        const char *dt = dna->getType(strc[0]);
        const char *dn = dna->getName(strc[1]);

        char newname[64];
        char newtype[64];
        if (dn[0] == '*' && strc[0] >=  dna->getStruct(0)[0] && !findStruct(dt, dna))
        {
            //printf("%s not found\n", dt);
            sprintf(newtype, "void");
        }
        else
            sprintf(newtype, "%s", dt);

        if (strncmp(dn, "pad", 3)==0)
            sprintf(newname, "pad%i", (++padding));
        else if (strncmp(dn, "*pad", 4)==0)
            sprintf(newname, "*pad%i", (++padding));
        else if (dn[0] == '[')
            sprintf(newname, "*pad%i", (++padding));
        else sprintf(newname, "%s", dn);

        fprintf(fp, "    %s %s;\n", newtype, newname);
    }
    fprintf(fp, "};\n\n");
}


// ----------------------------------------------------------------------------
void dumpForwards(FILE *fp, bParse::bDNA *dna)
{
	for (int i=0; i<dna->getNumStructs(); i++)
    {
        short *sp = dna->getStruct(i);

        char *type = dna->getType(sp[0]);
        fprintf(fp, "class %s;\n", type);
    }
}

// ----------------------------------------------------------------------------
struct StructName
{
    bParse::bDNA *dna;
    std::string name;
    short*      strc;

    bool operator ==(const std::string &a)
    {
        return a == name;
    }
    operator std::string() {return name;}
};


// ----------------------------------------------------------------------------
void dumpStructs(FILE *fp, bParse::bDNA *dna)
{
    typedef std::map<std::string, short *> LookUpTypesMap;
    typedef std::list<StructName> DepList;

    LookUpTypesMap look;
    DepList deps;

    /// solve dependencies
	for (int i=0; i<dna->getNumStructs(); i++)
    {
        short *sp = dna->getStruct(i);
        char *type = dna->getType(sp[0]);


        look.insert(std::make_pair(type, sp));

        int len = sp[1];
        sp+=2;


        for (short el = 0; el <len; el++, sp+=2)
        {
            const char *dt = dna->getType(sp[0]);
            const char *dn = dna->getName(sp[1]);

            if (dn[0] != '*' && sp[0] >=  dna->getStruct(0)[0])
            {
                if (std::find(deps.begin(), deps.end(), std::string(dt)) == deps.end())
                {
                    StructName sn = {dna, dt, findStruct(dt, dna)};
                    deps.push_front(sn);
                }
            }
        }


        if (std::find(deps.begin(), deps.end(), std::string(type)) == deps.end())
        {
            StructName sn = {dna, type, findStruct(type, dna)};
            deps.push_back(sn);
        }
    }


    dumpForwards(fp, dna);

    DepList::iterator it = deps.begin();
    while (it != deps.end())
    {
        StructName &sp = (*it);
        dumpClass(fp, dna, sp.strc);
        ++it;
    }
}

// ----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	if (argc <= 1)
		return 1;



	bParse::bBlenderFile file(argv[1]);
	if (!file.ok())
	{
		printf("Invalid blend file!\n");
		return 1;
	}

	file.parse(false);
	bParse::bDNA *fdna = file.getFileDNA();

    FILE *fp= fopen("Blender.h", "wb");
	if (!fp)
		return 1;

	fprintf(fp, "%s\n", HEADER);
    fprintf(fp, "#ifndef _Blender_h_\n");
    fprintf(fp, "#define _Blender_h_\n");
    fprintf(fp, "\n\n");
    fprintf(fp, "namespace Blender {\n");

    dumpStructs(fp, fdna);

    fprintf(fp, "}\n");
    fprintf(fp, "#endif//_Blender_h_\n");
    fclose(fp);

	return 0;
}
