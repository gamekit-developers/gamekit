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
#include "linux/EventHelpers.h"
#include "linux/LinuxPrereqs.h"
#include "linux/LinuxForceFeedback.h"
#include "OISException.h"
#include "OISJoyStick.h"

#include <linux/input.h>
#include <cstring>

//#define OIS_LINUX_JOY_DEBUG

#ifdef OIS_LINUX_JOY_DEBUG
# include <iostream>
  using namespace std;
#endif

using namespace OIS;

class DeviceComponentInfo
{
public:
	std::vector<int> buttons, relAxes, absAxes, hats;
};

bool inline isBitSet(unsigned long bits[], unsigned int bit)
{
	return (bits[bit/(sizeof(long)*8)] >> ((bit)%(sizeof(long)*8))) & 1;
}
//-----------------------------------------------------------------------------//
DeviceComponentInfo getComponentInfo( int deviceID )
{
	unsigned long info[2][((KEY_MAX-1)/(sizeof(long)*8)) +1];
	memset( info, 0, sizeof(info) );

	DeviceComponentInfo components;

	//Read "all" (hence 0) components of the device - read into first entry
	ioctl(deviceID, EVIOCGBIT(0, EV_MAX), info[0]);

	for (int i = 0; i < EV_MAX; i++)
	{
		if( isBitSet(info[0], i) )
		{
			memset( info[1], 0, sizeof(info) / 2 );
			ioctl(deviceID, EVIOCGBIT(i, KEY_MAX), info[1]);
			for (int j = 0; j < KEY_MAX; j++)
			{
				if( isBitSet(info[1], j) )
				{
					if(i == EV_ABS)
					{
						//input_absinfo abInfo;
						//ioctl( fd, EVIOCGABS(j), abInfo );
						if( j >= ABS_HAT0X && j <= ABS_HAT3Y )
						{
							components.hats.push_back(j);
						}
						else
						{
							components.absAxes.push_back(j);
							//input_absinfo absinfo;
							//ioctl(deviceID, EVIOCGABS(j), &absinfo);
							//We cannot actually change these values :|
							//absinfo.minimum = JoyStick::MIN_AXIS;
							//absinfo.maximum = JoyStick::MAX_AXIS;
							//ioctl(deviceID, EVIOCSABS(j), &absinfo);
						}
					}
					else if(i == EV_REL)
					{
						components.relAxes.push_back(j);
					}
					else if(i == EV_KEY)
					{
						components.buttons.push_back(j);
					}
				}
			}
		}
	}

	return components;
}

//-----------------------------------------------------------------------------//
bool EventUtils::isJoyStick( int deviceID, JoyStickInfo &js )
{
	if( deviceID == -1 ) OIS_EXCEPT( E_General, "Error with File Descriptor" );

	DeviceComponentInfo info = getComponentInfo( deviceID );

	int buttons = 0;
	bool joyButtonFound = false;
	js.button_map.clear();

	#ifdef OIS_LINUX_JOY_DEBUG
	  cout << "\n\nDisplaying ButtonMapping Status:";
	#endif
	for(std::vector<int>::iterator i = info.buttons.begin(), e = info.buttons.end(); i != e; ++i )
	{
		//Check to ensure we find at least one joy only button
		if( (*i >= BTN_JOYSTICK && *i <= BTN_THUMBR) || (*i >= BTN_WHEEL && *i <= BTN_GEAR_UP ) )
			joyButtonFound = true;

		js.button_map[*i] = buttons++;

		#ifdef OIS_LINUX_JOY_DEBUG
		  cout << "\nButton Mapping ID (hex): " << hex << *i << " OIS Button Num: " << dec << (buttons-1);
		#endif
	}

	//Joy Buttons found, so it must be a joystick or pad
	if( joyButtonFound )
	{
		js.joyFileD = deviceID;
		js.vendor = getName(deviceID);
		js.buttons = buttons;
		js.axes = info.relAxes.size() + info.absAxes.size();
		js.hats = info.hats.size();

		//Map the Axes
		#ifdef OIS_LINUX_JOY_DEBUG
		  cout << "\n\nDisplaying AxisMapping Status:";
		#endif
		int axes = 0;
		for(std::vector<int>::iterator i = info.absAxes.begin(), e = info.absAxes.end(); i != e; ++i )
		{
			js.axis_map[*i] = axes;

			input_absinfo absinfo;
			ioctl(deviceID, EVIOCGABS(*i), &absinfo);
			js.axis_range[axes] = Range(absinfo.minimum, absinfo.maximum);

			#ifdef OIS_LINUX_JOY_DEBUG
			  cout << "\nAxis Mapping ID (hex): " << hex << *i << " OIS Axis Num: " << dec << axes;
			#endif

			++axes;
		}
	}

	return joyButtonFound;
}

//-----------------------------------------------------------------------------//
std::string EventUtils::getName( int deviceID )
{
	char name[OIS_DEVICE_NAME];
	ioctl(deviceID, EVIOCGNAME(OIS_DEVICE_NAME), name);
	return std::string(name);
}

//-----------------------------------------------------------------------------//
void EventUtils::enumerateForceFeedback( int deviceID, LinuxForceFeedback** ff )
{
	//Linux Event to OIS Event Mappings
	std::map<int, Effect::EType> typeMap;
	typeMap[FF_CONSTANT] = Effect::Constant;
	typeMap[FF_RAMP]     = Effect::Ramp;
	typeMap[FF_SPRING]   = Effect::Spring;
	typeMap[FF_FRICTION] = Effect::Friction;
	typeMap[FF_SQUARE]   = Effect::Square;
	typeMap[FF_TRIANGLE] = Effect::Triangle;
	typeMap[FF_SINE]     = Effect::Sine;
	typeMap[FF_SAW_UP]   = Effect::SawToothUp;
	typeMap[FF_SAW_DOWN] = Effect::SawToothDown;
	typeMap[FF_DAMPER]   = Effect::Damper;
	typeMap[FF_INERTIA]  = Effect::Inertia;
	typeMap[FF_CUSTOM]   = Effect::Custom;

	std::map<int, Effect::EForce> forceMap;
	forceMap[FF_CONSTANT] = Effect::ConstantForce;
	forceMap[FF_RAMP] = Effect::RampForce;
	forceMap[FF_PERIODIC] = Effect::PeriodicForce;
	forceMap[FF_CUSTOM] = Effect::CustomForce;

	//Remove any previously existing memory and create fresh
	removeForceFeedback( ff );
	*ff = new LinuxForceFeedback();

	unsigned long info[4] = {0,0,0,0};
	unsigned long subinfo[4]= {0,0,0,0};

	//Read overall force feedback components of the device
	ioctl(deviceID, EVIOCGBIT(EV_FF, sizeof(long)*4), info);

	//FF Axes
	//if( isBitSet(info, ABS_X) ) //X Axis
	//if( isBitSet(info, ABS_Y) ) //Y Axis
	//if( isBitSet(info, ABS_WHEEL) ) //Wheel

	//FF Effects
	for( int effect = ABS_WHEEL+1; effect < FF_MAX; effect++ )
	{
		if(isBitSet(info, effect))
		{
			//std::cout << "\tEffect Type: " << effect << std::endl;
			memset(subinfo, 0, sizeof(subinfo));
			//Read any info about this supported effect
			ioctl(deviceID, EVIOCGBIT(effect, sizeof(long)*4), subinfo);
			for( int force = 0; force < FF_MAX; force++ )
			{
				if(isBitSet(subinfo, force))
					(*ff)->_addEffectTypes( forceMap[force], typeMap[effect] );
			}
		}
	}

	//Check to see if any effects were added, else destroy the pointer
	const ForceFeedback::SupportedEffectList &list = (*ff)->getSupportedEffects();
	if( list.size() == 0 )
		removeForceFeedback( ff );
}

//-----------------------------------------------------------------------------//
void EventUtils::removeForceFeedback( LinuxForceFeedback** ff )
{
	delete *ff;
	*ff = 0;
}
