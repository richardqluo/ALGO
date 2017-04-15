package com.bbg.test;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class BankAccount {
    double balance;
    int id;
    final Lock lock = new ReentrantLock();
     
    BankAccount(int id, double balance) {
        this.id = id;
        this.balance = balance;
    }
     
    void withdraw(double amount) {
        // Wait to make sure current thread wont finish before the other thread start
        try {Thread.sleep(10l);} catch (InterruptedException e) {}
        //TODO: 
        //while(balance<amount)
        //	wait();
        balance -= amount;
        System.out.println(Thread.currentThread().getId() + "withdraw");
    }
     
    void deposit(double amount) {
        try {Thread.sleep(10l);} catch (InterruptedException e) {}
        balance += amount;
        //TODO:
        //notify();
        System.out.println(Thread.currentThread().getId() + "deposit");
    }
    //serial sync on from and to account not atomic withdraw+deposit, effectively same as synchronized(this) on withdraw() and deposit()
    //that can only ensure either withdraw() or deposit() is running on 1 account instance, but not on 2 account instances
    //because reference para from/to account instances maybe changed out of this func, must parallel sync on both.   
    //since this is static method within account class, can not synchronized(this), 
    //but synchronized(BankAccount.class) will overly block all accounts instances
    static void transfer(BankAccount from, BankAccount to, double amount) {
        synchronized(from) {//nest sync/lock to atomic withdraw+deposit, but deadlock
        	from.lock.lock();
            if (from.balance < amount)
            	throw new RuntimeException("InsufficientFunds");
            from.withdraw(amount);
            System.out.println(Thread.currentThread().getId() + "waiting" + to.id);
            synchronized(to) {
            	to.lock.lock();
                to.deposit(amount);
                System.out.println(Thread.currentThread().getId() + "done");
                to.lock.unlock();
            }
            from.lock.unlock();
        }
    }

	public static void main(String[] args) {
		
        final BankAccount fooAccount = new BankAccount(1, 100d);
        final BankAccount barAccount = new BankAccount(2, 100d);
        /* 
        new Thread() {
            public void run() {
                BankAccount.transfer(fooAccount, barAccount, 10d);
            }
        }.start();
         
        new Thread() {
            public void run() {
                BankAccount.transfer(barAccount, fooAccount, 10d);
            }
        }.start();
        */
 
        new Thread(new Transaction(fooAccount, barAccount, 10d)).start();
        new Thread(new Transaction(barAccount, fooAccount, 10d)).start();
	}
}
class Transaction implements Runnable {
    private BankAccount from, to;
    private double amount;
    Transaction(BankAccount src, BankAccount dest, double amount) {
        this.from = src;
        this.to = dest;
        this.amount = amount;
    }
    public void run() {
    	//no effect because each thread on separate Transaction instance, sync on it wont stop other threads call withdraw/deposit on the same account
    	synchronized(this){   
	        from.withdraw(amount);
	        to.deposit(amount);
    	}
    }
}