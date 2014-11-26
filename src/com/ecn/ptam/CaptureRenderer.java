package com.ecn.ptam;


/*
 * Sends a frame from the camera to PTAM to update its tracking state and allows PTAM
 * to render OpenGL data into the GLSurfaceView.
 */
public class CaptureRenderer extends GLRenderer {
	
	private VideoSource _videosource;
	private CaptureViewer _viewer;
	
	public CaptureRenderer(VideoSource vs, CaptureViewer viewer) {
		_videosource = vs;
		_viewer = viewer;
	}
	
	@Override
	public void draw() {
		byte[] frame = _videosource.getFrame();
		if (frame != null) {
			PTAM.update(frame);
			_viewer.update();
		}
	}

	@Override
	public void init() {
		PTAM.init(_videosource.getSize());
	}
}