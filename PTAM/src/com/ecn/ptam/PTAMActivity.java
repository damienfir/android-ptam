package com.ecn.ptam;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

public class PTAMActivity extends Activity {
	private GLViewer _view;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		_view = new GLViewer(this);
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
	}

	
	@Override
	public void onStop() {
		super.onStop();
	}


	@Override
	public void onDestroy() {
		super.onDestroy();
	}
}