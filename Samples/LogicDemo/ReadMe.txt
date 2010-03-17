-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Nestor Silveira.

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

This is a sandbox demo for logic nodes.
Nodes allows the creation of game logic without having to touch the engine code.
The idea is to have a set of powerful and generic nodes that combined between them 
will allow us to make logic easily...

Following are the explanations for the keyboard and mouse control of this demo.

//////////////// Debug ////////////////////////////////////
<C>
	If pressed then shows physics shape

////////////////Arcball//////////////////////////////////////
<LEFT CTRL> 
	Shows a cursor (arrow) to allow interaction with objects.
	Disables control of Momo.

<LEFT CTRL> + <MOUSE MOVE> 
	Moves the cursor (arrow) to allow interaction with objects.
	
<LEFT CTRL> + <LEFT MOUSE BUTTON> 
	Makes the camera to look at the clicked point in the scene.
	(This will be the center of the arcball)

<LEFT CTRL> + <LEFT MOUSE BUTTON> + <MOUSE WHEEL MOVE> 
	Moves the camera far away from the arcball center or 
	moves the camera closer to the arcball center.

<LEFT CTRL> + <LEFT MOUSE BUTTON> + <MOUSE MOVE> 
	Moves the camera around the arcball perimeter.
	
////////////Drag&Drop/////////////////////////////////////////
	
<LEFT CTRL> + <RIGHT MOUSE BUTTON>
	Selects (pick) and object (to allow drag&drop)
	
	<LEFT CTRL> + <RIGHT MOUSE BUTTON> + <MOUSE MOVE> 
		Move selected object around the scene.
		
	(to drop the selected object just release <RIGHT MOUSE BUTTON>)


///////////////Momo control///////////////////////////////////

By default there is a track node (for the camera) that follows momo orientation and translation.
(Camera will not follow momo when <LEFT CTRL> is pressed)

<MOUSE MOVE> 
	Rotates camera around Momo. 
	
<MOUSE WHEEL MOVE> 	
	Moves the camera far away from Momo or 
	moves the camera closer to Momo.
	
<W>
	Moves momo forward.
	
<S> 
	Moves momo backward.
	
<U>
	Unload momo.
	This will disable automatically all the logic nodes related to momo
	
<L>
	Load momo.
	This will re-enable automatically all the logic nodes related to momo.
	(This returns momo to the original position and orientation.)
	
<R>
	Reload momo.
	(This returns momo to the original position and orientation.)
	
<RIGHT MOUSE BUTTON>
	When pressed then Momo grabs closest object.
	When released then Momo release the grabbed object.
	
<LEFT MOUSE BUTTON>
	If an object is carried by Momo then throw it (with velocity).
	
	
////////////////////////////////////////////////////////////////

Notes:

	The nodes used in this demo are just examples.
	The purpose of them is just to show how easy is to use and create logic nodes.
	