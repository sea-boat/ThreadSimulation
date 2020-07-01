package com.seaboat;

public class ThreadTest {

	public static void main(String[] args) {
		new MyThread().start();
		Thread.sleep(100, 0);
	}

	static class MyThread extends Thread {
		public void run() {
			System.out.println("simulates Java thread!");
			System.out.println("thread id is " + this.threadId);
		}
	}
}
