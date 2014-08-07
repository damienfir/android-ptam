package com.ecn.ptam;

import android.app.Activity;
import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.hardware.Camera.Parameters;
import android.util.Log;
import android.widget.FrameLayout;
import android.os.Bundle;

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
				startPTAM();
			}
		});
		_mainloop.start();
		
//		_updateloop = new Thread(new Runnable() {
//			@Override
//			public void run() {
//				for (int i = 0; i < 1000; ++i) {
//					double pose[] = get_pose();
//					Log.i("PTAM", "position: " + pose[0] + " " + pose[1]);
//				}
//			}
//		});
//		_updateloop.start();
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
//		_updateloop.interrupt();
		stopPTAM();
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
	
	public native void startPTAM();
	public native void stopPTAM();
	public native double[] get_pose();

	static {
		System.loadLibrary("ptam");
	}
}
