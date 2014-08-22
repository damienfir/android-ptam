package com.ecn.ptam;

public class PTAM {

	static {
		System.loadLibrary("ptam");
	}
	
	public static native void init(int[] size);
	public static native void update(byte[] im);
	public static native void send(String s);
//	public static native float[] getModelView();
//	public static native float[] getViewModel();
}