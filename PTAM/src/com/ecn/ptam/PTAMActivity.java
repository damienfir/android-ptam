package com.ecn.ptam;

import android.app.ActionBar;
import android.app.Activity;
import android.os.Bundle;
import android.view.Gravity;
import android.view.View;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.LinearLayout;

public class PTAMActivity extends Activity {
	private GLViewer _view;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		// hide system UI
		View decorView = getWindow().getDecorView();
		int uiOptions = View.SYSTEM_UI_FLAG_FULLSCREEN;
		decorView.setSystemUiVisibility(uiOptions);
		ActionBar actionBar = getActionBar();
		actionBar.hide();

		
		_view = new GLViewer(this);
		
//		Button btn_reset = new Button(this);
//		btn_reset.setText("Reset");
//		btn_reset.setOnClickListener(_view);

		Button btn_action = new Button(this);
		btn_action.setText("Start stereo init");
		btn_action.setOnClickListener(_view);

		LinearLayout layout = new LinearLayout(this);
//		layout.addView(btn_reset);
		layout.addView(btn_action);
		
		FrameLayout fl = new FrameLayout(this);
		fl.setForegroundGravity(Gravity.BOTTOM | Gravity.START);
		fl.addView(_view);
		fl.addView(layout);
		
		setContentView(fl);
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