/*
-------------------------------------------------------------------------------
--                                                                           --
--  This file is part of GameKit.                                            --
--  http://gamekit.googlecode.com/                                           --
--                                                                           --
--                                                                           --
--  Released under the zlib license.                                         --
--  http://www.gzip.org/zlib/zlib_license.html                               --
--                                                                           --
--                                                                           --
-------------------------------------------------------------------------------
*/
#include "OgreKitCore.h"
namespace OgreKit {


///////////////////////////////////////////////////////////////////////////////
// 
// OgreKit.Property
// 
///////////////////////////////////////////////////////////////////////////////

Property::Property(gkVariable *var) : m_prop(var)
{
}


Property::Property() : m_prop(0)
{
}



Property::Property(const String& name, bool value)
{
    m_prop = new gkVariable(name, false);
    m_prop->setValue(value);
}



Property::Property(const String& name, double value)
{
    m_prop = new gkVariable(name, false);
    m_prop->setValue((float)value);
}

Property::Property(const String& name, const String &value)
{
    m_prop = new gkVariable(name, false);
    m_prop->setValue(value);
}


Property::Property(const Property& oth)
{
    m_prop = new gkVariable(oth.getName(), false);
    m_prop->setValue(oth.getValue());
}


Property::~Property()
{
    makeDebug(false);
    delete m_prop;
}


const String& Property::getName(void) const
{
    static String localStr = "";
    if (m_prop)
        return m_prop->getName();
    return localStr;
}


const String& Property::getValue(void) const
{
    static String localStr;
    if (m_prop)
        localStr = m_prop->getValueString();
    return localStr;
}


void Property::makeDebug(bool v) 
{
    if (m_prop && gkEngine::getSingletonPtr())
    {
        m_prop->setDebug(v);
        if (v)
            gkEngine::getSingleton().addDebugProperty(m_prop);
        else
            gkEngine::getSingleton().removeDebugProperty(m_prop);
    }
}


PropertyType Property::getType(void)  const
{

    if (m_prop)
    {
        switch (m_prop->getType())
        {
        case gkVariable::VAR_BOOL:
            return PROP_BOOL;
        case gkVariable::VAR_INT:
        case gkVariable::VAR_REAL:
            return PROP_NUMBER;
        // TODO
        default:
            break;
        }
        return PROP_STRING;
    }
    return PROP_NULL;
}


bool Property::toBool(void)  const
{
    if (m_prop)
        return m_prop->getValueBool();
    return false;
}


double Property::toNumber(void)  const
{
    if (m_prop)
        return (double)m_prop->getValueReal();
    return -1;

}


String Property::toString(void)  const
{
    if (m_prop)
        return m_prop->getValueString();
    return "";
}


void Property::fromBool(bool v)
{
    if (m_prop)
        m_prop->setValue(v);
}

void Property::fromNumber(double v)
{
    if (m_prop)
        m_prop->setValue(v);
}


void Property::fromString(const String &v)
{
    if (m_prop)
        m_prop->setValue(v);
}


#define PROP_INSERT_T(name, var, cast)\
    m_properties.insert(name, new Property(name, (cast)m_defs->var));

#define PROP_INSERT_N(name, var) PROP_INSERT_T(name, var, double)
#define PROP_INSERT_B(name, var) PROP_INSERT_T(name, var, bool)
#define PROP_INSERT_S(name, var) PROP_INSERT_T(name, var, gkString)


static void gkVar_fromVar(gkVariable &v, bool &r) {
    r = v.getValueBool();
}

static void gkVar_fromVar(gkVariable &v, int &r) {
    r = v.getValueInt();
}
static void gkVar_fromVar(gkVariable &v, gkScalar &r) {
    r = v.getValueReal();
}
static void gkVar_fromVar(gkVariable &v, gkString &r) {
    r = v.getValueString();
}


#define PROP_SET_T(name, var, cast)\
    if (v.getName() == name )\
        gkVar_fromVar(v, (cast)m_defs->var);

#define PROP_SET_TE(name, var, cast)\
    if (v.getName() == name )\
    {\
        int c = (int)m_defs->var;\
        gkVar_fromVar(v, c);\
        m_defs->var = (cast)c;\
    }

#define PROP_SET_I(name, var) PROP_SET_T(name, var, int&)
#define PROP_SET_N(name, var) PROP_SET_T(name, var, gkScalar&)
#define PROP_SET_B(name, var) PROP_SET_T(name, var, bool&)
#define PROP_SET_S(name, var) PROP_SET_T(name, var, gkString&)
#define PROP_SET_E(name, var, E) PROP_SET_TE(name, var, E)



///////////////////////////////////////////////////////////////////////////////
// 
// OgreKit.UserDefs
// 
///////////////////////////////////////////////////////////////////////////////



UserDefs::UserDefs(gkUserDefs *defs)
{
    m_defs = defs;

    PROP_INSERT_N("renderSystem",       rendersystem);
    PROP_INSERT_N("sceneManager",       sceneManager);
    PROP_INSERT_B("verbose",            verbose);
    PROP_INSERT_N("winx",               winsize.x);
    PROP_INSERT_N("winy",               winsize.y);
    PROP_INSERT_S("winTitle",           wintitle);
    PROP_INSERT_B("fullScreen",         fullscreen);
    PROP_INSERT_B("blenderMat",         blendermat);
    PROP_INSERT_B("grabInput",          grabInput);
    PROP_INSERT_B("debugFps",           debugFps);
    PROP_INSERT_B("debugPhysics",       debugPhysics);
    PROP_INSERT_B("debugPhysicsAABB",   debugPhysicsAabb);
    PROP_INSERT_B("usebulletDBVT",      useBulletDbvt);
    PROP_INSERT_B("showDebugProps",     showDebugProps);
    PROP_INSERT_B("debugSounds",        debugSounds);
    PROP_INSERT_B("enableShadows",      enableshadows);
    PROP_INSERT_S("shadowTechnique",    shadowtechnique);
    PROP_INSERT_N("colourShadowR",      colourshadow.r);
    PROP_INSERT_N("colourShadowG",      colourshadow.g);
    PROP_INSERT_N("colourShadowB",      colourshadow.b);
    PROP_INSERT_N("farDistanceShadow",  fardistanceshadow);
}



UserDefs::~UserDefs()
{
    UTsize p=0; 
    for (; p< m_properties.size(); ++p)
        delete m_properties.at(p);

    m_properties.clear();
}


void UserDefs::setValueEvent(gkVariable &v)
{
    if (m_properties.find(gkHashedString(v.getName())) != UT_NPOS)
    {
        PROP_SET_E("renderSystem",       rendersystem, OgreRenderSystem);
        PROP_SET_I("sceneManager",       sceneManager);
        PROP_SET_B("verbose",            verbose);
        PROP_SET_N("winx",               winsize.x);
        PROP_SET_N("winy",               winsize.y);
        PROP_SET_S("winTitle",           wintitle);
        PROP_SET_B("fullScreen",         fullscreen);
        PROP_SET_B("blenderMat",         blendermat);
        PROP_SET_B("grabInput",          grabInput);
        PROP_SET_B("debugFps",           debugFps);
        PROP_SET_B("debugPhysics",       debugPhysics);
        PROP_SET_B("debugPhysicsAABB",   debugPhysicsAabb);
        PROP_SET_B("usebulletDBVT",      useBulletDbvt);
        PROP_SET_B("showDebugProps",     showDebugProps);
        PROP_SET_B("debugSounds",        debugSounds);
        PROP_SET_B("enableShadows",      enableshadows);
        PROP_SET_S("shadowTechnique",    shadowtechnique);
        PROP_SET_N("colourShadowR",      colourshadow.r);
        PROP_SET_N("colourShadowG",      colourshadow.g);
        PROP_SET_N("colourShadowB",      colourshadow.b);
        PROP_SET_N("farDistanceShadow",  fardistanceshadow);
    }
}



const Property& UserDefs::getProperty(const String& name)
{
    return __getitem__(name.c_str());
}



void UserDefs::addProperty(const Property& prop)
{
    if (!prop.getName().empty())
    {
        if (m_properties.find(prop.getName()) == UT_NPOS)
            m_properties.insert(prop.getName(), new Property(prop));
    }
}




bool UserDefs::hasProperty(const String& name)
{
    return m_properties.find(name) != UT_NPOS;
}



const Property& UserDefs::__getitem__(const char* name)
{
    static Property *prop=0;
    UTsize pos = 0;
    if ((pos = m_properties.find(gkHashedString(name))) != UT_NPOS)
    {
        delete prop;
        prop = new Property((const Property&)(*(m_properties.at(pos))));
    }
    else
    {
        if (prop)
            delete prop;
         prop = new Property();
    }
    return *prop;
}

void UserDefs::__setitem__(const char* name, bool  v)
{
    UTsize pos = 0;
    if ((pos = m_properties.find(gkHashedString(name))) != UT_NPOS)
    {
        (m_properties.at(pos))->fromBool(v);
        setValueEvent(m_properties.at(pos)->getClassRef());
    }
}

void UserDefs::__setitem__(const char* name, double v)
{
    UTsize pos = 0;
    if ((pos = m_properties.find(gkHashedString(name))) != UT_NPOS)
    {
        (m_properties.at(pos))->fromNumber(v);
        setValueEvent(m_properties.at(pos)->getClassRef());
    }
}

void UserDefs::__setitem__(const char* name, const String& v)
{
    UTsize pos = 0;
    if ((pos = m_properties.find(gkHashedString(name))) != UT_NPOS)
    {
        (m_properties.at(pos))->fromString(v);
        setValueEvent(m_properties.at(pos)->getClassRef());
    }
}




///////////////////////////////////////////////////////////////////////////////
// 
// OgreKit.Mouse
// 
///////////////////////////////////////////////////////////////////////////////

Mouse::Mouse()
:	xpos(0), ypos(0),
    xrel(0), yrel(0),
    winx(0), winy(0),
    wheel(0), moved(false)
{
}


Mouse::~Mouse()
{
}

void Mouse::capture(void)
{
	if (!gkWindowSystem::getSingletonPtr()) return;

	gkMouse *mse = gkWindowSystem::getSingleton().getMouse();
    xpos    = mse->position.x;
    ypos    = mse->position.y;
    xrel    = mse->relitave.x;
    yrel    = mse->relitave.y;
    winx    = mse->winsize.x;
    winy    = mse->winsize.y;
    wheel   = mse->wheelDelta;
    moved   = mse->moved;
}

bool Mouse::isButtonDown(MouseButton btn)
{
	if (!gkWindowSystem::getSingletonPtr()) return false;
	return gkWindowSystem::getSingleton().getMouse()->isButtonDown(btn);
}

///////////////////////////////////////////////////////////////////////////////
// 
// OgreKit.Keyboard
// 
///////////////////////////////////////////////////////////////////////////////
Keyboard::Keyboard()
{
}

Keyboard::~Keyboard()
{
}

bool Keyboard::isKeyDown(int sc)
{
	if (!gkWindowSystem::getSingletonPtr()) return false;
	return gkWindowSystem::getSingleton().getKeyboard()->isKeyDown((gkScanCode)sc);
}


///////////////////////////////////////////////////////////////////////////////
// 
// OgreKit.Engine
// 
///////////////////////////////////////////////////////////////////////////////


// local Events
class EngineEventObject : public gkLuaEvent
{
public:

    EngineEventObject(Function fnc) : gkLuaEvent(fnc) {}
    EngineEventObject(Self self, Function fnc) : gkLuaEvent(self, fnc) {}

    ~EngineEventObject() 
    {
    }
};



Engine::Engine()
{
    m_defs = 0;
    m_running = false;
    if (m_ctxOwner = ((m_engine = gkEngine::getSingletonPtr()) == 0))
    {
        m_engine = new gkEngine();
        m_engine->setListener(this);
    }
}



Engine::~Engine()
{
    if (m_defs)
        delete m_defs;


    for (UTsize i=0; i<m_ticks.size(); ++i)
        delete m_ticks.at(i);


    if (m_ctxOwner)
    {
        m_running = false;
        delete m_engine;
    }
}


void Engine::connect(int evt, Function func)
{
    if (evt == EVT_TICK)
    {
        EngineEventObject *ob = new EngineEventObject(func);
        m_ticks.push_back(ob);
    }
}

void Engine::connect(int evt, Self self, Function method)
{
    if (evt == EVT_TICK)
    {
        EngineEventObject *ob = new EngineEventObject(self, method);
        m_ticks.push_back(ob);
    }
}
 
void Engine::tick(gkScalar delta)
{
    UTsize i;
    for (i=0; i<m_ticks.size(); ++i)
    {
        EngineEventObject *ob = m_ticks.at(i);

        ob->beginCall();
        ob->addArgument(delta);
        if (!ob->call())
        {
            m_ticks.erase(i);
            delete ob;
            if (i > 0)
            {
                i -= 1;
            }
            else break;
            continue;
        }
    }
}



void Engine::initialize(void)
{
    if (m_ctxOwner && m_engine && !m_engine->isInitialized())
        m_engine->initialize();
}

void Engine::requestExit(void)
{
	if (m_engine)
		m_engine->requestExit();
}

UserDefs& Engine::getUserDefs(void)
{
    if (m_defs == 0)
        m_defs = new UserDefs(&m_engine->getUserDefs());
    return *m_defs;
}



Pointer<Scene> Engine::loadBlendFile(const String& name)
{
    if (m_engine && !m_running)
    {
        if (!m_engine->isInitialized())
            gkLogMessage("Engine: loadBlendFile on uninitialized engine.");

        gkBlendFile *gkb = m_engine->loadBlendFile(name, "<gkBuiltin>");
        if (gkb->getSceneIterator().hasMoreElements())
            return Pointer<Scene>(new Scene(gkb->getSceneIterator().getNext()));
        else
            gkLogMessage("Engine: no usable scenes found in blend.");
    }


    return Pointer<Scene>();
}


void Engine::run(void)
{
    if (!m_running && m_engine)
    {
        m_running = true;
        m_engine->run();
    }
}



///////////////////////////////////////////////////////////////////////////////
// 
// OgreKit.Loadable
// 
///////////////////////////////////////////////////////////////////////////////


Loadable::Loadable() : m_object(0)
{
}

Loadable::Loadable(gkObject *ob) : m_object(ob)
{
}


void Loadable::load(void)
{
    if (m_object)
        m_object->load();
}


void Loadable::unload(void)
{
    if (m_object)
        m_object->unload();
}



void Loadable::reload(void)
{
    if (m_object)
        m_object->reload();
}


String Loadable::getName(void)
{
    if (m_object)
        return m_object->getName();
    return "";
}

///////////////////////////////////////////////////////////////////////////////
// 
// OgreKit.Scene
// 
///////////////////////////////////////////////////////////////////////////////


Scene::Scene()
{
}

Scene::Scene(gkObject *ob) : Loadable(ob)
{
}


bool Scene::hasObject(const String&name)
{
    if (m_object)
        return cast<gkScene>()->hasObject(name);
    return false;
}



Pointer<GameObject> Scene::getObject(const String&name)
{
    if (m_object)
    {
        gkGameObject *gobj = cast<gkScene>()->getObject(name);
        if (gobj)
            return Pointer<GameObject>(new GameObject(gobj));
    }
    return Pointer<GameObject>();
}

Pointer<Entity> Scene::getEntity(const String& name)
{
    if (m_object)
    {
        gkGameObject *gobj = cast<gkScene>()->getObject(name);
        if (gobj && gobj->getType() == GK_ENTITY)
            return Pointer<Entity>(new Entity(gobj));
    }
    return Pointer<Entity>();
}

Pointer<Camera> Scene::getCamera(const String& name)
{
    if (m_object)
    {
        gkGameObject *gobj = cast<gkScene>()->getObject(name);
        if (gobj && gobj->getType() == GK_CAMERA)
            return Pointer<Camera>(new Camera(gobj));
    }
    return Pointer<Camera>();
}


Pointer<Light> Scene::getLight(const String& name)
{
    if (m_object)
    {
        gkGameObject *gobj = cast<gkScene>()->getObject(name);
        if (gobj && gobj->getType() == GK_LIGHT)
            return Pointer<Light>(new Light(gobj));
    }
    return Pointer<Light>();
}


Pointer<GameObject> Scene::createEmpty(const String &name)
{
    if (m_object)
    {
        gkScene *scene = cast<gkScene>();
        if (!scene->hasObject(name))
        {
            gkGameObject *obj = scene->createObject(name);
            return Pointer<GameObject>(new GameObject(obj));
        }
    }

    return Pointer<GameObject>();
}


//////////////////////////////////////////////////////////////////////////////
// 
// OgreKit.GameObject
// 
///////////////////////////////////////////////////////////////////////////////


GameObject::GameObject()
{
}

GameObject::GameObject(gkObject *ob) : Loadable(ob)
{
}



Vector3 GameObject::getPosition(void)
{
    if (m_object)
        return Vector3(cast<gkGameObject>()->getPosition());
    return Vector3();
}


Vector3 GameObject::getRotation(void)
{
    if (m_object)
        return Vector3(cast<gkGameObject>()->getRotation().toVector3());
    return Vector3();
}


Quaternion GameObject::getOrientation(void)
{
    if (m_object)
        return Quaternion(cast<gkGameObject>()->getOrientation());
    return Quaternion();
}


Vector3 GameObject::getScale(void)
{
    if (m_object)
        return Vector3(cast<gkGameObject>()->getScale());
    return Vector3();
}

Vector3 GameObject::getWorldPosition(void)
{
    if (m_object)
        return Vector3(cast<gkGameObject>()->getWorldPosition());
    return Vector3();
}


Vector3 GameObject::getWorldRotation(void)
{
    if (m_object)
        return Vector3(cast<gkGameObject>()->getWorldRotation().toVector3());
    return Vector3();
}


Quaternion GameObject::getWorldOrientation(void)
{
    if (m_object)
        return Quaternion(cast<gkGameObject>()->getWorldOrientation());
    return Quaternion();
}


Vector3 GameObject::getLinearVelocity(void)
{
    if (m_object)
        return Vector3(cast<gkGameObject>()->getLinearVelocity());
    return Vector3();
}



Vector3 GameObject::getAngularVelocity(void)
{
    if (m_object)
        return Vector3(cast<gkGameObject>()->getAngularVelocity());
    return Vector3();
}



void GameObject::setLinearVelocity(const Vector3 &v)
{
    if (m_object)
        cast<gkGameObject>()->setLinearVelocity(v);
}


void GameObject::setLinearVelocity(float x, float y, float z)
{
    if (m_object)
        cast<gkGameObject>()->setLinearVelocity(gkVector3(x,y,z));
}



void GameObject::setAngularVelocity(const Vector3 &v)
{
    if (m_object)
        cast<gkGameObject>()->setAngularVelocity(v);
}


void GameObject::setAngularVelocity(float x, float y, float z)
{
    if (m_object)
        cast<gkGameObject>()->setAngularVelocity(gkVector3(x,y,z));
}



void GameObject::setPosition(const Vector3 &v)
{
    if (m_object)
        cast<gkGameObject>()->setPosition(v);
}

void GameObject::setPosition(float x, float y, float z)
{
    if (m_object)
        cast<gkGameObject>()->setPosition(gkVector3(x,y,z));
}



void GameObject::setRotation(const Vector3 &v)
{
    if (m_object)
        cast<gkGameObject>()->setOrientation(gkEuler(v));
}

void GameObject::setRotation(float yaw, float pitch, float roll)
{
    if (m_object)
		cast<gkGameObject>()->setOrientation(gkEuler(yaw, pitch, roll));
}



void GameObject::setOrientation(const Quaternion &quat)
{
    if (m_object)
        cast<gkGameObject>()->setOrientation(quat);
}

void GameObject::setOrientation(float w, float x, float y, float z)
{
    if (m_object)
        cast<gkGameObject>()->setOrientation(gkQuaternion(w, x, y, z));
}



GameObjectTypes GameObject::getType(void)
{
    if (m_object)
        return (GameObjectTypes)cast<gkGameObject>()->getType();
    return OB_UNKNOWN;
}



void GameObject::rotate(float dx, float dy, float dz)
{
    if (m_object)
        cast<gkGameObject>()->rotate(gkEuler(dx, dy, dz), TRANSFORM_PARENT);
}


void GameObject::rotate(const Vector3 &v)
{
    if (m_object)
        cast<gkGameObject>()->rotate(gkEuler(v), TRANSFORM_PARENT);
}


void GameObject::rotate(const Quaternion &v)
{
    if (m_object)
        cast<gkGameObject>()->rotate(v, TRANSFORM_PARENT);
}


void GameObject::rotate(float dx, float dy, float dz, TransformSpace ts)
{
    if (m_object)
        cast<gkGameObject>()->rotate(gkEuler(dx, dy, dz), ts);
}

void GameObject::rotate(const Vector3 &v, TransformSpace ts)
{
    if (m_object)
        cast<gkGameObject>()->rotate(gkEuler(v), ts);
}

void GameObject::rotate(const Quaternion &v, TransformSpace ts)
{
    if (m_object)
        cast<gkGameObject>()->rotate(v, ts);
}


void GameObject::translate(const Vector3 &v)
{
    if (m_object)
        cast<gkGameObject>()->translate(v, TRANSFORM_PARENT);
}

void GameObject::translate(float x, float y, float z)
{
    if (m_object)
        cast<gkGameObject>()->translate(gkVector3(x,y,z), TRANSFORM_PARENT);
}

void GameObject::translate(const Vector3 &v, TransformSpace ts)
{
    if (m_object)
        cast<gkGameObject>()->translate(v, ts);
}

void GameObject::translate(float x, float y, float z, TransformSpace ts)
{
    if (m_object)
        cast<gkGameObject>()->translate(gkVector3(x,y,z), ts);
}




void GameObject::scale(const Vector3 &v)
{
    if (m_object)
        cast<gkGameObject>()->scale(v);
}


void GameObject::scale(float x, float y, float z)
{
    if (m_object)
        cast<gkGameObject>()->scale(gkVector3(x,y,z));
}



void GameObject::yaw(float deg)
{
    if (m_object)
        cast<gkGameObject>()->yaw(gkDegree(deg), TRANSFORM_PARENT);
}



void GameObject::yaw(float deg, TransformSpace ts)
{
    if (m_object)
        cast<gkGameObject>()->yaw(gkRadian(deg), ts);
}




void GameObject::pitch(float deg)
{
    if (m_object)
        cast<gkGameObject>()->pitch(gkRadian(deg), TRANSFORM_PARENT);
}

void GameObject::pitch(float deg, TransformSpace ts)
{
    if (m_object)
        cast<gkGameObject>()->pitch(gkRadian(deg), ts);
}

void GameObject::roll(float deg)
{
    if (m_object)
        cast<gkGameObject>()->roll(gkRadian(deg), TRANSFORM_PARENT);
}

void GameObject::roll(float deg, TransformSpace ts)
{
    if (m_object)
        cast<gkGameObject>()->roll(gkRadian(deg), ts);
}


Property GameObject::__getitem__(const String&prop)
{
    if (m_object)
    {
        gkVariable *var = cast<gkGameObject>()->getVariable(prop);
        if (var)
            return Property(var);
    }
    return Property();
}


void GameObject::__setitem__(const String&prop, bool  v)
{
    if (m_object)
    {
        gkVariable *var = cast<gkGameObject>()->getVariable(prop);
        if (var)
            (*var).setValue(v);
    }
}

void GameObject::__setitem__(const String&prop, float v)
{
    if (m_object)
    {
        gkVariable *var = cast<gkGameObject>()->getVariable(prop);
        if (var)
            (*var).setValue(v);
    }
}


void GameObject::__setitem__(const String&prop, const char* v)
{
    if (m_object)
    {
        gkVariable *var = cast<gkGameObject>()->getVariable(prop);
        if (var)
            (*var).setValue(gkString(v));
    }
}

Pointer<Scene> GameObject::getScene(void)
{
    if (m_object)
        return Pointer<Scene>(new Scene(cast<gkGameObject>()->getOwner()));
    return Pointer<Scene>();
}


int GameObject::getState(void)
{
    if (m_object)
        return cast<gkGameObject>()->getState();
    return -1;
}




Pointer<Entity> GameObject::getEntity(void)
{
    if (m_object && cast<gkGameObject>()->getType() == GK_ENTITY)
        return Pointer<Entity>(new Entity(m_object));
    return Pointer<Entity>();
}



Pointer<Camera> GameObject::getCamera(void)
{
    if (m_object && cast<gkGameObject>()->getType() == GK_CAMERA)
        return Pointer<Camera>(new Camera(m_object));
    return Pointer<Camera>();
}

Pointer<Light> GameObject::getLight(void)
{
    if (m_object && cast<gkGameObject>()->getType() == GK_LIGHT)
        return Pointer<Light>(new Light(m_object));
    return Pointer<Light>();
}


bool GameObject::hasParent()
{
    return m_object && cast<gkGameObject>()->getParent() != 0;
}


void GameObject::setParent(const Pointer<GameObject>& par)
{
    if (m_object && !par.isNull())
    {
        gkGameObject *gobj = cast<gkGameObject>();

        if (gobj->getParent() == 0)
            gobj->setParent(par->cast<gkGameObject>());
    }
}


Pointer<GameObject> GameObject::getParent(void)
{
    return m_object && hasParent() ? 
        Pointer<GameObject>(new GameObject(cast<gkGameObject>()->getParent())) :
        Pointer<GameObject>();
}



void GameObject::enableContacts(bool v)
{
    if (m_object)
    {
        if (v)
            m_object->setFlags(m_object->getFlags() | gkObject::RBF_CONTACT_INFO);
        else 
            m_object->setFlags(m_object->getFlags() & ~gkObject::RBF_CONTACT_INFO);
    }
}

bool GameObject::hasContacts()
{
    if (m_object)
        return !m_object->getContacts().empty();
    return false;
}

bool GameObject::hasContact(const String &object)
{
    if (m_object)
    {
        gkObject::ContactArray &arr = m_object->getContacts();
        utArrayIterator<gkObject::ContactArray> iter(arr);
        while (iter.hasMoreElements())
        {
            if (iter.getNext().collider->getName() == object)
                return true;
        }
    }
    return false;
}


///////////////////////////////////////////////////////////////////////////////
// 
// OgreKit.Entity
// 
///////////////////////////////////////////////////////////////////////////////


Entity::Entity()
{
}

Entity::Entity(gkObject *ob) : GameObject(ob)
{
}


void Entity::playAction(const String&name, float blend)
{
    if (m_object)
        cast<gkEntity>()->playAction(name, blend);
}



///////////////////////////////////////////////////////////////////////////////
// 
// OgreKit.Light
// 
///////////////////////////////////////////////////////////////////////////////



Light::Light()
{
}

Light::Light(gkObject *ob) : GameObject(ob)
{
}




///////////////////////////////////////////////////////////////////////////////
// 
// OgreKit.Camera
// 
///////////////////////////////////////////////////////////////////////////////



Camera::Camera()
{
}

Camera::Camera(gkObject *ob) : GameObject(ob)
{
}


void  Camera::setClipping(float start, float end)
{
    if (m_object)
        cast<gkCamera>()->setClip(gkMax(start, 0.0001f), end);
}

float Camera::getClipStart()
{
    if (m_object)
        return cast<gkCamera>()->getCameraProperties().m_clipstart;
    return 0;
}


float Camera::getClipEnd()
{
    if (m_object)
        return cast<gkCamera>()->getCameraProperties().m_clipend;
    return 0;
}

void  Camera::setFov(float fov)
{
    if (m_object)
        cast<gkCamera>()->setFov(gkDegree(fov));
}

float Camera::getFov()
{
    if (m_object)
        return cast<gkCamera>()->getCameraProperties().m_fov;
    return 0;
}

void Camera::makeCurrent()
{
    if (m_object)
        cast<gkCamera>()->makeCurrent();
}





///////////////////////////////////////////////////////////////////////////////
// 
// OgreKit.Debugger
// 
///////////////////////////////////////////////////////////////////////////////



Debugger::Debugger(const Pointer<Scene> &sc) 
    :   m_debugger(0)
{
    if (!sc.isNull())
    {
        gkScene *scene = sc->cast<gkScene>();
        if (scene)
            m_debugger = scene->getDebugger();

    }
}


void Debugger::drawLine(const Vector3& from, const Vector3& to, const Vector3& color)
{
    if (m_debugger)
        m_debugger->drawLine(from, to, color);
}


void Debugger::drawObjectAxis(const Pointer<GameObject> &ptr, float size)
{
    if (m_debugger && !ptr.isNull())
    {
        gkGameObject *ob = ptr->cast<gkGameObject>();
        if (ob)
        {
            const gkVector3     &axis   = ob->getWorldPosition();
            const gkQuaternion  &ori    = ob->getOrientation();

            gkVector3 x = (ori * gkVector3(size, 0, 0));
            gkVector3 y = (ori * gkVector3(0, size, 0));
            gkVector3 z = (ori * gkVector3(0, 0, size));


            m_debugger->drawLine(axis, axis + x, gkVector3(1,0,0));
            m_debugger->drawLine(axis, axis + y, gkVector3(0,1,0));
            m_debugger->drawLine(axis, axis + z, gkVector3(0,0,1));
        }
    }
}

void Debugger::clear(void)
{
    if (m_debugger)
        m_debugger->clear();
}


void DebugPrint(const char *str)
{
    gkDebugScreen::printTo(str);
}



}
