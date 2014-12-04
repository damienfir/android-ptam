package com.ecn.ptam;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.View;
import android.view.View.OnClickListener;

/*
 * Manages the state of the application, calls PTAM to update its state.
 * Sends KeyPress events to PTAM to simulate a user interaction.
 * Manages the Logger to save information on the disk.
 * Emits the beep sound to synchronize the video with the logged positions.
 */
public class CalibratorViewer extends GLSurfaceView implements OnClickListener {

	private BatchRenderer _renderer;

	public CalibratorViewer(Context context, VideoSource videosource) {
		super(context);

		_renderer = new BatchRenderer();
		_renderer.add(new CameraRenderer(videosource));
		_renderer.add(new CalibratorRenderer(videosource));
		setRenderer(_renderer);
	}
	
	public void reset() {
		PTAM.send("CameraCalibrator.Reset");
	}
	
	public void optimize() {
		PTAM.send("CameraCalibrator.Optimize=1");
	}

	@Override
	public void onClick(View v) {
		PTAM.send("CameraCalibrator.GrabNextFrame");
	}
}