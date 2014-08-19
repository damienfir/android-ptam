package com.ecn.ptam;

import java.io.IOException;
import java.util.List;

import android.content.Context;
import android.graphics.ImageFormat;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.hardware.Camera.Parameters;
import android.util.Log;

public class VideoSource {

	private Camera mCamera;

	public VideoSource(Context context) {
		if (mCamera == null) {
			mCamera = getCameraInstance();
		}
		
		Parameters newParam = mCamera.getParameters();
//		List<Camera.Size> sizes = newParam.getSupportedPreviewSizes();
//		for (int i = 0; i < sizes.size(); i++) {
//			Log.i("PTAM", "size: "+ sizes.get(i).width + " "+ sizes.get(i).height);
//		}
		newParam.setPreviewSize(640, 480);
		newParam.setPreviewFormat(ImageFormat.YV12);
		mCamera.setParameters(newParam);
	}
	
	
	public void set_texture(SurfaceTexture tex) {
		try {
			mCamera.setPreviewTexture(tex);
			mCamera.startPreview();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
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
	
	
	public void camera_release() {
		if (mCamera != null) {
			mCamera.release();
			mCamera = null;
		}
	}
	
	
	public byte[] getFrame() {
		byte[] data = null;
		CameraAnalyser ca = new CameraAnalyser();
		if (mCamera != null) {
			mCamera.setOneShotPreviewCallback(ca);
		}
		
		try {
			data = ca.waitResult();
		} catch (InterruptedException e) {
			e.printStackTrace();
			data = null;
		}
		
//		Log.i("PTAM","got frame");
		return data;
	}
}