package com.ecn.ptam;

import java.io.IOException;

import android.graphics.ImageFormat;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.hardware.Camera.Parameters;
import android.util.Log;


/*
 * Manages the camera allocation/deallocation and provides an interface to grab
 * a frame
 */
public class VideoSource implements Camera.PreviewCallback {

	private Camera mCamera;
	private byte[] _frame;

	public VideoSource() {
		if (mCamera == null) {
			mCamera = getCameraInstance();
		}
		_frame = null;

		Parameters newParam = mCamera.getParameters();
		newParam.setPreviewSize(640, 480);
		newParam.setPreviewFormat(ImageFormat.YV12);
		newParam.setFlashMode(Parameters.FLASH_MODE_TORCH);
		mCamera.setParameters(newParam);
		mCamera.setPreviewCallback(this);
	}
	
	@Override
	public void onPreviewFrame(byte[] data, Camera camera) {
		_frame = data;
	}
	
	public void set_texture(SurfaceTexture tex) {
		try {
			mCamera.setPreviewTexture(tex);
			mCamera.startPreview();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
	/** A safe way to get an instance of the Camera object. */
	public static Camera getCameraInstance() {
		Camera c = null;
		try {
			c = Camera.open();
		} catch (Exception e) {
			Log.i("PTAM","cannot get camera");
		}
		return c;
	}

	public int[] getSize() {
		int[] size = { mCamera.getParameters().getPreviewSize().width,
				mCamera.getParameters().getPreviewSize().height };
		return size;
	}
	
	public void camera_release() {
		if (mCamera != null) {
			mCamera.setPreviewCallback(null);
			mCamera.release();
			mCamera = null;
		}
	}
	
	public byte[] getFrame() {
		return _frame;
	}
}