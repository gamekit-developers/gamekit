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
#include "luMath.h"
#include "luScene.h"
#include "luGameObjects.h"
#include "OgreKit.h"


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
// NodeTree.Socket
class luSocket : public luClass
{
    luClassHeader;
protected:
    gkLogicSocket *m_socket;

public:

    luSocket(gkLogicSocket *sock) 
        :   m_socket(sock) 
    {
    }
    virtual ~luSocket() {}

    static int          create(luObject &L, gkLogicSocket *sock)
    {
        GK_ASSERT(sock);
        new (&Type, L) luSocket(sock);
        return 1;
    }

    static luSocket&    getArg(luObject &L, int v) 
    { 
        return L.toclassRefT<luSocket>(v); 
    }

    // nil Socket:setValue(Any)
    int setValue(luClass *self, luObject& L)
    {
        if (m_socket->getType() == gkLogicSocket::ST_GAME_OBJECT)
        {
            gkScene *sc = gkEngine::getSingleton().getActiveScene();
            if (L.isString(2))
                m_socket->setValue(sc->getObject(L.tostring(2)));
            if (luGameObject::isType(L, 2))
                m_socket->setValue(&luGameObject::getArg(L, 2).ref<gkGameObject>());
       }
        else if (m_socket->getType() != gkLogicSocket::ST_STRING &&
                 m_socket->getType() != gkLogicSocket::ST_VARIABLE)
        {
            if (L.isNumber(2))
                m_socket->setValue((gkScalar)L.tonumber(2));
            else if (L.isString(2))
                m_socket->setValue(L.tostring(2));
            else if(L.isBoolean(2))
                m_socket->setValue(L.toboolean(2));
            else if (luVector3::isType(L, 2))
                m_socket->setValue(luVector3::getArg(L, 2));
            else if (luQuat::isType(L, 2))
                m_socket->setValue(luQuat::getArg(L, 2));
            else if (luGameObject::isType(L, 2))
                m_socket->setValue(&luGameObject::getArg(L, 2).ref<gkGameObject>());
            // TODO other math types
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
            else if (luVector3::isType(L, 2))
                var.setValue(luVector3::getArg(L, 2));
            else if (luQuat::isType(L, 2))
                var.setValue(luQuat::getArg(L, 2));
            else if (luGameObject::isType(L, 2))
                var.setValue(luGameObject::getArg(L, 2).ref<gkGameObject>().getName());
            m_socket->setValue(var.getValueString());
        }
        return 0;
    }
    // nil Socket:link(Socket)
    int link(luClass *self, luObject& L)
    {
        if (!isType(L, 2))
            return L.pushError("expected link(Socket)");
        m_socket->link(getArg(L, 2).m_socket);
        return 0;
    }
};


// Globals
luGlobalTableBegin(luSocket)
luGlobalTableEnd()

// Locals
luClassTableBegin(luSocket)
    luClassTable("setValue",    luSocket,setValue,  LU_NOPARAM, ".")
    luClassTable("link",        luSocket,link,      LU_PARAM,   ".")
luClassTableEnd()

luClassImpl("Socket", luSocket, 0);


// ----------------------------------------------------------------------------
// NodeTree.Node
class luNode : public luClass
{
    luClassHeader;
protected:
    gkLogicNode *m_node;

public:

    luNode(gkLogicNode *lnode) 
        : m_node(lnode) 
    {
        GK_ASSERT(m_node);
    }
    virtual ~luNode(){}

    template<typename T> T &getRef(void)
    {GK_ASSERT(m_node); return *static_cast<T *>(m_node);}


    // Socket Node:getInput(Number)
    int getInput(luClass *self, luObject &L);

    // Socket Node:getOutput(Number)
    int getOutput(luClass *self, luObject &L);
};


// Socket Node:getInput(Number)
int luNode::getInput(luClass *self, luObject &L)
{
    gkLogicSocket *sock = m_node->getInputSocket(L.toint(2));
    if (!sock)
        return L.pushError("Socket index out of range!");

    return luSocket::create(L, sock);
}

// Socket Node:getOutput(Number)
int luNode::getOutput(luClass *self, luObject &L)
{
    gkLogicSocket *sock = m_node->getOutputSocket(L.toint(2));
    if (!sock)
        return L.pushError("Socket index out of range!");

    return luSocket::create(L, sock);
}


// Globals
luGlobalTableBegin(luNode)
luGlobalTableEnd()

// Locals
luClassTableBegin(luNode)
    luClassTable("getInput",    luNode,getInput,    LU_PARAM,   ".i")
    luClassTable("getOutput",   luNode,getOutput,   LU_PARAM,   ".i")
luClassTableEnd()

luClassImpl("Node", luNode, 0);



// ----------------------------------------------------------------------------
// NodeTree.AnimationNode
class luAnimationNode : public luNode
{
    luClassHeader;
public:
    luAnimationNode(gkLogicNode *lnode) : luNode(lnode) {}
    virtual ~luAnimationNode() {}


    // Socket AnimationNode:getAnimName()
    int getAnimName(luClass *self, luObject &L)
    {
        return luSocket::create(L, getRef<gkAnimationNode>().getAnimName());
    }

    // Socket AnimationNode:getBlend()
    int getBlend(luClass *self, luObject &L)
    {
        return luSocket::create(L, getRef<gkAnimationNode>().getBlend());
    }

    // Socket AnimationNode:getBlend()
    int getTarget(luClass *self, luObject &L)
    {
        return luSocket::create(L, getRef<gkAnimationNode>().getTarget());
    }

};

// Globals
luGlobalTableBegin(luAnimationNode)
luGlobalTableEnd()

// Locals
luClassTableBegin(luAnimationNode)
    luClassTable("getAnimName",     luAnimationNode,getAnimName,    LU_PARAM,   ".")
    luClassTable("getBlend",        luAnimationNode,getBlend,       LU_PARAM,   ".")
    luClassTable("getTarget",       luAnimationNode,getTarget,      LU_PARAM,   ".")
luClassTableEnd()

luClassImpl("AnimationNode", luAnimationNode, &luNode::Type);



// ----------------------------------------------------------------------------
// NodeTree.ButtonNode
class luButtonNode : public luNode
{
    luClassHeader;
public:
    luButtonNode(gkLogicNode *lnode) : luNode(lnode) {}
    virtual ~luButtonNode() {}

    // Socket ButtonNode:getUpdate()
    int getUpdate(luClass *self, luObject &L)
    {
        return luSocket::create(L, getRef<gkButtonNode>().getUpdate());
    }

    // Socket ButtonNode:getIsDown()
    int getIsDown(luClass *self, luObject &L)
    {
        return luSocket::create(L, getRef<gkButtonNode>().getIsDown());
    }

    // Socket ButtonNode:getNotIsDown()
    int getNotIsDown(luClass *self, luObject &L)
    {
        return luSocket::create(L, getRef<gkButtonNode>().getNotIsDown());
    }

    // Socket ButtonNode:getOnPress()
    int getOnPress(luClass *self, luObject &L)
    {
        return luSocket::create(L, getRef<gkButtonNode>().getPress());
    }

    // Socket ButtonNode:getOnRelease()
    int getOnRelease(luClass *self, luObject &L)
    {
        return luSocket::create(L, getRef<gkButtonNode>().getRelease());
    }
};


// Globals
luGlobalTableBegin(luButtonNode)
luGlobalTableEnd()

// Locals
luClassTableBegin(luButtonNode)
    luClassTable("getUpdate",       luButtonNode,getUpdate,     LU_PARAM,   ".")
    luClassTable("getIsDown",       luButtonNode,getIsDown,     LU_PARAM,   ".")
    luClassTable("getNotIsDown",    luButtonNode,getNotIsDown,  LU_PARAM,   ".")
    luClassTable("getOnPress",      luButtonNode,getOnPress,    LU_PARAM,   ".")
    luClassTable("getOnRelease",    luButtonNode,getOnRelease,  LU_PARAM,   ".")
luClassTableEnd()


luClassImpl("ButtonNode", luButtonNode, &luNode::Type);


// ----------------------------------------------------------------------------
// NodeTree.KeyNode
class luKeyNode : public luButtonNode
{
    luClassHeader;
public:
    luKeyNode(gkLogicNode *lnode) : luButtonNode(lnode) {}
    virtual ~luKeyNode() {}

    // nil KeyNode:setKey(Enum)
    int setKey(luClass *self, luObject &L)
    {
        gkKeyNode& node = getRef<gkKeyNode>();

        int val = L.toint(2);
        if (val > KC_NONE && val < KC_MAX)
            node.setKey((gkScanCode)val);
        return 0;
    }
};

// Globals
luGlobalTableBegin(luKeyNode)
luGlobalTableEnd()

// Locals
luClassTableBegin(luKeyNode)
    luClassTable("setKey", luKeyNode,setKey, LU_PARAM,   ".e")
luClassTableEnd()

luClassImpl("KeyNode", luKeyNode, &luButtonNode::Type);

// ----------------------------------------------------------------------------
// NodeTree.MouseButtonNode
class luMouseButtonNode : public luButtonNode
{
    luClassHeader;
public:
    luMouseButtonNode(gkLogicNode *lnode) : luButtonNode(lnode) {}
    virtual ~luMouseButtonNode() {}

    // nil MouseButtonNode:setButton(Enum)
    int setButton(luClass *self, luObject &L)
    {
        int val = L.toint(2);
        if (val >= gkMouse::Left && val <= gkMouse::Right)
            getRef<gkMouseButtonNode>().setButton((gkMouse::Buttons)val);
        return 0;
    }

};
// Globals
luGlobalTableBegin(luMouseButtonNode)
luGlobalTableEnd()

// Locals
luClassTableBegin(luMouseButtonNode)
    luClassTable("setButton", luMouseButtonNode,setButton, LU_PARAM,   ".e")
luClassTableEnd()

luClassImpl("MouseButtonNode", luMouseButtonNode, &luButtonNode::Type);


// ----------------------------------------------------------------------------
// NodeTree.ExitNode
class luExitNode : public luNode
{
    luClassHeader;
public:
    luExitNode(gkLogicNode *lnode) : luNode(lnode) {}
    virtual ~luExitNode() {}

    // Socket ExitNode:getExit()
    int getExit(luClass *self, luObject &L)
    {
        return luSocket::create(L, getRef<gkExitNode>().getExit());
    }

};
// Globals
luGlobalTableBegin(luExitNode)
luGlobalTableEnd()

// Locals
luClassTableBegin(luExitNode)
    luClassTable("getExit",     luExitNode,getExit,    LU_PARAM,   ".")
luClassTableEnd()

luClassImpl("ExitNode", luExitNode, &luNode::Type);


// ----------------------------------------------------------------------------
// NodeTree.IfNode
class luIfNode : public luNode
{
    luClassHeader;
public:
    luIfNode(gkLogicNode *lnode) : luNode(lnode) {}
    virtual ~luIfNode() {}

    // Socket IfNode:getA()
    int getA(luClass *self, luObject &L)
    {
        return luSocket::create(L, getRef<gkIfNode>().getA());
    }
    // Socket IfNode:getB()
    int getB(luClass *self, luObject &L)
    {
        return luSocket::create(L, getRef<gkIfNode>().getB());
    }
    // Socket IfNode:getTrue()
    int getTrue(luClass *self, luObject &L)
    {
        return luSocket::create(L, getRef<gkIfNode>().getTrue());
    }
    // Socket IfNode:getFalse()
    int getFalse(luClass *self, luObject &L)
    {
        return luSocket::create(L, getRef<gkIfNode>().getFalse());
    }

    // nil IfNode:setStatement()
    int setStatement(luClass *self, luObject &L)
    {
        gkIfNode& node = getRef<gkIfNode>();
        int val = L.toint(2);
        if (val >= CMP_TRUE && val <= CMP_LTHAN)
            node.setStatement((gkBoolStatement)val);
        return 0;
    }
};

// Globals
luGlobalTableBegin(luIfNode)
luGlobalTableEnd()

// Locals
luClassTableBegin(luIfNode)
    luClassTable("getA",            luIfNode,getA,          LU_PARAM,   ".")
    luClassTable("getB",            luIfNode,getB,          LU_PARAM,   ".")
    luClassTable("getTrue",         luIfNode,getTrue,       LU_PARAM,   ".")
    luClassTable("getFalse",        luIfNode,getFalse,      LU_PARAM,   ".")
    luClassTable("setStatement",    luIfNode,setStatement,  LU_PARAM,   ".e")
luClassTableEnd()

luClassImpl("IfNode", luIfNode, &luNode::Type);

// ----------------------------------------------------------------------------
// NodeTree.PrintNode
class luPrintNode : public luNode
{
    luClassHeader;
public:
    luPrintNode(gkLogicNode *lnode) : luNode(lnode) {}
    virtual ~luPrintNode() {}

    // Socket PrintNode:getPrint()
    int getPrint(luClass *self, luObject &L)
    {
        return luSocket::create(L, getRef<gkPrintNode>().getPrint());
    }

    // Socket PrintNode:getValue()
    int getValue(luClass *self, luObject &L)
    {
        return luSocket::create(L, getRef<gkPrintNode>().getValue());
    }
};


// Globals
luGlobalTableBegin(luPrintNode)
luGlobalTableEnd()

// Locals
luClassTableBegin(luPrintNode)
    luClassTable("getPrint",    luPrintNode,getPrint,   LU_PARAM,   ".")
    luClassTable("getValue",    luPrintNode,getValue,   LU_PARAM,   ".")
luClassTableEnd()

luClassImpl("PrintNode", luPrintNode, &luNode::Type);



// ----------------------------------------------------------------------------
// NodeTree.MouseMotionNode
class luMouseMotionNode : public luNode
{
    luClassHeader;
public:
    luMouseMotionNode(gkLogicNode *lnode) : luNode(lnode) {}
    virtual ~luMouseMotionNode() {}

    // Socket MouseMotionNode:getScaleX()
    int getScaleX(luClass *self, luObject &L)
    {
        return luSocket::create(L, getRef<gkMouseNode>().getScaleX());
    }

    // Socket MouseMotionNode:getScaleY()
    int getScaleY(luClass *self, luObject &L)
    {
        return luSocket::create(L, getRef<gkMouseNode>().getScaleX());
    }
    // Socket MouseMotionNode:getMotion()
    int getMotion(luClass *self, luObject &L)
    {
        return luSocket::create(L, getRef<gkMouseNode>().getMotion());
    }
    // Socket MouseMotionNode:getRelX()
    int getRelX(luClass *self, luObject &L)
    {
        return luSocket::create(L, getRef<gkMouseNode>().getRelX());
    }
    // Socket MouseMotionNode:getRelY()
    int getRelY(luClass *self, luObject &L)
    {
        return luSocket::create(L, getRef<gkMouseNode>().getRelY());
    }
    // Socket MouseMotionNode:getAbsX()
    int getAbsX(luClass *self, luObject &L)
    {
        return luSocket::create(L, getRef<gkMouseNode>().getAbsX());
    }
    // Socket MouseMotionNode:getAbsY()
    int getAbsY(luClass *self, luObject &L)
    {
        return luSocket::create(L, getRef<gkMouseNode>().getAbsY());
    }
    // Socket MouseMotionNode:getWheel()
    int getWheel(luClass *self, luObject &L)
    {
        return luSocket::create(L, getRef<gkMouseNode>().getWheel());
    }
};


// Globals
luGlobalTableBegin(luMouseMotionNode)
luGlobalTableEnd()

// Locals
luClassTableBegin(luMouseMotionNode)
    luClassTable("getScaleX",   luMouseMotionNode,getScaleX,    LU_PARAM,   ".")
    luClassTable("getScaleY",   luMouseMotionNode,getScaleY,    LU_PARAM,   ".")
    luClassTable("getMotion",   luMouseMotionNode,getMotion,    LU_PARAM,   ".")
    luClassTable("getRelX",     luMouseMotionNode,getRelX,      LU_PARAM,   ".")
    luClassTable("getRelY",     luMouseMotionNode,getRelY,      LU_PARAM,   ".")
    luClassTable("getAbsX",     luMouseMotionNode,getAbsX,      LU_PARAM,   ".")
    luClassTable("getAbsY",     luMouseMotionNode,getAbsY,      LU_PARAM,   ".")
    luClassTable("getWheel",    luMouseMotionNode,getWheel,     LU_PARAM,   ".")
luClassTableEnd()

luClassImpl("MouseMotionNode", luMouseMotionNode, &luNode::Type);


// ----------------------------------------------------------------------------
// NodeTree.MotionNode
class luMotionNode : public luNode
{
    luClassHeader;
public:
    luMotionNode(gkLogicNode *lnode) : luNode(lnode) {}
    virtual ~luMotionNode() {}

    // Socket MotionNode:getUpdate()
    int getUpdate(luClass *self, luObject &L)
    {
        return luSocket::create(L, getRef<gkMotionNode>().getUpdate());
    }

    // Socket MotionNode:getX()
    int getX(luClass *self, luObject &L)
    {
        return luSocket::create(L, getRef<gkMotionNode>().getX());
    }
    // Socket MotionNode:getY()
    int getY(luClass *self, luObject &L)
    {
        return luSocket::create(L, getRef<gkMotionNode>().getY());
    }
    // Socket MotionNode:getZ()
    int getZ(luClass *self, luObject &L)
    {
        return luSocket::create(L, getRef<gkMotionNode>().getZ());
    }
    // Socket MotionNode:getDamping()
    int getDamping(luClass *self, luObject &L)
    {
        return luSocket::create(L, getRef<gkMotionNode>().getDamping());
    }

    // nil MotionNode:setMotionType(Enum)
    int setMotionType(luClass *self, luObject &L)
    {
        int val = L.toint(2);
        if (val > MT_NONE && val <=MT_ANGV)
            getRef<gkMotionNode>().setMotionType((gkMotionTypes)val);
        return L.pushError("setMotionType, invalid enumeration");
    }

    // nil MotionNode:setKeepVelocity(bool)
    int setKeepVelocity(luClass *self, luObject &L)
    {
        if (L.toboolean(2))
            getRef<gkMotionNode>().keepVelocity();
        return 0;
    }

    // nil MotionNode:setClampX(Number, Number)
    int setClampX(luClass *self, luObject &L)
    {
        getRef<gkMotionNode>().setMinX(L.tofloat(2));
        getRef<gkMotionNode>().setMaxX(L.tofloat(3));
        return 0;
    }
    // nil MotionNode:setClampY(Number, Number)
    int setClampY(luClass *self, luObject &L)
    {
        getRef<gkMotionNode>().setMinY(L.tofloat(2));
        getRef<gkMotionNode>().setMaxY(L.tofloat(3));
        return 0;
    }
    // nil MotionNode:setClampZ(Number, Number)
    int setClampZ(luClass *self, luObject &L)
    {
        getRef<gkMotionNode>().setMinZ(L.tofloat(2));
        getRef<gkMotionNode>().setMaxZ(L.tofloat(3));
        return 0;
    }
};



// Globals
luGlobalTableBegin(luMotionNode)
luGlobalTableEnd()

// Locals
luClassTableBegin(luMotionNode)
    luClassTable("getUpdate",           luMotionNode,getUpdate,         LU_PARAM,   ".")
    luClassTable("getX",                luMotionNode,getX,              LU_PARAM,   ".")
    luClassTable("getY",                luMotionNode,getY,              LU_PARAM,   ".")
    luClassTable("getZ",                luMotionNode,getZ,              LU_PARAM,   ".")
    luClassTable("getDamping",          luMotionNode,getDamping,        LU_PARAM,   ".")
    luClassTable("setMotionType",       luMotionNode,setMotionType,     LU_PARAM,   ".e")
    luClassTable("setKeepVelocity",     luMotionNode,setKeepVelocity,   LU_PARAM,   ".b")
    luClassTable("setClampX",           luMotionNode,setClampX,         LU_PARAM,   ".ff")
    luClassTable("setClampY",           luMotionNode,setClampY,         LU_PARAM,   ".ff")
    luClassTable("setClampZ",           luMotionNode,setClampZ,         LU_PARAM,   ".ff")
luClassTableEnd()

luClassImpl("MotionNode", luMotionNode, &luNode::Type);

// ----------------------------------------------------------------------------
// NodeTree.Tree 
class luTree : public luClass
{
    luClassHeader;
private:
    gkLogicTree *m_tree;


public:
    luTree(gkLogicTree *tree) 
        : m_tree(tree) 
    {
        GK_ASSERT(m_tree);
    }

    virtual ~luTree() {}

    // bool Tree:isGroup()
    int isGroup(luClass *self, luObject &L);

    // nil Tree:solve()
    int solve(luClass *self, luObject &L);

    // Node Tree:createNode(Enum)
    int createNode(luClass *self, luObject &L);

    // nil Tree:attachObject(String or GameKit.GameObject)
    int attachObject(luClass *self, luObject &L);
};


// bool Tree:isGroup()
int luTree::isGroup(luClass *self, luObject &L)
{
    return L.push(m_tree->isGroup());
}

// nil Tree:solve()
int luTree::solve(luClass *self, luObject &L)
{
    m_tree->solveOrder();
    return 0;
}

// Node Tree:createNode(Enum)
int luTree::createNode(luClass *self, luObject &L)
{
#define luNodeCase(type, cse, nt, gkt)\
    if (type == cse) {new(&nt::Type, L) nt(m_tree->createNode<gkt>()); return 1; }

    int type = L.toint(2);
    luNodeCase(type, NT_ANIMATION,      luAnimationNode,    gkAnimationNode);
    luNodeCase(type, NT_EXIT,           luExitNode,         gkExitNode);
    luNodeCase(type, NT_KEY,            luKeyNode,          gkKeyNode);
    luNodeCase(type, NT_IF,             luIfNode,           gkIfNode);
    luNodeCase(type, NT_PRINT,          luPrintNode,        gkPrintNode);
    luNodeCase(type, NT_MOUSE_MOTION,   luMouseMotionNode,  gkMouseNode);
    luNodeCase(type, NT_MOUSE_BUTTON,   luMouseButtonNode,  gkMouseButtonNode);
    luNodeCase(type, NT_MOTION,         luMotionNode,       gkMotionNode);

#undef luNodeCase
    return 0;
}


// nil Tree:attachObject(String or GameKit.GameObject)
int luTree::attachObject(luClass *self, luObject &L)
{
    if (!L.isString(2) && !luGameObject::isType(L, 2))
        return L.pushError("expected attachObject(String or GameObject)");

    if (L.isString(2))
    {
        gkScene *sc = gkEngine::getSingleton().getActiveScene();
        gkGameObject *ob = sc->getObject(L.getValueString(2));
        if (ob)
            m_tree->attachObject(ob);

        return 0;
    }

    m_tree->attachObject(&luGameObject::getArg(L, 2).ref<gkGameObject>());
    return 0;
}


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


// Globals
luGlobalTableBegin(luTree)
    luGlobalTable("constructor", luTree_constructor, LU_PARAM,   "|s")
luGlobalTableEnd()


// Locals
luClassTableBegin(luTree)
    luClassTable("isGroup",         luTree,isGroup,         LU_NOPARAM, ".")
    luClassTable("solve",           luTree,solve,           LU_NOPARAM, ".")
    luClassTable("createNode",      luTree,createNode,      LU_PARAM,   ".e")
    luClassTable("attachObject",    luTree,attachObject,    LU_PARAM,   ".|s.")
luClassTableEnd()

luClassImpl("Tree", luTree, 0);


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
    lua.addClassType(&luTree::Type);
    lua.addClassType(&luSocket::Type);
    lua.addClassType(&luNode::Type);
    lua.addClassType(&luPrintNode::Type);
    lua.addClassType(&luMouseMotionNode::Type);
    lua.addClassType(&luMotionNode::Type);
    lua.addClassType(&luAnimationNode::Type);
    lua.addClassType(&luExitNode::Type);
    lua.addClassType(&luIfNode::Type);
    lua.addClassType(&luButtonNode::Type);
    lua.addClassType(&luMouseButtonNode::Type);
    lua.addClassType(&luKeyNode::Type);

    lua.endNamespace();
}
