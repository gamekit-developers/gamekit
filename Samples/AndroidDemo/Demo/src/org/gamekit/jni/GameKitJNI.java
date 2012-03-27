package org.gamekit.jni;

import org.gamekit.jni.Main.GameKitListener;

/**
 * 
 * @author dertom
 *
 */
public class GameKitJNI implements GameKitListener{



	public GameKitJNI() {
		
	}
	
	@Override
	public void onMessage(int type, int value) {
		// not used yet
		System.out.println("Message from gk:"+type+" - "+value);
	}

	/**
	 * Here you can catch every message that is fired inside gamekits
	 * gkMessageManager.
	 */
	@Override
	public void onMessage(String from, String to, String subject, String body) {
		System.out.println("Message from gk:"+subject);
		if (subject.equalsIgnoreCase("vibrate")){
			System.out.println("RING RING! Got 'virbate'-msg!  Do something!");
		}
	}
	
	

}
