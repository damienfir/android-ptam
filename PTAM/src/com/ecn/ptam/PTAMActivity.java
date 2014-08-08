package com.ecn.ptam;

import android.app.Activity;
import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.hardware.Camera.Parameters;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.FrameLayout;

public class PTAMActivity extends Activity {
	private CameraPreview mPreview;
	private static VideoSource vs;
	private Thread _mainloop;
	private Thread _updateloop;

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		vs = new VideoSource();
	}

	@Override
	public void onResume() {
		super.onResume();
		launchVideo();
	}
	
	public void launchPTAM() {
		_mainloop = new Thread(new Runnable() {
			@Override
			public void run() {
				byte[] im;
				double[] pos;
				initPTAM(vs.getSize());
//				spacePressed();
				while (!Thread.currentThread().isInterrupted()) {
					im = vs.getFrame();
					pos = updatePTAM(im);
					Log.i("PTAM","Position: "+ pos[0] + " " + pos[1]);
				}
			}
		});
		_mainloop.start();
	}

	public static VideoSource getVideoSource() {
		return vs;
	}

	private void launchVideo() {
		// Get an instance of Camera
		Camera mCamera = getVideoSource().getCamera();
		Parameters newParam = mCamera.getParameters();
		newParam.setPreviewFormat(ImageFormat.YV12);
		mCamera.setParameters(newParam);
		// Create our Preview view and set it as the content of our activity.
		mPreview = new CameraPreview(this, mCamera);
		FrameLayout preview = (FrameLayout) findViewById(R.id.camera_preview);
		if (preview == null) {
			preview = new FrameLayout(this);
		}
		preview.addView(mPreview);
	}

	@Override
	public void onPause() {
		super.onPause();
	}

	@Override
	public void onStop() {
		_mainloop.interrupt();
		camera_release();
		super.onStop();
	}
	

	@Override
	public void onDestroy() {
		camera_release();
		super.onDestroy();
	}

	private void camera_release() {
		if (getVideoSource().getCamera() != null) {
			getVideoSource().getCamera().release();
		}
	}
	
	
	public void spacebarPressed(View v) {
		sendEventPTAM("Space");
	}
	
	public native void initPTAM(int[] size);
	public native double[] updatePTAM(byte[] im);
	public native void sendEventPTAM(String s);

	static {
		System.loadLibrary("ptam");
	}
}
