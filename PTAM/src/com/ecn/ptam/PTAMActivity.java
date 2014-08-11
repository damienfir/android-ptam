package com.ecn.ptam;

import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;
import android.widget.LinearLayout;

public class PTAMActivity extends Activity {
	private GLSurfaceView _view;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		
		LinearLayout layout = new LinearLayout(this);
		_view = new Viewer(this);
		
		VideoSource vs = new VideoSource(this);
		_view.setRenderer(new PTAMRenderer(vs));
		
		layout.setOrientation(LinearLayout.VERTICAL);
		layout.addView(_view, new LinearLayout.LayoutParams(800,500));
		layout.addView(vs.get_view(), new LinearLayout.LayoutParams(800,500));
		setContentView(layout);
	}

	@Override
	public void onResume() {
		super.onResume();
		_view.onResume();
	}
	
	
	@Override
	public void onPause() {
		super.onPause();
		_view.onPause();
	}

	
	@Override
	public void onStop() {
		super.onStop();
	}
	

	@Override
	public void onDestroy() {
		super.onDestroy();
	}
	
	
	class Viewer extends GLSurfaceView {

		public Viewer(Context context) {
			super(context);
//			setEGLContextClientVersion(2);
		}
		
		@Override
		public boolean onTouchEvent(MotionEvent e) {
			PTAMWrapper.sendEventPTAM("Space");
			return true;
		}
	}
	
//	public void spacebarPressed(View v) {
//		_ptam.sendEventPTAM("Space");
//	}
}
