/*
-------------------------------------------------------------------------------
--                                                                           --
--  Compile N source files to C strings                                      --
--  Usage: ${TARGET} ${OUTFILE} ${SOURCE0} ... ${SOURCE[N]}                  --
--                                                                           --
--  Released under the zlib license.                                         --
--  http://www.gzip.org/zlib/zlib_license.html                               --
--                                                                           --
--                                                                           --
-------------------------------------------------------------------------------
*/
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <vector>
#include <stdio.h>


void Template_upper( std::string& str )
{
    std::transform( str.begin(), str.end(), str.begin(), toupper );
}


void Template_split(std::vector<std::string> &rval,  const std::string &spl, const std::string &expr )
{
    std::string string= spl;
    rval.reserve( 32 );

    for ( ;; )
    {
        size_t pos= string.find_first_of( expr );
        if ( pos != std::string::npos )
        {
            // chop first
            if ( pos == 0 )
                pos= pos + 1;

            std::string sub= string.substr( 0, pos );
            if ( !sub.empty() && expr.find( sub ) == std::string::npos )
                rval.push_back( sub );

            string.erase( 0, pos );
        }
        else
        {
            if ( !string.empty() )
                rval.push_back( string );

            break;
        }
    }
}

void Template_replace( std::string &in, const std::string& from, const std::string &to )
{

    if ( !from.empty() && from != to )
    {
        // erase
        if ( to.empty() )
        {
            size_t pos= 0;
            while ( pos != std::string::npos )
            {
                pos= in.find( from );
                if ( pos != std::string::npos )
                    in.erase( pos, from.size() );
            }
        }
        else
        {
            size_t pos= 0;
            while ( pos != std::string::npos )
            {
                pos= in.find( from );
                if ( pos != std::string::npos )
                {
                    in.erase( pos, from.size() );
                    in.insert( pos, to );
                }
            }
        }
    }
}



void Template_getAsString(const std::string& in, std::vector<char> &dest)
{
	dest.clear();

    FILE *fp = fopen(in.c_str(), "rb");  // always in binary
	if (!fp) return;

	fseek(fp, 0L, SEEK_END);
	int fileSize = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	if (fileSize > 0)
	{
		dest.resize(fileSize);
		fread(&dest[0], fileSize, 1, fp);
	}
	fclose(fp);
}

//return filename base
std::string Template_base(const std::string& in)
{
	std::string path = in;
	Template_replace(path, "\\", "/");

	std::vector<std::string> arr;
    Template_split(arr, path, "/");

    if (arr.empty())
        return "";
    return arr.at(arr.size() - 1);
}


//return filename ext
std::string Template_extension(const std::string& in)
{
    // split base on .
    std::string base = Template_base(in);

    if (base.empty())
        return "";

	std::vector<std::string> arr;
    Template_split(arr, base, ".");

    if (arr.empty())
        return "";
    return std::string(".") + arr.at(arr.size() - 1);
}




#define MAX 26


std::string Template_writeSource(FILE *fp, char *fileName, bool binary)
{
    std::vector<char> buffer;
	Template_getAsString(fileName, buffer);

	std::string ret;
    
	if (!buffer.empty())
    {
        std::string base = Template_base(fileName);
        Template_replace(base, Template_extension(fileName), "");
        Template_replace(base, ".", "_");
		
        Template_upper(base);
		if (binary)
		{
			char tmp[256];

			sprintf(tmp, "static const size_t %s_SIZE", base.c_str());
			fprintf(fp, "%s=%d;\n", tmp, buffer.size());

			ret += "extern " + std::string(tmp) + ";\n";

			sprintf(tmp, "static const unsigned char %s[]", base.c_str());			
			fprintf(fp, "%s={\n", tmp);

			ret += std::string(tmp) + ";\n";
		}
        else
			fprintf(fp, "static const char %s[]={\n", base.c_str());
		
        fprintf(fp, "    ");

        for (size_t i=0; i<buffer.size(); ++i)
        {
            char cp = buffer[i];
            fprintf(fp, "0x%02X", ((unsigned char)cp));
			
			if (!binary && cp == 0) 
				break;			
			else if (i < buffer.size() - 1) 
				fprintf(fp, ",");

            if (i %(MAX) == (MAX-1))
                fprintf(fp, "\n    ");			

        }
        if (!binary) fprintf(fp, ",0x00");
        fprintf(fp, "\n};// %s %d bytes %s\n", Template_base(fileName).c_str(), 
			binary ? buffer.size() : buffer.size() + 1,
			binary ? "binary" : "string");
    }

	return ret;
}

int main(int argc, char **argv)
{
    if (argc <= 2)
    {
        std::cout << "Usage: template [-b] destFile source0 ... sourceN (-b: binary mode)\n";
        return 1;
    }

	bool binary = false;
	int opt = 1;

    std::string infile = argv[opt++];
	if (infile == "-b")
	{
		binary = true;		
		infile = argv[opt++];
	}

    FILE *fp = fopen(infile.c_str(), "w");
    if (!fp)
    {
        printf("Failed to load file %s\n", argv[1]);
        return 1;
    }

    //fprintf(fp, "// Auto generated by %s\n", argv[0]);

	std::string ret;
    for (int i=opt; i<argc; ++i)
    {
#ifdef TEMPLATE_VERBOSE
        printf("Template ==> %s\n", Template_base(argv[i]).c_str());
#endif
        ret += Template_writeSource(fp, argv[i], binary);
    }

	fputs("/*\n", fp);
	fputs(ret.c_str(), fp);
	fputs("*/\n", fp);
    fclose(fp);
    return 0;
}
