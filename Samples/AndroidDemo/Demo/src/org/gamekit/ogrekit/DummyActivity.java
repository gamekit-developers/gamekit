/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 harkon.kr.

    Contributor(s): 
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

package org.gamekit.ogrekit;

import android.app.NativeActivity;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;

//Used for loading other shared native library for OgreKit.
public class DummyActivity extends Activity 
{	
	static {
	    System.loadLibrary("openal");
	}
	

    @Override
    public void onCreate(Bundle savedInstanceState) {       
    	
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        startActivity(new Intent(getBaseContext(), android.app.NativeActivity.class));
    } 
}