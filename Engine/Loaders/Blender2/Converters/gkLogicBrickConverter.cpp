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
#include "Blender.h"
#include "gkBlenderDefines.h"
#include "gkLogicBrickConverter.h"
#include "OgreKit.h"


gkLogicLoader::gkLogicLoader()
{
}

gkLogicLoader::~gkLogicLoader()
{
}

void gkLogicLoader::resolveLinks(void)
{
	utListIterator<CreatedList> oblist(m_createdLinks);
	while (oblist.hasMoreElements())
	{
		gkLogicLink* link = oblist.getNext();

		utListIterator<ResolveObjectList> missing(m_missing);
		while (missing.hasMoreElements())
		{
			ResolveObject& ob = missing.getNext();

			if (ob.cont && ob.lsens)
			{
				gkLogicController* lc = link->findController(ob.cont);
				if (lc)
					ob.lsens->link(lc);
			}

			if (ob.act && ob.lcont)
			{
				gkLogicActuator* la = link->findActuator(ob.act);
				if (la)
					ob.lcont->link(la);
			}

		}
	}
}


gkString gkLogicLoader_formatText(const gkString& in)
{
	// trim any whitespace
	gkString ret = in;
	utStringUtils::trim(ret);
	return ret;
}



void gkLogicLoader::convertObject(Blender::Object* bobj, gkGameObject* gobj)
{
	GK_ASSERT(gobj && bobj);

	gkScalar animFps = gkEngine::getSingleton().getUserDefs().animFps;
	gkString groupName = gobj->getGroupName();

	if (!bobj->sensors.first && !bobj->controllers.first && !bobj->actuators.first)
		return;

	gkLogicManager* rlm = gkLogicManager::getSingletonPtr();

	// new link buffer
	gkLogicLink* lnk = rlm->createLink();

	m_createdLinks.push_back(lnk);

	lnk->setState(bobj->init_state ? bobj->init_state : bobj->state);
	gobj->setState(lnk->getState());
	lnk->setObject(gobj);
	gobj->attachLogic(lnk);



	for (Blender::bActuator* bact = (Blender::bActuator*)bobj->actuators.first; bact; bact = bact->next)
	{
		gkLogicActuator* la = 0;
		switch (bact->type)
		{
		case ACT_EDIT_OBJECT:
			{
				gkEditObjectActuator* ea = new gkEditObjectActuator(gobj, lnk, bact->name);
				la = ea;

				Blender::bEditObjectActuator* bea = (Blender::bEditObjectActuator*)bact->data;

				ea->setLifeSpan(bea->time);
				ea->setAngV(gkVector3(bea->angVelocity[0], bea->angVelocity[1], bea->angVelocity[2]));
				ea->setAngVL((bea->localflag & ACT_EDOB_LOCAL_ANGV) != 0);
				ea->setLinV(gkVector3(bea->linVelocity[0], bea->linVelocity[1], bea->linVelocity[2]));
				ea->setLinVL((bea->localflag & ACT_EDOB_LOCAL_LINV) != 0);
				ea->setObject(GKB_IDNAME(bea->ob));

				int mode = -1;
				switch (bea->type)
				{
				case ACT_EDOB_ADD_OBJECT:   {mode = gkEditObjectActuator::EO_ADDOBJ;        break;}
				case ACT_EDOB_END_OBJECT:   {mode = gkEditObjectActuator::EO_ENDOBJ;        break;}
				case ACT_EDOB_REPLACE_MESH: {mode = gkEditObjectActuator::EO_REPLACEMESH;   break;}
				case ACT_EDOB_TRACK_TO:     {mode = gkEditObjectActuator::EO_TRACKTO;       break;}
				case ACT_EDOB_DYNAMICS:     {mode = gkEditObjectActuator::EO_DYNAMICS;      break;}
				}
				ea->setMode(mode);

				int dyn = -1;

				switch (bea->dyn_operation)
				{
				case 0: {dyn = gkEditObjectActuator::EOD_RESTORE;       break;}
				case 1: {dyn = gkEditObjectActuator::EOD_SUSPEND;       break;}
				case 2: {dyn = gkEditObjectActuator::EOD_ENABLE_BODY;   break;}
				case 3: {dyn = gkEditObjectActuator::EOD_DISABLE_BODY;  break;}
				case 4: {dyn = gkEditObjectActuator::EOD_SETMASS;       break;}
				}
				ea->setDynMode(dyn);


			} break;
		case ACT_STATE:
			{
				gkStateActuator* sa = new gkStateActuator(gobj, lnk, bact->name);
				la = sa;

				Blender::bStateActuator* bst = (Blender::bStateActuator*)bact->data;

				int op = gkStateActuator::OP_NILL;


				switch (bst->type)
				{
				case 0: {op = gkStateActuator::OP_CPY; break;}
				case 1: {op = gkStateActuator::OP_ADD; break;}
				case 2: {op = gkStateActuator::OP_SUB; break;}
				case 3: {op = gkStateActuator::OP_INV; break;}
				}

				sa->setOp(op);
				sa->setMask(bst->mask);

			} break;
		case ACT_PROPERTY:
			{
				gkPropertyActuator* pa = new gkPropertyActuator(gobj, lnk, bact->name);
				la = pa;

				Blender::bPropertyActuator* bpa = (Blender::bPropertyActuator*)bact->data;

				int op = -1;

				switch (bpa->type)
				{
				case ACT_PROP_ASSIGN:   {op = gkPropertyActuator::PA_ASSIGN; break;}
				case ACT_PROP_ADD:      {op = gkPropertyActuator::PA_ADD; break;}
				case ACT_PROP_COPY:     {op = gkPropertyActuator::PA_COPY; break;}
				case ACT_PROP_TOGGLE:   {op = gkPropertyActuator::PA_TOGGLE; break;}
				}

				pa->setType(op);

				pa->setValue(bpa->value);
				pa->setProperty(gkLogicLoader_formatText(bpa->name));
				if (bpa->ob)
					pa->setObject(GKB_IDNAME(bpa->ob));

			} break;
		case ACT_OBJECT:
			{
				gkMotionActuator* ma = new gkMotionActuator(gobj, lnk, bact->name);
				la = ma;

				Blender::bObjectActuator* objact = (Blender::bObjectActuator*)bact->data;
				ma->setRotation(gkVector3(objact->drot[0], objact->drot[1], objact->drot[2]), (objact->flag & ACT_DROT_LOCAL) != 0);
				ma->setTranslation(gkVector3(objact->dloc[0], objact->dloc[1], objact->dloc[2]), (objact->flag & ACT_DLOC_LOCAL) != 0);
				ma->setForce(gkVector3(objact->forceloc[0], objact->forceloc[1], objact->forceloc[2]), (objact->flag & ACT_FORCE_LOCAL) != 0);
				ma->setTorque(gkVector3(objact->forcerot[0], objact->forcerot[1], objact->forcerot[2]), (objact->flag & ACT_TORQUE_LOCAL) != 0);

				ma->setLinearVelocity(gkVector3(objact->linearvelocity[0], objact->linearvelocity[1], objact->linearvelocity[2]),
				                      (objact->flag & ACT_LIN_VEL_LOCAL) != 0);
				ma->setAngularVelocity(gkVector3(objact->angularvelocity[0], objact->angularvelocity[1], objact->angularvelocity[2]),
				                       (objact->flag & ACT_ANG_VEL_LOCAL) != 0);

				ma->setIncrementalVelocity((objact->flag & ACT_ADD_LIN_VEL) != 0);
				ma->setDamping(gkScalar(objact->damping));
			}
			break;
		case ACT_ACTION:
			{
				gkActionActuator* aa = new gkActionActuator(gobj, lnk, bact->name);
				la = aa;

				Blender::bActionActuator* baa = (Blender::bActionActuator*)bact->data;

				aa->setStart(baa->sta/animFps);
				aa->setEnd(baa->end/animFps);
				aa->setBlend(baa->blendin);

				int mode = 0;
				switch (baa->type)
				{
				case ACT_ACTION_PINGPONG:   {mode = gkActionActuator::AA_PONG;          break;}
				case ACT_ACTION_FLIPPER:    {mode = gkActionActuator::AA_FLIPPER;       break;}
				case ACT_ACTION_LOOP_STOP:  {mode = gkActionActuator::AA_LOOP_STOP;     break;}
				case ACT_ACTION_LOOP_END:   {mode = gkActionActuator::AA_LOOP_END;      break;}
				case ACT_ACTION_FROM_PROP:  {mode = gkActionActuator::AA_PROPERTY;      break;}
				case ACT_ACTION_PLAY:
				case ACT_ACTION_KEY2KEY:
				case ACT_ACTION_MOTION:
				default:
					{mode = gkActionActuator::AA_PLAY; break;}
				}
				aa->setMode(mode);

				aa->setPriority(baa->priority);
				aa->setReset(baa->end_reset != 0);

				aa->setAnimation(baa->act ? GKB_IDNAME(baa->act) : "");
				aa->setProperty(gkLogicLoader_formatText(baa->frameProp));

			} break;
		case ACT_IPO:
			{
				gkActionActuator* aa = new gkActionActuator(gobj, lnk, bact->name);
				la = aa;
				Blender::bIpoActuator* bia = (Blender::bIpoActuator*)bact->data;
				
				aa->setStart(bia->sta / animFps);
				aa->setEnd(bia->end / animFps);
				aa->setBlend(0);

				int mode = 0;
				switch (bia->type)
				{
				case ACT_ACTION_PINGPONG:   {mode = gkActionActuator::AA_PONG;          break;}
				case ACT_ACTION_FLIPPER:    {mode = gkActionActuator::AA_FLIPPER;       break;}
				case ACT_ACTION_LOOP_STOP:  {mode = gkActionActuator::AA_LOOP_STOP;     break;}
				case ACT_ACTION_LOOP_END:   {mode = gkActionActuator::AA_LOOP_END;      break;}
				case ACT_ACTION_FROM_PROP:  {mode = gkActionActuator::AA_PROPERTY;      break;}
				case ACT_ACTION_PLAY:
				case ACT_ACTION_KEY2KEY:
				case ACT_ACTION_MOTION:
				default:
					{mode = gkActionActuator::AA_PLAY; break;}
				}
				aa->setMode(mode);

				aa->setPriority(0);
				aa->setReset(false);

				
				if (bobj->adt)	// 2.5
					aa->setAnimation(bobj->adt->action ? GKB_IDNAME(bobj->adt->action) : "");
				else			// older files
					aa->setAnimation(bobj->ipo ? GKB_IDNAME(bobj->ipo) : "");
				
				aa->setProperty(gkLogicLoader_formatText(bia->frameProp));
			} break;
		case ACT_GAME:
			{
				gkGameActuator* ga = new gkGameActuator(gobj, lnk, bact->name);
				la = ga;
				Blender::bGameActuator* bga = (Blender::bGameActuator*)bact->data;

				int mode = -1;
				switch (bga->type)
				{
				case ACT_GAME_START:
				case ACT_GAME_LOAD:
					mode = gkGameActuator::GM_START_NEW;
					break;
				case ACT_GAME_RESTART:
					mode = gkGameActuator::GM_RESTART;
					break;
				case ACT_GAME_QUIT:
					mode = gkGameActuator::GM_QUIT;
					break;
				case ACT_GAME_SAVECFG:
					mode = gkGameActuator::GM_SAVE;
					break;
				case ACT_GAME_LOADCFG:
					mode = gkGameActuator::GM_LOAD;
					break;
				}

				ga->setMode(mode);
				if (bga->filename[0] != 0)
					ga->setGameFile(bga->filename);

			} break;

		case ACT_VISIBILITY:
			{
				gkVisibilityActuator* va = new gkVisibilityActuator(gobj, lnk, bact->name);
				la = va;

				Blender::bVisibilityActuator* bva = (Blender::bVisibilityActuator*)bact->data;


				int fl = 0;
				if (bva->flag & ACT_VISIBILITY_INVISIBLE)
					fl |= gkVisibilityActuator::VA_INVIS_FLAG;
				if (bva->flag & ACT_VISIBILITY_OCCLUSION)
					fl |= gkVisibilityActuator::VA_OCCLUDER;
				if (bva->flag & ACT_VISIBILITY_RECURSIVE)
					fl |= gkVisibilityActuator::VA_CHILDREN;

				va->setFlag(fl);

			} break;
		case ACT_MESSAGE:
			{
				gkMessageActuator* ma = new gkMessageActuator(gobj, lnk, bact->name);
				la = ma;
				Blender::bMessageActuator* bma = (Blender::bMessageActuator*)bact->data;

				ma->setTo(bma->toPropName ? bma->toPropName : "");
				ma->setSubject(bma->subject ? bma->subject : "");

				switch (bma->bodyType)
				{
				case ACT_MESG_MESG:
					ma->setBodyType(gkMessageActuator::BT_TEXT);
					ma->setBodyText(bma->body ? bma->body : "");
					break;
				case ACT_MESG_PROP:
					ma->setBodyType(gkMessageActuator::BT_TEXT);
					ma->setBodyProperty(bma->body ? bma->body : "");
					break;
				default:
					break;
				}

			} break;
		case ACT_RANDOM:
			{
				gkRandomActuator* ra = new gkRandomActuator(gobj, lnk, bact->name);
				la = ra;
				Blender::bRandomActuator* bra = (Blender::bRandomActuator*)bact->data;

				ra->setProperty(bra->propname ? bra->propname : "");
				ra->setSeed(bra->seed);

				switch (bra->distribution)
				{
				case ACT_RANDOM_BOOL_CONST:
					ra->setDistribution(gkRandomActuator::RA_BOOL_CONSTANT);
					ra->setConstant(bra->int_arg_1);
					break;
				case ACT_RANDOM_BOOL_UNIFORM:
					ra->setDistribution(gkRandomActuator::RA_BOOL_UNIFORM);
					break;
				case ACT_RANDOM_BOOL_BERNOUILLI:
					ra->setDistribution(gkRandomActuator::RA_BOOL_BERNOUILLI);
					ra->setMean(bra->float_arg_1);
					break;
				case ACT_RANDOM_INT_CONST:
					ra->setDistribution(gkRandomActuator::RA_INT_CONSTANT);
					ra->setConstant(bra->int_arg_1);
					break;
				case ACT_RANDOM_INT_UNIFORM:
					ra->setDistribution(gkRandomActuator::RA_INT_UNIFORM);
					ra->setMin(bra->int_arg_1);
					ra->setMax(bra->int_arg_2);
					break;
				case ACT_RANDOM_INT_POISSON:
					ra->setDistribution(gkRandomActuator::RA_INT_POISSON);
					ra->setMean(bra->float_arg_1);
					break;
				case ACT_RANDOM_FLOAT_CONST:
					ra->setDistribution(gkRandomActuator::RA_FLOAT_CONSTANT);
					ra->setConstant(bra->float_arg_1);
					break;
				case ACT_RANDOM_FLOAT_UNIFORM:
					ra->setDistribution(gkRandomActuator::RA_FLOAT_UNIFORM);
					ra->setMin(bra->float_arg_1);
					ra->setMax(bra->float_arg_2);
					break;
				case ACT_RANDOM_FLOAT_NORMAL:
					ra->setDistribution(gkRandomActuator::RA_FLOAT_NORMAL);
					ra->setMean(bra->float_arg_1);
					ra->setDeviation(bra->float_arg_2);
					break;
				case ACT_RANDOM_FLOAT_NEGATIVE_EXPONENTIAL:
					ra->setDistribution(gkRandomActuator::RA_FLOAT_NEGEXP);
					ra->setHalfLife(bra->float_arg_1);
					break;
				default:
					break;
				}

			} break;
		case ACT_PARENT:
			{
				gkParentActuator* pa = new gkParentActuator(gobj, lnk, bact->name);
				la = pa;
				Blender::bParentActuator* bpa = (Blender::bParentActuator*)bact->data;

				pa->setParent(GKB_IDNAME(bpa->ob));
				pa->setCompound((bpa->flag & ACT_PARENT_COMPOUND) != 0);
				pa->setGhost((bpa->flag & ACT_PARENT_GHOST) != 0);

				switch (bpa->type)
				{
				case ACT_PARENT_REMOVE:
					pa->setMode(gkParentActuator::PA_CLEAR);
					break;
				case ACT_PARENT_SET:
					pa->setMode(gkParentActuator::PA_SET);
					break;
				default:
					break;
				}

			} break;
		case ACT_SCENE:
			{
				gkSceneActuator* sa = new gkSceneActuator(gobj, lnk, bact->name);
				la = sa;
				Blender::bSceneActuator* bsa = (Blender::bSceneActuator*)bact->data;

				sa->setCamera(GKB_IDNAME(bsa->camera));
				sa->setScene(GKB_IDNAME(bsa->scene));

				switch (bsa->type)
				{
				case ACT_SCENE_RESTART:
					sa->setMode(gkSceneActuator::SC_RESTART);
					break;
				case ACT_SCENE_SET:
					sa->setMode(gkSceneActuator::SC_SET_SCENE);
					break;
				case ACT_SCENE_CAMERA:
					sa->setMode(gkSceneActuator::SC_SET_CAMERA);
					break;
				case ACT_SCENE_ADD_FRONT:
					sa->setMode(gkSceneActuator::SC_ADD_FRONT);
					break;
				case ACT_SCENE_ADD_BACK:
					sa->setMode(gkSceneActuator::SC_ADD_BACK);
					break;
				case ACT_SCENE_REMOVE:
					sa->setMode(gkSceneActuator::SC_REMOVE);
					break;
				case ACT_SCENE_SUSPEND:
					sa->setMode(gkSceneActuator::SC_SUSPEND);
					break;
				case ACT_SCENE_RESUME:
					sa->setMode(gkSceneActuator::SC_RESUME);
					break;
				default:
					break;
				}

			} break;
		case ACT_SOUND:
			{
#if OGREKIT_OPENAL_SOUND
				gkSoundActuator* sa = new gkSoundActuator(gobj, lnk, bact->name);
				la = sa;

				Blender::bSoundActuator* bsa = (Blender::bSoundActuator*)bact->data;

				int mode = 0;

				switch (bsa->type)
				{
				case ACT_SND_PLAY_STOP_SOUND:
					mode = gkSoundActuator::SA_PLAY_STOP;
					break;
				case ACT_SND_PLAY_END_SOUND:
					mode = gkSoundActuator::SA_PLAY_END;
					break;
				case ACT_SND_LOOP_STOP_SOUND:
					mode = gkSoundActuator::SA_LOOP_STOP;
					break;
				case ACT_SND_LOOP_END_SOUND:
					mode = gkSoundActuator::SA_LOOP_END;
					break;
				case ACT_SND_LOOP_BIDIRECTIONAL_SOUND:
					mode = gkSoundActuator::SA_LOOP_PPONG;
					break;
				case ACT_SND_LOOP_BIDIRECTIONAL_STOP_SOUND:
					mode = gkSoundActuator::SA_LOOP_PPONG_STOP;
					break;
				}


				gkSoundProperties& props = sa->getProperties();

				props.m_volume  = bsa->volume;
				props.m_pitch   = bsa->pitch;


				if (bsa->flag == 1) // 3d
				{
					props.m_3dSound = true;

					props.m_gainClamp.x = bsa->sound3D.min_gain;
					props.m_gainClamp.y = bsa->sound3D.max_gain;
					props.m_refDistance = bsa->sound3D.reference_distance;
					props.m_maxDistance = bsa->sound3D.max_distance;
					props.m_rolloff     = bsa->sound3D.rolloff_factor;
					props.m_coneAngle.x = bsa->sound3D.cone_inner_angle;
					props.m_coneAngle.y = bsa->sound3D.cone_outer_angle;
					props.m_coneOuterGain = bsa->sound3D.cone_outer_gain;
				}

				sa->setMode(mode);
				sa->setSoundFile(GKB_IDNAME(bsa->sound));
#endif
			} break;

		}



		if (la)
			lnk->push(la, bact);

	}

	int uuid = 0;

	for (Blender::bController* bcont = (Blender::bController*)bobj->controllers.first; bcont; bcont = bcont->next)
	{
		gkLogicController* lc = 0;
		switch (bcont->type)
		{
		case CONT_LOGIC_OR:
		case CONT_LOGIC_XOR:
		case CONT_LOGIC_AND:
		case CONT_LOGIC_NAND:
		case CONT_LOGIC_NOR:
		case CONT_LOGIC_XNOR:
			{
				gkLogicOpController* ac = new gkLogicOpController(gobj, lnk, bcont->name);
				if (bcont->type == CONT_LOGIC_OR)
					ac->setOp(gkLogicOpController::OP_OR);
				else if (bcont->type == CONT_LOGIC_XOR)
					ac->setOp(gkLogicOpController::OP_XOR);
				else if (bcont->type == CONT_LOGIC_AND)
					ac->setOp(gkLogicOpController::OP_AND);
				else if (bcont->type == CONT_LOGIC_NAND)
					ac->setOp(gkLogicOpController::OP_NAND);
				else if (bcont->type == CONT_LOGIC_NOR)
					ac->setOp(gkLogicOpController::OP_NOR);
				else if (bcont->type == CONT_LOGIC_XNOR)
					ac->setOp(gkLogicOpController::OP_XNOR);
				lc = ac;
			}
			break;
		case CONT_EXPRESSION:
			{
#ifdef OGREKIT_USE_LUA
				gkExpressionController* sc = new gkExpressionController(gobj, lnk, bcont->name);
				lc = sc;

				Blender::bExpressionCont* pcon = (Blender::bExpressionCont*)bcont->data;
				sc->setModule(false);

				if (pcon->str)
				{
					sc->setExpression(pcon->str);
				}
#endif
			} break;
		case CONT_PYTHON:
			{
#ifdef OGREKIT_USE_LUA
				gkScriptController* sc = new gkScriptController(gobj, lnk, bcont->name);
				lc = sc;

				Blender::bPythonCont* pcon = (Blender::bPythonCont*)bcont->data;
				sc->setModule(false);

				if (pcon->text)
				{
					gkLuaManager& lua = gkLuaManager::getSingleton();
					gkResourceName scriptName(GKB_IDNAME(pcon->text), groupName);
					if (lua.exists(scriptName))
						sc->setScript(lua.getByName<gkLuaScript>(scriptName));
					else
						sc->setScript(lua.create<gkLuaScript>(scriptName));
				}
#endif

			} break;


		}

		if (lc)
		{
			if ((bcont->flag & OB_DEBUGSTATE) || (bobj->scaflag & OB_DEBUGSTATE))
				lc->setDebugMask(bcont->state_mask);

			lc->setMask(bcont->state_mask);
			lc->setPriority((bcont->flag & CONT_PRIO) != 0);


			for (int i = 0; i < bcont->totlinks; ++i)
			{
				Blender::bActuator* a = bcont->links[i];
				if (a)
				{
					gkLogicActuator* la = lnk->findActuator(a);
					if (la)
						lc->link(la);
					else
					{
						ResolveObject robj = {0, 0, 0, 0};
						robj.lcont  = lc;
						robj.act    = a;
						m_missing.push_back(robj);
					}

				}
			}

			lnk->push(lc, bcont);
		}
	}


	for (Blender::bSensor* bsen = (Blender::bSensor*)bobj->sensors.first; bsen; bsen = bsen->next)
	{

		gkLogicSensor* ls = 0;
		switch (bsen->type)
		{
		case SENS_RAY:
			{
				gkRaySensor* rs = new gkRaySensor(gobj, lnk, bsen->name);
				ls = rs;
				Blender::bRaySensor* brs = (Blender::bRaySensor*)bsen->data;

				int axis = 0;
				switch (brs->axisflag)
				{
				case 1: {axis = gkRaySensor::RA_XPOS; break;}
				case 0: {axis = gkRaySensor::RA_YPOS; break;}
				case 2: {axis = gkRaySensor::RA_ZPOS; break;}
				case 3: {axis = gkRaySensor::RA_XNEG; break;}
				case 4: {axis = gkRaySensor::RA_YNEG; break;}
				case 5: {axis = gkRaySensor::RA_ZNEG; break;}
				}

				rs->setRange(brs->range);
				rs->setAxis(axis);

				if (brs->matname[0] != '\0')
					rs->setMaterial(gkLogicLoader_formatText(brs->matname));
				if (brs->propname[0] != '\0')
					rs->setProperty(gkLogicLoader_formatText(brs->propname));

			} break;
		case SENS_TOUCH:
		case SENS_COLLISION:
			{
				// tell the gk world we want to collect collsion information
				gkCollisionSensor* col = new gkCollisionSensor(gobj, lnk, bsen->name);
				ls = col;

				if (bsen->type == SENS_COLLISION)
				{
					Blender::bCollisionSensor* cse = (Blender::bCollisionSensor*)bsen->data;

					if (cse->mode & SENS_COLLISION_MATERIAL)
						col->setMaterial(gkLogicLoader_formatText(cse->materialName));
					else
						col->setProperty(gkLogicLoader_formatText(cse->name));
				}
				else
				{
					Blender::bTouchSensor* bto = (Blender::bTouchSensor*)bsen->data;
					if (bto->ma)
						col->setMaterial(GKB_IDNAME(bto->ma));
				}

			} break;

		case SENS_ALWAYS:
			{
				gkAlwaysSensor* asn = new gkAlwaysSensor(gobj, lnk, bsen->name);
				ls = asn;
			}
			break;
		case SENS_PROPERTY:
			{
				gkPropertySensor* psn = new gkPropertySensor(gobj, lnk, bsen->name);
				ls = psn;

				Blender::bPropertySensor* bps = (Blender::bPropertySensor*)bsen->data;

				psn->setProperty(gkLogicLoader_formatText(bps->name));
				psn->setValue(bps->value);
				psn->setMaxValue(bps->maxvalue);

				switch (bps->type)
				{
				case SENS_PROP_EQUAL:       psn->setType(gkPropertySensor::PS_EQUAL);       break;
				case SENS_PROP_NEQUAL:      psn->setType(gkPropertySensor::PS_NEQUAL);      break;
				case SENS_PROP_INTERVAL:    psn->setType(gkPropertySensor::PS_INTERVAL);    break;
				case SENS_PROP_CHANGED:     psn->setType(gkPropertySensor::PS_CHANGED);     break;
				}
			}
			break;
		case SENS_MOUSE:
			{
				gkMouseSensor* ms = new gkMouseSensor(gobj, lnk, bsen->name);
				ls = ms;

				Blender::bMouseSensor* mse = (Blender::bMouseSensor*)bsen->data;

				int type = 0;
				if (mse->type == BL_SENS_MOUSE_LEFT_BUTTON)
					type = gkMouseSensor::MOUSE_LEFT;
				else if (mse->type == BL_SENS_MOUSE_MIDDLE_BUTTON)
					type = gkMouseSensor::MOUSE_MIDDLE;
				else if (mse->type == BL_SENS_MOUSE_RIGHT_BUTTON)
					type = gkMouseSensor::MOUSE_RIGHT;
				else if (mse->type == BL_SENS_MOUSE_WHEEL_UP)
					type = gkMouseSensor::MOUSE_WHEEL_UP;
				else if (mse->type == BL_SENS_MOUSE_WHEEL_DOWN)
					type = gkMouseSensor::MOUSE_WHEEL_DOWN;
				else if (mse->type == BL_SENS_MOUSE_MOVEMENT)
					type = gkMouseSensor::MOUSE_MOTION;
				else if (mse->type == BL_SENS_MOUSE_MOUSEOVER)
					type = gkMouseSensor::MOUSE_MOUSE_OVER;
				else if (mse->type == BL_SENS_MOUSE_MOUSEOVER_ANY)
					type = gkMouseSensor::MOUSE_MOUSE_OVER_ANY;

				ms->setType(type);
			}
			break;
		case SENS_KEYBOARD:
			{
				gkKeyboardSensor* ks = new gkKeyboardSensor(gobj, lnk, bsen->name);
				ls = ks;

				Blender::bKeyboardSensor* bks = (Blender::bKeyboardSensor*)bsen->data;
				ks->setKey(getKey(bks->key));
				ks->setMod0(getKey(bks->qual));
				ks->setMod1(getKey(bks->qual2));
				ks->setAllKeys(bks->type == SENS_ALL_KEYS);
			} break;
		case SENS_RADAR:
			{
				gkRadarSensor* rs = new gkRadarSensor(gobj, lnk, bsen->name);
				ls = rs;
				Blender::bRadarSensor* brs = (Blender::bRadarSensor*)bsen->data;

				int axis = 0;
				switch (brs->axis)
				{
				case 1: {axis = gkRadarSensor::RA_XPOS; break;}
				case 0: {axis = gkRadarSensor::RA_YPOS; break;}
				case 2: {axis = gkRadarSensor::RA_ZPOS; break;}
				case 3: {axis = gkRadarSensor::RA_XNEG; break;}
				case 4: {axis = gkRadarSensor::RA_YNEG; break;}
				case 5: {axis = gkRadarSensor::RA_ZNEG; break;}
				}

				rs->setRange(brs->range);
				rs->setAxis(axis);
				rs->setAngle(brs->angle * gkRPD);

				if (brs->name[0] != '\0')
					rs->setProperty(gkLogicLoader_formatText(brs->name));

			} break;

		case SENS_NEAR:
			{
				gkNearSensor* rs = new gkNearSensor(gobj, lnk, bsen->name);
				ls = rs;
				Blender::bNearSensor* brs = (Blender::bNearSensor*)bsen->data;

				rs->setRange(brs->dist);
				rs->setResetRange(brs->resetdist);

				if (brs->name[0] != '\0')
					rs->setProperty(gkLogicLoader_formatText(brs->name));

			} break;
		case SENS_JOYSTICK:
			{
				gkJoystickSensor* jss = new gkJoystickSensor(gobj, lnk, bsen->name);
				ls = jss;
				Blender::bJoystickSensor* bjss = (Blender::bJoystickSensor*)bsen->data;

				jss->setJoystickIndex(bjss->joyindex);
				jss->setAllElementEvents(bjss->flag & SENS_JOY_ANY_EVENT);
				jss->setAxisThreshold(bjss->precision);
				switch (bjss->type)
				{
				case SENS_JOY_AXIS_SINGLE:
					jss->setEventType(gkJoystickSensor::JT_AXIS);
					jss->setElementIndex(bjss->axis_single - 1);
					break;
				case SENS_JOY_AXIS:
					jss->setEventType(gkJoystickSensor::JT_AXIS_PAIR);
					jss->setElementIndex(bjss->axis - 1);
					switch (bjss->axisf)
					{
					case 2:
						jss->setAxisDirection(gkJoystickSensor::AD_LEFT);
						break;
					case 0:
						jss->setAxisDirection(gkJoystickSensor::AD_RIGHT);
						break;
					case 3:
						jss->setAxisDirection(gkJoystickSensor::AD_BOTTOM);
						break;
					case 1:
						jss->setAxisDirection(gkJoystickSensor::AD_TOP);
						break;
					}
					break;
				case SENS_JOY_BUTTON:
					jss->setEventType(gkJoystickSensor::JT_BUTTON);
					jss->setElementIndex(bjss->button);
					break;
				case SENS_JOY_HAT:
					jss->setEventType(gkJoystickSensor::JT_HAT);
					jss->setElementIndex(bjss->hat - 1);
					break;
				default:
					printf("JoystickSensor converter bad case test\n");
					break;
				}
			} break;
		case SENS_RANDOM:
			{
				gkRandomSensor* rs = new gkRandomSensor(gobj, lnk, bsen->name);
				ls = rs;
				Blender::bRandomSensor* brs = (Blender::bRandomSensor*)bsen->data;
				rs->setSeed(brs->seed);
			} break;
		case SENS_DELAY:
			{
				gkDelaySensor* ds = new gkDelaySensor(gobj, lnk, bsen->name);
				ls = ds;
				Blender::bDelaySensor* bds = (Blender::bDelaySensor*)bsen->data;
				ds->setDuration(bds->duration);
				ds->setDelay(bds->delay);
				ds->setRepeat(bds->flag & SENS_DELAY_REPEAT);
			} break;
		case SENS_ACTUATOR:
			{
				gkActuatorSensor* as = new gkActuatorSensor(gobj, lnk, bsen->name);
				ls = as;
				Blender::bActuatorSensor* bas = (Blender::bActuatorSensor*)bsen->data;
				as->setActuatorName(bas->name);
			} break;
		case SENS_MESSAGE:
			{
				gkMessageSensor* ms = new gkMessageSensor(gobj, lnk, bsen->name);
				ls = ms;
				Blender::bMessageSensor* bms = (Blender::bMessageSensor*)bsen->data;
				ms->setSubject(bms->subject);
			} break;
		}

		if (ls)
		{
			for (int i = 0; i < bsen->totlinks; ++i)
			{
				Blender::bController* c = bsen->links[i];
				if (c)
				{
					gkLogicController* lc = lnk->findController(c);
					if (lc)
						ls->link(lc);
					else
					{
						ResolveObject robj = {0, 0, 0, 0};
						robj.lsens = ls;
						robj.cont = c;
						m_missing.push_back(robj);
					}
				}
			}
			ls->setDetector(bsen->level != 0);
			ls->setTap(bsen->tap != 0);
			ls->setFrequency(bsen->freq);
			ls->invert(bsen->invert != 0);
			int pulse = 0;

			if (bsen->pulse & SENS_PULSE_REPEAT)
				pulse |= gkLogicSensor::PM_TRUE;
			if (bsen->pulse & SENS_NEG_PULSE_MODE)
				pulse |= gkLogicSensor::PM_FALSE;

			if (pulse == 0)
				pulse = gkLogicSensor::PM_IDLE;

			ls->setStartState(lnk->getState());
			ls->setMode(pulse);
			lnk->push(ls);
		}
	}
}


int gkLogicLoader::getKey(int kc)
{
#define gkLogicCase(x) case x: return KC_##x;
	switch (kc)
	{
		gkLogicCase(AKEY)
		gkLogicCase(BKEY)
		gkLogicCase(CKEY)
		gkLogicCase(DKEY)
		gkLogicCase(EKEY)
		gkLogicCase(FKEY)
		gkLogicCase(GKEY)
		gkLogicCase(IKEY)
		gkLogicCase(JKEY)
		gkLogicCase(KKEY)
		gkLogicCase(LKEY)
		gkLogicCase(MKEY)
		gkLogicCase(NKEY)
		gkLogicCase(OKEY)
		gkLogicCase(PKEY)
		gkLogicCase(QKEY)
		gkLogicCase(RKEY)
		gkLogicCase(SKEY)
		gkLogicCase(TKEY)
		gkLogicCase(UKEY)
		gkLogicCase(VKEY)
		gkLogicCase(WKEY)
		gkLogicCase(XKEY)
		gkLogicCase(YKEY)
		gkLogicCase(ZKEY)
		gkLogicCase(ZEROKEY)
		gkLogicCase(ONEKEY)
		gkLogicCase(TWOKEY)
		gkLogicCase(THREEKEY)
		gkLogicCase(FOURKEY)
		gkLogicCase(FIVEKEY)
		gkLogicCase(SIXKEY)
		gkLogicCase(SEVENKEY)
		gkLogicCase(EIGHTKEY)
		gkLogicCase(NINEKEY)
		gkLogicCase(CAPSLOCKKEY)
		gkLogicCase(LEFTCTRLKEY)
		gkLogicCase(LEFTALTKEY)
		gkLogicCase(RIGHTALTKEY)
		gkLogicCase(RIGHTCTRLKEY)
		gkLogicCase(RIGHTSHIFTKEY)
		gkLogicCase(LEFTSHIFTKEY)
		gkLogicCase(ESCKEY)
		gkLogicCase(TABKEY)
		gkLogicCase(RETKEY)
		gkLogicCase(SPACEKEY)
		gkLogicCase(LINEFEEDKEY)
		gkLogicCase(BACKSPACEKEY)
		gkLogicCase(DELKEY)
		gkLogicCase(SEMICOLONKEY)
		gkLogicCase(PERIODKEY)
		gkLogicCase(COMMAKEY)
		gkLogicCase(QUOTEKEY)
		gkLogicCase(ACCENTGRAVEKEY)
		gkLogicCase(MINUSKEY)
		gkLogicCase(SLASHKEY)
		gkLogicCase(BACKSLASHKEY)
		gkLogicCase(EQUALKEY)
		gkLogicCase(LEFTBRACKETKEY)
		gkLogicCase(RIGHTBRACKETKEY)
		gkLogicCase(LEFTARROWKEY)
		gkLogicCase(DOWNARROWKEY)
		gkLogicCase(RIGHTARROWKEY)
		gkLogicCase(UPARROWKEY)
		gkLogicCase(PAD0)
		gkLogicCase(PAD1)
		gkLogicCase(PAD2)
		gkLogicCase(PAD3)
		gkLogicCase(PAD4)
		gkLogicCase(PAD5)
		gkLogicCase(PAD6)
		gkLogicCase(PAD7)
		gkLogicCase(PAD8)
		gkLogicCase(PAD9)
		gkLogicCase(PADPERIOD)
		gkLogicCase(PADSLASHKEY)
		gkLogicCase(PADASTERKEY)
		gkLogicCase(PADMINUS)
		gkLogicCase(PADENTER)
		gkLogicCase(PADPLUSKEY)
		gkLogicCase(F1KEY)
		gkLogicCase(F2KEY)
		gkLogicCase(F3KEY)
		gkLogicCase(F4KEY)
		gkLogicCase(F5KEY)
		gkLogicCase(F6KEY)
		gkLogicCase(F7KEY)
		gkLogicCase(F8KEY)
		gkLogicCase(F9KEY)
		gkLogicCase(F10KEY)
		gkLogicCase(F11KEY)
		gkLogicCase(F12KEY)
		gkLogicCase(PAUSEKEY)
		gkLogicCase(INSERTKEY)
		gkLogicCase(HOMEKEY)
		gkLogicCase(PAGEUPKEY)
		gkLogicCase(PAGEDOWNKEY)
		gkLogicCase(ENDKEY)
		gkLogicCase(UNKNOWNKEY)
		gkLogicCase(COMMANDKEY)
		gkLogicCase(GRLESSKEY)
	}

	if (kc == _HKEY)
		return KC_HKEY;

#undef gkLogicCase
	return KC_NONE;
}
