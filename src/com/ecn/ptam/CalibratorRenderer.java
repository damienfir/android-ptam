package com.ecn.ptam;


/*
 * Sends a frame from the camera to PTAM to update its tracking state and allows PTAM
 * to render OpenGL data into the GLSurfaceView.
 */
public class CalibratorRenderer extends GLRenderer {
	
	private VideoSource _videosource;
	
	public CalibratorRenderer(VideoSource vs) {
		_videosource = vs;
	}
	
	@Override
	public void draw() {
		byte[] frame = _videosource.getFrame();
		if (frame != null) {
//			PTAM.updatecalib(frame);
		}
	}

	@Override
	public void init() {
//		PTAM.initcalib(_videosource.getSize());
	}
}