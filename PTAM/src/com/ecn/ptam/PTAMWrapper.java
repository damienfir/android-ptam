package com.ecn.ptam;


public class PTAMWrapper {

	static {
		System.loadLibrary("ptam");
	}
	
	public static native void initPTAM(int[] size);
	public static native double[] updatePTAM(byte[] im);
	public static native void sendEventPTAM(String s);
}