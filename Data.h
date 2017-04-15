#ifndef CAPP_TEST_H
#define CAPP_TEST_H
#include <iostream>
#include <mutex>
#include <thread>
using namespace std;
class Data {
private:
    int data;
    int datas[2];//if int datas[]; undertermined on value init
public:
    //Data(int a);
    static volatile int s;
    void proc(recursive_mutex& mtx, int n);
    int getData();
};


#endif //CAPP_TEST_H
