/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 zcube(JiSeop Moon)

    Contributor(s): harkon.kr
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

package org.gamekit.jni;

import com.nvidia.devtech.NvGLES2Activity;
import com.nvidia.devtech.NvGLESActivity;


import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Window;
import android.view.WindowManager;
import android.widget.TextView;

public class Main
	//extends NvGLESActivity {
	extends NvGLES2Activity {
  /** Called when the activity is first created. */
  @Override
  public void onCreate(Bundle savedInstanceState) {
	  initArg = "/sdcard/momo_ogre_i.blend";
	  
	  requestWindowFeature(Window.FEATURE_NO_TITLE);  
	  getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,   
			  WindowManager.LayoutParams.FLAG_FULLSCREEN);  
      super.onCreate(savedInstanceState);
  			setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
	
	/*  
  	  super.onCreate(savedInstanceState);
      TextView  tv = new TextView(this);
      tv.setText( "c"); 
      setContentView(tv);
      */
  }

  @Override
	public void onWindowFocusChanged(boolean hasFocus) {
		int screenHeight = this.getWindowManager().getDefaultDisplay().getHeight();
		int viewHeight = surfaceView.getHeight();
		
		// Use the difference as the cursor offset
		setOffsets(0, viewHeight - screenHeight);
		
		super.onWindowFocusChanged(hasFocus);
	}

  // Native

	@Override
	public native boolean render(int drawWidth, int drawHeight, boolean forceRedraw);

	@Override
	public native void cleanup();

	@Override
	public native boolean init(String initArg);

	@Override
	public native boolean inputEvent(int action, float x, float y, MotionEvent event);

	@Override
	public native boolean keyEvent(int action, int unicodeChar, int keyCode, KeyEvent event);
	
	public native void setOffsets(int x, int y);
	
	static
	{	
		System.loadLibrary("ogrekit");
	}
}

