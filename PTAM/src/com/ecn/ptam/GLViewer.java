package com.ecn.ptam;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.MotionEvent;

class GLViewer extends GLSurfaceView  {
	private VideoSource _vs;
	private BatchRenderer _renderer;

	public GLViewer(Context context) {
		super(context);
		setDebugFlags(DEBUG_CHECK_GL_ERROR | DEBUG_LOG_GL_CALLS);
		
		_vs = new VideoSource();
		
		_renderer = new BatchRenderer();
		_renderer.add_renderer(new CameraRenderer(_vs));
		_renderer.add_renderer(new PTAMRenderer(_vs));
		
		setRenderer(_renderer);
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent e) {
		switch (e.getAction()) {
		case MotionEvent.ACTION_DOWN:
			PTAMWrapper.sendEventPTAM("Space");
		}
		return true;
	}
	
	
	@Override
	public void onResume() {
	}
	
	
	@Override
	public void onPause() {
		_vs.camera_release();
	}
}