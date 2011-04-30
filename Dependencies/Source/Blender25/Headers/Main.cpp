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
#include <assert.h>
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <string>
#include <stdio.h>
#include <string.h>


#define ADD_DOXYGEN_COMMENTS	1
#define WRITE_DNA_FILE			1

typedef std::string hgString;


// ----------------------------------------------------------------------------
class DNA
{
public:
    typedef std::vector<hgString>           NameArray;
    typedef std::vector<short>              ShortArray;
    typedef std::vector<short *>             ShortPtrArray;

public:
    DNA();
    ~DNA();

    const short        &getStructType(size_t idx);
    int                 getSize(short type, short name);
    int                 getArraySize(const char *str);
    int                 getNumStructs() {return (int)m_structs.size();}

    short *getStruct(int i)
    {
        if (i>=0 && i<getNumStructs())
            return m_structs.at(i);
        return 0;
    }

    hgString getName(size_t i)
    {
        if (i>=0 && i<m_names.size())
            return m_names.at(i);
        return "NULL";
    }
    hgString getType(size_t i)
    {
        if (i>=0 && i<m_types.size())
            return m_types.at(i);
        return "NULL";
    }



    void parse(char *dp);

protected:
    NameArray       m_names;        // list of variable names
    NameArray       m_types;        // list if type names
    ShortArray      m_lens;         // length of types
    ShortPtrArray   m_structs;      // struct list
    ShortArray      m_structType;   // find struct for m_types[idx]
    int             m_ptrSize;      // size of pointers in the file
};



// ----------------------------------------------------------------------------
DNA::DNA() : m_ptrSize(0)
{
}


// ----------------------------------------------------------------------------
DNA::~DNA()
{
}


// ----------------------------------------------------------------------------
void DNA::parse(char *dp)
{
    /// dna tables
    int *ip = (int *)dp;

    if (strncmp(dp, "SDNA", 4)==0)
    {
        // skip ++ NAME
        ip++; ip++;
    }

    // -- Name Section --

    m_names.reserve(*ip++);
    char *bp = (char *)ip;

    size_t len = m_names.capacity();
    for (size_t i=0; i<len; i++)
    {
        hgString id;
        while (*bp) id.push_back(*bp++);
        ++bp;
        m_names.push_back(id);
    }

    size_t balign;

    balign = (size_t)bp;
    balign = (balign + 3) & ~3;
    ip = (int *)balign;

    // -- Type Section --
    bp = (char *)ip;
    assert(strncmp(bp, "TYPE", 4)==0); ip++;


    m_types.reserve(*ip++);
    bp = (char *)ip;

    len = m_types.capacity();
    for (size_t i=0; i<len; i++)
    {
        hgString id;
        while (*bp) id.push_back(*bp++);
        ++bp;

        m_types.push_back(id);
    }

    balign = (size_t)bp;
    balign = (balign + 3) & ~3;
    ip = (int *)balign;

    // -- Type Length Section --
    bp = (char *)ip;
    assert(strncmp(bp, "TLEN", 4)==0); ip++;

    m_lens.reserve(m_types.capacity());

    short *sp = (short *)ip;
    len = m_lens.capacity();
    for (size_t i=0; i<len; i++)
        m_lens.push_back(*sp++);

    balign = (size_t)sp;
    balign = (balign + 3) & ~3;
    ip = (int *)balign;

    bp = (char *)ip;
    assert(strncmp(bp, "STRC", 4)==0); ip++;

    m_structs.reserve(*ip++);

    short *strc = (short *)ip;

    len = m_structs.capacity();
    m_structType.resize(m_types.size());

    for (size_t i=0; i<len; i++)
    {
        m_structs.push_back(strc);

        const char *id = m_types[strc[0]].c_str();
        if (m_ptrSize== 0 && !strcmp(id, "ListBase"))
            m_ptrSize = m_lens[strc[0]]/2;

        m_structType[strc[0]] = i;

        size_t nr = strc[1];
        strc += 2;
        strc += (nr * 2);
    }
}

// ----------------------------------------------------------------------------
int DNA::getSize(short type, short name)
{
    const char *pt = m_names[name].c_str();
    size_t namelen = m_names[name].size();

    int ret =0;
    int mult=1;

    if (pt[0] == '*' || pt[1] == '*')
    {
        if (pt[namelen-1] == ']')
            mult = getArraySize(pt);
        ret = m_ptrSize*mult;
    }

    else if (type <= (short)m_lens.size())
    {
        if (pt[namelen-1] == ']')
            mult = getArraySize(pt);
        ret= m_lens[type]*mult;
    }
    return ret;
}

// ----------------------------------------------------------------------------
int DNA::getArraySize(const char *str)
{
    int ret = 1;
    char *next = 0;
    char *cp = (char *)str;

    while (*cp)
    {
        char c = *cp++;
        if (c == '[')
            next = cp++;
        else if (c==']')
        {
            if (next)
                ret *= atoi(next);
        }
    }

    return ret;
}

// ----------------------------------------------------------------------------
short *findStruct(const char *type, DNA *dna)
{
    for (int i=0; i<dna->getNumStructs(); i++)
    {
        short *sp = dna->getStruct(i);
        hgString st = dna->getType(sp[0]);

        if (st == type)
            return sp;
    }
    return 0;
}


// ----------------------------------------------------------------------------
void dumpClass(FILE *fp, DNA *dna, short *strc, int max0, int max1, int index)
{
    hgString type = dna->getType(strc[0]);

    short typ = strc[0];
    short len = strc[1];
    strc+=2;

#ifdef _DEBUG
    char comment[256];
    fprintf(fp, "// %i DNA structure %i, %i\n", index, typ, len);
#endif



    fprintf(fp, "struct %s\n{\n", type.c_str());

    int padding = 0;

    for (short el = 0; el <len; el++, strc+=2)
    {
        hgString dt = dna->getType(strc[0]);
        hgString dn = dna->getName(strc[1]);



        int arrlen = dna->getArraySize(dn.c_str());

        char newname[64];
        char newtype[64];
        if (dn[0] == '*' && strc[0] >=  dna->getStruct(0)[0] && !findStruct(dt.c_str(), dna))
        {
            // replace with void ptr
            sprintf(newtype, "void");
#ifdef _DEBUG
            sprintf(comment, "// Note: using void* on undefined DNA type: %s", dt.c_str());
#else
            printf("Note: using void* on undefined DNA type: %s\n", dt.c_str());
#endif
        }
        else
        {

            sprintf(newtype, "%s", dt.c_str());
#ifdef _DEBUG

            sprintf(comment, "// %i type, name %i, %i.", el+1, strc[0], strc[1]);
#endif
        }

        sprintf(newname, "%s", dn.c_str());

#ifdef _DEBUG

        dt = hgString(newtype);
        dn = hgString(newname);

        hgString pad0((max0 - (int)dt.size()) + 4, ' ');
        hgString pad1((max1 - (int)dn.size()) + 0, ' ');


        fprintf(fp, "    %s%s%s;%s%s\n", newtype, pad0.c_str(), newname, pad1.c_str(), comment);
#else
        fprintf(fp, "    %s %s;\n", newtype, newname);
#endif
    }
    fprintf(fp, "};\n\n");
}


// ----------------------------------------------------------------------------
void dumpForwards(FILE *fp, DNA *dna)
{
    for (int i=0; i<dna->getNumStructs(); i++)
    {
        short *sp = dna->getStruct(i);

        hgString type = dna->getType(sp[0]);
        fprintf(fp, "struct %s;\n", type.c_str());
    }
    fprintf(fp, "\n\n\n");
}

// ----------------------------------------------------------------------------
struct StructName
{
    DNA *dna;
    hgString    name;
    short      *strc;
    int         pri, max, max2;


    bool operator ==(const hgString &a)
    {
        return a == name;
    }
    operator hgString() {return name;}
};

bool sortCmp(const StructName &a, const StructName &b)
{
    return a.pri < b.pri;
}



// Place dependent structures higher in the generated file.
//      order:
//          1). all forward declarations
//          2). dependency free
//          3). whatever is remaining
//
//      class ListBase
//      {
//      public:
//          ... no dependencies
//      };
//
//      class IDPropertyData
//      {
//      public:
//          ...
//          ... ListBase group <- is dependent type
//          ...
//      };

void dumpStructs(FILE *fp, DNA *dna)
{
    typedef std::list<StructName> StructList;
    StructList structs;

    for (int i=0; i<dna->getNumStructs(); i++)
    {
        short *sp = dna->getStruct(i);
        hgString type = dna->getType(sp[0]);

        int len = sp[1];
        sp+=2;

        size_t max = 0, max2 = 0;

        bool hasDeps = false;
        for (short el = 0; el <len; el++, sp+=2)
        {
            hgString dt = dna->getType(sp[0]);
            hgString dn = dna->getName(sp[1]);
            if (dn[0] != '*' && sp[0] >=  dna->getStruct(0)[0])
                hasDeps = true;

            if (max < dt.size())
                max = dt.size();
            if (max2 < dn.size())
                max2 = dn.size();
        }


        StructName sn = {dna, type, findStruct(type.c_str(), dna), hasDeps ? 1 : 0, max, max2};
        structs.push_back(sn);
    }

    dumpForwards(fp, dna);
    structs.sort(sortCmp);

    StructList::iterator it = structs.begin();

    int index = 0;
    while (it != structs.end())
    {
        StructName &sp = (*it);
        dumpClass(fp, dna, sp.strc, sp.max, sp.max2, index++);
        ++it;
    }
}

// ----------------------------------------------------------------------------
int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        printf("Usage: %s infile\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "rb");
    if (!fp)
    {
        printf("File loading failed %s\n", argv[1]);
        return 1;
    }

    fseek(fp, 0L, SEEK_END);
    int len = ftell(fp);
    fseek(fp, 0L, SEEK_SET);


    char *buf = new char[len+1];
    fread(buf, len, 1, fp);
    fclose(fp);

    bool blender = !strncmp("BLENDER", buf, 6);
    bool bullet  = !strncmp("BULLET", buf, 5);

    if (!blender && !bullet)
    {
        printf("Invalid header, compressed file ?\n");
        delete []buf;
        return 1;
    }

    char *ver = buf+9;
    int version = atoi(ver);

    bool is64Bit = false;
    if (buf[7]=='-')
        is64Bit = true;



    int dnastart = -1;

    char *tbuf = buf;
    for (int i=0; i<len; ++i)
    {
        if (!strncmp("SDNA", tbuf, 4))
        {
            dnastart = i;
            break;
        }

        ++tbuf;
    }

    if (dnastart != -1)
    {
        DNA *dna = new DNA;
        dna->parse(buf + dnastart);


        char specname[12], fname[12];
        sprintf(specname,   "%s", (bullet ? "Bullet" : "Blender"));
        sprintf(fname,      "%s.h", (bullet ? "Bullet" : "Blender"));

        FILE *bfp= fopen(fname, "wb");
        if (bfp)
        {
            printf("Writing %s header from file version %i...\n", specname, version);


            fprintf(bfp, "#ifndef _%s_h_\n", specname);
            fprintf(bfp, "#define _%s_h_\n", specname);

            fprintf(bfp, "// Generated from a %s(%i) file.\n\n", specname, version);


            if (blender)
            {
                fprintf(bfp, "#ifdef near\n");
                fprintf(bfp, "#undef near\n");
                fprintf(bfp, "#endif\n");
                fprintf(bfp, "#ifdef far\n");
                fprintf(bfp, "#undef far\n");
                fprintf(bfp, "#endif\n");
				fprintf(bfp, "#ifdef rad2\n");
				fprintf(bfp, "#undef rad2\n");
				fprintf(bfp, "#endif\n");
            }

            fprintf(bfp, "\n\n");

            fprintf(bfp, "namespace %s {\n", specname);

#if ADD_DOXYGEN_COMMENTS
			fprintf(bfp, "/** \\addtogroup %s\n"
				"*  @{\n"
				"*/\n\n",
				specname
			);
#endif
            dumpStructs(bfp, dna);

#if ADD_DOXYGEN_COMMENTS
			fprintf(bfp, "/** @}*/\n");            
#endif

            fprintf(bfp, "}\n");

			fprintf(bfp, "#endif//_%s_h_\n", specname);

            fclose(bfp);
        }


#if WRITE_DNA_FILE
        char name[32];
        sprintf(name, "dna_%i_%s.cpp", version, (is64Bit ? "64" : "32"));

        bfp= fopen(name, "wb");
        if (bfp)
        {
            printf("Writing %s bit DNA...\n", (is64Bit ? "64" : "32"));

            fprintf(bfp, "unsigned char DNAstr%s[]={\n    ", (is64Bit ? "64" : ""));

            char *dnaStr = buf + dnastart;
            for (int d=0; d<len-dnastart; ++d)
            {

                unsigned char uch = (unsigned char)dnaStr[d];
                fprintf(bfp, "0x%02X,", uch);
                if (d %24 == 23)
                    fprintf(bfp, "\n    ");
            }
            fprintf(bfp, "\n};\n");
            fprintf(bfp, "int DNAlen%s=sizeof(DNAstr%s);\n", (is64Bit ? "64" : ""), (is64Bit ? "64" : ""));
            fclose(bfp);
        }
#endif

        delete dna;
    }


    delete []buf;
    return 0;
}
