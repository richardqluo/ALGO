package com.bbg.test;

import java.util.concurrent.atomic.AtomicInteger;

public class PrintThread implements Runnable {
	/*
	volatile int count=0;
	*/
	final AtomicInteger count = new AtomicInteger(0);//final for pointer not value, better for being used as lock 
	
	/*
	static final AtomicInteger count = new AtomicInteger(0);
	private final int threadPosition;
	public PrintThread(int threadPosition) {
		this.threadPosition = threadPosition;
	}
	*/
	@Override
	public void run() {
		//no guarantee how many each thread print
		/*
		while (count < 101) {
			synchronized (this) {
				//double check because count may be incremented by other thread btw 20 and 21, otherwise may output 101 102
				if(count < 101)	
					System.out.println(Thread.currentThread().getId() + ":" + count++);
			}
		}
		*/		
		int cur;
		while ((cur=count.get()) < 101) {
			synchronized (count) {//no need to sync atomic count, but need to sync on print
				//CAS: make last get to this set atomic, count.getAndIncrement() only current get and set atomic, need double check as volatile
				if (count.compareAndSet(cur, cur+1))
					System.out.println(Thread.currentThread().getId() + ":" + cur);
			}
		}			
		
		//9:0 10:1 11:2 9:3 10:4 11:5 9:6 10:7 11:8 9:9 10:10
		/*
		int cur=0;
		while ((cur=count.get()) < 101) {
			if (cur % 3 == this.threadPosition) {
				//synchronize on static object because each thread has a separate instance
				synchronized (count) {
					if (count.compareAndSet(cur, cur+1))//CAS
						System.out.println(Thread.currentThread().getId() + ":" + cur);
				}
			}
		}
		*/
	}

	public static void main(String[] args) throws InterruptedException {
		
		PrintThread pr = new PrintThread();
		Thread T1 = new Thread(pr);
		Thread T2 = new Thread(pr);
		Thread T3 = new Thread(pr);
		

		/*
		Thread T1 = new Thread(new PrintThread(0));
		Thread T2 = new Thread(new PrintThread(1));
		Thread T3 = new Thread(new PrintThread(2));
		*/
		
		T1.start();
		T2.start();
		T3.start();
		
		T1.join();
		T2.join();
		T3.join();
		
	}
}
