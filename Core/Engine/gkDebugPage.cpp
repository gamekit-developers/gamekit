/*
-------------------------------------------------------------------------------
	This file is part of the Ogre GameKit port.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#if 0 // ADD Proper GUI

#include "OgreOverlayManager.h"
#include "OgreOverlayElement.h"
#include "OgreTextAreaOverlayElement.h"

#include "gkDebugPage.h"
#include "gkWindowSystem.h"
#include "gkVariable.h"
#include "gkLogger.h"


using namespace Ogre;



// ----------------------------------------------------------------------------
gkDebugPage::gkDebugPage() :
		mInitialized(false), mShow(false),
		mOver(0), mElement(0), mCharHeight(60)
{
}

// ----------------------------------------------------------------------------
gkDebugPage::~gkDebugPage()
{
	clearProps();
}

// ----------------------------------------------------------------------------
void gkDebugPage::clearProps()
{
	mProps.clear();
	show(false);
}

// ----------------------------------------------------------------------------
void gkDebugPage::initialize()
{
	if (mInitialized)
		return;

	try   /* ok if not present */
	{

		OverlayManager &over_mgr= OverlayManager::getSingleton();

		/* the main display */
		mOver= over_mgr.getByName("Blender/DebugProps");
		mElement= over_mgr.getOverlayElement("Blender/DebugProps/Container/TextArea");

		/* sync display sizes */

		gkRectangle dims= gkWindowSystem::getSingleton().getMouse().area;
		mElement->setDimensions(dims.width, dims.height - 2);

		if (mElement->getTypeName() == "TextArea")
			mCharHeight= 1.0 / ((Ogre::TextAreaOverlayElement*)mElement)->getCharHeight();
	}

	catch (Ogre::Exception &e)
	{
		gkPrint(e.getDescription());
		return;
	}

	mInitialized= true;
}

// ----------------------------------------------------------------------------
void gkDebugPage::addVariable(gkVariable *prop)
{
	show(true);
	mProps.push_back(prop);
}

// ----------------------------------------------------------------------------
bool gkDebugPage::hasVariable(gkVariable *prop)
{
	return  std::find(mProps.begin(), mProps.end(), prop) != mProps.end();
}

// ----------------------------------------------------------------------------
void gkDebugPage::removeVariable(gkVariable *prop)
{
	PropertyList::iterator it= std::find(mProps.begin(), mProps.end(), prop);
	if (it != mProps.end())
		mProps.erase(it);
}

// ----------------------------------------------------------------------------
void gkDebugPage::show(bool doit)
{
	if (mOver != 0 && mShow != doit)
	{
		mShow= doit;
		if (mShow)
			mOver->show();
		else
			mOver->hide();
	}
}

// ----------------------------------------------------------------------------
void gkDebugPage::draw()
{
	if (!mOver || !mElement)	return;
	if (mProps.empty())		 return;


	PropertyIterator iter= PropertyIterator(mProps.begin(), mProps.end());

	String text= "";
	while (iter.hasMoreElements())
	{

		gkVariable *prop= iter.getNext();
		if (prop->isDebug())
			text += prop->getName() + ": " + prop->getValueString() + '\n';
	}

	if (!text.empty())
		mElement->setCaption(text);
}

#endif
