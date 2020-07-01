package com.seaboat;

import java.util.concurrent.atomic.AtomicInteger;

public class Thread {
	static {
		ai = new AtomicInteger();
		System.load("/root/seaboat/native_test/com_seaboat_Thread.so");
	}
	static AtomicInteger ai;
	public int threadId;

	public Thread() {
		this.threadId = ai.incrementAndGet();
	}

	public void run() {
	}

	public void start() {
		start0();
	}

	private native void start0();

	public native void yield();

	public static native void sleep(long millis, int threadId);

	public static native void monitorEnter();

	public static native void monitorExit();

	public native boolean isInterrupted();

	public native boolean interrupt();

}
