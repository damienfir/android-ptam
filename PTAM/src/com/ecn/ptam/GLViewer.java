package com.ecn.ptam;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.View;
import android.widget.Button;




class GLViewer extends GLSurfaceView implements View.OnClickListener  {
	private VideoSource _vs;
	private BatchRenderer _renderer;
	
	private enum State {INIT, STEREO_STARTED, STEREO_ENDED, BOTTOM_LEFT, TOP_LEFT, GOT_OBJECT};
	private State _state;
	
	public GLViewer(Context context) {
		super(context);
		setDebugFlags(DEBUG_CHECK_GL_ERROR | DEBUG_LOG_GL_CALLS);
//		setEGLConfigChooser(8,8,8,8,16,0);
		_state = State.INIT;
		
		_vs = new VideoSource();
		
		_renderer = new BatchRenderer();
		_renderer.add_renderer(new CameraRenderer(_vs));
		_renderer.add_renderer(new PTAMRenderer(_vs));
		
		setRenderer(_renderer);
	}
	
	@Override
	public void onResume() {
	}
	
	@Override
	public void onPause() {
		_vs.camera_release();
	}

	@Override
	public void onClick(View v) {
		Button btn = (Button)v;
		
		switch (_state) {
		case INIT:
			PTAMWrapper.sendEventPTAM("Space");
			btn.setText("Finish stereo init");
			_state = State.STEREO_STARTED;
			break;
		case STEREO_STARTED:
			PTAMWrapper.sendEventPTAM("Space");
			btn.setText("Bottom left");
			_state = State.STEREO_ENDED;
			break;
		case STEREO_ENDED:
			PTAMWrapper.sendEventPTAM("Enter");
			btn.setText("Top left");
			_state = State.BOTTOM_LEFT;
			break;
		case BOTTOM_LEFT:
			PTAMWrapper.sendEventPTAM("Enter");
			btn.setText("Top right");
			_state = State.TOP_LEFT;
			break;
		case TOP_LEFT:
			PTAMWrapper.sendEventPTAM("Enter");
			btn.setText("Top left");
			_state = State.GOT_OBJECT;
			break;
		case GOT_OBJECT:
			break;
		default:
			break;
		}
	}
}