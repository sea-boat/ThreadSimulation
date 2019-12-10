package com.seaboat;

public class ThreadTest {

	public static void main(String[] args) {
		new MyThread().start();
	}

	static class MyThread extends Thread {
		public void run() {
			System.out.println("simulates Java thread!");
		}
	}
}
