#ifndef CAPP_TFRINGBUFFER_H
#define CAPP_TFRINGBUFFER_H
#include <atomic>

using namespace std;

template <typename T>

class TFRingBuffer {
    T* m_buffer;
    atomic<size_t> m_write;
    atomic<size_t> m_read;
    const size_t m_size; //typedef unsigned long size_t;

    size_t next(size_t current) {
        return (current + 1) % m_size;
    }

public:

    TFRingBuffer(const size_t size) : m_size(size), m_write(0), m_read(0) {
        m_buffer = new T[size];//fixed size
    }

    virtual ~TFRingBuffer() {
        delete[] m_buffer;
    }
    //single write read thread
    bool empty(){
        return m_write == m_read;
    }
    bool full(){
        return m_read == next(m_write);
    }
    bool push(const T &object) {
        if(full()}
            return false;
        m_write = next(m_write);
        m_buffer[m_write] = object;
        return true;
    }
    bool pop(T &object) {
        if(empty())
            return false;
        m_read = next(m_read);
        object = m_buffer[m_read];
        return true;
    }
    //multiple write read threads
    bool push(const T &object) {
        size_t head = m_write.load(memory_order_relaxed);//only thread to produce m_write, surely latest, so no need sync
        size_t nextHead = next(head);
        if (nextHead == m_read.load(memory_order_acquire)) {//read=write+1 full
            return false;
        }
        m_buffer[head] = object;//before store to guaranteed the save is visible
        m_write.store(nextHead, memory_order_release);

        return true;
    }
    
    bool pop(T &object) {
        size_t tail = m_read.load(memory_order_relaxed);
        if (tail == m_write.load(memory_order_acquire)) {//read=write empty
            return false;
        }

        object = m_buffer[tail];
        m_read.store(next(tail), memory_order_release);
        return true;
    }
};


#endif //CAPP_TFRINGBUFFER_H
