package com.ecn.ptam;

import java.nio.FloatBuffer;
import java.nio.IntBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.graphics.SurfaceTexture;
import static android.opengl.GLES11.*;
import static android.opengl.GLES11Ext.GL_TEXTURE_EXTERNAL_OES;
import android.opengl.GLSurfaceView;
import android.util.Log;


public class PTAMRenderer implements GLSurfaceView.Renderer, SurfaceTexture.OnFrameAvailableListener {
	
	private VideoSource _vs;
	private SurfaceTexture _tex;
	private IntBuffer _id;
	private IntBuffer _tex_id;
	private GLViewer _view;
	
	public PTAMRenderer(GLViewer view, VideoSource vs) {
		_view = view;
		_vs = vs;
	}

	@Override
	public void onDrawFrame(GL10 arg0) {
		_tex.updateTexImage();
		
		glClear(GL_COLOR_BUFFER_BIT);
		glBindTexture(GL_TEXTURE_EXTERNAL_OES, _tex_id.get(0));
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		
		glBindBuffer(GL_ARRAY_BUFFER, _id.get(0));
		glVertexPointer(2, GL_FLOAT, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, _id.get(1));
		glTexCoordPointer(2, GL_FLOAT, 0, 0);
		
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		
		byte[] frame = _vs.getFrame();
		double pos[] = PTAMWrapper.updatePTAM(frame);
		Log.i("PTAM", "position: " + pos[0] + " " + pos[1]);
	}
	
	
	@Override
	public void onSurfaceChanged(GL10 arg0, int width, int height) {
		glViewport(0, 0, width, height);
//		float ratio = (float) width / height;
//		glFrustumf(-ratio, ratio, -1, 1, 0, 10);
		_vs.set_texture(_tex);
	}
	
	
	@Override
	public void onSurfaceCreated(GL10 arg0, EGLConfig config) {
		PTAMWrapper.initPTAM(_vs.getSize());
		init_gl();
	}
	
	public void onFrameAvailable(SurfaceTexture surfaceTexture) {
	}
	
	private void init_gl() {
		glClearColor(0, 0, 0, 1);
		
		_tex_id = IntBuffer.allocate(1);
		glGenTextures(1, _tex_id);
		
		glEnable(GL_TEXTURE_EXTERNAL_OES);
		glBindTexture(GL_TEXTURE_EXTERNAL_OES, _tex_id.get(0));
		glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER,GL_LINEAR);        
		glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		_tex = new SurfaceTexture(_tex_id.get(0));
		_tex.setOnFrameAvailableListener(this);
		
		_id = IntBuffer.allocate(2);
		glGenBuffers(2, _id);
		
		FloatBuffer tex_array = FloatBuffer.wrap(new float[] {
				-1.f,-1.f, -1.f,1.f, 1.f,-1.f, 1.f,1.f});
		glBindBuffer(GL_ARRAY_BUFFER, _id.get(0));
		glBufferData(GL_ARRAY_BUFFER, tex_array.capacity()*4, tex_array, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		FloatBuffer tex_coord = FloatBuffer.wrap(new float[] {
				0,1, 0,0, 1,1, 1,0});
		glBindBuffer(GL_ARRAY_BUFFER, _id.get(1));
		glBufferData(GL_ARRAY_BUFFER, tex_coord.capacity()*4, tex_coord, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}