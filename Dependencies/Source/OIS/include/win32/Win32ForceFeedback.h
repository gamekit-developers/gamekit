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
#ifndef OIS_Win32ForceFeedBack_H
#define OIS_Win32ForceFeedBack_H

#include "OISPrereqs.h"
#include "OISForceFeedback.h"
#include "Win32/Win32Prereqs.h"

namespace OIS
{
	class Win32ForceFeedback : public ForceFeedback
	{
		Win32ForceFeedback() {}
	public:
		Win32ForceFeedback(IDirectInputDevice8* joy);
		~Win32ForceFeedback();

		/** @copydoc ForceFeedback::upload */
		void upload( const Effect* effect );

		/** @copydoc ForceFeedback::modify */
		void modify( const Effect* effect );

		/** @copydoc ForceFeedback::remove */
		void remove( const Effect* effect );

		/** @copydoc ForceFeedback::setMasterGain */
		void setMasterGain( float level );
		
		/** @copydoc ForceFeedback::setAutoCenterMode */
		void setAutoCenterMode( bool auto_on );

		/** @copydoc ForceFeedback::getFFAxesNumber 
			xxx todo - Actually return correct number
		*/
		short getFFAxesNumber() {return 1;}

		/**
			@remarks
			Internal use.. Used during enumeration to build a list of a devices
			support effects.
		*/
		void _addEffectSupport( LPCDIEFFECTINFO pdei );

	protected:
		//Specific Effect Settings
		void _updateConstantEffect( const Effect* effect );
		void _updateRampEffect( const Effect* effect );
		void _updatePeriodicEffect( const Effect* effect );
		void _updateConditionalEffect( const Effect* effect );
		void _updateCustomEffect( const Effect* effect );
		//Sets the common properties to all effects
		void _setCommonProperties( DIEFFECT* diEffect, DWORD* rgdwAxes,
									LONG* rglDirection, DWORD struct_size, 
									LPVOID struct_type, const Effect* effect );
		//Actually do the upload
		void _upload( GUID, DIEFFECT*, const Effect* );

		typedef std::map<int,LPDIRECTINPUTEFFECT> EffectList;
		EffectList mEffectList;
		//Simple unique handle creation - allows for upto 2+ million effects
		//during the lifetime of application. Hopefully, that is enough.
		int mHandles;

		IDirectInputDevice8* mJoyStick;
	};
}
#endif //OIS_Win32ForceFeedBack_H
