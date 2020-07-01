package com.seaboat;

public class SynchronizedThreadTest {

	public static void main(String[] args) {
		new MyThread().start();
		new MyThread2().start();
		Thread.sleep(8000, 0);
	}

	static class MyThread extends Thread {
		public void run() {
			Thread.monitorEnter();
			System.out.println("mythread sleeps 3s.");
			Thread.sleep(3000, this.threadId);
			Thread.monitorExit();
		}
	}

	static class MyThread2 extends Thread {
		public void run() {
			Thread.monitorEnter();
			System.out.println("mythread2 thread sleeps 2s.");
			Thread.sleep(2000, this.threadId);
			Thread.monitorExit();
		}
	}
}
