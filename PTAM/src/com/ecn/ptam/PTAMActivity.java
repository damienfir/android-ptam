package com.ecn.ptam;

import android.app.ActionBar;
import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.Gravity;
import android.view.View;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.LinearLayout;

public class PTAMActivity extends Activity {

	private VideoSource _vs;
	private CaptureViewer _viewer;
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		_vs = new VideoSource();
		_viewer = new CaptureViewer(this, _vs);

		// hide system UI
		View decorView = getWindow().getDecorView();
		int uiOptions = View.SYSTEM_UI_FLAG_FULLSCREEN;
		decorView.setSystemUiVisibility(uiOptions);
		ActionBar actionBar = getActionBar();
		actionBar.hide();
		
//		Button btn_reset = new Button(this);
//		btn_reset.setText("Reset");
//		btn_reset.setOnClickListener(_view);

		Button btn_action = new Button(this);
		btn_action.setText("Start stereo init");
		btn_action.setOnClickListener(_viewer);
		
		LinearLayout layout = new LinearLayout(this);
//		layout.addView(btn_reset);
		layout.addView(btn_action);
		
		FrameLayout fl = new FrameLayout(this);
		fl.setForegroundGravity(Gravity.BOTTOM | Gravity.START);
		fl.addView(_viewer);
		fl.addView(layout);
		
		setContentView(fl);
	}
	
	
	@Override
	public void onResume() {
		super.onResume();
		_viewer.onResume();
	}


	@Override
	public void onPause() {
		super.onPause();
		_viewer.onPause();
		_vs.camera_release();
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