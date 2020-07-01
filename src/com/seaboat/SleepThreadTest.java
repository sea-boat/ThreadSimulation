package com.seaboat;

public class SleepThreadTest {

	public static void main(String[] args) {
		new MyThread().start();
		Thread.sleep(4000, 0);
	}

	static class MyThread extends Thread {
		public void run() {
			System.out.println("当前线程睡眠3000ms");
			Thread.sleep(3000, this.threadId);
			System.out.println("睡眠结束");
		}
	}
}
