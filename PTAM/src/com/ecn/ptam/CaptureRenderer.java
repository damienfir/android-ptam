package com.ecn.ptam;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/*
 * The draw() and init() methods are called by the BatchRenderer when the GLSurfaceView has to
 * be redrawn.
 * Sends a frame from the camera to PTAM to update its tracking state and allows PTAM
 * to render OpenGL data into the GLSurfaceView.
 */
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
