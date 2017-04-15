#ifndef CAPP_SIGRINGBUFFER_H
#define CAPP_SIGRINGBUFFER_H
#include <atomic>

using namespace std;

class SigRingBuffer {
    int *m_buffer;
    atomic<size_t> m_unread;//num been read
    int r,w;//access by single read | write thread
    const size_t m_size;
public:

    SigRingBuffer(const size_t size) : m_size(size) {
        m_buffer = new int[size];//fixed size
    }

    virtual ~SigRingBuffer() {
        delete[] m_buffer;
    }

    bool Write(int value) {
        if(!m_unread.load(memory_order_acquire)>=m_size){//remove if overwrite
            m_buffer[w]=value;
            w=(w+1)%m_size;
            m_unread.fetch_and(1,memory_order_acq_rel);
        }
    }
    int Read(){
        if(m_unread.load(memory_order_acquire)<=0)//even write increment
            return -1;
        int temp = m_buffer[r];
        r=(r+1)%m_size;
        m_unread.fetch_sub(1,memory_order_acq_rel);
        return temp;
    }

};


#endif //CAPP_SIGRINGBUFFER_H
