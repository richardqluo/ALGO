#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <forward_list>
#include <memory>
#include <string.h>
#include <string>
#include <unordered_map>
#include <map>
//<map|set> cover <multimap|set> as b-tree
#include <unordered_set>
#include <set>
#include <queue>
#include <stack>
#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include <algorithm>
#include <cmath>
#include <time.h>
#include <assert.h>
#include "Data.h"
#include "Datas.h"
//<algorithm>std::sort std::reverse std::remove_if std::max(1,2);std::abs(-1);pow(3,2)==9;
//#include <limits.h> INT_MIN UINT_MAX;numeric_limits<int>::min();
//<memory>unique_ptr shared_ptr
//<mutex>unique_lock
//<utility> std::pair
using namespace std;//using std::vector;

struct mcomparer {//functor
    bool operator() (const char& lhs, const char& rhs) const
    {return lhs<rhs;}
};
bool mcomparer (int first, int second){ return ( first<second ); }
class key
{
    char m_str[];//int m_value;
public:
    bool operator<(const key& src)const
    {
        return strcmp(this->m_str,src.m_str);
        //return (this->m_value < src.m_value);
    }
};

struct is_odd {
    bool operator() (const int& value) { return (value%2)==1; }//function object
};
bool is_odd (const int& value) { return (value%2)==1; }//function pointer

void stl(){
    //treemap
    map<key,int> mk;//customer object as key
    map<char,int> mp;//map<char,int,mcomparer> mp;
    //map<char,int>::key_compare mcomp = mp.key_comp(); //sorted by default less as <, same for set::value_compare value_comp()
    map<char,int>::iterator itlow,itup;
    mp['c']=60;//mp.end()=c
    mp['a']=20;//mp.begin()=a
    mp['b']=40;
    map<char,int> m = { // nested list-initialization
            {'c', 60},
            {'a', 20 },
            {'b', 40}
    };
    pair<std::map<char,int>::iterator,bool> ret;
    ret = mp.insert ( pair<char,int>('c',50) );//err insert( 'c',50 )
    //pair <int,int> p=make_pair (10.5,'A') | pair<int,int>(10,20); //make_pair implicit conversion
    if (ret.second==false) {
        //insert only create and fails if key exists
        cout << ret.first->second << endl;//existing value
    }
    itlow= mp.lower_bound ('a');  // itlow points to b
    itup= mp.upper_bound ('b');   // itup points to c (not b!)
    mp.erase(itlow, itup);        // erases [itlow,itup)

    map<int,vector<char>> mv;
    vector<char> vc={'a'};
    mv[0]=vc;
    mv[0].push_back('b');

    set<int> ms;
    ms.insert(1);//unchanged if val exists, returns a pair<iterator,bool> r.first to val, r.second false
    if(ms.find(1)!=ms.end())//
        ms.erase(1);
    ms.erase(ms.find(1));
    //hashmap
    unordered_map<string,int> map ={
            {"IBM",150},
            {"GS",100},
            {"MS",200}
    };
    auto r = map.insert(make_pair("GS",250));//move temp obj
    if (!r.second){
        cout << r.first->second << endl;//existing value
    }
    if (map.count("MS")>0) {//exists
        map.erase("MS");
    }
    map["GE"]=250;//if key exists replace value otherwise insert
    //r =  map.emplace("GE",300);//in-place construct item avoid extra move copy op

    int val =map.at("MS"); //Type& t = map.at("key"); get reference to object
    //refernece can not be in container like vector|unordered_map<int,T&>
    //use shared point instead  vector|unordered_map<int,std::shared_ptr<T>>
    unordered_map<string,int>::iterator mit;
    mit =map.find("GS");
    if(mit != map.end()){
        mit->second += stoi("200");
    }
    mit = map.begin();
    while(mit != map.end()){
        cout << mit->first << " ";
        mit++;
    }
    cout << endl;
    for(const auto& item:map){
        cout << item.second << " ";
    }
    cout << endl;
    //for(auto it : map){ it.second
    for(auto it = map.begin(); it != map.end(); ) {//not ++it here
        if (it->second < 100)
            it = map.erase(it);//return it pointing to the next
        else
            ++it;
    }
    int num=map.erase("MS");//by key and return Number of elements removed.
    multimap<int, char> dict {{3, 'A'},{2, 'B'},{2, 'C'},{4, 'E'}};
    auto range = dict.equal_range(2);//return pair< multimap<int, char>::iterator to first element not less than | greater than key >
    for (auto it = range.first; it != range.second; ++it)
    {cout << it->first << ": " << it->second << '\n';  }//2:B 2:C
    for ( std::multimap< int, char, std::less< int > >::const_iterator iter =dict.begin(); iter != dict.end(); ++iter )
    { cout << iter->first << ':' << iter->second << '\n';}//2:B 2:C 3:A 4:B

    vector<char> *vec = new std::vector<char>();//vector<char> *vec = new vector<char>{ 'a', 'b'};
    std::sort(vec->begin(), vec->end());
    string str{vec->begin(), vec->end()};
    vec->push_back('a');
    vec->front() = vec->back();
    vec->pop_back();//void
    delete vec;//destruct
    unique_ptr<vector<int>> vp(new vector<int>);
    vector<int> vt(3);//0 0 0 destructor of vt will be called when out of scope
    vt.push_back(1); //0 0 0 1 append to back, 1 if  vector<int> vt;
    vt.emplace_back(2);//vector<std::pair<int,string>> vs; vs.emplace_back(1,"s") = vs.push_back(std::make_pair(1,"s"));
    //always encapsulate a dynamic array stored in the heap
    vector<int> v{ 1, 2};
    for (unsigned i=0; i<vt.size(); i++)
        cout << ' ' << vt[i];//0 0 0 1 2
    cout << endl;
    for (auto rit = vt.rbegin(); rit!= vt.rend(); ++rit)//cbegin:const rbegin:reverse
        cout << ' ' << *rit;//2 1 0 0 0
    cout << endl;
    for (vector<int>::iterator it = std::begin(vt); it!=std::end(vt); ++it)
        cout << ' ' << *it;
    cout << endl;
    for( auto d : vt )
        cout << ' ' << d;
    cout << endl;
    vt.erase( std::remove( vt.begin(), vt.end(), 1 ), vt.end() );
    vt.erase( std::remove_if(vt.begin(), vt.end(), [](int i){return (i % 2) == 0;}), vt.end() );
    //vt.erase( std::remove_if(vt.begin(), vt.end(), is_even), vt.end() ); bool is_even(int i){return (i % 2) == 0;}
    vector<int>::iterator it =find_if(vt.begin(), vt.end(),is_odd);
    std::sort(std::begin(vt),std::end(vt), std::greater<int>());//sort desc, default std::less<int>
    if (std::binary_search (vt.begin(), vt.end(), 2)){
        vt.erase (vt.begin()+1);//2nd item irrelevant to 2
    }
    int* p = vt.data();//pointer to first item in internal array
    ++p;
    *p = 3; //0 3 0 1 2
    p[1]=4; //0 3 4 1 2
    vt.reserve(10);//change capacity: size() 3 capacity() 10
    vt.resize(6);//change size: size() 5 in|decrease from back with default value  0 3 4 1 2 0
    cout << vt.capacity() << endl;//10
    vt.shrink_to_fit();
    cout << vt.capacity() << endl;//6
    vt.clear();
    vt.shrink_to_fit();//deallocate momory
    cout << vt.capacity() << endl;//0

    list<int> ls,lt;//double link
    ls.emplace_back(1);//if object new element is constructed in place vs push_back
    list<int>::iterator lit = ls.begin();
    ++lit;// advance(lit,1);
    ls.insert(lit,2);//need iterate to the position  1 2
    cout << ls.back()<< endl;
    ls.sort();//O(n*log(n))
    ls.reverse();
    ls.unique();//remove dup
    ls.remove(1);//by value vs erase by iterator
    ls.pop_back();
    if (!ls.empty())
        ls.remove_if(is_odd);//predicate is_odd() for functor
    ls.merge(lt,mcomparer);//both sorted already
    ls.splice(lit,lt);//insert lt into ls at iterator position
    forward_list<int> lf={2, 1};//single link
    lf.front()=3;//int& 3 1
    lf.push_front(3);//front only 3 3 1
    cout << lf.front()<< endl;  //const int&

    queue<int> mq;
    mq.push(1);//pop|front|back() //old|new
    priority_queue<int> pq;//<int, vector<int>, std::greater<int>> in reverse order
    int max=pq.top();
    deque<int> md;
    md.push_front(1);md.push_back(2);
    stack<int> mt;
    int last=mt.top();

    //encapsulate a statically-sized array, if init class on the stack, the array will be allocated on the stack
    array<int,3> mr;//= {1,2,3};  fixed-size allocate on stack
    mr.fill(4);
    mr[2]=1;
    int mi=mr.at(2);//mr[2]

    char ca[]={'a','b','\0'};
    cout << ca << endl;//ab
    cout << *ca << endl;//a
    for (auto it = std::begin(ca); it!=std::end(ca); ++it)
        cout << ' ' << *it;
    cout << endl;
    for( auto d : ca )
        cout << ' ' << d;
    cout << endl;
    std::sort(ca, ca + 3); //std::sort(std::begin(ca), std::end(ca)); not work
    std::string st(ca);//std::string st("ab");
    cout << st[0] << endl;
    char ci=st.at(1);
    string sb=st.substr(1,1);//b
    if(st.compare(sb)!= 0){//st==sb value
        if(st.compare(1,1,sb)== 0){
            cout << "ne"<< endl;
        }
    }
    size_t found = st.find(sb);
    if (found!=string::npos) {//npos -1
        cout << found << endl;
    }
    st.replace(0,1,"c");
    st.append("c");//st+="c";
    st.append(string(1, 'c'));//convert char to string
    st.append(std::to_string(1));
    st.erase(0,1);
    st.erase(std::remove_if(st.begin(), st.end(), ::isspace), st.end());//delete space, err on std::isspace
    char * cstr = new char [st.length()+1];
    if ( st.length() == strlen(cstr) ) {
        strcpy(cstr, st.c_str());
        st.copy(cstr, 2, 1);
    }
    delete[] cstr;
    const char * cst = st.data();//st.c_str(); chars value + terminating null-character ('\0') at the end
    for(char& c : st) {
        cout << c << endl;
    }
    for(std::string::iterator it = st.begin(); it != st.end(); ++it) {
        cout << *it << endl;//char
    }
    for(std::string::size_type i = 0; i < st.size(); ++i) {
        cout << st[i] << endl;
    }

    const size_t row=3,col=2;
    int av[row][col] = { {0,0}, {1,2}, {2,4}};//static: {{0}} fill all { 0,0,1,2,2,4} unchangable size must be known at compile-time
    //int **ap = new int[row][col];//wrong
    //int* ap[row];  ap=new int*[row]; //error: incompatible types in assignment of 'int**' to 'int* [3]'
    //int** allocMatrix(int row, int col) {return ** from a func
    int **va=new int*[row];//dynamic: size can be variable, array is implicit pointer
    for (int i=0;i<row;++i) {//O(N) by size of rows|columns
        va[i] = new int[col];
        //std::fill_n(va, row, -1);
    }
    //return va;
    //}
    //int** mtx=allocMatrix(3,4);
    //int *arr = (int *)malloc(r * c * sizeof(int)); allocate|access memory block by simple pointer O(1)
    //for (i = 0; i <  r; i++) for (j = 0; j < c; j++) *(arr + i*c + j) =0;
    va[1][2]=1;
    for (int i=0;i<row;++i) {
        delete[] va[i];
    }
    delete[] va;//multi delete must match multi new

    vector<vector<int>> vv{ {2},{3,4},{5,6,7}};
    vector<int> vr; vr.push_back(1);vr.push_back(2);
    vv.push_back(vr);
    vr.clear();
    vr.push_back(3);vr.push_back(4);
    vv.push_back(vr);
    for(int row=0;row<vv.size();++row)
        for(int col=0;col<vv[0].size();++col)
            cout << vv[row][col] << endl;
    vv[2][1]=4;
    cout << vv[2][1] << endl; //4
    for (vector<vector<int>>::const_iterator row = vv.begin(); row != vv.end(); ++row)
    {
        for (vector<int>::const_iterator col = row->begin(); col != row->end(); ++col)
            cout << *col;
    }
    array<array<int,col>,row> aa {{ {1,2},{3,4},{5,6} }};
    aa[1][2]=1;
    //void foo(std::array<std::array<int, 3>, 2>& arr){} foo(aa)

    //std::tuple<int,double,string> boo() {return std::make_tuple(1,2.0,"s");}//pair<int,string>.first|second
    //std::tie(i,d,s) = boo();//return multiple map to  d=2.0,s="s"
}

Data* proc(Data dv){
    Data* p = &dv;
    return p;
}

void procd(Datas const & df){
    Datas da;
    //da=move(df);//default op=(const Datas&) is deleted by define move ctr
}
void proca(Data da[]){
    for (int i=0 ; i<3 ; i++){ int data = da[i].getData();}
}
void procs(Data* dp){
/*
    Data* p = nullptr; //Data* p; {Data*|0x0}nil
    p=dp;//Data* p=dp; {Data*|0x2c1580}0x2c1580
    int data = dp->getData();//(*dp).getData(); 0
    data =p->getData();//0
    delete p; //both dp p data=2914816
*/
    for (int i = 0; i < 3; i++)
    {
        int data = dp[i].getData();
    }
    delete[] dp;
}
unique_ptr<Data> proct(unique_ptr<Data> up)
{
    int data =up.get()->getData();
    return move(up);
}

int * getPtr( )
{

    static int i;//without static compile warning address local var, runtime crash but no SIGSEGV
    i=0;
    return &i;

/*
    int* i= new int();//ok: dynamic alloc on heap wont be auto deallocated ,init 0 as new int(0); new int; not init i*=1163005939
    //int* i; int j =0; i=&j;//err: all on stack auto deleted out of scope
    return i;//i on stack auto destructed out of scope, pass address of int value on heap to called pointer
*/
/*
    static int  r[3];
    for (int i = 0; i < 3; ++i)
    {
        r[i] = rand();
    }
    return r;
*/
}
void setPtr(int r[])//(int* r)
{
    for (int i=0 ; i<3 ; i++)
    {
        r[i] = 0;//*(r + i)=0;
    }
}
int & getRef(int& i)
{
    static int j=i;//can not return ref on local temp var without static
    return j;
}
int getCopy(int& i)
{
    int j=i;
    return j;
}
void updtPtr(){
    int i = 5;
    int j = 1;
    *(&j + 1) = 0;//stack earlier in at higher address, &j + 1 is &i
    cout << "i: " << i << endl;//0 not 5

    char *s; //readonly memory unless = new char[1];
    //char s[1]; //writable memory
    strcpy(s, "hello");//Segmentation: fault on char *s
    cout << *s << endl;//h, s - hello
}
void ptr(){
    updtPtr();
/*
    char *a=nullptr;
    char& c = *a;//warning
*/
/*
    int *d;
    d = getPtr();
    cout << *d << endl;
    //for ( int i = 0; i < 3; i++ ){
    //    cout << *(d + i) << endl;
    //}


    int* b = new int[3] {1,2,3};
    setPtr(b);
    delete[] b;//all item become like 3766784
    int e[]{1,2,3};//e[3]{1,2,3} e[]={1,2,3}
    setPtr(e);
    //no need delete e; because local pointer without new will auto dealloc
    for( auto d : e )
        cout << ' ' << d ;//0 0 0
    cout << endl;

    int* pp=new int;
    int* qq=new int(1);
    *pp=*qq;
    pp=qq;
    //int* r=&int();    int& o=int(); //invalid


    int i = 1;
    int k = 2;
    int& j = i; //same: int* t =&i|new int();  int& j = *t;   err: int& j= new int();
    j = k; //wont reassign j to k address, but assign k value to i referred by j
    cout << j <<' ' << i << endl;//2 2
    int& l = j;
    l=3;
    cout << j <<' ' << i << ' ' << l <<endl;//3 3 3
    l=getRef(k);//2
    const int &m= getCopy(k);//illegal without const: getCopy is rvalue
    cout << m <<endl;//2
*/
/*
    Data dt;//default init: dt.data uninit 1966640341,  not Data dt();
    Data* pn = new Data;//default init
    delete pn; //otherwise mem leak after reassign pn to another address
    pn=&dt; //no need to delete local controled by scope
    //pn = dt; //cause Segmentation fault by accessing deleted mem

    Data db=Data();//value init or  Data db{}; aggregate init: dt.data init 0 dt.datas uninit
    *pn=db;//pn still pointer to &dt, value in dt change to db
    auto dm=move(db);//Data has not move ctr, implicit copy ctr is used
    auto pm = new Data();//value init, auto* ok
    //delete pm;

    Datas const *ps, *pi ; //pointer to constant Datas
    //const Datas ds; //ds.data uninit without initializer
    const Datas ds(1);//direct init overload ctr, const need initializer or explicit default ctr
    ps=&ds;//ps=ds; err readonly var, but can repoint ps= nullptr;
    //const pointer must init, Datas * const pc = &ds; err const pointer discard constant of ds
    Datas const * const pc=&ds;// pc= nullptr; err can not repoint
    pi=ps;
    Datas dc(2);//Datas dc = Datas()//value-init but dc.data undetermined due to explicit default ctr
    auto dn=move(dc);//2move ctr0, err if const dc due to constexpr Datas(const Datas&) is deleted by define move ctr
    Datas dd=Datas(3);
    dc=move(dd);//3move assign0, err dc=dd default op=(const Datas&) is deleted by define move ctr, only op=(Datas&&)

    Data* pa = new Data[3];//default init: dt.data uninit 1966640341
    Data* pr = new Data[3]();//value init dt.data init 0
    delete[] pa;
    //delete[] pr;

    Data& r=dt;
    Data& rn=r;
    Data da;
    r=da;
    rn=*pn;
    Data ra[3];//uninit ,Data ra[3]{} 0

    Data* dp=proc(dt);//dp point to dt
    proca(ra);
    //proca(pr); //delete[] pr in proca
    procd(*pi);
    //procs(&dt);
    procs(pr);
    //procs(pm);
*/
/*
    auto pt = unique_ptr<Data>(new Data());
        auto pl=move(pt);
        cout << pl->getData() <<endl;
        //cout << pt->getData() <<endl;//SIGSEGV (Segmentation fault)
    auto rp=proct(move(pl));//move not copy
    //cout << pl->getData() <<endl;//SIGSEGV (Segmentation fault)
    cout << rp->getData() <<endl;//Data will be deallocated after final rp out of scope

    auto pv = unique_ptr<Data>(new Data);
    vector<unique_ptr<Data>> v;
    v.push_back(move(pv));// push_back(value_type&& __x) { emplace_back(std::move(__x)); }
    v.push_back(unique_ptr<Data>(new Data()));
    v.emplace_back(new Data);


    auto sp = make_shared<Data>(int(i));//rvalue in ctr or shared_ptr<Data> sp( new Data(i) ); //value init 0
    {
        shared_ptr<Data> sq = sp;
        cout <<sq.use_count()<<endl;//2
        cout <<sq->getData()<<endl;//0
        sp.reset(new Data);//use_count 1, default init 16764578
        sq.reset();//deallocate
        cout <<sq.use_count()<<endl;//0
    }
    cout <<sp.use_count()<<endl;//1
    cout << sp->getData() <<endl;
    //func(const std::shared_ptr<T>&) //pass value std::shared_ptr<T> will increment use_count then decrement when out func for no reason
*/
}

atomic<int> counter (0);
//volatile int counter (0);
recursive_mutex mtx;//reentry
void incCounter (int n)
{
    for (int i=0; i<n; ++i) {//same effect lock out | in for
        lock_guard<recursive_mutex> guard(mtx);
        //mtx.lock();//block if fail: all threads eventually get in
        //if (mtx.try_lock()) { //return if fail: not all threads get in
        cout << this_thread::get_id() << ':' << ++counter << endl;
        //atomic op is synced by default, but not output which need mtx to sync
        //cout << this_thread::get_id() << ':' << counter.fetch_add(1) <<endl;
        //mtx.unlock();
        //}
    }
}

void divCounter (int d)
{
/*
    while (counter<100)
    {
        lock_guard<recursive_mutex> guard(mtx); //unique_lock<recursive_mutex> mlock(mtx);
        if(counter<100 && counter%3 ==d)
            cout << this_thread::get_id() << ':' << ++counter << endl;

    }
*/
    int cur;
    while((cur = counter.load())<100)
    {
        if(cur%3 ==d)
        {
            mtx.lock();
            if (counter.compare_exchange_weak(cur, cur + 1)) {
                cout << this_thread::get_id() << ':' << counter << endl;
            }
            mtx.unlock();
        }
    }
}

void incRefCounter (recursive_mutex& mtx,volatile int& counter, int n)//atomic<int>& counter
{
    for (int i=0; i<n; ++i)
    {
        unique_lock<recursive_mutex> mlock(mtx);
        cout << this_thread::get_id() << ':' << ++counter << endl;
    }
}


void tfunc(string& s)
{
    s="b";
    cout <<this_thread::get_id();
}
void tfunct(string s)
{
    s="b";
    cout <<this_thread::get_id();
}

unique_lock<recursive_mutex> acquire_lock()
{
    return unique_lock<recursive_mutex>(mtx);
}
void release_lock(unique_lock<recursive_mutex> ml)
{
    ml.unlock();
}
struct Node {
//public: //by default
    int value;
    Node* next;
    Node(int v, Node* n):value(v),next(n){}
};
atomic<Node*> head (nullptr);//can form lockless stack

void push (int val) {     // append an element to the list
    Node* oldHead = head;//head.load();
    Node* newNode = new Node {val,oldHead};
    // what follows is equivalent to: head = newNode, but in a thread-safe way:
    while (!head.compare_exchange_weak(oldHead,newNode)) {//return if succeed set head to new node, with random order
        //if fail current thread keep updating  newNode->latest head and trying set its newnode as head,
        // incorrect if oldHead->next =  newNode because multithread modify head, need lock
        newNode->next = oldHead;
    }
}
int pop() {
    Node *oldHead = head;
    int ret = oldHead == nullptr ? 0 : oldHead->value;
    while (!head.compare_exchange_weak(oldHead, oldHead->next)) {
        //if fail current thread keep updating ret to latest head set successfully by another thread
        ret = oldHead == nullptr ? 0 : oldHead->value;
    }
    //?delert oldHead
    return ret;
}

int func(int v){
    return v+1;
}

void func(promise<int> & p) {
    //service call
    p.set_value(3);
}

struct price {
    int seq;
    int buy;
    int sell;
    price(int a, int b, int s):seq(a),buy(b),sell(s){}
};
//a group of maps storing price data from different banks for each symbol
struct maps
{
    //hastable to store and lookup buy/sell prices based on bank for updating
    unordered_map<string, price> banks; /*Bank Name*/
    //map sorted by price, store the lastest buy or sell price and providing bank as key & value
    //multimap because the same price maybe provided by multiple banks
    multimap<int, string, std::greater<int>> buys;  /* Buy rate*100 */
    multimap<int, string> sells;  /* Sell rate*100 */
};
//convert decimal string to int in order to be used as key in map, for example "1.05" to 105, "1.0" to 100
int strToInt(string s){
    std::size_t pos =s.find('.');
    if(pos !=std::string::npos && pos != s.size()-1){//case "1."
        if(s.substr (pos).size()<3){
            s.append("0");
        }
        else{
            s.erase(pos+3);//keep only 2 digits decimal
        }
        s.erase(pos,1);//remove '.'
    }
    else {
        s.append("00");
    }
    //s.erase( std::remove(s.begin(), s.end(), '.' ), s.end());
    return stoi(s);
}
string intToStr(int i){
    string s= std::to_string(i);
    s.insert(s.end()-2,'.');
    if(s.substr(s.find('.'))==".00") {
        s.erase(s.find('.'));
    }
    else if(s.back()=='0'){
        s.pop_back();
    }
    return s;
}

class TimeSeries{
private:
    unordered_map<time_t,double> series;
    vector<time_t> times;//store sorted time for binay search by findPrev, time_t is same as unsigned int, so ptr wont be better
    size_t limit;
    time_t cur=0;
    time_t findPrev(time_t val){
        int start = 0;
        int end = times.size()-1;
        while(start+1<end){//leave 1 in middle
            int mid=(start+end)/2;
            if (times[mid]<val){
                start=mid;
            }else if(times[mid]>val) {
                end = mid;
            }//already know val not in times
        }
        return times[start];
    }
public:
    TimeSeries(size_t window){
        limit=window;
    }
    void AddValue(time_t current_time,double value){
        if (current_time<cur)//only increment
            return;
        times.push_back(current_time);
        series[current_time]=value;
        cur=current_time;
        while (current_time-times.front()>limit){//delete all that less than the stored window
            series.erase(times.front());
            times.erase(times.begin());
        }
        return;
    }
    double GetValue(time_t desired_time){
        if(series.count(desired_time))
            return series[desired_time];
        else if(times.back()-desired_time>limit){//if less than the stored window
            return std::nan("");//result as undefined
        }
        else if(desired_time<times.front())//if within the stored window but not exists before 1st
            return 0;
        else{
            time_t prev=findPrev(desired_time);
            return series[prev];
        }
    }

};
class b
{
public:
    virtual ~b(){cout<<'4';}
};
class s:public b
{
public:
    s():b(){cout<<'1';}
    s(const s& rhs):b(rhs){cout<<'2';}
    virtual s* clone() const{return new s(*this);}
    ~s(){cout<<'3';}
};
template<class T>
class M
{
public:
    void foo(T val){
        cout<<to_string(val)<<endl;
    }
};
template<>
class M<string>{
public:
    void foo(string val){
        cout<<val<<endl;
    }
};
struct Event{
    int data;
    Event(int dt):data(dt){}
    virtual void Show() const {
        cout<<data<<endl;
    }
};
struct SubEvent : Event{//default public inherit
    string subData;
    SubEvent(int dt,string sdt) : Event(dt){
        subData=sdt;
    }
    void Show() const {//must be const because called from const evt instance
        cout<<data<<"|"<<subData<<endl;
    }
};

class Listener{
public:
    virtual ~Listener(){}
    virtual void OnEvent(const Event& evt)=0;
};
class SubListener : public Listener{
public:
    ~SubListener(){}
    void OnEvent(const Event& evt) override {//Event& evt=sevt;
        evt.Show();//err if direct evt.subData
    }
};
class Producer{
private:
    vector<Listener*> listeners;
public:
    void Add(Listener* ls){
        listeners.push_back(ls);
    }
    void Remove(Listener* ls){
        //listeners.erase(remove( listeners.begin(), listeners.end(), ls), listeners.end() );
        for(auto it=listeners.begin(); it!=listeners.end();){
            if(*it==ls)
                it=listeners.erase(it);
            else
                ++it;
        }
    }
    void Produce(const Event& evt){//Event& evt=sevt;
        for(auto ls:listeners)
            ls->OnEvent(evt);
    }
};
class Sig{
private:
    Sig(){}
    ~Sig(){}
    static volatile Sig* inst;
    static mutex mx;
public:
    static volatile Sig* getInst(){//consistent return type volatile
        if(inst==nullptr){
            lock_guard<mutex> lg(mx);
            if(inst==nullptr)
                inst=new Sig();
        }
        return inst;
    }
    static void dest(){//instead of ~Sig() to avoid being auto destructed
        if(inst!=nullptr) {
            delete inst;
            inst = nullptr;
        }
    }
};

volatile Sig* Sig::inst=nullptr;//static member must init out of class
mutex Sig::mx;
int main() {
    /*
    volatile Sig* si=Sig::getInst();
    delete si; err:private destructor
    Sig::dest();//not si->dest()
    */
    /*
    Producer prod;
    SubListener* sln=new SubListener();
    prod.Add(sln);
    SubEvent sevt(2,"s");
    prod.Produce(sevt);
    prod.Remove(sln);
    delete sln;
    */

    /*
    M<int> ma;
    ma.foo(2);
    ma.foo("2");//error
    M<string> mb;
    mb.foo("2");
    */

    /*
    //123434
    s* sa=new s();
    s* sb=sa->clone();
    delete sa;
    delete sb;
     */

    /*
    TimeSeries ts(5000);
    ts.AddValue(10,2.0);
    ts.AddValue(12,3.0);
    ts.AddValue(14,3.5);
    //cout << ts.GetValue(13) << endl;//2.0
    ts.AddValue(5011,3.5);
    cout << ts.GetValue(9) << endl;//nan
    */

    /*
    //hashtable to store and lookup symbol and its maps, using pointer to reduce memory usage
    //unordered_map<string, maps> symbols;
    unordered_map<string,std::unique_ptr<maps>> symbols;

    std::string line;
    std::vector<std::string> quote;
    std::string item;
    while (std::getline(std::cin, line)) {
        //symbols.clear();
        if (!line.empty()) {
            quote.clear();
            std::istringstream ss(line);
            while (std::getline(ss, item, '|')) {
                quote.push_back(item);
            }
            int i=0,buy,sell,seq;
            std::string sym,bank;
            for(auto s:quote){
                switch(i){
                    case 0 :
                        seq=stoi(s);
                    case 1 :
                        sym=s;
                        break;
                    case 2 :
                        bank=s;
                        break;
                    case 3:
                        buy=strToInt(s);
                    case 4:
                        sell=strToInt(s);
                        break;
                    default:
                        break;
                }
                i++;
            }
            //try to locate the maps data on given symbol from current quote, if it exists, try to find the previous price
            //based on the bank, if it exists and different from the price in the current quote, update it
            //otherwise add a new data pair of price under current bank to the hashtable banks
            auto si=symbols.find(sym);
            if(si!=symbols.end()){
                int preBuy=-1,preSell=-1,preSeq=-1;
                bool updateBuy=false, updateSell=false;
                auto ai=si->second->banks.find(bank);//si->second.banks if copy instead of pointer
                if(ai!=si->second->banks.end()){
                    //update existing price
                    preBuy=ai->second.buy;
                    preSell=ai->second.sell;
                    preSeq=ai->second.seq;
                    //only update when price changed in newer quote from the same bank
                    if(seq>preSeq) {
                        if (preBuy != buy) {
                            ai->second.buy = buy;
                            updateBuy = true;
                        }
                        if (preSell != sell) {
                            ai->second.sell = sell;
                            updateSell = true;
                        }
                    }
                }
                else{
                    si->second->banks.emplace(std::make_pair(bank,price(seq,buy,sell)));
                }
                //if no price exists for current bank, add entry of current price & bank to the map
                //otherwise if existing price need to be updated, remove existing price from the same bank
                //and add entry of current price & bank to the map so it is sorted properly
                if (preBuy==-1){
                    si->second->buys.emplace(std::make_pair(buy, bank));
                }
                else if(updateBuy){
                    auto bi=si->second->buys.find(preBuy);
                    if(bi!=si->second->buys.end()){
                        //find the existing price entry of current bank
                        while(bi->second!=bank){
                            bi++;
                        }
                        si->second->buys.erase(bi);
                    }
                    si->second->buys.emplace(std::make_pair(buy, bank));
                }
                if (preSell==-1) {
                    si->second->sells.emplace(std::make_pair(sell, bank));
                }
                else if(updateSell) {
                    auto ci = si->second->sells.find(preSell);
                    if (ci != si->second->sells.end()) {
                        while (ci->second != bank) {
                            ci++;
                        }
                        si->second->sells.erase(ci);
                    }
                    si->second->sells.emplace(std::make_pair(sell, bank));

//                    typedef multimap<int, string>::iterator miterator;
//                    pair<miterator, miterator> ip = si->second.sells.equal_range(preSell);//equal_range also ok
//                    miterator ci=ip.first;
//                    for (; ci != ip.second; ++ci) {
//                        if (ci->second == bank) {
//                            si->second.sells.erase(ci);
//                            std::cout<<"erased "<<preSell<<" from "<<bank<<std::endl;
//                            break;
//                        }
//                    }
                }
            }
            //if no data exists for current symbol in the hashtable symbols, init the maps for it with the price and
            //bank data in current quote and store a pointer to it with symbol in the hashtable
            else{
                //maps mps;
                auto pm = unique_ptr<maps>(new maps());
                //emplace to avoid copy
                pm->banks.emplace(std::make_pair(bank, price(seq,buy,sell)));
                //mps.banks.insert(std::pair<std::string,price>(bank,price(buy,sell)));
                pm->buys.emplace(std::make_pair(buy,bank));
                //mps.buys.insert(std::pair<int,std::string>(buy,bank));
                pm->sells.emplace(std::make_pair(sell,bank));
                //mps.sells.insert(std::pair<int,std::string>(sell,bank));

                symbols.insert(std::make_pair(sym,std::move(pm)));
                //symbols.insert(std::pair<std::string,maps>(sym,mps));
            }
            //retrieve the top of price map on given symbol, which are the highest buy or lowest sell
            auto it=symbols.find(sym);
            auto bt=it->second->buys.begin();
            auto st=it->second->sells.begin();
            std::cout<<"Best "<<sym<<" Buy = "<<intToStr(bt->first)<<" from "<<bt->second<<std::endl;
            std::cout<<"Best "<<sym<<" Sell = "<<intToStr(st->first)<<" from "<<st->second<<std::endl;
        }
        //return 0;
    }
    */


/*
                    auto r = quotes.insert(pair<string,price>(sym,cp));
                    if (!r.second){
                        quotes[sym]=cp;//not working
                    }
*/
    //stl();
    //ptr();
/*
    unique_lock<recursive_mutex> lk = acquire_lock();
    release_lock(move(lk));//no copy ctr, move ctr take rvalue
*/
/*
    volatile int refCounter (0);//std::atomic<int> refCounter(0);
    recursive_mutex refMtx;
    vector<thread> threads;
    Data d;
    for (int i = 0; i < 3; i++){
        //threads.push_back(thread(incCounter,10));//&|threadFunc same, uncopyable but movable thread added by move ctr as rvalue
        //threads.push_back(std::thread(incRefCounter, std::ref(refMtx), std::ref(refCounter), 10));//refCounter: mismatch must be rvalue
        //threads.push_back(std::thread(&Data::proc, &d, std::ref(refMtx), 10));//member function
        //threads.emplace_back(&divCounter,i);//somehow (divCounter,i) not work as suppose, err with divCounter(int&)
        threads.emplace_back(&push,i);
    }

    for (auto& th : threads)
    {
        assert(th.joinable());
        th.join();
    }

    for (Node* it = head; it!=nullptr; it=it->next)
        cout << ' ' << it->value;
    cout << '\n';

    function<int (int)> fc=func;//&func
    int f=fc(1);
    int (*fp)(int) = func;
    f=fp(2);

    future<int> ret=async(launch::async,pop);
    int i = ret.get();
    cout << i << endl;
    ret = async([i](){ return 3; });
    i = ret.get();
    cout << i << endl;
*/

/*
    packaged_task<int(int)> task(func, 0);
    future<int> ret = task.get_future();
    thread t(std::move(task), 0);//packaged_task moveable not copyable
    int r = ret.get();

    promise<int> p;
    future<int> f = p.get_future();
    thread t(func, std::ref(p));
    t.join();
    i = f.get();
    cout << i << endl;

    string s="a";
    thread t1(&tfunc,std::ref(s));
    thread t2(&tfunct,s);
    t1.join();
    t2.join();

    vector<thread> workers;
    for (int i = 0; i < 5; i++) {
        auto t = std::thread([i]()
                             {
                                 std::cout << "thread function: " << i << "\n";
                             });
        workers.push_back(std::move(t));
    }
    for_each(workers.begin(), workers.end(), [](thread &t)//<algorithm>
    {
        assert(t.joinable());
        t.join();
    });
*/
/*
    for (unsigned i=0;i<vt.size();++i){}
    bool char double const enum float long short void volatile string int null break case catch continue default
        do else for if return switch throw try while class final new delete protected private public static this
*/
/*
    std::string name;
    getline (std::cin, name);
    cout << "Hello, " << name << "!\n";
*/
    return 0;
}

/*
 notes:
 settings > build exec & deploy > toolchain > MinGW home = v4 to C:\Program Files (x86)\mingw-w64\i686-8.1.0-posix-dwarf-rt_v6-rev0\mingw32
 ctrl-shift-v then delete hist on clipboard, then copy from other source and paste
 */
