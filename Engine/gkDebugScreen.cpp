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
#include "gkDebugScreen.h"
#include "gkLogger.h"
#include "gkWindowSystem.h"
#include "gkEngine.h"

#include "OgreOverlayManager.h"
#include "OgreOverlayElement.h"
#include "OgreOverlayContainer.h"
#include "OgreFont.h"
#include "OgreFontManager.h"
#include "OgreTextAreaOverlayElement.h"

//#include "User/gkmonofont.inl"
#include "User/gkfont.inl"

static gkDebugScreen* gConsole = 0;
#define SCREEN_SIZE 14



class gkBuiltinFont : public Ogre::ManualResourceLoader
{
public:

	virtual ~gkBuiltinFont() {}

	void loadResource(Ogre::Resource* res)
	{
		Ogre::Font* font = (Ogre::Font*)res;

		font->setTrueTypeSize(SCREEN_SIZE);
		font->setTrueTypeResolution(SCREEN_SIZE * 4);

		Ogre::DataStreamPtr memStream(OGRE_NEW Ogre::MemoryDataStream((void*)gkBuiltinFontPtr, gkBuiltinFontSize));
		font->loadManualResource(memStream);
	}

};



gkDebugScreen::gkDebugScreen() :
	m_isInit(false), m_isLocked(false), m_isShown(false),
	m_txtBuffer(""), m_over(0), m_ele(0), m_font(0), m_lineBuffer(128)
{
}


gkDebugScreen::~gkDebugScreen()
{
	finalize();
}



void gkDebugScreen::initialize()
{
	if (m_isInit)
		return;

	try
	{

		m_font = new gkBuiltinFont;
		Ogre::FontPtr fp = Ogre::FontManager::getSingleton().create("<gkBuiltin/Font>", GK_BUILTIN_GROUP, true, m_font);
		fp->load();

		Ogre::OverlayManager& mgr = Ogre::OverlayManager::getSingleton();
		m_over  = mgr.create("<gkBuiltin/gkDebugScreen>");
		m_ele   = mgr.createOverlayElement("TextArea", "<gkBuiltin/gkDebugScreen/TextArea>");

		Ogre::OverlayContainer* cont = (Ogre::OverlayContainer*)mgr.createOverlayElement("BorderPanel", "<gkBuiltin/gkDebugScreen/Containter>");
		cont->setMetricsMode(Ogre::GMM_PIXELS);
		cont->setVerticalAlignment(Ogre::GVA_TOP);

		const gkVector2& dims = gkWindowSystem::getSingleton().getMouse()->winsize;

		m_ele->setMetricsMode(Ogre::GMM_PIXELS);
		m_ele->setVerticalAlignment(Ogre::GVA_TOP);
		m_ele->setPosition(0, 0);
		m_ele->setDimensions(dims.x, dims.y);

		Ogre::TextAreaOverlayElement* textArea = static_cast<Ogre::TextAreaOverlayElement*>(m_ele);
		textArea->setFontName("<gkBuiltin/Font>");
		textArea->setCharHeight(SCREEN_SIZE);
		textArea->setColour(Ogre::ColourValue::White);

		m_over->setZOrder(500);
		cont->addChild(m_ele);
		m_over->add2D(cont);
	}
	catch (Ogre::Exception& e)
	{
		gkPrintf("%s", e.getDescription().c_str());
		return;
	}


	m_isInit = true;
	gConsole = this;
}



void gkDebugScreen::finalize()
{
	if (!m_isInit)
		return;

	clear();
	delete m_font;
	m_isInit = false;
	gConsole = 0;
}



void gkDebugScreen::show(bool doit)
{
	if (m_over != 0 && m_isShown != doit)
	{
		m_isShown = doit;
		if (m_isShown)
			m_over->show();
		else
			m_over->hide();
	}
}


void gkDebugScreen::clear()
{
	if (!m_lines.empty())
	{
		m_txtBuffer.resize(0);

		utListIterator<ScreenBufferList> iter(m_lines);
		while (iter.hasMoreElements())
			m_lineBuffer.dealloc(iter.getNext());

		m_lines.clear();
	}
}




void gkDebugScreen::pushText(const gkString& text)
{
	utStringUtils::split(m_splitBuf, text, "\n\r");
	Ogre::TextAreaOverlayElement* textArea = static_cast<Ogre::TextAreaOverlayElement*>(m_ele);

	const gkVector2& winSize = gkWindowSystem::getSingleton().getMouse()->winsize;



	gkScalar che = textArea->getCharHeight();

	for (UTsize i = 0; i < m_splitBuf.size(); ++i)
	{
		ScreenBufferItem* item  = m_lineBuffer.alloc();
		item->clear();
		item->m_buf = m_splitBuf.at(i);

		m_lines.push_back(item);
		gkScalar size = gkScalar(m_lines.size()) * che;
		if (size > winSize.y)
			m_lineBuffer.dealloc(m_lines.pop_front());
	}


	m_splitBuf.clear(true);

	if (!m_lines.empty())
	{
		// write lines
		m_txtBuffer.resize(0);


		ScreenBufferItem* node = m_lines.begin();
		while (node)
		{
			m_txtBuffer += node->m_buf + "\n";
			node = node->getNext();
		}
	}


	_getClipped();
}

void gkDebugScreen::_getClipped()
{
	if (!m_isLocked && m_ele != 0)
	{
		if (!m_isShown) show(true);

		m_ele->setCaption(m_txtBuffer);
	}
}

void gkDebugScreen::printTo(const gkString& buf)
{
	if (gConsole != 0)
	{
		if (!gConsole->isLocked())
			gConsole->pushText(buf);
	}

}

void dsPrintf(const char* fmt, ...)
{
	if (gConsole == 0)
		return;

	static char buf[513];

	va_list lst;
	va_start(lst, fmt);
	int size = vsnprintf(buf, 512, fmt, lst);
	va_end(lst);


	if (size < 0)
	{
		buf[512] = 0;
		size = 512;
	}

	if (size > 0)
	{
		buf[size] = 0;
		gConsole->printTo(buf);
	}
}
