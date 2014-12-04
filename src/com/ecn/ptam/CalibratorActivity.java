package com.ecn.ptam;

import android.app.ActionBar;
import android.app.Activity;
import android.os.Bundle;
import android.view.Gravity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.LinearLayout;

public class CalibratorActivity extends Activity {
	
	VideoSource _videosource;
	CalibratorViewer _view;
	CalibratorActivity _self;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		_self = this;
		
		// hide system UI
		View decorView = getWindow().getDecorView();
		int uiOptions =  View.SYSTEM_UI_FLAG_FULLSCREEN;
		decorView.setSystemUiVisibility(uiOptions);
		ActionBar actionBar = getActionBar();
		actionBar.hide();

		_videosource = new VideoSource();
		_view = new CalibratorViewer(this, _videosource);
		
		Button btn_grab = new Button(this);
		btn_grab.setOnClickListener(_view);
		btn_grab.setText("grab");
		
		Button btn_quit = new Button(this);
		btn_quit.setText("Quit");
		btn_quit.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				_self.finish();
			}
		});
		
		Button btn_reset = new Button(this);
		btn_reset.setText("reset");
		btn_reset.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				_view.reset();
			}
		});
		
		Button btn_optimize = new Button(this);
		btn_optimize.setText("optimize");
		btn_optimize.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				_view.optimize();
			}
		});

		LinearLayout llayout = new LinearLayout(this);
		llayout.setGravity(Gravity.BOTTOM);
		llayout.setHorizontalGravity(Gravity.END);
		llayout.addView(btn_quit);
		llayout.addView(btn_grab);
		llayout.addView(btn_reset);
		llayout.addView(btn_optimize);
		
		FrameLayout flayout = new FrameLayout(this);
		flayout.addView(_view);
		flayout.addView(llayout);

		setContentView(flayout);
	}
}