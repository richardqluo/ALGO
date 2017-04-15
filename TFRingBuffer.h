#ifndef CAPP_TFRINGBUFFER_H
#define CAPP_TFRINGBUFFER_H
#include <atomic>

using namespace std;

template <typename T>

class TFRingBuffer {
    T *m_buffer;
    atomic<size_t> m_head;
    atomic<size_t> m_tail;
    const size_t m_size; //typedef unsigned long size_t;

    size_t next(size_t current) {
        return (current + 1) % m_size;
    }

public:

    TFRingBuffer(const size_t size) : m_size(size), m_head(0), m_tail(0) {
        m_buffer = new T[size];//fixed size
    }

    virtual ~TFRingBuffer() {
        delete[] m_buffer;
    }
    //consume m_tail and produce m_head
    bool push(const T &object) {
        size_t head = m_head.load(memory_order_relaxed);//only thread to produce m_head, surely latest, so no need sync
        size_t nextHead = next(head);
        if (nextHead == m_tail.load(memory_order_acquire)) {//read=write+1 full
            return false;
        }
        m_buffer[head] = object;//before store to guaranteed the save is visible
        m_head.store(nextHead, memory_order_release);

        return true;
    }
    //consume m_head and produce m_tail
    bool pop(T &object) {
        size_t tail = m_tail.load(memory_order_relaxed);
        if (tail == m_head.load(memory_order_acquire)) {//read=write empty
            return false;
        }

        object = m_buffer[tail];
        m_tail.store(next(tail), memory_order_release);
        return true;
    }
};


#endif //CAPP_TFRINGBUFFER_H
