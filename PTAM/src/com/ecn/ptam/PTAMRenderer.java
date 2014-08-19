package com.ecn.ptam;

import android.util.Log;


public class PTAMRenderer extends GLRenderer {
	
	private VideoSource _vs;
	
	public PTAMRenderer(VideoSource vs) {
		_vs = vs;
	}
	
	@Override
	public void draw() {
		byte[] frame = _vs.getFrame();
		if (frame != null) {
			double pos[] = PTAMWrapper.updatePTAM(frame);
			Log.i("PTAM", "position: " + pos[0] + " " + pos[1]);			
		}
	}	
	
	@Override
	public void init() {
		PTAMWrapper.initPTAM(_vs.getSize());
	}
}