package com.ecn.ptam;

import static android.opengl.GLES10.GL_COLOR_BUFFER_BIT;
import static android.opengl.GLES10.glClear;
import static android.opengl.GLES10.glClearColor;
import static android.opengl.GLES10.glColor4f;
import static android.opengl.GLES10.glViewport;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView;

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
		glClearColor(0, 0, 0, 1);
		PTAM.init(_videosource.getSize());
	}
}
