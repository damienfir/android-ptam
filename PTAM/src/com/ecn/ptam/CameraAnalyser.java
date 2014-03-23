package com.ecn.ptam;

import java.util.concurrent.CountDownLatch;

import android.hardware.Camera;

public class CameraAnalyser implements Camera.PreviewCallback {

	private final CountDownLatch latch = new CountDownLatch(1);

	private byte[] data;

	@Override
	public void onPreviewFrame(byte[] data, Camera camera) {
		this.data = data;
		latch.countDown();
	}

	public byte[] waitResult() throws InterruptedException {
		latch.await();
		return data;
	}
}
