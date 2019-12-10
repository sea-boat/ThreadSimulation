package com.seaboat;

public class YieldThreadTest {

	public static void main(String[] args) {
		MyThread mt = new MyThread();
		mt.start();
		while (true) {
			System.out.println("主线程");
		}
	}

	static class MyThread extends Thread {
		public void run() {
			while (true) {
				System.out.println("MyThread放弃CPU时间");
				this.yield();
			}
		}
	}
}
