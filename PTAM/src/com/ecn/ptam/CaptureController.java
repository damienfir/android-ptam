package com.ecn.ptam;

import java.util.ArrayList;

public class CaptureController {
	
	private ArrayList<double[]> _corners;
	private static ArrayList<double[]> _matrices;
	private Thread _thread;
	
	public CaptureController() {
		_corners = new ArrayList<double[]>();
		_matrices = new ArrayList<double[]>();
	}
	
	public Boolean store_corner() {
		if (_corners.size() <= 3) {
			double v[] = PTAM.get_pose();
			_corners.add(v);
			return false;
		}
		else {
			compute_object();
			return true;
		}
	}
	
	public void update_matrix(double[] mat) {
		_matrices.add(mat);
	}
	
	public void start_capture() {
		
	}
	
	public void stop_capture() {
		
	}
	
	private void compute_object() {
		
	}
}
