package com.ecn.ptam;


/*
 * Interface to the native library
 */
public class PTAM {

	static {
		System.loadLibrary("gnustl_shared");
		System.loadLibrary("ptam");
	}
	
	public static native void init(int[] size);
	public static native void update(byte[] im);
	public static native void send(String s);
	public static native void start();
	public static native void stop();
	public static native boolean mapIsGood();
	public static native boolean objectIsGood();
	public static native float[] getModelView();
	public static native float[] getCorners();
	public static native long getTime();
}