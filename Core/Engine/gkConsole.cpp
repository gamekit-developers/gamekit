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
#if 1 // ADD Proper GUI

#include "OgreOverlayManager.h"
#include "OgreOverlayElement.h"
#include "OgreTextAreaOverlayElement.h"

#include "gkConsole.h"
#include "gkLogger.h"
#include "gkWindowSystem.h"



using namespace Ogre;

static gkConsole *gConsole= 0;

// ----------------------------------------------------------------------------
gkConsole::gkConsole() :
		mInitialized(false), mLocked(false), mShow(false),
		mText(""), mOver(0), mCharHeight(60)
{
}

// ----------------------------------------------------------------------------
gkConsole::~gkConsole()
{
	finalize();
}

// ----------------------------------------------------------------------------
void gkConsole::initialize()
{
	if (mInitialized)
		return;

	try   /* ok if not present */
	{

		OverlayManager &over_mgr= OverlayManager::getSingleton();

		/* the main display */
		mOver= over_mgr.getByName("Blender/ScreenConsole");
		mElement= over_mgr.getOverlayElement("Blender/ScreenConsole/Container/TextArea");

		/* sync display sizes */

		gkRectangle dims= gkWindowSystem::getSingleton().getMouse().area;

		mElement->setLeft(0);
		mElement->setTop(2);
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
	gConsole= this;
}

// ----------------------------------------------------------------------------
void gkConsole::finalize()
{
	if (!mInitialized)
		return;

	mInitialized= false;
	gConsole= 0;
}

// ----------------------------------------------------------------------------
void gkConsole::show(bool doit)
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
void pop(StringVector &vec, size_t index)
{
	size_t idx= 0;
	for (StringVector::iterator it= vec.begin(); it != vec.end(); ++it, idx++)
	{
		if (idx == index)
		{
			vec.erase(vec.begin(), it);
			return;
		}
	}
}

// ----------------------------------------------------------------------------
void gkConsole::pushText(const String &text)
{
	StringVector vec= StringUtil::split(text, "\n\r");

	if (mLines.size() > ((mElement->getHeight() *.876543) * mCharHeight))
		pop(mLines, vec.size());

	for (size_t i= 0; i < vec.size(); i++)
		mLines.push_back(vec[i]);

	mText.clear();

	for (size_t i= 0; i < mLines.size(); i++)
		mText += mLines[i] + "\n";

	_getClipped();
}

// ----------------------------------------------------------------------------
void gkConsole::_getClipped()
{
	if (!mLocked && mElement != 0)
	{
		if (!mShow) show(true);

		mElement->setCaption(mText);
	}
}


// ----------------------------------------------------------------------------
void gkConsole::printTo(const String &buf)
{
	if (gConsole != 0)
	{
		if (!gConsole->isLocked())
			gConsole->pushText(buf);
	}
}

#endif
