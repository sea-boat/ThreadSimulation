package com.seaboat;

public class Thread {
	static {
		System.load("/root/seaboat/native_test/com_seaboat_Thread.so");
	}
	
	public void run() {
		System.out.println("simulates Java thread!");
	}

	public void start() {
		start0();
	}

	private native void start0();
	
	//test
	public static void main(String[] args) {
		new Thread().start();
	}
}

