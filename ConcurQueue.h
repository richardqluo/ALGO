//
// Created by holly on 6/5/2016.
//

#ifndef CAPP_CONCURQUEUE_H
#define CAPP_CONCURQUEUE_H

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

using namespace std;

template<typename T>

class ConcurQueue {
public:
    ConcurQueue(const size_t size) : capacity(size) { }

    T pop() {
        unique_lock<std::mutex> mlock(mutex_);
        while (queue_.empty()) {
            not_empty.wait(mlock);
        }
        T item = queue_.front();
        queue_.pop();
        mlock.unlock();
        not_full.notify_one();
        return item;
    }

    void pop(T &item) {
        unique_lock<mutex> mlock(mutex_);
        while (queue_.empty()) {
            not_empty.wait(mlock);
        }
        item = queue_.front();//item in queue_outlive this func, can be returned as reference para
        queue_.pop();
        mlock.unlock();
        //can not &T pop(){return item;} because auto copy item to temp var which is deallocated out scope
        not_full.notify_one();
    }

    void push(const T &item) {
        unique_lock<mutex> mlock(mutex_);
        while (queue_.size() == capacity) {
            not_full.wait(mlock);
        }
        queue_.push(item);
        mlock.unlock();
        not_empty.notify_one();
    }

    void push(T &&item) {
        unique_lock<mutex> mlock(mutex_);
        while (queue_.size() == capacity) {
            not_full.wait(mlock);
        }
        queue_.push(move(item));//std::queue::void push (value_type&& val);
        mlock.unlock();
        not_empty.notify_one();
    }

    void singleWrite(void** s)
    {
        unique_lock<mutex> mlock(write);
        while(readers > 0) {
            noReader.wait(mlock);
        }// We wait till everyone finishes read.
        int* p=static_cast<int*>(s); // do write
        mlock.unlock();
    }
    void multiRead(void** s)
    {
        unique_lock<mutex> mlock(write);// wait if it is being written.
        readers++;
        mlock.unlock();//release right after add readers so other readers can get in
        *s = new int(1);// do read
        readers--;
        noReader.notify_all();
    }

private:
    queue<T> queue_;
    mutex mutex_;
    condition_variable not_empty, not_full;
    const size_t capacity;

    mutex write;
    condition_variable noReader;
    std::atomic<int> readers=0;
};

template<typename T>
class DoubleQueue {//?lock free read. lock write
    void push(T & e) {
        lock_guard<mutex> lock(mtx);
        inQueue.push(e);
    }
    void swap(std::queue<T> & outQueue) {
        std::queue<T> empty;
        std::swap(outQueue, empty);//clean outQueue
        lock_guard<mutex> lock(mtx);
        inQueue.swap(outQueue); //std::swap(inQueue, outQueue);
    }
private:
    std::queue<T> inQueue;
    mutex mtx;
};

#endif //CAPP_CONCURQUEUE_H
