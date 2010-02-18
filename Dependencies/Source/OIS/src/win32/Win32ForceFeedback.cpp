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
#include "Win32/Win32ForceFeedback.h"
#include "OISException.h"
#include <Math.h>

#if defined (_DEBUG)
  #include <sstream>
#endif

using namespace OIS;

//--------------------------------------------------------------//
Win32ForceFeedback::Win32ForceFeedback(IDirectInputDevice8* joy) :
	mHandles(0), mJoyStick(joy)
{
}

//--------------------------------------------------------------//
Win32ForceFeedback::~Win32ForceFeedback()
{
	//Get the effect - if it exists
	for(EffectList::iterator i = mEffectList.begin(); i != mEffectList.end(); ++i )
	{
		LPDIRECTINPUTEFFECT dxEffect = i->second;
		if( dxEffect )
			dxEffect->Unload();
	}

	mEffectList.clear();
}

//--------------------------------------------------------------//
void Win32ForceFeedback::upload( const Effect* effect )
{
	switch( effect->force )
	{
		case OIS::Effect::ConstantForce: _updateConstantEffect(effect);	break;
		case OIS::Effect::RampForce: _updateRampEffect(effect);	break;
		case OIS::Effect::PeriodicForce: _updatePeriodicEffect(effect);	break;
		case OIS::Effect::ConditionalForce:	_updateConditionalEffect(effect); break;
		//case OIS::Effect::CustomForce: _updateCustomEffect(effect); break;
		default: OIS_EXCEPT(E_NotImplemented, "Requested Force not Implemented yet, sorry!"); break;
	}
}

//--------------------------------------------------------------//
void Win32ForceFeedback::modify( const Effect* eff )
{
	//Modifying is essentially the same as an upload, so, just reuse that function
	upload(eff);
}

//--------------------------------------------------------------//
void Win32ForceFeedback::remove( const Effect* eff )
{
	//Get the effect - if it exists
	EffectList::iterator i = mEffectList.find(eff->_handle);
	if( i != mEffectList.end() )
	{
		LPDIRECTINPUTEFFECT dxEffect = i->second;
		if( dxEffect )
		{
			dxEffect->Stop();
			//We care about the return value - as the effect might not
			//have been unlaoded
			if( SUCCEEDED(dxEffect->Unload()) )
				mEffectList.erase(i);
		}
		else
			mEffectList.erase(i);
	}
}

//--------------------------------------------------------------//
void Win32ForceFeedback::setMasterGain( float level )
{
	//Between 0 - 10,000
	int gain_level = (int)(10000.0f * level);

	if( gain_level > 10000 )
		gain_level = 10000;
	else if( gain_level < 0 )
		gain_level = 0;

	DIPROPDWORD DIPropGain;
	DIPropGain.diph.dwSize       = sizeof(DIPropGain);
	DIPropGain.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	DIPropGain.diph.dwObj        = 0;
	DIPropGain.diph.dwHow        = DIPH_DEVICE;
	DIPropGain.dwData            = gain_level;

	mJoyStick->SetProperty(DIPROP_FFGAIN, &DIPropGain.diph);
}

//--------------------------------------------------------------//
void Win32ForceFeedback::setAutoCenterMode( bool auto_on )
{
	//DI Property DIPROPAUTOCENTER_OFF = 0, 1 is on
	DIPROPDWORD DIPropAutoCenter;
	DIPropAutoCenter.diph.dwSize       = sizeof(DIPropAutoCenter);
	DIPropAutoCenter.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	DIPropAutoCenter.diph.dwObj        = 0;
	DIPropAutoCenter.diph.dwHow        = DIPH_DEVICE;
	DIPropAutoCenter.dwData            = auto_on;

	//hr =
	mJoyStick->SetProperty(DIPROP_AUTOCENTER, &DIPropAutoCenter.diph);
}

//--------------------------------------------------------------//
void Win32ForceFeedback::_updateConstantEffect( const Effect* effect )
{
	DWORD           rgdwAxes[2]     = { DIJOFS_X, DIJOFS_Y };
	LONG            rglDirection[2] = { 0, 0 };
	DICONSTANTFORCE cf;
	DIEFFECT        diEffect;

	//Currently only support 1 axis
	//if( effect->getNumAxes() == 1 )
	cf.lMagnitude = static_cast<ConstantEffect*>(effect->getForceEffect())->level;

	_setCommonProperties(&diEffect, rgdwAxes, rglDirection, sizeof(DICONSTANTFORCE), &cf, effect);
	_upload(GUID_ConstantForce, &diEffect, effect);
}

//--------------------------------------------------------------//
void Win32ForceFeedback::_updateRampEffect( const Effect* effect )
{
	DWORD           rgdwAxes[2]     = { DIJOFS_X, DIJOFS_Y };
	LONG            rglDirection[2] = { 0, 0 };
	DIRAMPFORCE     rf;
	DIEFFECT        diEffect;

	//Currently only support 1 axis
	rf.lStart = static_cast<RampEffect*>(effect->getForceEffect())->startLevel;
	rf.lEnd = static_cast<RampEffect*>(effect->getForceEffect())->endLevel;

	_setCommonProperties(&diEffect, rgdwAxes, rglDirection, sizeof(DIRAMPFORCE), &rf, effect);
	_upload(GUID_RampForce, &diEffect, effect);
}

//--------------------------------------------------------------//
void Win32ForceFeedback::_updatePeriodicEffect( const Effect* effect )
{
	DWORD           rgdwAxes[2]     = { DIJOFS_X, DIJOFS_Y };
	LONG            rglDirection[2] = { 0, 0 };
	DIPERIODIC      pf;
	DIEFFECT        diEffect;

	//Currently only support 1 axis
	pf.dwMagnitude = static_cast<PeriodicEffect*>(effect->getForceEffect())->magnitude;
	pf.lOffset = static_cast<PeriodicEffect*>(effect->getForceEffect())->offset;
	pf.dwPhase = static_cast<PeriodicEffect*>(effect->getForceEffect())->phase;
	pf.dwPeriod = static_cast<PeriodicEffect*>(effect->getForceEffect())->period;

	_setCommonProperties(&diEffect, rgdwAxes, rglDirection, sizeof(DIPERIODIC), &pf, effect);

	switch( effect->type )
	{
	case OIS::Effect::Square: _upload(GUID_Square, &diEffect, effect); break;
	case OIS::Effect::Triangle: _upload(GUID_Triangle, &diEffect, effect); break;
	case OIS::Effect::Sine: _upload(GUID_Sine, &diEffect, effect); break;
	case OIS::Effect::SawToothUp: _upload(GUID_SawtoothUp, &diEffect, effect); break;
	case OIS::Effect::SawToothDown:	_upload(GUID_SawtoothDown, &diEffect, effect); break;
	default: break;
	}
}

//--------------------------------------------------------------//
void Win32ForceFeedback::_updateConditionalEffect( const Effect* effect )
{
	DWORD           rgdwAxes[2]     = { DIJOFS_X, DIJOFS_Y };
	LONG            rglDirection[2] = { 0, 0 };
	DICONDITION     cf;
	DIEFFECT        diEffect;

	cf.lOffset = static_cast<ConditionalEffect*>(effect->getForceEffect())->deadband;
	cf.lPositiveCoefficient = static_cast<ConditionalEffect*>(effect->getForceEffect())->rightCoeff;
	cf.lNegativeCoefficient = static_cast<ConditionalEffect*>(effect->getForceEffect())->leftCoeff;
	cf.dwPositiveSaturation = static_cast<ConditionalEffect*>(effect->getForceEffect())->rightSaturation;
	cf.dwNegativeSaturation = static_cast<ConditionalEffect*>(effect->getForceEffect())->leftSaturation;
	cf.lDeadBand = static_cast<ConditionalEffect*>(effect->getForceEffect())->deadband;

	_setCommonProperties(&diEffect, rgdwAxes, rglDirection, sizeof(DICONDITION), &cf, effect);

	switch( effect->type )
	{
	case OIS::Effect::Friction:	_upload(GUID_Friction, &diEffect, effect); break;
	case OIS::Effect::Damper: _upload(GUID_Damper, &diEffect, effect); break;
	case OIS::Effect::Inertia: _upload(GUID_Inertia, &diEffect, effect); break;
	case OIS::Effect::Spring: _upload(GUID_Spring, &diEffect, effect); break;
	default: break;
	}
}

//--------------------------------------------------------------//
void Win32ForceFeedback::_updateCustomEffect( const Effect* /*effect*/ )
{
	//DWORD           rgdwAxes[2]     = { DIJOFS_X, DIJOFS_Y };
	//LONG            rglDirection[2] = { 0, 0 };
	//DICUSTOMFORCE cf;
	//DIEFFECT        diEffect;
	//cf.cChannels = 0;
	//cf.dwSamplePeriod = 0;
	//cf.cSamples = 0;
	//cf.rglForceData = 0;
	//_setCommonProperties(&diEffect, rgdwAxes, rglDirection, sizeof(DICUSTOMFORCE), &cf, effect);
	//_upload(GUID_CustomForce, &diEffect, effect);
}

//--------------------------------------------------------------//
void Win32ForceFeedback::_setCommonProperties(
		DIEFFECT* diEffect, DWORD* rgdwAxes,
		LONG* rglDirection, DWORD struct_size,
		LPVOID struct_type, const Effect* effect )
{
	ZeroMemory(diEffect, sizeof(DIEFFECT));

	diEffect->dwSize                  = sizeof(DIEFFECT);
	diEffect->dwFlags                 = DIEFF_CARTESIAN | DIEFF_OBJECTOFFSETS;
	diEffect->dwDuration              = effect->replay_length;
	diEffect->dwSamplePeriod          = 0;
	diEffect->dwGain                  = DI_FFNOMINALMAX;
	diEffect->dwTriggerButton         = DIEB_NOTRIGGER;
	diEffect->dwTriggerRepeatInterval = 0;
	diEffect->cAxes                   = effect->getNumAxes();
	diEffect->rgdwAxes                = rgdwAxes;
	diEffect->rglDirection            = rglDirection;
	diEffect->lpEnvelope              = 0;
	diEffect->cbTypeSpecificParams    = struct_size;
	diEffect->lpvTypeSpecificParams   = struct_type;
	diEffect->dwStartDelay            = effect->replay_delay;
}

//--------------------------------------------------------------//
void Win32ForceFeedback::_upload( GUID guid, DIEFFECT* diEffect, const Effect* effect)
{
	LPDIRECTINPUTEFFECT dxEffect = 0;

	//Get the effect - if it exists
	EffectList::iterator i = mEffectList.find(effect->_handle);
	//It has been created already
	if( i != mEffectList.end() )
		dxEffect = i->second;
	else //This effect has not yet been created - generate a handle
		effect->_handle = mHandles++;

	if( dxEffect == 0 )
	{
		//This effect has not yet been created, so create it
		HRESULT hr = mJoyStick->CreateEffect(guid, diEffect, &dxEffect, NULL);
		if(SUCCEEDED(hr))
		{
			mEffectList[effect->_handle] = dxEffect;
			dxEffect->Start(INFINITE,0);
		}
		else if( hr == DIERR_DEVICEFULL )
			OIS_EXCEPT(E_DeviceFull, "Remove an effect before adding more!");
		else
			OIS_EXCEPT(E_General, "Unknown error creating effect->..");
	}
	else
	{
		//ToDo -- Update the Effect
		HRESULT hr = dxEffect->SetParameters( diEffect, DIEP_DIRECTION |
			DIEP_DURATION | DIEP_ENVELOPE | DIEP_STARTDELAY | DIEP_TRIGGERBUTTON |
			DIEP_TRIGGERREPEATINTERVAL | DIEP_TYPESPECIFICPARAMS | DIEP_START );

		if(FAILED(hr)) OIS_EXCEPT(E_InvalidParam, "Error updating device!");
	}
}

//--------------------------------------------------------------//
void Win32ForceFeedback::_addEffectSupport( LPCDIEFFECTINFO pdei )
{
	//Determine what the effect is and how it corresponds to our OIS's Enums
	//We could save the GUIDs too, however, we will just use the predefined
	//ones later
	if( pdei->guid == GUID_ConstantForce )
		_addEffectTypes((Effect::EForce)DIEFT_GETTYPE(pdei->dwEffType), Effect::Constant );
	else if( pdei->guid == GUID_Triangle )
		_addEffectTypes((Effect::EForce)DIEFT_GETTYPE(pdei->dwEffType), Effect::Triangle );
	else if( pdei->guid == GUID_Spring )
		_addEffectTypes((Effect::EForce)DIEFT_GETTYPE(pdei->dwEffType), Effect::Spring );
	else if( pdei->guid == GUID_Friction )
		_addEffectTypes((Effect::EForce)DIEFT_GETTYPE(pdei->dwEffType), Effect::Friction );
	else if( pdei->guid == GUID_Square )
		_addEffectTypes((Effect::EForce)DIEFT_GETTYPE(pdei->dwEffType), Effect::Square );
	else if( pdei->guid == GUID_Sine )
		_addEffectTypes((Effect::EForce)DIEFT_GETTYPE(pdei->dwEffType), Effect::Sine );
	else if( pdei->guid == GUID_SawtoothUp )
		_addEffectTypes((Effect::EForce)DIEFT_GETTYPE(pdei->dwEffType), Effect::SawToothUp );
	else if( pdei->guid == GUID_SawtoothDown )
		_addEffectTypes((Effect::EForce)DIEFT_GETTYPE(pdei->dwEffType), Effect::SawToothDown );
	else if( pdei->guid == GUID_Damper )
		_addEffectTypes((Effect::EForce)DIEFT_GETTYPE(pdei->dwEffType), Effect::Damper );
	else if( pdei->guid == GUID_Inertia )
		_addEffectTypes((Effect::EForce)DIEFT_GETTYPE(pdei->dwEffType), Effect::Inertia );
	else if( pdei->guid == GUID_CustomForce )
		_addEffectTypes((Effect::EForce)DIEFT_GETTYPE(pdei->dwEffType), Effect::Custom );
	else if( pdei->guid == GUID_RampForce )
		_addEffectTypes((Effect::EForce)DIEFT_GETTYPE(pdei->dwEffType), Effect::Ramp );
#if defined (_DEBUG)
	//Only care about this for Debugging Purposes
	//else
	//{
	//	std::ostringstream ss;
	//	ss << "Win32ForceFeedback, DirectInput8 Effect not found. Reported as: "
	//	   << pdei->tszName;
	//	OIS_EXCEPT( E_General, ss.str().c_str());
	//}
#endif
}
