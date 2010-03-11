This is a sandbox demo for logic nodes.
Nodes allows the creation of game logic without having to touch the engine code.
The idea is to have a set of powerful and generic nodes that combined between them 
will allow us to make logic easily...

Following are the explanations for the keyboard and mouse control.

////////////////Arcball//////////////////////////////////////
<LEFT CTRL> 
	You enter in interaction mode.
	Shows a cursor (arrow) to allow interaction with objects.
	Disables control of Momo.

<LEFT CTRL> + <MOUSE MOVE> 
	Moves the cursor (arrow) to allow interaction with objects.
	
<LEFT CTRL> + <LEFT MOUSE BUTTON> 
	Makes the camera to look at the clicked point in the scene.
	(This will be the center of the arcball)

<LEFT CTRL> + <MOUSE WHEEL MOVE> 
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
	Rotates momo to look around (this functionality is disabled by logic when <LEFT CTRL> is pressed). 
	
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
	
////////////////////////////////////////////////////////////////

Notes:

	The nodes used in this demo are just examples.
	The purpose of them is just to show how easy is to use and create logic nodes.
	Arcball has to be improved in order to smooth movement around 90/-90.
	