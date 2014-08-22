package com.ecn.ptam;

import java.util.ArrayList;

import android.util.Log;

public class CaptureModel {

	private ArrayList<float[]> _corners;
	private static ArrayList<float[]> _matrices;

	public CaptureModel() {
		_corners = new ArrayList<float[]>();
		_matrices = new ArrayList<float[]>();
	}
	
	public Boolean store_corner() {
		if (_corners.size() < 3) {
//			float v[] = PTAM.getModelView();
//			_corners.add(v);
			Log.i("PTAM", "# corners: " +_corners.size());
		}
		if (_corners.size() >= 3){
			compute_object();
			return true;
		}
		return false;
	}
	
	private void compute_object() {
		
	}
	
	public void update_matrix() {
//		_matrices.add(PTAM.getModelView());
	}
	
	public float[] get_rectangle() {
//		float[] c = new float[_corners.size()*2];
//		for (int i = 0; i < _corners.size(); ++i) {
//			c[2*i] = -_corners.get(i)[3];
//			c[2*i+1] = -_corners.get(i)[7];
//		}
		float[] c = new float[] {0,0,0};
		return c;
	}
}