package com.ecn.ptam;


import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.View;
import android.widget.Button;


public class CaptureViewer extends GLSurfaceView implements View.OnClickListener {
	
	private BatchRenderer _renderer;
	private CaptureModel _model;
	
	private enum State {INIT, STEREO_STARTED, STEREO_ENDED, READY, RUNNING};
	private State _state;
	
	public CaptureViewer(Context ctx, VideoSource vs) {
		super(ctx);
		
		_state = State.INIT;
		
		_model = new CaptureModel();
		
		_renderer = new BatchRenderer();
		_renderer.add(new CameraRenderer(vs));
		_renderer.add(new CaptureRenderer(vs, _model));
		setRenderer(_renderer);
	}
	
	
	public void start() {
		// sync
		// flashlight
	}
	
	
	@Override
	public void onClick(View v) {
		
		Button btn = (Button)v;
		
		switch (_state) {
		case INIT:
			PTAM.send("Space");
			btn.setText("Finish stereo init");
			_state = State.STEREO_STARTED;
			break;
		case STEREO_STARTED:
			PTAM.send("Space");
			btn.setText("Store");
			_state = State.STEREO_ENDED;
			break;
		case STEREO_ENDED:
			PTAM.send("Enter");
//			Boolean done = _model.store_corner();
//			if (done) {
//				_state = State.READY;
//				btn.setText("Start");
//			}
			break;
//		case READY:
//			start();
//			btn.setText("Stop");
//			_state = State.RUNNING;
//			break;
//		case RUNNING:
//			btn.setText("Start");
//			_state = State.READY;
		default:
			break;
		}
	}
	
	
	@Override
	public void onPause() {
		
	}
}
