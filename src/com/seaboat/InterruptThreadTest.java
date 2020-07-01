package com.seaboat;

public class InterruptThreadTest {

	static MyThread thread1 = new MyThread();
	static MyThread2 thread2 = new MyThread2();

	public static void main(String[] args) {
		thread1.start();
		Thread.sleep(500, 0);
		thread2.start();
		Thread.sleep(100, 0);
	}

	static class MyThread extends Thread {
		public void run() {
			System.out.println("MyThread1�ж�״̬Ϊ" + this.isInterrupted());
			Thread.sleep(500000, this.threadId);
			System.out.println("MyThread1�ɹ����ж�" + this.isInterrupted());
		}
	}

	static class MyThread2 extends Thread {
		public void run() {
			thread1.interrupt();
			System.out.println("MyThread1�ж�״̬Ϊ" + thread1.isInterrupted());
			System.out.println("MyThread2�ж�״̬Ϊ" + this.isInterrupted());
		}
	}
}
