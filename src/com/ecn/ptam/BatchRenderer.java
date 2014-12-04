package com.ecn.ptam;

import static android.opengl.GLES10.GL_COLOR_BUFFER_BIT;
import static android.opengl.GLES10.glClear;
import static android.opengl.GLES10.glClearColor;
import static android.opengl.GLES10.glColor4f;
import static android.opengl.GLES10.glViewport;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView;
import android.util.Log;


/*
 * Keeps a list of GLRenderers and calls their appropriate methods to initialize and
 * draw OpenGL content. It allows the separation between rendering the different part of a scene.
 */
public class BatchRenderer implements GLSurfaceView.Renderer {
	
	private List<GLRenderer> _list;
	private long lastTime;
	private double _fpsvec[];
	private int _idx;
	private int LENGTH = 24;
	
	public BatchRenderer() {
		_list = new ArrayList<GLRenderer>();
		lastTime = 0;
		_fpsvec = new double[LENGTH];
		_idx = 0;
	}
	
	public void add(GLRenderer renderer) {
		_list.add(renderer);
	}
	
	private void update_fps() {
		long t = System.nanoTime();
		double fps = 1e8 / (t - lastTime);
		lastTime = t;
		_fpsvec[_idx] = fps;
		_idx = (_idx+1) % LENGTH;
		double avgfps = 0;
		for (int i = 0; i<LENGTH; i++) avgfps += _fpsvec[i];
		avgfps /= LENGTH;
		Log.i("PTAM", "FPS: "+avgfps);
	}
	
	@Override
	public void onDrawFrame(GL10 arg0) {
		update_fps();
//		Log.i("PTAM", "draw");
//		glClear(GL_COLOR_BUFFER_BIT);
//		
//		for (GLRenderer r : _list) {
//			r.draw();
//		}
//		
//		// cancel color otherwise it will show on texture
//		glColor4f(1,1,1,1);
	}
	

	@Override
	public void onSurfaceChanged(GL10 arg0, int width, int height) {
		glViewport(0, 0, width, height);
		
		for (GLRenderer r : _list) {
			r.changed(width, height);
		}
	}

	
	@Override
	public void onSurfaceCreated(GL10 arg0, EGLConfig arg1) {
		glClearColor(0, 0, 0, 1);
		
		for (GLRenderer r : _list) {
			r.init();
		}
	}
}