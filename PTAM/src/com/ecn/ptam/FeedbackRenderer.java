package com.ecn.ptam;


public class FeedbackRenderer extends GLRenderer {
	
	private VideoSource _vs;
	private double[] _world2cam;
	
	public FeedbackRenderer(VideoSource vs) {
		_vs = vs;
	}
	
	@Override
	public void draw() {
		byte[] frame = _vs.getFrame();
		if (frame != null) {
			PTAM.update(frame);
		}
		_world2cam = PTAM.get_matrix();
	}	
	
	@Override
	public void init() {
		PTAM.init(_vs.getSize());
	}
}