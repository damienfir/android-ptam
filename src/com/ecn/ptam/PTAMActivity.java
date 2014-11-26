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

	private VideoSource _videosource;
	private CaptureViewer _capture;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		// hide system UI
		View decorView = getWindow().getDecorView();
		int uiOptions = View.SYSTEM_UI_FLAG_FULLSCREEN;
		decorView.setSystemUiVisibility(uiOptions);
		ActionBar actionBar = getActionBar();
		actionBar.hide();

		_videosource = new VideoSource();
		_capture = new CaptureViewer(this, _videosource);

		Button btn_action = new Button(this);
		btn_action.setOnClickListener(_capture);
		_capture.set_action_button(btn_action);
		_capture.reset_all();

		Button btn_reset_all = new Button(this);
		btn_reset_all.setText("Reset");
		btn_reset_all.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				_capture.reset_all();
			}
		});

		Button btn_reset_zone = new Button(this);
		btn_reset_zone.setText("Reset Zone");
		btn_reset_zone.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				_capture.reset_zone();
			}
		});

		Button btn_reset_tracking = new Button(this);
		btn_reset_tracking.setText("Reset Tracking");
		btn_reset_tracking.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				_capture.reset_tracking();
			}
		});

		LinearLayout llayout = new LinearLayout(this);
		llayout.addView(btn_reset_all);
		llayout.addView(btn_reset_zone);
		llayout.addView(btn_reset_tracking);
		llayout.addView(btn_action);

		FrameLayout flayout = new FrameLayout(this);
		flayout.addView(_capture);
		flayout.addView(llayout);

		setContentView(flayout);
	}

	@Override
	public void onResume() {
		super.onResume();
		_capture.onResume();
	}

	@Override
	public void onPause() {
		_capture.onPause();
		_videosource.camera_release();
		super.onPause();
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