/*
The zlib/libpng License

Copyright (c) 2005-2007 Phillip Castaneda (pjcast -- www.wreckedgames.com)

This software is provided 'as-is', without any express or implied warranty. In no event will
the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial 
applications, and to alter it and redistribute it freely, subject to the following
restrictions:

    1. The origin of this software must not be misrepresented; you must not claim that 
		you wrote the original software. If you use this software in a product, 
		an acknowledgment in the product documentation would be appreciated but is 
		not required.

    2. Altered source versions must be plainly marked as such, and must not be 
		misrepresented as being the original software.

    3. This notice may not be removed or altered from any source distribution.
*/
#include "OISEffect.h"
#include "OISException.h"

using namespace OIS;

//VC7.1 had a problem with these not getting included.. 
//Perhaps a case of a crazy extreme optimizer :/ (moved to header)
//const unsigned int Effect::OIS_INFINITE = 0xFFFFFFFF;

//------------------------------------------------------------------------------//
Effect::Effect() : 
	force(UnknownForce), 
	type(Unknown),
	effect(0),
	axes(1)
{
}

//------------------------------------------------------------------------------//
Effect::Effect(EForce ef, EType et) : 
	force(ef), 
	type(et),
	direction(North), 
	trigger_button(-1),
	trigger_interval(0),
	replay_length(Effect::OIS_INFINITE),
	replay_delay(0),
	_handle(-1),
	axes(1)
{
	effect = 0;

	switch( ef )
	{
	case ConstantForce:    effect = new ConstantEffect(); break;
	case RampForce:	       effect = new RampEffect(); break;
	case PeriodicForce:    effect = new PeriodicEffect(); break;
	case ConditionalForce: effect = new ConditionalEffect(); break;
	default: break;
	}
}

//------------------------------------------------------------------------------//
Effect::~Effect()
{
	delete effect;
}

//------------------------------------------------------------------------------//
ForceEffect* Effect::getForceEffect() const
{
	//If no effect was created in constructor, then we raise an error here
	if( effect == 0 )
		OIS_EXCEPT( E_NotSupported, "Requested ForceEffect is null!" );

	return effect;
}

//------------------------------------------------------------------------------//
void Effect::setNumAxes(short nAxes)
{
	//Can only be set before a handle was assigned (effect created)
	if( _handle != -1 )
        axes = nAxes;
}

//------------------------------------------------------------------------------//
short Effect::getNumAxes() const
{
	return axes;
}
