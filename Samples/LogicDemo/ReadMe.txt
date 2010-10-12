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
	If pressed then shows physics shape & pathfinding.

////////////////Arcball//////////////////////////////////////
<TAB> 
	Shows a cursor (arrow) to allow interaction with objects.
	Disables Momo's camera.

<TAB> + <MOUSE MOVE> 
	Moves the cursor (arrow) to allow interaction with objects.
	
<TAB> + <LEFT MOUSE BUTTON> 
	Makes the camera to look at the clicked point in the scene.
	(This will be the center of the arcball)

<TAB> + <MOUSE WHEEL MOVE> 
	Moves the camera far away from the arcball center or 
	moves the camera closer to the arcball center.

<TAB> + <LEFT MOUSE BUTTON> + <MOUSE MOVE> 
	Moves the camera around the arcball perimeter.
	
////////////Drag&Drop/////////////////////////////////////////
	
<TAB> + <RIGHT MOUSE BUTTON>
	Selects (pick) and object (to allow drag&drop)
	
	<TAB> + <RIGHT MOUSE BUTTON> + <MOUSE MOVE> 
		Move selected object around the scene.
		
	(to drop the selected object just release <RIGHT MOUSE BUTTON>)


///////////////Momo control///////////////////////////////////


<MOUSE MOVE> 
	Rotates camera around Momo. 
	
<MOUSE WHEEL MOVE> 	
	Moves the camera far away from Momo or 
	moves the camera closer to Momo.
	
<W>
	Moves momo forward (First Walk then Run...)
	
<LEFT SHIFT> + <W>
	Moves momo forward (Run Faster and create dust trail [particle FX])
	
<LEFT SHIFT> + <LEFT MOUSE BUTTON>
	 Momo will move to the clicked point in the scene.
	
<S> 
	Moves momo backward.
	
<LEFT MOUSE BUTTON>
	Kick. (try to kick a rat)
	If an object is carried by Momo then throw it (with velocity).
	
<RIGHT MOUSE BUTTON>
	When pressed Momo grabs closest object (cubes only).
	When released Momo release the grabbed object.
	
<SPACE>
	Momo jumps
	Keep <SPACE> pressed after jump to glide.
	Keep <SPACE> + <W> to fly.
	

	
////////////////////////////////////////////////////////////////

Notes:

	The nodes used in this demo are just examples.
	The purpose of them is just to show how easy is to use and create logic nodes.
	