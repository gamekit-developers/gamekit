/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): none yet.
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
#include "luUtils.h"
#include "OgreKit.h"

#define LU_GetNamdSocketFunc(cls, name, base)               \
static int cls##_##name(luObject& L)                        \
{                                                           \
    luNode *ob = L.getValueClassT<luNode>(1);               \
    if (ob)                                                 \
    {                                                       \
    base *ncls = static_cast<base*>(&ob->node());           \
    new (&luSocket::Type, L) luSocket(ncls->name());        \
    return 1;                                               \
    }                                                       \
    return 0;                                               \
}                                                           \
 

enum luNodes
{
    NT_ANIMATION,   // ^
    NT_ARCBALL,     // TODO
    NT_CURSOR,      // TODO
    NT_EXIT,        // ^
    NT_GROUP,       // TODO
    NT_IF,          // ^
    NT_KEY,         // ^
    NT_MATH,        // TODO
    NT_MOTION,      // ^
    NT_MOUSE_BUTTON,// ^
    NT_MOUSE_MOTION,// ^
    NT_OBJECT,      // TODO
    NT_PICK,        // TODO
    NT_PRINT,       // ^
    NT_RANDOM,      // TODO
    NT_SWITCH,      // TODO
    NT_TIMER,       // TODO
    NT_VALUE,       // TODO
    NT_VARIABLE,    // TODO
    // TODO newer nodes
};


// ----------------------------------------------------------------------------
//
// Socket Class
//
// ----------------------------------------------------------------------------
class luSocket : public luClass
{
protected:
    gkLogicSocket *m_socket;

public:
    static luMethodDef Methods[];
    static luTypeDef Type;

public:

    luSocket(gkLogicSocket *sock) : m_socket(sock) {}
    gkLogicSocket &socket(void) {GK_ASSERT(m_socket); return *m_socket;}

    luTypeDef *getType(void)
    {
        return &Type;
    }
};


static int luSocket_setValue(luObject &L)
{
    luSocket *sock = L.getValueClassT<luSocket>(1);
    if (sock)
    {
        gkLogicSocket &socket = sock->socket();
        if (socket.getType() == gkLogicSocket::ST_GAME_OBJECT)
        {
            gkScene *sc = gkEngine::getSingleton().getActiveScene();

            if (L.isString(2))
                socket.setValue(sc->getObject(L.getValueString(2)));
            // TODO object
        }
        else if (socket.getType() != gkLogicSocket::ST_STRING &&
                 socket.getType() != gkLogicSocket::ST_VARIABLE)
        {
            if (L.isNumber(2))
                socket.setValue((gkScalar)L.getValueNumber(2));
            else if (L.isString(2))
                socket.setValue(L.getValueString(2));
            else if(L.isBoolean(2))
                socket.setValue(L.getValueBool(2));

        }
        else
        {
            gkVariable var("", false);
            if (L.isNumber(2))
                var.setValue(L.getValueNumber(2));
            else if (L.isString(2))
                var.setValue(L.getValueString(2));
            else if(L.isBoolean(2))
                var.setValue(L.getValueBool(2));
            socket.setValue(var.getValueString());
        }
    }
    return 0;
}

static int luSocket_link(luObject &L)
{
    luSocket *sock = L.getValueClassT<luSocket>(1);
    if (sock)
    {
        luClass *cls = L.getValueClass(2);
        if (cls && cls->isTypeOf(cls->getType(), &luSocket::Type))
        {
            luSocket *osock = (luSocket *)cls;
            sock->socket().link(&osock->socket());
        }
    }
    return 0;
}

luMethodDef luSocket::Methods[] =
{
    {"setValue",    luSocket_setValue,  LU_PARAM, "."},
    {"link",        luSocket_link,      LU_PARAM, ".."},
    {0, 0, 0}
};
luTypeDef luSocket::Type =  { "Socket", 0,  Methods};




// ----------------------------------------------------------------------------
//
// Node Base class
//
// ----------------------------------------------------------------------------
class luNode : public luClass
{
protected:
    gkLogicNode *m_node;

public:

    static luMethodDef Methods[];
    static luTypeDef Type;

public:

    luNode(gkLogicNode *lnode) : m_node(lnode) {}

    gkLogicNode &node(void) {GK_ASSERT(m_node); return *m_node;}

    template<typename T> T &getRef(void)
    {GK_ASSERT(m_node); return *static_cast<T *>(m_node);}


    virtual luTypeDef *getType(void)
    {
        return &Type;
    }

};

static int luNode_getInput(luObject &L)
{
    luNode *ob = L.getValueClassT<luNode>(1);
    if (ob)
    {
        gkLogicSocket *sock = ob->node().getInputSocket(L.getValueInt(2));
        if (!sock)
            return L.pushError("Socket index out of range!");

        new (&luSocket::Type, L) luSocket(sock);
        return 1;
    }
    return 0;
}

static int luNode_getOutput(luObject &L)
{
    luNode *ob = L.getValueClassT<luNode>(1);
    if (ob)
    {
        gkLogicSocket *sock = ob->node().getOutputSocket(L.getValueInt(2));
        if (!sock)
            return L.pushError("Socket index out of range!");

        new (&luSocket::Type, L) luSocket(sock);
        return 1;
    }
    return 0;
}

luMethodDef luNode::Methods[] =
{
    {"getInputSocket",      luNode_getInput,  LU_PARAM, ".i"},
    {"getOutputSocket",     luNode_getOutput, LU_PARAM, ".i"},
    {0, 0, 0}
};

luTypeDef luNode::Type =  { "Node", 0,  Methods};



// ----------------------------------------------------------------------------
//
// Animation Node
//
// ----------------------------------------------------------------------------
class luAnimationNode : public luNode
{
public:
    static luMethodDef Methods[];
    static luTypeDef Type;;

    luAnimationNode(gkLogicNode *lnode) : luNode(lnode) {}


    luTypeDef *getType(void)
    {
        return &Type;
    }
};

LU_GetNamdSocketFunc(luAnimationNode, getAnimName,  gkAnimationNode);
LU_GetNamdSocketFunc(luAnimationNode, getBlend,     gkAnimationNode);
LU_GetNamdSocketFunc(luAnimationNode, getTarget,    gkAnimationNode);

luMethodDef luAnimationNode::Methods[] =
{
    {"getAnimName",     luAnimationNode_getAnimName,    LU_NOPARAM, "."},
    {"getBlend",        luAnimationNode_getBlend,       LU_NOPARAM, "."},
    {"getTarget",       luAnimationNode_getTarget,      LU_NOPARAM, "."},
    {0, 0, 0}
};
luTypeDef luAnimationNode::Type =  { "AnimNode", &luNode::Type,  Methods};


// ----------------------------------------------------------------------------
//
// Button Node
//
// ----------------------------------------------------------------------------
class luButtonNode : public luNode
{
public:
    static luMethodDef Methods[];
    static luTypeDef Type;;

    luButtonNode(gkLogicNode *lnode) : luNode(lnode) {}


    luTypeDef *getType(void)
    {
        return &Type;
    }
};

LU_GetNamdSocketFunc(luButtonNode, getUpdate,       gkButtonNode);
LU_GetNamdSocketFunc(luButtonNode, getIsDown,       gkButtonNode);
LU_GetNamdSocketFunc(luButtonNode, getNotIsDown,    gkButtonNode);
LU_GetNamdSocketFunc(luButtonNode, getPress,        gkButtonNode);
LU_GetNamdSocketFunc(luButtonNode, getRelease,      gkButtonNode);

luMethodDef luButtonNode::Methods[] =
{
    {"getUpdate",       luButtonNode_getUpdate,     LU_NOPARAM, "."},
    {"getIsDown",       luButtonNode_getIsDown,     LU_NOPARAM, "."},
    {"getNotIsDown",    luButtonNode_getNotIsDown,  LU_NOPARAM, "."},
    {"getOnPress",      luButtonNode_getPress,      LU_NOPARAM, "."},
    {"getOnRelease",    luButtonNode_getRelease,    LU_NOPARAM, "."},
    {0, 0, 0}
};

luTypeDef luButtonNode::Type =  { "ButtonNode", &luNode::Type,  Methods};


// ----------------------------------------------------------------------------
//
// Button Key Node
//
// ----------------------------------------------------------------------------
class luKeyNode : public luButtonNode
{
public:
    static luMethodDef Methods[];
    static luTypeDef Type;;

    luKeyNode(gkLogicNode *lnode) : luButtonNode(lnode) {}


    luTypeDef *getType(void)
    {
        return &Type;
    }
};

static int luKeyNode_setKey(luObject &L)
{
    luNode *ob = L.getValueClassT<luNode>(1);
    if (ob)
    {
        gkKeyNode *nd = static_cast<gkKeyNode *>(&ob->node());
        int val = L.getValueInt(2);
        if (val > KC_NONE && val < KC_MAX)
            nd->setKey((gkScanCode)val);
    }
    return 0;
}

luMethodDef luKeyNode::Methods[] =
{
    {"setKey",      luKeyNode_setKey,    LU_PARAM, ".e"},
    {0, 0, 0}
};

luTypeDef luKeyNode::Type =  { "KeyNode", &luButtonNode::Type,  Methods};


// ----------------------------------------------------------------------------
//
// Button Mouse Node
//
// ----------------------------------------------------------------------------
class luMouseButtonNode : public luButtonNode
{
public:
    static luMethodDef Methods[];
    static luTypeDef Type;;

    luMouseButtonNode(gkLogicNode *lnode) : luButtonNode(lnode) {}


    luTypeDef *getType(void)
    {
        return &Type;
    }
};

static int luMouseButtonNode_setButton(luObject &L)
{
    luNode *ob = L.getValueClassT<luNode>(1);
    if (ob)
    {
        gkMouseButtonNode *nd = static_cast<gkMouseButtonNode *>(&ob->node());
        int val = L.getValueInt(2);
        if (val >= gkMouse::Left && val <= gkMouse::Right)
            nd->setButton((gkMouse::Buttons)val);
    }
    return 0;
}

luMethodDef luMouseButtonNode::Methods[] =
{
    {"setButton", luMouseButtonNode_setButton,    LU_PARAM, ".e"},
    {0, 0, 0}
};

luTypeDef luMouseButtonNode::Type =  { "MouseButtonNode", &luButtonNode::Type,  Methods};


// ----------------------------------------------------------------------------
//
// Exit Node
//
// ----------------------------------------------------------------------------
class luExitNode : public luNode
{
public:
    static luMethodDef Methods[];
    static luTypeDef Type;;

    luExitNode(gkLogicNode *lnode) : luNode(lnode) {}


    luTypeDef *getType(void)
    {
        return &Type;
    }
};

LU_GetNamdSocketFunc(luExitNode, getExit,  gkExitNode);

luMethodDef luExitNode::Methods[] =
{
    {"getExit", luExitNode_getExit, LU_NOPARAM, "."},
    {0, 0, 0}
};
luTypeDef luExitNode::Type =  { "ExitNode", &luNode::Type,  Methods};



// ----------------------------------------------------------------------------
//
// If Node
//
// ----------------------------------------------------------------------------
class luIfNode : public luNode
{
public:
    static luMethodDef Methods[];
    static luTypeDef Type;;

    luIfNode(gkLogicNode *lnode) : luNode(lnode) {}


    luTypeDef *getType(void)
    {
        return &Type;
    }
};

static int luIfNode_setStatement(luObject &L)
{
    luNode *ob = L.getValueClassT<luNode>(1);
    if (ob)
    {
        gkIfNode *nd = static_cast<gkIfNode *>(&ob->node());
        int val = L.getValueInt(2);
        if (val >= CMP_TRUE && val <= CMP_LTHAN)
            nd->setStatement((gkBoolStatement)val);
    }
    return 0;
}


LU_GetNamdSocketFunc(luIfNode, getA,        gkIfNode);
LU_GetNamdSocketFunc(luIfNode, getB,        gkIfNode);
LU_GetNamdSocketFunc(luIfNode, getTrue,     gkIfNode);
LU_GetNamdSocketFunc(luIfNode, getFalse,    gkIfNode);

luMethodDef luIfNode::Methods[] =
{
    {"getA",            luIfNode_getA,          LU_NOPARAM, "."},
    {"getB",            luIfNode_getB,          LU_NOPARAM, "."},
    {"getTrue",         luIfNode_getTrue,       LU_NOPARAM, "."},
    {"getFalse",        luIfNode_getFalse,      LU_NOPARAM, "."},
    {"setStatement",    luIfNode_setStatement,  LU_PARAM,   ".e"},
    {0, 0, 0}
};
luTypeDef luIfNode::Type =  { "IfNode", &luNode::Type,  Methods};




// ----------------------------------------------------------------------------
//
// Print Node
//
// ----------------------------------------------------------------------------
class luPrintNode : public luNode
{
public:
    static luMethodDef Methods[];
    static luTypeDef Type;;

    luPrintNode(gkLogicNode *lnode) : luNode(lnode) {}


    luTypeDef *getType(void)
    {
        return &Type;
    }
};

LU_GetNamdSocketFunc(luPrintNode, getPrint, gkPrintNode);
LU_GetNamdSocketFunc(luPrintNode, getValue, gkPrintNode);

luMethodDef luPrintNode::Methods[] =
{
    {"getPrint",    luPrintNode_getPrint,       LU_NOPARAM, "."},
    {"getValue",    luPrintNode_getValue,       LU_NOPARAM, "."},
    {0, 0, 0}
};
luTypeDef luPrintNode::Type =  { "PrintNode", &luNode::Type,  Methods};



// ----------------------------------------------------------------------------
//
// Mouse Motion Node
//
// ----------------------------------------------------------------------------
class luMouseMotionNode : public luNode
{
public:
    static luMethodDef Methods[];
    static luTypeDef Type;;

    luMouseMotionNode(gkLogicNode *lnode) : luNode(lnode) {}


    luTypeDef *getType(void)
    {
        return &Type;
    }
};

LU_GetNamdSocketFunc(luMouseMotionNode, getScaleX,   gkMouseNode);
LU_GetNamdSocketFunc(luMouseMotionNode, getScaleY,   gkMouseNode);
LU_GetNamdSocketFunc(luMouseMotionNode, getMotion,   gkMouseNode);
LU_GetNamdSocketFunc(luMouseMotionNode, getRelX,     gkMouseNode);
LU_GetNamdSocketFunc(luMouseMotionNode, getRelY,     gkMouseNode);
LU_GetNamdSocketFunc(luMouseMotionNode, getAbsX,     gkMouseNode);
LU_GetNamdSocketFunc(luMouseMotionNode, getAbsY,     gkMouseNode);
LU_GetNamdSocketFunc(luMouseMotionNode, getWheel,    gkMouseNode);


luMethodDef luMouseMotionNode::Methods[] =
{
    {"getScaleX",   luMouseMotionNode_getScaleX,        LU_NOPARAM, "."},
    {"getScaleY",   luMouseMotionNode_getScaleY,        LU_NOPARAM, "."},
    {"getMotion",   luMouseMotionNode_getMotion,        LU_NOPARAM, "."},
    {"getRelX",     luMouseMotionNode_getRelX,          LU_NOPARAM, "."},
    {"getRelY",     luMouseMotionNode_getRelY,          LU_NOPARAM, "."},
    {"getAbsX",     luMouseMotionNode_getAbsX,          LU_NOPARAM, "."},
    {"getAbsY",     luMouseMotionNode_getAbsY,          LU_NOPARAM, "."},
    {"getWheel",    luMouseMotionNode_getWheel,         LU_NOPARAM, "."},
    {0, 0, 0}

};
luTypeDef luMouseMotionNode::Type =  { "MouseMotion", &luNode::Type,  Methods};




// ----------------------------------------------------------------------------
//
// Object Motion Node
//
// ----------------------------------------------------------------------------
class luMotionNode : public luNode
{
public:
    static luMethodDef Methods[];
    static luTypeDef Type;;

    luMotionNode(gkLogicNode *lnode) : luNode(lnode) {}
    luTypeDef *getType(void)
    {
        return &Type;
    }
};

static int luMotionNode_setMotionType(luObject &L)
{
    luNode *ob = L.getValueClassT<luNode>(1);
    if (ob)
    {
        gkMotionNode *mn = static_cast<gkMotionNode *>(&ob->node());

        int val = L.getValueInt(2);
        if (val > MT_NONE && val <=MT_ANGV)
            mn->setMotionType((gkMotionTypes)val);
        else
            return L.pushError("setMotionType, invalid enumeration");
    }
    return 0;
}


static int luMotionNode_setKeepVelocity(luObject &L)
{
    luNode *ob = L.getValueClassT<luNode>(1);
    if (ob)
    {
        gkMotionNode *mn = static_cast<gkMotionNode *>(&ob->node());
        if (L.getValueBool(2))
            mn->keepVelocity();
    }
    return 0;
}

static int luMotionNode_setClampX(luObject &L)
{
    luNode *ob = L.getValueClassT<luNode>(1);
    if (ob)
    {
        gkMotionNode *mn = static_cast<gkMotionNode *>(&ob->node());
        mn->setMinX(L.getValueFloat(2));
        mn->setMaxX(L.getValueFloat(3));
    }
    return 0;
}
static int luMotionNode_setClampY(luObject &L)
{
    luNode *ob = L.getValueClassT<luNode>(1);
    if (ob)
    {
        gkMotionNode *mn = static_cast<gkMotionNode *>(&ob->node());
        mn->setMinY(L.getValueFloat(2));
        mn->setMaxY(L.getValueFloat(3));
    }
    return 0;
}

static int luMotionNode_setClampZ(luObject &L)
{
    luNode *ob = L.getValueClassT<luNode>(1);
    if (ob)
    {
        gkMotionNode *mn = static_cast<gkMotionNode *>(&ob->node());
        mn->setMinZ(L.getValueFloat(2));
        mn->setMaxZ(L.getValueFloat(3));
    }
    return 0;
}


LU_GetNamdSocketFunc(luMotionNode, getUpdate,   gkMotionNode);
LU_GetNamdSocketFunc(luMotionNode, getX,        gkMotionNode);
LU_GetNamdSocketFunc(luMotionNode, getY,        gkMotionNode);
LU_GetNamdSocketFunc(luMotionNode, getZ,        gkMotionNode);
LU_GetNamdSocketFunc(luMotionNode, getDamping,  gkMotionNode);

luMethodDef luMotionNode::Methods[] =
{
    {"getUpdate",           luMotionNode_getUpdate,         LU_NOPARAM, "."},
    {"getX",                luMotionNode_getX,              LU_NOPARAM, "."},
    {"getY",                luMotionNode_getY,              LU_NOPARAM, "."},
    {"getZ",                luMotionNode_getZ,              LU_NOPARAM, "."},
    {"getDamping",          luMotionNode_getDamping,        LU_NOPARAM, "."},
    {"setMotionType",       luMotionNode_setMotionType,     LU_PARAM,   ".e"},
    {"setKeepVelocity",     luMotionNode_setKeepVelocity,   LU_PARAM,   ".b"},
    {"setClampX",           luMotionNode_setClampX,         LU_PARAM,   ".ff"},
    {"setClampY",           luMotionNode_setClampY,         LU_PARAM,   ".ff"},
    {"setClampZ",           luMotionNode_setClampZ,         LU_PARAM,   ".ff"},
    {0, 0, 0}

};

luTypeDef luMotionNode::Type =  { "Motion", &luNode::Type,  Methods};

// ----------------------------------------------------------------------------
//
// Node Tree Wrapper
//
// ----------------------------------------------------------------------------


class luTree : public luClass
{
private:
    gkLogicTree *m_tree;

public:
    static luMethodDef Methods[];
    static luTypeDef Type;

    luTree(gkLogicTree *tree) : m_tree(tree) {}
    gkLogicTree &tree(void) {GK_ASSERT(m_tree); return *m_tree;}

    luTypeDef *getType(void)
    {
        return &Type;
    }
};


// new tree
static int luTree_constructor(luObject &L)
{
    gkLogicTree *ltree = 0;
    if (L.isString(1))
        ltree = gkNodeManager::getSingleton().create(L.getValueString(1));
    else
        ltree = gkNodeManager::getSingleton().create();

    luTree *tree = new (L) luTree(ltree);
    return 1;
}

static int luTree_isGroup(luObject &L)
{
    luTree *tree = L.getValueClassT<luTree>(1);
    if (tree)
        return L.push(tree->tree().isGroup());
    return 0;
}

static int luTree_solve(luObject &L)
{
    // solve tree execution order
    luTree *tree = L.getValueClassT<luTree>(1);
    if (tree)
        tree->tree().solveOrder(true);
    return 0;
}

static int luTree_attachObject(luObject &L)
{
    luTree *tree = L.getValueClassT<luTree>(1);
    if (tree)
    {
        gkScene *sc = gkEngine::getSingleton().getActiveScene();
        gkGameObject *ob = sc->getObject(L.getValueString(2));
        if (ob)
            tree->tree().attachObject(ob);
    }
    return 0;
}



static int luTree_create(luObject &L)
{
#define luNodeCase(type, cse, nt, gkt)\
    if (type == cse) {new(&nt::Type, L) nt(tree->tree().createNode<gkt>()); return 1; }


    // solve tree execution order
    luTree *tree = L.getValueClassT<luTree>(1);
    if (tree)
    {
        int type = L.getValueInt(2);

        luNodeCase(type, NT_ANIMATION,      luAnimationNode,    gkAnimationNode);
        luNodeCase(type, NT_EXIT,           luExitNode,         gkExitNode);
        luNodeCase(type, NT_KEY,            luKeyNode,          gkKeyNode);
        luNodeCase(type, NT_IF,             luIfNode,           gkIfNode);
        luNodeCase(type, NT_PRINT,          luPrintNode,        gkPrintNode);
        luNodeCase(type, NT_MOUSE_MOTION,   luMouseMotionNode,  gkMouseNode);
        luNodeCase(type, NT_MOUSE_BUTTON,   luMouseButtonNode,  gkMouseButtonNode);
        luNodeCase(type, NT_MOTION,         luMotionNode,       gkMotionNode);
    }

#undef luNodeCase
    return 0;
}

// global methods
luMethodDef luTree::Methods[] =
{
    {"constructor", luTree_constructor, LU_PARAM,   "|s"},
    {"isGroup",     luTree_isGroup,     LU_NOPARAM, "."},
    {"solve",       luTree_solve,       LU_NOPARAM, "."},
    {"createNode",  luTree_create,      LU_PARAM,   ".e"},
    {"attachObject",luTree_attachObject,LU_PARAM,   ".s"},
    {0, 0, 0}
};

// node tree definition
luTypeDef luTree::Type =  { "Tree", 0, luTree::Methods };


// Lua NodeTree module
void luNodeTree_Open(ltState *L)
{
    luBinder lua(L);
    lua.beginNamespace("NodeTree");

    // node types
    lua.addConstant("NT_ANIMATION",     NT_ANIMATION);
    lua.addConstant("NT_ARCBALL",       NT_ARCBALL);
    lua.addConstant("NT_CURSOR",        NT_CURSOR);
    lua.addConstant("NT_EXIT",          NT_EXIT);
    lua.addConstant("NT_GROUP",         NT_GROUP);
    lua.addConstant("NT_IF",            NT_IF);
    lua.addConstant("NT_KEY",           NT_KEY);
    lua.addConstant("NT_MATH",          NT_MATH);
    lua.addConstant("NT_MOTION",        NT_MOTION);
    lua.addConstant("NT_MOUSE_BUTTON",  NT_MOUSE_BUTTON);
    lua.addConstant("NT_MOUSE_MOTION",  NT_MOUSE_MOTION);
    lua.addConstant("NT_OBJECT",        NT_OBJECT);
    lua.addConstant("NT_PICK",          NT_PICK);
    lua.addConstant("NT_PRINT",         NT_PRINT);
    lua.addConstant("NT_RANDOM",        NT_RANDOM);
    lua.addConstant("NT_SWITCH",        NT_SWITCH);
    lua.addConstant("NT_TIMER",         NT_TIMER);
    lua.addConstant("NT_VALUE",         NT_VALUE);
    lua.addConstant("NT_VARIABLE",      NT_VARIABLE);

    // motion types
    lua.addConstant("MT_ROTATION",      MT_ROTATION);
    lua.addConstant("MT_TRANSLATION",   MT_TRANSLATION);
    // needs fixed, lua.addConstant("MT_SCALE", MT_SCALE);
    lua.addConstant("MT_FORCE",         MT_FORCE);
    lua.addConstant("MT_TORQUE",        MT_TORQUE);
    lua.addConstant("MT_LINV",          MT_LINV);
    lua.addConstant("MT_ANGV",          MT_ANGV);

    // statement
    lua.addConstant("CMP_TRUE",         CMP_TRUE);
    lua.addConstant("CMP_FALSE",        CMP_FALSE);
    lua.addConstant("CMP_AND",          CMP_AND);
    lua.addConstant("CMP_OR",           CMP_OR);
    lua.addConstant("CMP_NOT",          CMP_NOT);
    lua.addConstant("CMP_EQUALS",       CMP_EQUALS);
    lua.addConstant("CMP_NOT_EQUAL",    CMP_NOT_EQUAL);
    lua.addConstant("CMP_GREATER",      CMP_GREATER);
    lua.addConstant("CMP_LESS",         CMP_LESS);
    lua.addConstant("CMP_GREATER_EQ",   CMP_GTHAN); // >= ... Rename
    lua.addConstant("CMP_LESS_EQ",      CMP_LTHAN); // <=


    // types
    lua.addType(&luTree::Type);
    lua.addType(&luSocket::Type);
    lua.addType(&luNode::Type);
    lua.addType(&luPrintNode::Type);
    lua.addType(&luMouseMotionNode::Type);
    lua.addType(&luMotionNode::Type);
    lua.addType(&luAnimationNode::Type);
    lua.addType(&luExitNode::Type);
    lua.addType(&luIfNode::Type);
    lua.addType(&luButtonNode::Type);
    lua.addType(&luMouseButtonNode::Type);
    lua.addType(&luKeyNode::Type);

    lua.endNamespace();
}
