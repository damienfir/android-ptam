package com.ecn.ptam;

import static android.opengl.GLES10.GL_COLOR_BUFFER_BIT;
import static android.opengl.GLES10.glClear;
import static android.opengl.GLES10.glClearColor;
import static android.opengl.GLES10.glColor4f;
import static android.opengl.GLES10.glViewport;

import java.util.ArrayList;
import java.util.List;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView;
import android.util.Log;


public class BatchRenderer implements GLSurfaceView.Renderer {
	
	private List<GLRenderer> _list;
	
	public BatchRenderer() {
		Log.i("PTAM","init");
		_list = new ArrayList<GLRenderer>();
	}
	
	public void add_renderer(GLRenderer renderer) {
		_list.add(renderer);
	}
	
	
	@Override
	public void onDrawFrame(GL10 arg0) {
		glClear(GL_COLOR_BUFFER_BIT);
		
		for (GLRenderer r : _list) {
			r.draw();
		}
		
		// cancel color otherwise it will show on texture
		glColor4f(1,1,1,1);
	}
	

	@Override
	public void onSurfaceChanged(GL10 arg0, int width, int height) {
		glViewport(0, 0, width, height);
		
		for (GLRenderer r : _list) {
			r.changed();
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