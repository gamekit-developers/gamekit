/*
 * This source file is part of libRocket, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://www.librocket.com
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */
/*  RocketApplication.h is modifed for OgreKit by harkon.kr.
	http://gamekit.googlecode.com/
*/

#ifndef ROCKETRENDERLISTENER_H
#define ROCKETRENDERLISTENER_H

#include <Rocket/Core/String.h>
#include <Ogre.h>

#include "RocketEventListener.h"

namespace Rocket {
namespace Core {

class Context;

}
}

class SystemInterfaceOgre3D;
class RenderInterfaceOgre3D;


/**
	@author Peter Curry
 */

class RocketRenderListener : public Ogre::RenderQueueListener
{
public:
	RocketRenderListener(Ogre::RenderWindow* window, Ogre::SceneManager* smgr, Rocket::Core::Context* context);
	virtual ~RocketRenderListener();


	/// Called from Ogre before a queue group is rendered.
	virtual void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation);
	/// Called from Ogre after a queue group is rendered.
    virtual void renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& repeatThisInvocation);

private:
	// Configures Ogre's rendering system for rendering Rocket.
	void ConfigureRenderSystem();
	// Builds an OpenGL-style orthographic projection matrix.
	void BuildProjectionMatrix(Ogre::Matrix4& matrix);

	Rocket::Core::Context* m_context;
	Ogre::SceneManager* m_sceneMgr;
	Ogre::RenderWindow* m_window;
};

#endif //ROCKETRENDERLISTENER_H
