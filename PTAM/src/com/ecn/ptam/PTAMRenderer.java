package com.ecn.ptam;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLES11;
import android.opengl.GLSurfaceView;
import android.util.Log;


public class PTAMRenderer implements GLSurfaceView.Renderer {
	
	private VideoSource _vs;
	
	public PTAMRenderer(VideoSource vs) {
		_vs = vs;
	}

	
	@Override
	public void onDrawFrame(GL10 arg0) {
		GLES11.glClear(GLES11.GL_COLOR_BUFFER_BIT);
		Log.i("ptam", "onDrawFrame");
		byte[] frame = _vs.getFrame();
		double[] pos = PTAMWrapper.updatePTAM(frame);
		Log.i("ptam", "position: " + pos[0] + " " + pos[1]);
	}
	
	
	@Override
	public void onSurfaceChanged(GL10 arg0, int width, int height) {
		Log.i("ptam", "onSurfaceChanged");
		GLES11.glViewport(0, 0, width, height);
	}
	
	
	@Override
	public void onSurfaceCreated(GL10 arg0, EGLConfig config) {
		Log.i("ptam", "onSurfaceCreated");
		GLES11.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		PTAMWrapper.initPTAM(_vs.getSize());
	}
}