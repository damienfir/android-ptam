package com.ecn.ptam;

import static android.opengl.GLES11.*;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import android.util.Log;


public class CaptureRenderer extends GLRenderer {
	
	private VideoSource _vs;
	private float[] _modelview;
	private CaptureModel _model;
	private FloatBuffer _center;
	
	public CaptureRenderer(VideoSource vs, CaptureModel m) {
		_vs = vs;
		_model = m;
	}
	
	@Override
	public void draw() {
		byte[] frame = _vs.getFrame();
		if (frame != null) {
			PTAM.update(frame);
//			_modelview = PTAM.getModelView();
//			_model.update_matrix();
		}
	}
	
	@Override
	public void init() {
		PTAM.init(_vs.getSize());
		
		ByteBuffer bb = ByteBuffer.allocateDirect(8);
		bb.order(ByteOrder.nativeOrder());
		_center = bb.asFloatBuffer();
		_center.put(new float[] {0,0});
		_center.rewind();
	}
	
	private void draw_center() {
		glEnableClientState(GL_VERTEX_ARRAY);
		glPointSize(20);
		glColor4f(1,0,0,1);
		glVertexPointer(2, GL_FLOAT, 0, _center);
		glDrawArrays(GL_POINTS, 0, 1);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
	
	private void draw_rectangle() {
		FloatBuffer mat = FloatBuffer.wrap(_modelview);
//		glMatrixMode(GL_MODELVIEW);
//		glPushMatrix();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glLoadMatrixf(mat);
		glPushMatrix();
		
		
		String s = "";
		for (float f : _modelview) {
			s += f + " ";
		}
		Log.i("PTAM","viewmodel: "+ s);
		
		float[] rectf = _model.get_rectangle();
		rectf[0] += _modelview[12];
		rectf[1] += _modelview[13];
		rectf[2] += _modelview[14];
		if (rectf.length > 0) {
			ByteBuffer bb = ByteBuffer.allocateDirect(rectf.length*4);
			bb.order(ByteOrder.nativeOrder());
			FloatBuffer rect = bb.asFloatBuffer();
			rect.put(rectf);
			rect.rewind();
			
			glPointSize(100);
			glEnableClientState(GL_VERTEX_ARRAY);
			glColor4f(0,1,0,1);
			glVertexPointer(3, GL_FLOAT, 0, rect);
			glDrawArrays(GL_POINTS, 0, rect.capacity()/3);
			glDisableClientState(GL_VERTEX_ARRAY);
		}
		
		glPopMatrix();
//		glMatrixMode(GL_MODELVIEW);
//		glPopMatrix();
	}
}
