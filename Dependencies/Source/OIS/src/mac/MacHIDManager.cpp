/*
 The zlib/libpng License
 
 Copyright (c) 2006 Phillip Castaneda
 
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
#include "mac/MacHIDManager.h"
#include "OISException.h"
#include "OISObject.h"

#include <iostream>
using namespace std;

using namespace OIS;

//------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------//
template<typename T>
T getDictionaryItemAsRef(CFDictionaryRef dict, const char* keyName)
{
	return CFDictionaryGetValue(dict, OIS_CFString(keyName));
}

template<>
CFArrayRef getDictionaryItemAsRef(CFDictionaryRef dict, const char* keyName)
{
	CFTypeRef temp = CFDictionaryGetValue(dict, OIS_CFString(keyName));

	if(temp && CFGetTypeID(temp) == CFArrayGetTypeID())
		return (CFArrayRef)temp;
	else
		return 0;
}

template<>
CFStringRef getDictionaryItemAsRef(CFDictionaryRef dict, const char* keyName)
{
	CFTypeRef temp = CFDictionaryGetValue(dict, OIS_CFString(keyName));

	if(temp && CFGetTypeID(temp) == CFStringGetTypeID())
		return (CFStringRef)temp;
	else
		return 0;
}

template<>
CFNumberRef getDictionaryItemAsRef(CFDictionaryRef dict, const char* keyName)
{
	CFTypeRef temp = CFDictionaryGetValue(dict, OIS_CFString(keyName));

	if(temp && CFGetTypeID(temp) == CFNumberGetTypeID())
		return (CFNumberRef)temp;
	else
		return 0;
}

//------------------------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------------------------//
template<typename T>
T getArrayItemAsRef(CFArrayRef array, CFIndex idx)
{
	return CFArrayGetValueAtIndex(array, idx);
}

template<>
CFDictionaryRef getArrayItemAsRef(CFArrayRef array, CFIndex idx)
{
	CFTypeRef temp = CFArrayGetValueAtIndex(array, idx);

	if(temp && CFGetTypeID(temp) == CFDictionaryGetTypeID())
		return (CFDictionaryRef)temp;
	else
		return 0;
}

//------------------------------------------------------------------------------------------------------//
int getInt32(CFNumberRef ref)
{
   int r = 0;
   if (r) 
      CFNumberGetValue(ref, kCFNumberIntType, &r);
   return r;
}

//--------------------------------------------------------------------------------//
MacHIDManager::MacHIDManager()
{
}

//--------------------------------------------------------------------------------//
MacHIDManager::~MacHIDManager()
{
}

//------------------------------------------------------------------------------------------------------//
void MacHIDManager::initialize()
{
	CFMutableDictionaryRef deviceLookupMap = IOServiceMatching(kIOHIDDeviceKey);
	if(!deviceLookupMap)
		OIS_EXCEPT(E_General, "Could not setup HID device search parameters");

	//Make the search more specific by adding usage flags
	int usage = kHIDUsage_GD_GamePad | kHIDUsage_GD_Joystick,
	    page  = kHIDPage_GenericDesktop;

	CFNumberRef usageRef = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &usage),
				pageRef  = CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &page);

	CFDictionarySetValue(deviceLookupMap, CFSTR(kIOHIDPrimaryUsageKey), usageRef);
	CFDictionarySetValue(deviceLookupMap, CFSTR(kIOHIDPrimaryUsagePageKey), pageRef);

	//IOServiceGetMatchingServices consumes the map so we do not have to release it ourself
	io_iterator_t iterator = 0;
	IOReturn result = IOServiceGetMatchingServices(kIOMasterPortDefault, deviceLookupMap, &iterator);
	if (result == kIOReturnSuccess && iterator)
	{
		io_object_t hidDevice = 0;
		while ((hidDevice = IOIteratorNext(iterator)) !=0)
		{
			//Get the current registry items property map
			CFMutableDictionaryRef propertyMap = 0;
			if (IORegistryEntryCreateCFProperties(hidDevice, &propertyMap, kCFAllocatorDefault, kNilOptions) == KERN_SUCCESS && propertyMap)
			{
				//Go through device to find all needed info
				HidInfo* hid = enumerateDeviceProperties(propertyMap);
				if(hid)
					mDeviceList.push_back(hid);
					
				//todo - we need to hold an open interface so we do not have to enumerate again later
				//should be able to watch for device removals also

				/// Testing opening / closing interface
				//IOCFPlugInInterface **pluginInterface = NULL;
				//SInt32 score = 0;
				//if (IOCreatePlugInInterfaceForService(hidDevice, kIOHIDDeviceUserClientTypeID, kIOCFPlugInInterfaceID, &pluginInterface, &score) == kIOReturnSuccess)
				//{
				//	IOHIDDeviceInterface **interface;
				//	HRESULT pluginResult = (*pluginInterface)->QueryInterface(pluginInterface, CFUUIDGetUUIDBytes(kIOHIDDeviceInterfaceID), (void **)&(interface));
				//	if(pluginResult == S_OK)
				//		cout << "Successfully created plugin interface for device\n";
				//	else
				//		cout << "Not able to create plugin interface\n";

				//	IODestroyPlugInInterface(pluginInterface);

				//	if ((*interface)->open(interface, 0) == KERN_SUCCESS)
				//		cout << "Opened interface.\n";
				//	else
				//		cout << "Failed to open\n";

				//	(*interface)->close(interface);
				//}
				//
			}
		}

		IOObjectRelease(iterator);
	}

	CFRelease(usageRef);
	CFRelease(pageRef);
}

//------------------------------------------------------------------------------------------------------//
HidInfo* MacHIDManager::enumerateDeviceProperties(CFMutableDictionaryRef propertyMap)
{
	HidInfo* info = new HidInfo();
	
	info->type = OISJoyStick;
	
	CFStringRef str = getDictionaryItemAsRef<CFStringRef>(propertyMap, kIOHIDManufacturerKey);
	if (str)
		info->vendor = CFStringGetCStringPtr(str, CFStringGetSystemEncoding());

	str = getDictionaryItemAsRef<CFStringRef>(propertyMap, kIOHIDProductKey);
	if (str)
		info->productKey = CFStringGetCStringPtr(str, CFStringGetSystemEncoding());
		
	info->combinedKey = info->vendor + " " + info->productKey;

	//Go through all items in this device (i.e. buttons, hats, sticks, axes, etc)
	CFArrayRef array = getDictionaryItemAsRef<CFArrayRef>(propertyMap, kIOHIDElementKey);
	if (array)
		for (int i = 0; i < CFArrayGetCount(array); i++)
			parseDeviceProperties(getArrayItemAsRef<CFDictionaryRef>(array, i));

	return info;
}

//------------------------------------------------------------------------------------------------------//
void MacHIDManager::parseDeviceProperties(CFDictionaryRef properties)
{
	if(!properties)
		return;

	CFArrayRef array = getDictionaryItemAsRef<CFArrayRef>(properties, kIOHIDElementKey);
	if (array)
	{
		for (int i = 0; i < CFArrayGetCount(array); i++)
		{
			CFDictionaryRef element = getArrayItemAsRef<CFDictionaryRef>(array, i);
			if (element)
			{
				if(getInt32(getDictionaryItemAsRef<CFNumberRef>(element, kIOHIDElementTypeKey)) == kIOHIDElementTypeCollection) 
				{	//Check if we need to recurse further intoi another collection
					if(getInt32(getDictionaryItemAsRef<CFNumberRef>(element, kIOHIDElementUsagePageKey)) == kHIDPage_GenericDesktop)
						parseDeviceProperties(element);
				}
				else
				{
					switch(getInt32(getDictionaryItemAsRef<CFNumberRef>(element, kIOHIDElementUsagePageKey)))
					{
					case kHIDPage_GenericDesktop:
						switch(getInt32(getDictionaryItemAsRef<CFNumberRef>(element, kIOHIDElementUsageKey)))
						{
						case kHIDUsage_GD_Pointer:
							cout << "\tkHIDUsage_GD_Pointer\n";
							parseDevicePropertiesGroup(element);
							break;
						case kHIDUsage_GD_X:
						case kHIDUsage_GD_Y:
						case kHIDUsage_GD_Z:
						case kHIDUsage_GD_Rx:
						case kHIDUsage_GD_Ry:
						case kHIDUsage_GD_Rz:
							cout << "\tAxis\n";
							break;
						case kHIDUsage_GD_Slider:
						case kHIDUsage_GD_Dial:
						case kHIDUsage_GD_Wheel:
							cout << "\tUnsupported kHIDUsage_GD_Wheel\n";
							break;
						case kHIDUsage_GD_Hatswitch:
							cout << "\tUnsupported - kHIDUsage_GD_Hatswitch\n";
							break;
						}
						break;
					case kHIDPage_Button:
						cout << "\tkHIDPage_Button\n";
						break;
					}
				}
			}
		}
	}
}

//------------------------------------------------------------------------------------------------------//
void MacHIDManager::parseDevicePropertiesGroup(CFDictionaryRef properties)
{
	if(!properties)
		return;

	CFArrayRef array = getDictionaryItemAsRef<CFArrayRef>(properties, kIOHIDElementKey);
	if(array)
	{
		for (int i = 0; i < CFArrayGetCount(array); i++)
		{
			CFDictionaryRef element = getArrayItemAsRef<CFDictionaryRef>(array, i);
			if (element)
			{
				switch(getInt32(getDictionaryItemAsRef<CFNumberRef>(element, kIOHIDElementUsagePageKey)))
				{
				case kHIDPage_GenericDesktop:
					switch(getInt32(getDictionaryItemAsRef<CFNumberRef>(element, kIOHIDElementUsageKey)))
					{
					case kHIDUsage_GD_X:
					case kHIDUsage_GD_Y:
					case kHIDUsage_GD_Z:
					case kHIDUsage_GD_Rx:
					case kHIDUsage_GD_Ry:
					case kHIDUsage_GD_Rz:
						cout << "\t\tAxis\n";
						break;
					case kHIDUsage_GD_Slider:
					case kHIDUsage_GD_Dial:
					case kHIDUsage_GD_Wheel:
						cout << "\tUnsupported - kHIDUsage_GD_Wheel\n";
						break;
					case kHIDUsage_GD_Hatswitch:
						cout << "\tUnsupported - kHIDUsage_GD_Hatswitch\n";
						break;
					}
					break;
				case kHIDPage_Button:
					break;
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------//
DeviceList MacHIDManager::freeDeviceList()
{
	DeviceList ret;
	HidInfoList::iterator it = mDeviceList.begin(), end = mDeviceList.end();
	for(; it != end; ++it)
	{
		if((*it)->inUse == false)
			ret.insert(std::make_pair((*it)->type, (*it)->combinedKey));
	}

	return ret;
}

//--------------------------------------------------------------------------------//
int MacHIDManager::totalDevices(Type iType)
{
	int ret = 0;
	HidInfoList::iterator it = mDeviceList.begin(), end = mDeviceList.end();

	for(; it != end; ++it)
	{
		if((*it)->type == iType)
			ret++;
	}
	
	return ret;
}

//--------------------------------------------------------------------------------//
int MacHIDManager::freeDevices(Type iType)
{
	int ret = 0;
	HidInfoList::iterator it = mDeviceList.begin(), end = mDeviceList.end();

	for(; it != end; ++it)
	{
		if((*it)->inUse == false && (*it)->type == iType)
			ret++;
	}

	return ret;
}

//--------------------------------------------------------------------------------//
bool MacHIDManager::vendorExist(Type iType, const std::string & vendor)
{
	HidInfoList::iterator it = mDeviceList.begin(), end = mDeviceList.end();

	for(; it != end; ++it)
	{
		if((*it)->type == iType && (*it)->combinedKey == vendor)
			return true;
	}
	
	return false;
}

//--------------------------------------------------------------------------------//
Object* MacHIDManager::createObject(InputManager* creator, Type iType, bool bufferMode, 
									  const std::string & vendor)
{
	Object *obj = 0;

	HidInfoList::iterator it = mDeviceList.begin(), end = mDeviceList.end();
	for(; it != end; ++it)
	{
		if((*it)->inUse == false && (*it)->type == iType && (vendor == "" || (*it)->combinedKey == vendor))
		{
			//create device
		}
	}

	if( obj == 0 )
		OIS_EXCEPT(E_InputDeviceNonExistant, "No devices match requested type.");

	return obj;
}

//--------------------------------------------------------------------------------//
void MacHIDManager::destroyObject(Object* obj)
{
	delete obj;
}
