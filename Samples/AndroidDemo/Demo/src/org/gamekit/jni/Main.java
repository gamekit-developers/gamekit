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

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.Field;
import java.util.LinkedList;
import java.util.List;

import android.content.pm.ActivityInfo;
import android.content.res.AssetManager;
import android.hardware.Sensor;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.Window;
import android.view.WindowManager;

import com.nvidia.devtech.NvGLES2Activity;

public class Main
	//extends NvGLESActivity {
	extends NvGLES2Activity {
	
	private static List<GameKitListener> listener = new LinkedList<Main.GameKitListener>();
	
	
  /** Called when the activity is first created. */
  @Override
  public void onCreate(Bundle savedInstanceState) {
	  initArg = "/sdcard/gamekit/gk_android.blend";
	  // copy all files from the assets folder to the specified folder (in this case /sdcard/gamekit
	  CopyAssets("gamekit");
	  
	  requestWindowFeature(Window.FEATURE_NO_TITLE);  
	  getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,   
			  WindowManager.LayoutParams.FLAG_FULLSCREEN);  
      super.onCreate(savedInstanceState);
  			setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
  			

  }

  @Override
public void onBackPressed() {
		super.onBackPressed();

//		System.out.println("AND: BACK");
		
		sendMessage("android", "", "back", "");
		// kill the application after 1sec. 
		handler.postDelayed(new Runnable()
		{
			public void run() {
				System.exit(0);
			}
		},1000);
}
  
  private void CopyAssets(String folderName) {
	    AssetManager assetManager = getAssets();
	    String[] files = null; 
	    try {
	        files = assetManager.list("");
	    } catch (IOException e) {
	        Log.e("tag", e.getMessage());
	    }
	    File f = new File("/sdcard/"+folderName);
	    if (f.isFile()) {
	    	throw new RuntimeException("Folder /sdcard/"+folderName+" is a file");
	    } else if (!f.isDirectory()){
	    	f.mkdir();
	    }
	    
	    for(String filename : files) {
	        InputStream in = null;
	        OutputStream out = null;
	        try {
	          in = assetManager.open(filename);
	          
	          f = new File("/sdcard/"+folderName+"/" + filename);
	          if (f.exists()){
	        	  Log.i("gamekit", "fileexists!");
//	        	  continue;
	          }
	          
	          out = new FileOutputStream("/sdcard/"+folderName+"/" + filename);
	          copyFile(in, out);
	          in.close();
	          in = null;
	          out.flush();
	          out.close();
	          out = null;
	        } catch(Exception e) {
	            Log.e("tag", e.getMessage());
	        }       
	    }
	}
	private void copyFile(InputStream in, OutputStream out) throws IOException {
	    byte[] buffer = new byte[1024];
	    int read;
	    while((read = in.read(buffer)) != -1){
	      out.write(buffer, 0, read);
	    }
	}
  

@Override
	public void onWindowFocusChanged(boolean hasFocus) {
		int screenHeight = this.getWindowManager().getDefaultDisplay().getHeight();
		int viewHeight = surfaceView.getHeight();
		
		// Use the difference as the cursor offset
		setOffsets(0, viewHeight - screenHeight);
		
		super.onWindowFocusChanged(hasFocus);
	}


	private static final float accel_threshold = 0.5f;
	private static final float input_frame = 0.05f;
	private float inputFrameTimer  = input_frame;
	private static final StringBuffer stb = new StringBuffer();
	private float[] gravity = new float[3];
	private float[] linear_acceleration = new float[3];
	
	private float[] linearAccel(float x,float y,float z) {

        final float alpha = 0.8f;

        gravity[0] = alpha * gravity[0] + (1 - alpha) * x;
        gravity[1] = alpha * gravity[1] + (1 - alpha) * y;
        gravity[2] = alpha * gravity[2] + (1 - alpha) * z;

        
        linear_acceleration[0] = x - gravity[0];
        linear_acceleration[1] = y - gravity[1];
        linear_acceleration[2] = z - gravity[2];
        return linear_acceleration;
	}
	
	@Override
	public boolean sensorEvent(int sensorType, float x, float y,
			float z) {
			
			if (sensorType==Sensor.TYPE_ACCELEROMETER){
				sendSensor(sensorType, x, y, z);

			}	
			
			return true;
	}

	 @Override
	   public boolean onKeyUp(int keyCode, KeyEvent event) {
	    if(keyCode == KeyEvent.KEYCODE_MENU) { 
	    	// send menu-message to gkMessageManager. e.g. use message-sensor with "menu" as subject to react on this
	    	sendMessage("android", "", "menu", "");
	    	return true;
	    } else if (keyCode == KeyEvent.KEYCODE_BACK){
	    	// send menu-message to gkMessageManager. e.g. use message-sensor with "back" as subject to react on this
	    	// for now this will still kill the game. but you can just repalce onBackPressed() and call it whenever you
	    	// want. (see GameKitJNI.java for for to handle messages that comes from gamekit)
	    	sendMessage("android", "", "back", "");
	    	onBackPressed();
	    	return true;
	    }
	    return false;
	 }
	
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
	
	// needs to be implemented yet and called at onPause()-Event
	public native void pauseSound();
	// needs to be implemented yet and called at onResume()-Event
	public native void restartSound();
	
	public native void sendSensor(int sensorType,float x,float y, float z);
	public native void sendMessage(String from,String to,String topic,String body);
	
	static
	{	
		// Uncomment if you use OpenAL
//		 System.loadLibrary("OpenAL");
		System.loadLibrary("ogrekit");
		addListener(new GameKitJNI());
	}
	
	public static void fireTypeIntMessage(int type,int value){
		for (int i=0;i<listener.size();i++){
			listener.get(i).onMessage(type, value);
		}
	}
	
	public static void fireStringMessage(String from,String to,String subject,String body){
		for (int i=0;i<listener.size();i++){
			listener.get(i).onMessage(from,to,subject,body);
		}
	}
	
	public static void addListener(GameKitListener gkListener){
		listener.add(gkListener);
	}
	
	public static interface GameKitListener{
		void onMessage(int type,int value); 
		void onMessage(String from,String to,String subject, String body);
	}
}

