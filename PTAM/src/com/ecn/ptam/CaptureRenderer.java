package com.ecn.ptam;

import static android.opengl.GLES11.*;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import android.util.Log;


public class CaptureRenderer extends GLRenderer {
	
	private VideoSource _vs;
	private CaptureViewer _viewer;
	
	public CaptureRenderer(VideoSource vs, CaptureViewer viewer) {
		_vs = vs;
		_viewer = viewer;
	}
	
	@Override
	public void draw() {
		byte[] frame = _vs.getFrame();
		if (frame != null) {
			PTAM.update(frame);
			_viewer.update();
		}
	}
	
	@Override
	public void init() {
		PTAM.init(_vs.getSize());
		
		ByteBuffer bb = ByteBuffer.allocateDirect(8);
		bb.order(ByteOrder.nativeOrder());
	}
}
