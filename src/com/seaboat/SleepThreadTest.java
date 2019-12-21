package com.seaboat;

public class SleepThreadTest {

	public static void main(String[] args) {
		System.out.println("是当前线程睡眠3000ms");
		Thread.sleep(3000);
		System.out.println("睡眠结束");
	}

}
