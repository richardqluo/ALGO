#ifndef CAPP_TESTS_H
#define CAPP_TESTS_H
#include <iostream>
using namespace std;

class Datas {
private:
    int data;
    int datas[];//int datas[2][3]; must size for 2D array int* datas;
public:
    Datas(){} //dup to Datas():data(1){}  ?Datas():datas{1,2}{} works with size int datas[2];
    Datas(int a):data(a){//the compiler will not create a default no-parameter constructor Datas():data(0){}
        //this->data=a;//data=a;
        //this->datas={0,1};//?assigning to an array from an initializer list
    }
    Datas(Datas&& other)
    {
        data=other.data;//move same as copy for int which has no move ctr
        other.data=0;
        cout << data <<"move ctr" << other.data << endl;
    }
    Datas& operator=(Datas&& other)
    {
        //if(this!=&other) //need to explicitly define != as well
        {
            data=other.data;
            other.data=0;
            cout << data <<"move assign" << other.data << endl;
        }
        return *this;
    }
    void proc(int* a){
        return;
    }//no ;
};//end with ;


#endif //CAPP_TESTS_H
