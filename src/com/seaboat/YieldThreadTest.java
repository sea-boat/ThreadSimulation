package com.seaboat;

public class YieldThreadTest {

	public static void main(String[] args) {
		MyThread mt = new MyThread();
		mt.start();
		while (true) {
			System.out.println("���߳�");
		}
	}

	static class MyThread extends Thread {
		public void run() {
			while (true) {
				System.out.println("MyThread����CPUʱ��");
				this.yield();
			}
		}
	}
}
