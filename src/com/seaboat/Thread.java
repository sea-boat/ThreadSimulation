package com.seaboat;

public class Thread {
	static {
		System.load("/root/seaboat/native_test/com_seaboat_Thread.so");
	}

	public void run() {
	}

	public void start() {
		start0();
	}

	public native void yield();

	private native void start0();

	public static native void sleep(long millis);
	
	public static native void monitorEnter();
	
	public static native void monitorExit();

}
