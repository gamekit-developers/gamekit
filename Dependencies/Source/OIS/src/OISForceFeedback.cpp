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
#include "OISForceFeedback.h"
#include "OISException.h"

using namespace OIS;

//-------------------------------------------------------------//
void ForceFeedback::_addEffectTypes( Effect::EForce force, Effect::EType type )
{
	if( force == Effect::UnknownForce || type == Effect::Unknown )
		OIS_EXCEPT( E_General, "Unknown Force||Type was added too effect list..." );

	mSupportedEffects[force] = type;
}

//-------------------------------------------------------------//
const ForceFeedback::SupportedEffectList& 
						ForceFeedback::getSupportedEffects() const
{
	return mSupportedEffects;
}
