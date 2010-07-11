// -----------------------------------------------------------------------------
// See the LICENSE file for information on copyright, usage and redistribution
// of SWIG, and the README file for authors - http://www.swig.org/release.html.
//
// V8.cxx
//
// V8 JavaScript language module for SWIG.
//  -----------------------------------------------------------------------------
#include "swigmod.h"


//  -----------------------------------------------------------------------------
static const char *usage = (char *) "\
\
V8 JavaScript Options (available with -V8)\n\
     -gcpad <nr>    - Amount of extra padding to add to the garbage collector.\n\
     -scope         - Scope function calls.\n\
\n";


#define REPORT(T,D)   {Printf(stdout,T"\n");if(D)Swig_print_node(D);}      // the works


#define IMPLEMENT_BASE_VARIABLE(CALLBACK, TYPE, ISCONSTANT, NAMEFNC, DEST)\
    String *rname = Getattr(n, "real_name");                    \
    String *iname = Getattr(n, "sym:name");                     \
    String *tname = Getattr(n, "type");                         \
    bool canSet = !(ISCONSTANT) && is_assignable(n) != 0;       \
    String *gname = Swig_name_wrapper(Swig_name_get(NAMEFNC));  \
    String *sname = Swig_name_wrapper(Swig_name_set(NAMEFNC));  \
    m_current = TYPE;                                           \
    Language::CALLBACK(n);                                      \
    m_current =NO_CPP;                                          \
    Printv(DEST,    tab4,                                       \
           "{ \"",  rname,                                      \
           "\", ",  gname,                                      \
           ", ",    ((canSet)? sname : "0"),                    \
           ", v8::DEFAULT",                                     \
           ", ", (  (canSet) ? "v8::None" : "v8::ReadOnly"),    \
           "},\n",  NIL);                                       \
    Delete(sname);                                              \
    Delete(gname);



#define IMPLEMENT_VARIABLE(CALLBACK, CONSTANT, DEST) {IMPLEMENT_BASE_VARIABLE(CALLBACK, VARIABLE,   CONSTANT, iname, DEST)}
#define IMPLEMENT_MEMBER(CALLBACK, CONSTANT, DEST)   {IMPLEMENT_BASE_VARIABLE(CALLBACK, MEMBER_VAR, CONSTANT, Swig_name_member(m_class_name, iname), DEST)}

//  -----------------------------------------------------------------------------
class V8 : public Language
{
private:
    File    *m_begin;
    String  *m_runtime;
    String  *m_header;
    String  *m_init;
    String  *m_wrappers;
    String  *m_initbeforefunc;
    String  *m_globals, *m_constants, *m_methods, *m_members;
    String  *m_class_name;

    String  *m_ctorName;
    bool    m_hasCtor;
    int     m_gcpad;
    bool    m_scope;



    enum
    {
        NO_CPP,
        VARIABLE,
        MEMBER_FUNC,
        CONSTRUCTOR,
        MEMBER_VAR,
        CLASS_CONST,
        STATIC_FUNC,
        STATIC_VAR
    } m_current;

public:

    V8()
    {
        m_begin = 0;
        m_runtime = 0;
        m_header = 0;
        m_init = 0;
        m_wrappers = 0;
        m_initbeforefunc = 0;
        m_globals = 0;
        m_constants = 0;
        m_class_name = 0;
        m_hasCtor = false;
        m_ctorName = 0;
        m_gcpad = 0;
        m_scope = false;
    }


    virtual void main(int argc, char *argv[])
    {
        // Set location of SWIG library
        SWIG_library_directory("V8");

        // Look for certain command line options
        for (int i = 1; i < argc; i++)
        {
            if (argv[i])
            {
                if (strcmp(argv[i], "-help") == 0)      // usage flags
                    fputs(usage, stderr);
                else if (strcmp(argv[i], "-scope") == 0)
                {
                    Swig_mark_arg(i);
                    m_scope = true;
                }
                else if (strcmp(argv[i], "-gcpad") == 0)
                {
                    if (argv[i + 1])
                    {
                        m_gcpad = atoi(argv[i + 1]);
                        if (m_gcpad < 0)
                            m_gcpad = 0;
                        if (m_gcpad > 4194304)
                            m_gcpad = 4194304;

                        Swig_mark_arg(i);
                        Swig_mark_arg(i + 1);
                        i++;
                    }
                    else
                        Swig_arg_error();

                }
            }

        }
        // Add a symbol to the parser for conditional compilation
        Preprocessor_define("SWIG_V8 1", 0);

        // Set language-specific configuration file
        SWIG_config_file("V8.swg");

        // Set typemap language
        SWIG_typemap_lang("V8");

        // Enable overloaded methods support
        allow_overloading();
    }

    virtual int top(Node *n)
    {
        // Get the module name
        String *module = Getattr(n, "name");
        // Get the output file name
        String *outfile = Getattr(n, "outfile");

        // Open the output file
        m_begin = NewFile(outfile, "w", SWIG_output_files());
        if (!m_begin)
        {
            FileErrorDisplay(outfile);
            SWIG_exit(EXIT_FAILURE);
        }
        m_current = NO_CPP;

        m_runtime           = NewString("");
        m_header            = NewString("");
        m_initbeforefunc    = NewString("");
        m_wrappers          = NewString("");
        m_init              = NewString("");
        m_globals           = NewString("");
        m_constants         = NewString("");

        Printf(m_runtime, "#define SWIG_V8\n");
        Printf(m_runtime, "#define SWIG_V8_GCPAD %i\n", m_gcpad);

        // Register file targets with the SWIG file handler
        Swig_register_filebyname("header",          m_header);
        Swig_register_filebyname("wrapper",         m_wrappers);
        Swig_register_filebyname("runtime",         m_runtime);
        Swig_register_filebyname("init",            m_init);
        Swig_register_filebyname("initbeforefunc",  m_initbeforefunc);

        Printf(m_header, "#define SWIG_name      \"%s\"\n", module);
        Printf(m_header, "#define SWIG_init      %s_OpenV8\n", module);
        Printf(m_globals, "\nstatic const struct V8_Function  v8_globals[] = {\n");
        Printf(m_constants, "\nstatic const struct V8_GetSetter v8_constants[] = {\n");


        Swig_banner(m_begin);
        Language::top(n);


        // reg tables
        Printv(m_globals, tab4, "{0,0}\n", "};\n", NIL);
        Printv(m_constants, tab4, "{0,0}\n", "};\n", NIL);

        Printv(m_wrappers, m_globals, m_constants, NIL);
        SwigType_emit_type_table(m_runtime, m_wrappers);


        // Dump contents
        Dump(m_runtime,         m_begin);
        Dump(m_header,          m_begin);
        Dump(m_wrappers,        m_begin);
        Dump(m_initbeforefunc,  m_begin);




        // Close all of the files
        Close(m_begin);

        Delete(outfile);
        Delete(module);
        Delete(m_init);
        Delete(m_wrappers);
        Delete(m_runtime);
        Delete(m_header);

        // Done
        return SWIG_OK;
    }

    virtual int importDirective(Node *n)
    {
        return Language::importDirective(n);
    }

    void openFunction(String *dest, String *name)
    {
        Printf(dest, "static v8::Handle<v8::Value> %s(const v8::Arguments &args){\n", name);
        if (m_scope)
            Printf(dest, "V8_SCOPE_LOCK;\n");

    }

    void openFunctionGet(String *dest, String *name)
    {
        Printf(dest, "static v8::Handle<v8::Value> %s(v8::Local<v8::String> prop, const v8::AccessorInfo& inf){\n", name);
        if (m_scope)
            Printf(dest, "V8_SCOPE_LOCK;\n");
    }

    void openFunctionSet(String *dest, String *name)
    {
        Printf(dest, "static void %s(v8::Local<v8::String> prop, v8::Local<v8::Value> args, const v8::AccessorInfo& inf){\n", name);
        if (m_scope)
            Printf(dest, "V8_SCOPE_LOCK;\n");
    }
    void openFunctionDtor(String *dest, String *name)
    {
        Printf(dest,  "static void %s(v8::Persistent<v8::Value> obj, void *ptr){\n", name);
        if (m_scope)
            Printf(dest, "V8_SCOPE_LOCK;\n");
    }

    void closeFunction(String *dest)
    {
        Printf(dest, "}\n");
    }

    virtual int functionWrapper(Node *n)
    {
        return writeFunction(n);
    }

    virtual int variableWrapper(Node *n)
    {
        return Language::variableWrapper(n);
    }

    virtual int constantWrapper(Node *n)
    {
        Setattr(n, "real_name", Getattr(n, "name"));
        m_current = VARIABLE;
        IMPLEMENT_VARIABLE(variableWrapper, true, m_constants);
        m_current = NO_CPP;
        return SWIG_OK;
    }

    virtual int nativeWrapper(Node *n)
    {
        return Language::nativeWrapper(n);
    }

    virtual int enumDeclaration(Node *n)
    {
        return Language::enumDeclaration(n);
    }

    virtual int enumvalueDeclaration(Node *n)
    {
        return Language::enumvalueDeclaration(n);
    }

    virtual int classDeclaration(Node *n)
    {
        return Language::classDeclaration(n);
    }

    virtual int membervariableHandler(Node *n)
    {
        Setattr(n, "real_name", Getattr(n, "name"));
        m_current = MEMBER_VAR;
        IMPLEMENT_MEMBER(membervariableHandler, false, m_members);
        m_current = NO_CPP;
        return SWIG_OK;
    }

    virtual int memberconstantHandler(Node *n)
    {
        Setattr(n, "real_name", Getattr(n, "name"));
        m_current = MEMBER_VAR;
        IMPLEMENT_MEMBER(memberconstantHandler, true, m_members);
        m_current = NO_CPP;
        return SWIG_OK;
    }

    virtual int staticmembervariableHandler(Node *n)
    {

        Setattr(n, "real_name", Getattr(n, "name"));
        m_current = MEMBER_VAR;
        IMPLEMENT_MEMBER(staticmembervariableHandler, false, m_members);
        m_current = NO_CPP;
        return SWIG_OK;
    }


    virtual int constructorHandler(Node *n)
    {
        m_ctorName  = NewString(Getattr(n, "sym:name"));
        m_hasCtor = true;
        m_current = CONSTRUCTOR;
        int res = Language::constructorHandler(n);
        m_current = NO_CPP;
        return res;
    }


    virtual int destructorHandler(Node *n)
    {
        return SWIG_OK;
    }

    virtual int staticmemberfunctionHandler(Node *n)
    {
        return Language::staticmemberfunctionHandler(n);
    }


    virtual int memberfunctionHandler(Node *n)
    {
        String *mname   = Getattr(n, "name");
        String *sname   = GetChar(n, "sym:name");

        m_current = MEMBER_FUNC;
        Language::memberfunctionHandler(n);
        m_current = NO_CPP;

        String *rname   = sname ? sname : mname;
        String *wname   = Swig_name_wrapper(Swig_name_member(m_class_name, rname));
        if (!Getattr(n, "sym:nextSibling"))
            Printv(m_methods, tab4, "{\"", rname, "\", ", wname, "}, \n", NIL);

        Delete(wname);
        return SWIG_OK;
    }


    int writeFunction(Node *n)
    {
        String *fname   = Getattr(n, "name");
        String *iname   = Getattr(n, "sym:name");
        SwigType *type     = Getattr(n, "type");
        ParmList *prml     = Getattr(n, "parms");

        String *overname = 0;
        if (Getattr(n, "sym:overloaded"))
            overname = Getattr(n, "sym:overname");
        else
        {
            if (!addSymbol(iname, n))
            {
                Printf(stderr, "addSymbol(%s) failed\n",iname);
                return SWIG_ERROR;
            }
        }

        Wrapper *func = NewWrapper();
        String *wname = Swig_name_wrapper(iname);
        if (overname)
            Append(wname, overname);

        bool isGet = false;
        bool isSet = false;
        bool isGetSet = m_current == MEMBER_VAR || m_current == VARIABLE;



        // write function heafer
        if (isGetSet)
        {
            if (m_current == MEMBER_VAR && Strstr(iname, "_get"))
            {
                openFunctionGet(func->def, wname);
                isGet = true;
            }
            else if (m_current == MEMBER_VAR  && Strstr(iname, "_set"))
            {
                openFunctionSet(func->def, wname);
                isSet = true;
            }
            else if (m_current == VARIABLE && Strstr(iname, "_get"))
            {
                openFunctionGet(func->def, wname);
                isGet = true;
            }
            else if ( m_current == VARIABLE && Strstr(iname, "_set"))
            {
                openFunctionSet(func->def, wname);
                isSet = true;
            }
            else
            {
                assert(0);
            }
        }
        else
        {
            openFunction(func->def, wname);
        }



        // locals
        emit_parameter_variables(prml, func);
        emit_attach_parmmaps(prml, func);
        Setattr(n, "wrap:parms", prml);


        // Get number of required and total arguments
        int num_arguments   = emit_num_arguments(prml);
        int num_required    = emit_num_required(prml);
        int varargs         = emit_isvarargs(prml);



        String *argument_check = NewString("");
        String *argument_parse = NewString("");
        String *checkfn = NULL;
        String *tm;

        char source[72];
        char accessor[72];
        char argcheckfail[72];

        int i, offs = 0;
        Parm *p;
        for (i = 0, p = prml; i < num_arguments; i++)
        {
            while (checkAttribute(p, "tmap:in:numinputs", "0"))
                p = Getattr(p, "tmap:in:next");

            SwigType *pt = Getattr(p, "type");
            String *ln = Getattr(p, "lname");
            String *ptrfail = NewString("");

            if (isGet)
            {
                // comes from const v8::AccessorInfo& inf
                sprintf(source, "inf");
                sprintf(accessor, "inf");

                Printf(ptrfail, "SWIG_PtrCheckFail(\"%s\")", fname);
                sprintf(argcheckfail, "SWIG_ArgCheckFail");

            }
            else if (isSet)
            {
                // comes from const v8::AccessorInfo& inf
                sprintf(accessor, "inf");
                sprintf(source, "args");
                Printf(ptrfail, "SWIG_PtrCheckFailV(\"%s\")", fname);
                sprintf(argcheckfail, "SWIG_ArgCheckFailV");
            }
            else
            {
                bool isSelf = !isGetSet && m_current == MEMBER_FUNC;
                if (isSelf)
                    offs = 1;

                if (isSelf && i==0)
                {
                    // self comes from arg structure
                    sprintf(source, "args");
                    sprintf(accessor, "args");
                }
                else
                {

                    // offset of self
                    sprintf(source, "args[%d]", i-offs);
                    sprintf(accessor, "args[%d]", i-offs);
                }

                Printf(ptrfail, "SWIG_PtrCheckFail(\"%s\")", fname);
                sprintf(argcheckfail, "SWIG_ArgCheckFail");

            }

            // Look for an input typemap
            if ((tm = Getattr(p, "tmap:in")))
            {
                Replaceall(tm, "$source",   source);
                Replaceall(tm, "$target",   ln);
                Replaceall(tm, "$input",    source);
                Replaceall(tm, "$accessor", accessor);
                Replaceall(tm, "$ptrfail",  ptrfail);

                Setattr(p, "emit:input", source);


                if (Getattr(p, "wrap:disown") || (Getattr(p, "tmap:in:disown")))
                    Replaceall(tm, "$disown", "SWIG_POINTER_DISOWN");
                else
                    Replaceall(tm, "$disown", "0");

                if ((checkfn = Getattr(p, "tmap:in:checkfn")))
                {
                    Printf(argument_check, "if(!%s(args, %i)){\n", checkfn, i-offs);
                    Printf(argument_check, " %s(\"%s\",\"%s\",%d);\n}\n", argcheckfail, fname, SwigType_str(pt, 0), i+1);
                }

                Printf(argument_parse, "%s\n", tm);
            }
            else
            {
                Swig_warning(WARN_TYPEMAP_IN_UNDEF, input_file, line_number,
                             "Unable to use type %s as a function argument.\n", SwigType_str(pt, 0));
            }

            p = Getattr(p, "tmap:in:next");

            Delete(ptrfail);

        }
        // add all argcheck code
        Printv(func->code, argument_check, argument_parse, NIL);


        // Check for trailing varargs
        if (varargs)
        {
            if (p && (tm = Getattr(p, "tmap:in")))
            {
                Replaceall(tm, "$input", "varargs");
                Printv(func->code, tm, "\n", NIL);
            }
        }


        // Insert constraint checking code
        for (p = prml; p;)
        {
            if ((tm = Getattr(p, "tmap:check")))
            {
                Replaceall(tm, "$target", Getattr(p, "lname"));
                Printv(func->code, tm, "\n", NIL);
                p = Getattr(p, "tmap:check:next");
            }
            else
                p = nextSibling(p);
        }

        // Insert cleanup code
        String *cleanup = NewString("");
        for (p = prml; p;)
        {
            if ((tm = Getattr(p, "tmap:freearg")))
            {
                Replaceall(tm, "$source", Getattr(p, "lname"));
                Printv(cleanup, tm, "\n", NIL);
                p = Getattr(p, "tmap:freearg:next");
            }
            else
                p = nextSibling(p);
        }

        // Insert argument output code
        String *outarg = NewString("");
        for (p = prml; p;)
        {
            if ((tm = Getattr(p, "tmap:argout")))
            {
                Replaceall(tm, "$source", Getattr(p, "lname"));
                Replaceall(tm, "$target", "result");
                Replaceall(tm, "$arg", Getattr(p, "emit:input"));
                Replaceall(tm, "$input", Getattr(p, "emit:input"));
                Printv(outarg, tm, "\n", NIL);
                p = Getattr(p, "tmap:argout:next");
            }
            else
                p = nextSibling(p);
        }

        Setattr(n, "wrap:name", wname);
        // Emit the function call
        String *actioncode = emit_action(n);

        // Return value if necessary
        if ((tm = Swig_typemap_lookup_out("out", n, "result", func, actioncode)))
        {
            String *typecode = SwigType_str(type, 0);
            Replaceall(typecode, "*", "");
            Replaceall(typecode, "&", "");


            if (isGet)
                Replaceall(tm, "$accessor", "inf");
            else
                Replaceall(tm, "$accessor", "args");

            Replaceall(tm, "$templatecode", typecode);
            Replaceall(tm, "$source", "result");
            if (GetFlag(n, "feature:new"))
                Replaceall(tm, "$owner", "1");
            else
                Replaceall(tm, "$owner", "0");
            Printf(func->code, "%s\n", tm);
        }
        else
            Swig_warning(WARN_TYPEMAP_OUT_UNDEF, input_file, line_number,
                         "Unable to use return type %s in function %s.\n", SwigType_str(type, 0), fname);

        if (!isSet)
        {
            if (!Strcmp(SwigType_str(type, 0), "void"))
                Printf(func->code, "return v8::Undefined();");
        }



        emit_return_variable(n, type, func);


        // Output argument output code
        Printv(func->code, outarg, NIL);

        // Output cleanup code
        Printv(func->code, cleanup, NIL);


        // Look to see if there is any newfree cleanup code
        if (GetFlag(n, "feature:new"))
        {
            if ((tm = Swig_typemap_lookup("newfree", n, "result", 0)))
            {
                Replaceall(tm, "$source", "result");
                Printf(func->code, "%s\n", tm);
            }
        }

        // See if there is any return cleanup code
        if ((tm = Swig_typemap_lookup("ret", n, "result", 0)))
        {
            Replaceall(tm, "$source", "result");
            Printf(func->code, "%s\n", tm);
        }
        Printf(func->code, "}\n");

        // Substitute the cleanup code
        Replaceall(func->code, "$cleanup", cleanup);

        // ubstitute the function name
        Replaceall(func->code, "$symname", iname);
        Replaceall(func->code, "$result", "result");

        // Dump the function out
        Wrapper_print(func, m_wrappers);


        // Now register the function with the interpreter.
        if (!Getattr(n, "sym:overloaded"))
        {
            if (m_current==NO_CPP || m_current==STATIC_FUNC) // emit normal fns & static fns
                Printv(m_globals, tab4, "{ \"", iname, "\", ", Swig_name_wrapper(iname), "},\n", NIL);
        }
        else
        {
            if (!Getattr(n, "sym:nextSibling"))
                dispatchFunction(n);
        }

        Delete(argument_check);
        Delete(argument_parse);
        Delete(cleanup);
        Delete(outarg);
        Delete(wname);
        DelWrapper(func);
        return SWIG_OK;
    }


    String *Protostr(ParmList *p) {
      String *out = NewStringEmpty();
      int i=0;
      while (p) {
          if (i==0)
          {
              i = 1;
                p = nextSibling(p);
                continue;
          }
        String *pstr = SwigType_str(Getattr(p, "type"), 0);
        Append(out, pstr);
        p = nextSibling(p);
        if (p) {
          Append(out, ",");
        }
        Delete(pstr);
      }
      return out;
    }

    void dispatchFunction(Node *n)
    {
        // Last node in overloaded chain

        int maxargs;
        String *tmp = NewString("");

        String *dispatch;

        bool isGlobal = m_current==NO_CPP || m_current==STATIC_FUNC || m_current==CONSTRUCTOR;

        if (isGlobal)
            dispatch = Swig_overload_dispatch(n, "return %s(args);", &maxargs);
        else
            dispatch = Swig_overload_dispatch_self(n, "return %s(args);", &maxargs);


        bool isGetSet = m_current == MEMBER_VAR || m_current == VARIABLE;


        // Generate a dispatch wrapper for all overloaded functions

        Wrapper *f = NewWrapper();
        String *name = Getattr(n, "name");
        String *symname = Getattr(n, "sym:name");
        String *wname = Swig_name_wrapper(symname);


        openFunction(f->def, wname);
        Wrapper_add_local(f, "argc", "int argc");

        Printf(tmp, "int argv[%d]={0", maxargs);
        for (int i = 1; i < maxargs; i++)
            Printf(tmp, ",%d", (i));

        Printf(tmp, "}");
        Wrapper_add_local(f, "argv", tmp);

        Printf(f->code, "argc = args.Length();\n");
        Replaceall(dispatch, "$args", "self,args");
        Replaceall(dispatch, "$accessor", "args");

        Printv(f->code, dispatch, "\n", NIL);

        Node *sibl = n;
        while (Getattr(sibl, "sym:previousSibling"))
            sibl = Getattr(sibl, "sym:previousSibling");    // go all the way up

        String *protoTypes = NewString("");
        do
        {
            String *protostr = isGlobal ? ParmList_protostr(Getattr(sibl, "wrap:parms")) : Protostr(Getattr(sibl, "wrap:parms"));

            Printf(protoTypes, "\n\"    %s(%s)\\n\"", SwigType_str(Getattr(sibl, "name"), 0), protostr);
        }
        while ((sibl = Getattr(sibl, "sym:nextSibling")));


        Printf(f->code, "return V8_Error(\"Wrong arguments for overloaded function '%s'\\n\"\n"
               "\"  Possible C/C++ prototypes are:\\n\"%s);\n",name,protoTypes);

        Delete(protoTypes);
        Printv(f->code, "}\n", NIL);
        Wrapper_print(f, m_wrappers);

        if (m_current==NO_CPP || m_current==STATIC_FUNC) // emit normal fns & static fns
            Printv(m_globals, tab4, "{ \"", symname, "\",", wname, "},\n", NIL);

        DelWrapper(f);
        Delete(dispatch);
        Delete(tmp);
        Delete(wname);
    }


    virtual int classHandler(Node *n)
    {
        String *rname  = Getattr(n, "name");
        String *cname  = Getattr(n, "sym:name");
        String *mname  = Swig_name_mangle(rname);

        if (!addSymbol(cname, n))
            return SWIG_ERROR;

        m_hasCtor = false;
        m_ctorName= 0;

        m_class_name = cname;
        m_members = NewString("");
        m_methods = NewString("");


        Printv(m_methods,  "\nstatic struct V8_Function v8_", mname, "_Methods[] = {\n", NIL);
        Printv(m_members,  "\nstatic struct V8_GetSetter v8_", mname, "_Members[] = {\n", NIL);

        Language::classHandler(n);

        SwigType *t = Copy(Getattr(n, "name"));
        SwigType_add_pointer(t);

        // Catch all: eg. a class with only static functions and/or variables will not have 'remembered'
        String *wcname = NewStringf("&v8_wrap_class_%s", mname);
        SwigType_remember_clientdata(t, wcname);


        String *rt = Copy(Getattr(n, "classtype"));
        SwigType_add_pointer(rt);


        Printf(m_methods, "    {0,0}\n};\n");
        Printv(m_wrappers, m_methods, NIL);

        Printf(m_members, "    {0,0,0}\n};\n");
        Printv(m_wrappers, m_members, NIL);


        String *bc = NewString("");
        String *bcn = NewString("");

        List *baselist = Getattr(n, "bases");
        if (baselist && Len(baselist))
        {
            Iterator b;
            int index = 0;
            b = First(baselist);
            while (b.item)
            {
                String *bname = Getattr(b.item, "name");
                if ((!bname) || GetFlag(b.item, "feature:ignore") || (!Getattr(b.item, "module")))
                {
                    b = Next(b);
                    continue;
                }

                String *bmangle = Swig_name_mangle(bname);
                Printf(bc, "0,");
                Printf(bcn, "\"%s\",", bmangle);
                Delete(bmangle);

                b = Next(b);
                index++;
            }
        }

        Printv(m_wrappers, "static V8_Class *v8_", mname, "_Bases[] = {", bc, "0};\n", NIL);
        Delete(bc);
        Printv(m_wrappers, "static const char *v8_", mname, "_BaseNames[] = {", bcn, "0};\n", NIL);
        Delete(bcn);


        Printv(m_wrappers, "static V8_Class v8_wrap_class_", mname, "= { \"", cname, "\",&SWIGTYPE", SwigType_manglestr(t), ", ", NIL);


        if (m_hasCtor)
        {
            Printf(m_wrappers, "%s", Swig_name_wrapper(Swig_name_construct(m_ctorName)));
            Delete(m_ctorName);
            m_ctorName = 0;
        }
        else
            Printf(m_wrappers, "0");


        Replaceall(rname, "(", "");
        Replaceall(rname, ")", "");

        Printf(m_wrappers, ",\n\tv8_%s_Methods, v8_%s_Members, v8_%s_Bases, v8_%s_BaseNames, sizeof(%s)};\n\n",
               mname, mname, mname, mname,
               rname);

        Delete(m_members);
        m_members = 0;
        Delete(m_methods);
        m_methods = 0;

        return SWIG_OK;
    }



};


extern "C" Language *swig_V8(void)
{
    return new V8();
}
