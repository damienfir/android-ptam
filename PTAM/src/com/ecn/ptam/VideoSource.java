package com.ecn.ptam;

import java.io.IOException;

import android.content.Context;
import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.hardware.Camera.Parameters;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class VideoSource implements SurfaceHolder.Callback {

	private Camera mCamera;
	private SurfaceView _view;

	public VideoSource(Context context) {
		if (mCamera == null) {
			mCamera = getCameraInstance();
		}
		
		Parameters newParam = mCamera.getParameters();
		newParam.setPreviewFormat(ImageFormat.YV12);
		mCamera.setParameters(newParam);
		
		
		_view = new SurfaceView(context);
		SurfaceHolder holder = _view.getHolder();
		holder.addCallback(this);
//		try {
//			mCamera.setPreviewDisplay(holder);
//		} catch (IOException e) {
//			e.printStackTrace();
//		}
//		mCamera.startPreview();
	}
	
	public SurfaceView get_view() {
		return _view;
	}
	
	
	/** A safe way to get an instance of the Camera object. */
	public static Camera getCameraInstance() {
		Camera c = null;
		try {
			c = Camera.open(); // attempt to get a Camera instance
		} catch (Exception e) {
			Log.i("PTAM","cannot get camera");
			// Camera is not available (in use or does not exist)
		}
		return c; // returns null if camera is unavailable
	}

	public int[] getSize() {
		int[] size = { mCamera.getParameters().getPreviewSize().width,
				mCamera.getParameters().getPreviewSize().height };
		Log.i("source", "size: " + size[0] + " " + size[1]);
		return size;
	}

	public Camera getCamera() {
		return mCamera;
	}

	public byte[] getFrame() {
		byte[] data = null;
		CameraAnalyser ca = new CameraAnalyser();
		mCamera.setPreviewCallback(ca);
		
		try {
			data = ca.waitResult();
		} catch (InterruptedException e) {
			e.printStackTrace();
			data = null;
		}
		
//		Log.i("PTAM","got frame");
		return data;
	}

	
	@Override
	public void surfaceChanged(SurfaceHolder holder, int arg1, int arg2, int arg3) {
		try {
			mCamera.setPreviewDisplay(holder);
			mCamera.startPreview();
			Log.i("PTAM","started preview");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	@Override
	public void surfaceCreated(SurfaceHolder arg0) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder arg0) {
		// TODO Auto-generated method stub
		
	}
}