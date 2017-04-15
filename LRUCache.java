package com.liqnet.test;

import java.util.HashMap;

//Least Recently Used Cache O(1) insert and O(1) lookup: maintain the HashMap entries in a doubly linked list, 
//everytime an entry is accessed it is also moved to the end of the list, purge is done from the top (elder) of the list. 
//JDK class LinkedHashMap maintains the entries in a HashMap in a doubly linked list either in AccessOrder or InsertionOrder = true. 
//method removeOldestEntry() override to return true when the cache size exceeds the specified capacity
//import java.util.LinkedHashMap;
//import java.util.Map.Entry;
// 
//public class LRUCache < K, V > extends LinkedHashMap < K, V > {
// 
//    private int capacity; // Maximum number of items in the cache.
//     
//    public LRUCache(int capacity) { 
//        super(capacity+1, 1.0f, true); // Pass 'true' for accessOrder.
//        this.capacity = capacity;
//    }
//     
//    protected boolean removeEldestEntry(Entry entry) {
//        return (size() > this.capacity);
//    } 
//}

//the latest n stock quotes, node(sym,orice)
//O(1) for Insert(X)  Delete(X) Get()
public class LRUCache {
    int capacity;
    HashMap<Integer, Node> map = new HashMap<Integer, Node>();//duplicate store for fast access
    Node head=null;
    Node end=null;
 
    public LRUCache(int capacity) {
        this.capacity = capacity;
    }
    //each time access get/set will remove and sethead the node in linkedlist
    public int get(int key) {
        if(map.containsKey(key)){
            Node n = map.get(key);
            remove(n);
            setHead(n);
            return n.value;
        }
 
        return -1;
    }
    
    public void set(int key, int value) {
        if(map.containsKey(key)){
            Node old = map.get(key);
            old.value = value;
            remove(old);
            setHead(old);
        }else{
            Node created = new Node(key, value);
            if(map.size()>=capacity){
                map.remove(end.key);
                remove(end);
                setHead(created);
            }else
                setHead(created);
 
            map.put(key, created);
        }
    }
 
    public void remove(Node n){
        if(n.pre!=null)
            n.pre.next = n.next;//link forward
        else
            head = n.next;//n is head
 
        if(n.next!=null)
            n.next.pre = n.pre;//link backward
        else
            end = n.pre;//n is tail
    }
 
    public void setHead(Node n){
        n.next = head;//link forward
        n.pre = null;
 
        if(head!=null)
            head.pre = n;//link backward
 
        head = n;
 
        if(end ==null)
            end = head;
    }
}

class Node{
    int key;
    int value;
    Node pre;//doubly linked list know predecessor O(1) , Singly linked list have to traverse the list to find predecessor to remove an item O(n) 

    Node next;
 
    public Node(int key, int value){
        this.key = key;
        this.value = value;
    }
}


