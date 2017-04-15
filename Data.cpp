#include "Data.h"

//Data::Data(int a){
//    data=a;
//}
void Data::proc(recursive_mutex& mtx, int n) {
    for (int i=0; i<n; ++i)
    {
        unique_lock<recursive_mutex> mlock(mtx);
        cout << this_thread::get_id() << ':' << ++s<< endl;
    }
}
int Data::getData(){
    return data;
}
volatile int Data::s = 0;