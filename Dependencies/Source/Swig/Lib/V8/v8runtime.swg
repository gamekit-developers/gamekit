/* --------------------------------------------------------------------------
 *
 * v8runtime.swg
 *
 * ------------------------------------------------------------------------- */
%runtime                    "swigrun.swg";
%runtime                    "v8run.swg";
%insert(initbeforefunc)     "swiginit.swg"

%insert(initbeforefunc) %{

/* ------------------------------------------------------------------------- */
SWIGRUNTIME bool
V8_HasBase(swig_type_info *inf) {
  if (inf && inf->clientdata) {
    V8_Class *cls = (V8_Class *)inf->clientdata;
    return cls && cls->bases && cls->bases[0] != 0;
  }
  return false;
}

/* ------------------------------------------------------------------------- */
SWIGRUNTIME swig_type_info *
V8_GetBaseClass(const char *name) {
  int i;
  for (i = 0; swig_types[i]; i++) {
    swig_type_info *inf = swig_types[i];
    V8_Class *cls = (V8_Class *)inf->clientdata;
    if (cls && !strcmp(cls->name, name))
      return inf;
  }
  return 0;
}

/* ------------------------------------------------------------------------- */
SWIGRUNTIME void
V8_BindClass(v8::Persistent<v8::ObjectTemplate>& module, V8_Class *cls, V8_Class *base = 0) {
  int i;

  /* Store in function template */
  cls->tmpl = v8::Persistent<v8::FunctionTemplate>::New(v8::FunctionTemplate::New());
  v8::Persistent<v8::FunctionTemplate>& tmpl = cls->tmpl;

  /* write constructor */
  tmpl->SetClassName(v8::String::New(cls->name));
  if (cls->ctor)
    tmpl->SetCallHandler(cls->ctor);


  v8::Local<v8::ObjectTemplate> clsInst = tmpl->PrototypeTemplate();
  clsInst->SetInternalFieldCount(1);
  if (cls->methods) {
    for (i = 0; cls->methods[i].name; i++)
      clsInst->Set(v8::String::New(cls->methods[i].name), v8::FunctionTemplate::New(cls->methods[i].func));
  }

  /* Apply accessors */
  if (cls->members) {

    for (i = 0; cls->members[i].name; i++) {
      clsInst->SetAccessor( v8::String::New(cls->members[i].name),
                            cls->members[i].getf,
                            cls->members[i].setf,
                            v8::Handle<v8::Value>(),
                            cls->members[i].accm,
                            cls->members[i].prop
                          );
    }
  }

  if (base != 0) {
    /* Inherit from base class */
    tmpl->Inherit(base->tmpl);

  }
  module->Set(v8::String::New(cls->name), tmpl);
}

/* ------------------------------------------------------------------------- */
SWIGRUNTIME void
V8_BindClasses(v8::Persistent<v8::ObjectTemplate>& module)
{
  int i;

  /* Roots */
  for (i = 0; swig_types[i]; i++) {
    if (!V8_HasBase(swig_types[i])) {
      if (swig_types[i]->clientdata)
        V8_BindClass(module, (V8_Class *)swig_types[i]->clientdata, 0);
    }
  }


  /* Derrived */
  for (i = 0; swig_types[i]; i++) {
    if (V8_HasBase(swig_types[i])) {
      if (swig_types[i]->clientdata) {
        V8_Class *cls = (V8_Class *)swig_types[i]->clientdata;
        swig_type_info *bse = V8_GetBaseClass(cls->base_names[0]);
        V8_BindClass(module, cls, bse ? (V8_Class *)bse->clientdata : 0);
      }
    }
  }
}


/* ------------------------------------------------------------------------- */
SWIGRUNTIME void
V8_InitBaseClasses(void)
{
  int i;
  for (i = 0; swig_types[i]; i++) {
    swig_type_info *inf = swig_types[i];
    V8_Class *cls = (V8_Class *)inf->clientdata;

    if (cls && cls->base_names != 0) {
      int j;
      for (j=0; cls->base_names[j]; ++j) {
        swig_type_info *binf = V8_GetBaseClass(cls->base_names[j]);
        if (binf)
          cls->bases[j] = (V8_Class *)binf->clientdata;
      }
    }

  }
}

/* ------------------------------------------------------------------------- */
SWIGEXPORT v8::Persistent<v8::ObjectTemplate> SWIG_init(v8::Persistent<v8::ObjectTemplate>& context)
{
  int i;

  // create the module template
  v8::Persistent<v8::ObjectTemplate> module = v8::Persistent<v8::ObjectTemplate>::New(v8::ObjectTemplate::New());
  context->Set(v8::String::New(SWIG_name), module);


  SWIG_InitializeModule((void *)(*module));
  SWIG_PropagateClientData();


  // Module global variables, constants
  for (i = 0; v8_constants[i].name; i++) {
    module->SetAccessor( v8::String::New(v8_constants[i].name),
                         v8_constants[i].getf,
                         v8_constants[i].setf,
                         v8::Handle<v8::Value>(),
                         v8_constants[i].accm,
                         v8_constants[i].prop
                       );
  }


  // Module global functions
  for (i = 0; v8_globals[i].name; i++)
    module->Set(v8::String::New(v8_globals[i].name), v8::FunctionTemplate::New(v8_globals[i].func));


  V8_InitBaseClasses();
  V8_BindClasses(module);
  return module;
}

%}
