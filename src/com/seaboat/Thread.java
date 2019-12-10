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

	public void yield() {
		yield0();
	}

	private native void yield0();

	private native void start0();

}
