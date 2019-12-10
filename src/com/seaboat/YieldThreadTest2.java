package com.seaboat;

public class YieldThreadTest2 {
	public static void main(String[] args) {
		MyThread mt = new MyThread();
		mt.setDaemon(true);
		mt.start();
		while (true) {
			System.out.println("主线程");
		}
	}

	static class MyThread extends java.lang.Thread {
		public void run() {
			while (true) {
				System.out.println("让出线程CPU时间");
				java.lang.Thread.currentThread().yield();
			}
		}
	}
}
