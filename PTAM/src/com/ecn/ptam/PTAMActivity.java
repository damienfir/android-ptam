package com.ecn.ptam;

import android.app.Activity;
import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.hardware.Camera.Parameters;
import android.widget.FrameLayout;
import android.os.Bundle;

public class PTAMActivity extends Activity {
	private CameraPreview mPreview;
	private static VideoSource vs;

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
		new Thread(new Runnable() {
			@Override
			public void run() {
				stringFromJNI();
			}
		}).start();
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
		if (getVideoSource().getCamera() != null) {
			getVideoSource().getCamera().release();
		}
		super.onPause();
	}

	@Override
	public void onStop() {
		if (getVideoSource().getCamera() != null) {
			getVideoSource().getCamera().release();
		}
		super.onStop();
	}

	@Override
	public void onDestroy() {
		if (getVideoSource().getCamera() != null) {
			getVideoSource().getCamera().release();
		}
		super.onDestroy();
	}

	/*
	 * A native method that is implemented by the 'hello-ptam' native library,
	 * which is packaged with this application.
	 */
	public native void stringFromJNI();

	/*
	 * this is used to load the 'hello-ptam' library on application startup. The
	 * library has already been unpacked into
	 * /data/data/com.example.HelloJni/lib/libhello-jni.so at installation time
	 * by the package manager.
	 */
	static {
		System.loadLibrary("testptam");
	}
}
