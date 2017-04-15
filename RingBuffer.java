package com.bbg.test;

import java.util.*;
import java.util.concurrent.atomic.*;
//Disrupter-pattern:queues where producers put objects on it that are sent to all the consumers for parallel consumption
public class RingBuffer<T> {
	private final int size=10;

	volatile int r=0,w=0;//pointer of read/tail write/head position
	
	private Integer[] buf=new Integer[size];//init with null, int can't null 
	private final AtomicInteger unread = new AtomicInteger(0);//accessed by both threads
	
	private AtomicReferenceArray<T> bufAtomic=new AtomicReferenceArray<T>(size);///accessed by both threads
	
	private List<Item> bufCAS=new ArrayList<Item>();	
	
	public int Read(){
		if(unread.get()==0){//? still repeat read if overwrite
			return -1;//no wait
		}
		int temp=buf[r];
		unread.decrementAndGet();//only read thread decrement it, so it is ok even write thread increment it (wont make it less)  
		r=(r+1)%size; //2%3=2 4%3=1
		return temp;
	}
	public boolean Write(int val){
		if(unread.get()!=size){//skip this line if overwrite
			buf[w]=val;
			unread.incrementAndGet();
			w=(w+1)%size;
			return true;
		}
		return false;
	}
	//only for one write and one read thread, volatile r | w is only updated by one read|write thread, but be accessed by both 
	public int ReadNoNoLock(){
		if (r==w){//empty( read and write pointers at same), only work without overwrite 
			return -1;
		}
		int temp=buf[r];
		//the int value of r two billion will overflow and become negative! operator % preserves the sign -
		r=(r+1)& (size-1);//use bitwise mask, size should be power of 2^k 
		//r=(r+1)%size;
		return temp;//r wont be affected by w being updated since last access 
	}
	public boolean WriteNoLock(int val){
		if(r!=w+1){//no overwrite if full(write pointer is immediately behind the read pointer)
			buf[w]=val;
			w=(w+1)& (size-1);
			return true;
		}
		return false;
	}
	//if one write and one read thread, r w no need to be volatile since only be accessed by one  
	public T ReadAtomic(){
		T temp= bufAtomic.getAndSet(r, null);
		r=(r+1)%size; //2%3=2 4%3=1
		return temp;//Immediately return null if already null, otherwise mark it as read by set to null
	}
	public void WriteAtomic(T val){
		while(true){
			if(bufAtomic.compareAndSet(w, null,val)){//only if it has been read without overwrite
				w=(w+1)%size; //TODO: make this atomic if multiple threads
				return;
			}
		}
		//bufAtomic.set(w, val); //for overwrite
	}
	//?similar to NoLock version,  but overwrite
	//read thread only on oldVal
	public T ReadCAS(){
		Item cur= bufCAS.get(r);
		if (cur!=null && cur.newVal!=cur.oldVal){//CAS avoid repeating read the same value already being read 
			cur.oldVal=cur.newVal;
			r=(r+1)%size;
			return (T)cur.oldVal;
		}
		return null;//wait free
	}
	//write thread only on newVal
	public boolean WriteCAS(T val){
		boolean ret = false;
		Item cur= bufCAS.get(w);
		if(cur==null){
			Item it=new Item();
			it.newVal=val;
			ret=true;
		}
		else{//if(cur.newVal==cur.oldVal) CAS avoid override unread value
			cur.newVal=val;
			ret=true;
		}
		return ret;
	}
}
class Item<T>{
	T oldVal;
	T newVal;
}
//thread safe stack
class CASStack {
    private final AtomicReference<Element> head = new AtomicReference<Element>(null);
    public void push(String value){
        Element newElement = new Element(value);
        while(true){//keep trying till compareAndSet succeed
            Element oldHead = head.get();
            newElement.next = oldHead;//Outer classes can access inner classes private instance member
            if(head.compareAndSet(oldHead, newElement)){
                return;
            }
        }
    }
    public String pop(){
        while(true){
            Element oldHead = head.get();
            if(oldHead == null){
                return null;
            }
            Element newHead = oldHead.next;
            if(head.compareAndSet(oldHead, newHead)){
                return oldHead.value;
            }
        }
    }
    final class Element {//immutable
        private final String value;
        private Element next;
        private Element(String value) {
            this.value = value;
        }
    }
}

