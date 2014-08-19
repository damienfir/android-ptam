package com.ecn.ptam;

import android.app.Activity;
import android.os.Bundle;

public class PTAMActivity extends Activity {
	private GLViewer _view;
	private VideoSource _vs;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		_view = new GLViewer(this);
		
		_vs = new VideoSource(this);
		_view.setRenderer(new PTAMRenderer(_view, _vs));
		setContentView(_view);
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
		_vs.camera_release();
	}

	
	@Override
	public void onStop() {
		super.onStop();
		_vs.camera_release();
	}


	@Override
	public void onDestroy() {
		super.onDestroy();
	}
}
