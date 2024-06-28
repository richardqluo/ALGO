struct custCmp{
    bool operator()(const pair<int,string>& l,const pair<int,string>& r){
        return l.first > r.first;//smallest on top, default < std::less largest on top for priority_queue, similar to end for map
    }
};

class Wp{
private:
    unordered_map<string,pair<int,int>> mp; //<symbol, violatility, last price>
    priority_queue<pair<int,string>,vector<pair<int,string>>,custCmp> pq;//<violatility,symbol> vector needed with custom comparer
    const int k;

    priority_queue<pair<uint64_t,int>, vector<pair<uint64_t,int>>,cmt> tq; //<timestamp,price> add a similar cust cmt
    const uint64_t n;
    int sm=0;
public:
    Wp(int sz, uint64_t tm):k(sz),n(tm){};
    //top k violatile priced stock
    void add(string sym, int prc){
        int vol=0;
        auto im=mp.find(sym);
        if(im!=mp.end()){
            int pre=im->second.second;
            vol=abs(prc-pre)/pre;
            im->second.first=vol;
            im->second.second=prc;
        }else{
            mp.insert(make_pair(sym,make_pair(vol,prc)));
        }
        /*
        if(pq.size()==k){
            if(vol>pq.top().first){
                pq.pop();
                pq.push(make_pair(vol,sym));//may has duplicate sym item
            }
        }else{
            pq.push(make_pair(vol,sym));
        }
        */
    };
    void peek(){
        for(const auto& im : mp){
            string sym = itm.first;
            int vol = itm.second.first;
            if(pq.size()==k){
                if(vol>pq.top().first){
                    pq.pop();
                    pq.push(make_pair(vol,sym));//no duplicate sym item
                }
            }else{
                pq.push(make_pair(vol,sym));
            }
        }
        while(!pq.empty()){//go thru and empty the pq
            cout<<pq.top().second<<":"<<pq.top().first<<'\n';
            pq.pop();
        }
    };
    //avg price during given time window, use queue instead of priority_queue if tm in sequence
    void push(uint64_t tm,int prc){
        while(!tq.empty() && tm-tq.top().first>n){
            sm-=tq.top().second;
            tq.pop();  
        }
        sm+=pr;
        tq.push(make_pair(tm,pr));
    };
    int avg(){
        return sm/tq.size();
    };
};
//for avg price only
unordered_map<string,shared_ptr<Wp>> tm; //<symbol,Wp>
void setPrc(uint64_t ts,string sym,int pr){
    shared_ptr<Wp> wptr;
    auto im=tm.find(sym);
    if(im!=tm.end()){
        wptr=im->second;
    }else{
        wptr=make_shared<Wp>(10,1000);
        tm[sym]=wptr;
    }
    wptr->push(ts,pr);
}
int getAvg(string sym){
    auto im=tm.find(sym);
    if(im!=tm.end()){
        return im->second->avg();
    }
    return 0;
}

int main() {
    setPrc(100001,"C",999);
    setPrc(100010,"C",888);
    setPrc(100100,"C",777);
    cout<<getAvg("C")<<endl;}

//average of last n prices
//vector (sequence containers array with consecutive memory) to replace queue (container adaptor to underlying container linked list with scattered memory) given size
class Pq{
private: 
    queue<int> ps; 
    vector<int> pv;
    int sm = 0;
    int ct = 0;
public: 
    Pq(int s):sz(s){
        pv.reserve(sz);//reallocate if size > orig capacity to make new capacity >= size to avoid multiple reallocations during grrowth 
    };
    const int sz;
    
    void add(int p){
        if(ps.size() >= sz){
            sm-=ps.front();
            ps.pop();
        }
        ps.push(p);
        sm+=p;
    };
    void push(int p){
        if(pv.size() == sz){
            if(ct==sz){
                ct=0;
            }
            sm-=pv[ct];
            pv[ct]=p;
            sm+=p;
        }else{
            pv.push_back(p);
            sm+=p;
            if(pv.size() == sz){
                pv.resize(sz);//add/remove item to fit the same size
            }
        }
        ct++;
    };
    int peek(int i){
        return vt.at(i); //check out_of_range exp before vt[i] to avoid undefined behavvior
    };
    int avg(){
        return sm/sz;
    };
};

int main() {
    Pq pq(2);
    pq.push(2);
    pq.push(2);
    pq.push(4);
    cout<< "avg = " << pq.avg() << "\n";
}

#include <map>
#include <functional>
struct moreComp {
  bool operator() (const int& lhs, const int& rhs) const
  {return lhs>rhs;} //default < std::less<int> .end() is largest
};
class StkEx{
private:
    std::multimap<int,int,std::greater<int>> asks; //.end() is smallest
    //multimap<int,int,moreComp> //multi key
    
public:
    vector<int> match(int S){
        vector<int> r;
        auto range = asks.equal_range(S); //asks key[1,1,3,5] S(1|2|3)
        std::cout << range.first->first << '\n'; //asks.lower_bound <= 1|1|3 inner
        std::cout << range.second->first << '\n'; //asks.upper_bound < eof=size|1|1 outer
        for (auto it = range.first; it != range.second; ++it){
            //S(1|2) [1,1]|[]
            std::cout << it->first << " : " << it->second << '\n';
            r.push_back(it->second);
        }
        
        auto itlow = asks.lower_bound(S);
        if (itlow != asks.end()) //moreComp 
            std::cout << itlow->first << '\n';
        else
            std::cout << "no" << '\n';
        
        auto itup = asks.upper_bound(S);
        if (itup != asks.end()) //asks.begin() for default lessComp 
            std::cout << itup->first << '\n';
        else
            std::cout << "no" << '\n';
        
        for (auto it=itlow; it!=itup; ++it){
            //S(1|2) [1,1]|[]
            std::cout << it->first << " : " << it->second << '\n';
            r.push_back(it->second);
        }
        return r;
    }
};

struct Job{
    int id;
    int time;
    int subId;
    Job(int i, int t, int s) : id(i),time(t),subId(s){ }
};
struct Init{
    int fid;
    int sum;
    int count;
    Init(int i, int s, int c) : fid(i),sum(s),count(c){ }
};

class JobChain {
private:
    unordered_map<int, Init> jobs;
    map<int, int> seqs; //<fid,lid>
    int tm = 0;
public:
    void Add(Job& j) {
        auto io = jobs.find(j.id);
        if(io != jobs.end()){
            if(j.subId == 0){//last in chain
                io->second.sum+=j.time;
                io->second.count++;
            }else{
                Init i(io->second.id, io->second.sum+j.time, io->second.count++);
                jobs.insert(make_pair(j.subId,i));
                seqs[io->second.id] = j.subId;
                jobs.erase(io);
            }
        }else{
            Init i(j.id, j.time, 1);
            jobs.insert(make_pair(j.subId,i));
            seqs[j.id] = j.subId;
        }
    }
    void Print() {
        for(auto it = seqs.begin(); it != seqs.end(); ++it) {
            int fid = it->first;
            int lid = it->second;
            auto io = jobs.find(lid);
            int sm = io->second.sum;
            int ct = io->second.count;
        }
    }

};

//from a list of intervals (begin, end), find overlaps count at given time (uint_64 as millsec)
//map<begin, pair<end,count>, std::greater<int>> or <pair<begin,end>,count> (2,5):1 (7,9):1 in map, to return on vector<rt>
//given (3,4)|(4,7) i(b,e); it = map.lower_bound(b); vector<pair<begin,end>>news; vector<pair<begin,end>>dups; vector<int>dupCounts;
//it = map.lower_bound(rt); 4 > (3,4):2 | 6 > (6,6):1
//rc = it->second.count;
#include <iostream>
#include <vector>
#include <tuple>
#include <map>

using namespace std;
class Counter{
private:
    map<long, pair<long,int>, greater<long>> mp;
    vector<tuple<long,long,int>> adds;

public:
    void save(vector<pair<long,long>>& I){
        for (auto i : I) {
            long b = i.first;
            long e = i.second;
            auto it = mp.lower_bound(b);
            while(b <= e){
                if(it->first < b){//it->first is const key ordered by map
                    if(it->second.first < b){
                        adds.emplace_back(make_tuple(b, e, 1));
                        break;
                    }
                    if(it->second.first == b){
                        it->second.first = b-1;
                        adds.emplace_back(make_tuple(b, b, it->second.second++));
                        adds.emplace_back(make_tuple(b+1, e, 1));
                        break;
                    }
                    if(it->second.first > e){
                        it->second.first = b-1;
                        adds.emplace_back(make_tuple(b, e, it->second.second++));
                        adds.emplace_back(make_tuple(e+1, it->second.first, 1));
                        break;
                    }
                    if(it->second.first == e){
                        it->second.first = b-1;
                        adds.emplace_back(make_tuple(b, e, it->second.second++));
                        break;
                    }
                    if(it->second.first < e){
                        adds.emplace_back(make_tuple(b, it->second.first, it->second.second++));
                        long oe = it->second.first;
                        it->second.first = b-1;
                        b = oe+1;
                        it++;
                        continue;
                    }
                }
                if(it->first == b){
                    it->second.second += 1;
                    if(it->second.first > e){
                        it->second.first = e;
                        adds.emplace_back(make_tuple(e+1, it->second.first, 1));
                        break;
                    }
                    if(it->second.first == e){
                        break;
                    }
                    if(it->second.first < e){
                        b = it->second.first+1;
                        it++;
                        continue;
                    }
                }
                if(it->first > b){
                    adds.emplace_back(make_tuple(b, it->first-1, 1));
                    it->second.second += 1;
                    if(it->second.first > e){
                        adds.emplace_back(make_tuple(e+1, it->second.first, 1));
                        it->second.first = e;
                        break;
                    }
                    if(it->second.first == e){
                        break;
                    }
                    if(it->second.first < e){
                        b = it->second.first+1;
                        it++;
                        continue;
                    }
                }
            }

            for (tuple<long,long,int> add : adds) {
                long b = get<0>(add);
                long e = get<1>(add);
                int  c = get<2>(add);

                mp.insert(make_pair(b, make_pair(e, c)));
            }
            adds.clear();
        }
    };

    int query(long T){
        auto it = mp.lower_bound(T);
        if(it->first != 0){//exceed existing key
            if (it->first <= T & it->second.first >= T){
                return it->second.second;
            }
        }
        return 0;
    }
};

int main() {
    cout << "begin" << endl;

    Counter ct;
    vector<pair<long,long>> i;
    i.emplace_back(make_pair(2,5));
    i.emplace_back(make_pair(7,9));

    i.emplace_back(make_pair(3,4));
    ct.save(i);
    int r = ct.query(3);
    cout << r << endl;
    cout << "end" << endl;
    return 0;
}


sort array contains 0s 1s 2s
    int a[]= {1,1,0,0,1,1,0,0,1,1,2,2,2,2,1,1,1,1,0,0};
    int n = sizeof(a) / sizeof(a[0]);
    int c[3]={0};  
    for(int i=0; i<n; i++){
        c[a[i]]++;
    }   
    for(int i=0; i<3; i++){
        while(c[i]-- > 0){
            cout<< i << " ";
        }
    }
}

Check if 2 arrays have the same elements that are in a range 0-9
int c[10]={0}; 
for i=0; i<size; {c[a[i]]++; c[b[i]]—; }//assume 2 arrays have the same size
if c[k]!=0 return false;

counting sort array with max value by placing value at index (ordered) with frequency of each element 
O(N+M), where N and M are the size of array[] and count[]
vector<int> countSort(vector<int>& a)
{
    int N = a.size();
    int M = 0;
    for (int i = 0; i < N; i++)
        M = max(M, a[i]);
    vector<int> c(M + 1, 0);//values in a range from 0 to M
    for (int i = 0; i < N; i++)
        c[a[i]]++;

   vector<int> s;//s(N); for s[i]=v, s.push_back(v) pend after N 0s
    for (int i = 0; i <= M; i++){
      while (c[i]>0){
         s.push_back(i);
         c[i]--;
      }
    }
    return s; //return value instead of reference to avoid null pointer because s will be de-allocated by the end of this function 
}

int main()
 {
  vector<int> input= { 4, 3, 1, 3};
  vector<int> output= countSort(input);
  for(int i:output)
      cout<<i;

//find element repeat more than n/2 times
int repeatHalf(int a[],int n){
    int num = -1;
    for(int i=0; i<n-3; i++){
//must appear within 2 consecutive 
      if(a[i]==a[i+1] ||  a[i]==a[i+2])
          num=a[i];
          break;
    }
    return num;
}
stock prices serial = [7,1,5,3,6,4] buy then sell once
int maxProfit(vector<int>& prices) {
        int min= INT_MAX;
        int max= 0;
        int profit= 0;
        
        for(int i = 0; i < prices.size(); i++){
            if(prices[i] < min){
                min = prices[i];
            }
            profit= prices[i] - min;
            if(max< profit){
                max= profit;
            }
        }
        return max;
    }

//Max Sum  consecutive elements (Kadane’s Algorithm)
int maxSub(int a[],int n){
    int m=INT_MIN,c=0;
    queue<pair<int,int>> q;
    q.push(make_pair(0,0));
    for(int i=0;i<n;i++){
        c+=a[i];
        if(c>m){
            m=c;
            if(q.size()==2)
                q.pop();
            //ref& to update existing 
            pair<int,int>& p=q.front();//final
            p.second=i;
        }
        if(c<0){
            c=0;
            if(q.size()==2){
                auto& p=q.back();//restart
                p.first=i+1;
                p.second=i+1;
            }else
                q.push(make_pair(i+1,i+1));
        }
    }
    auto p=q.front();//read no need ref&
    cout<<p.first<<":"<<p.second<<endl;
    return m;
}

int main() {
    int a[]={3,2,2,-8,1,-2,3,5}; 
    int n = sizeof(a) / sizeof(a[0]);
    cout<< maxSubArraySum(a, n);  //6:7 8
}

//priority queue by 2 queues
template <class T>//pq<int> q;
class pq{
  public:
    void push(T c){
      if(n.empty())
          push(c,m,n);
      else
          push(c,n,m);
    }
private:
    queue<T> m;
    queue<T> n;
    void push(T c, queue<T>& a,queue<T>& b){
//.front|back() runtime err if .empty()
      if(a.empty()||c>a.back()){
          a.push(c);
      }else{
          bool d=false;//wether c pushed
          while(!a.empty()){     
            int f=a.front();
            if(d||c>f)
                b.push(f);
            else{ 
                b.push(c);
                b.push(f);
                d=true;
            }
            a.pop();
          }
      }
    }
};

//find point left summary equilibrium right O(n)
int equalMid(int a[], int size)
{
    int i=0,j=size-1;
    int sum1=a[i],sum2=a[j];
    do{
        if(sum1>sum2){
            j--;
            sum2+=a[j];
        }else if(sum1<sum2){
            i++;
            sum1+=a[i];
        }else{
            //return sum1; //first equal 
            i++;
            sum1+=a[i];
            j--;
            sum2+=a[j];
        }
    }while(i < j-2); //…i m j…
    if(sum1==sum2)
        return sum1; //return i+1; index
    else
        return INT_MIN;
}

//minimize convert currency steps, same as flights 
struct CurrencyNode {
  string source;
  double value;
  string steps; //int steps;
  CurrencyNode(string src, double val, string stp):source(src), value(val), steps(stp){};
}; //type def must end ; func need not

unordered_ map<string, unordered_ map<string, double> > mp;
//unordered_ map<string, unordered_ set<string> > mp; least flights transfer

double convert (string src, double amount, string dest) {
  if (mp[src].empty()) {
    return -1;
  }
  queue<CurrencyNode> q;
  q.push(CurrencyNode(src, amount, src));//0
  while (!q.empty()) {//BFS search
      CurrencyNode node = q.front();
      q.pop();
      // if the dest currency is found then return the rate
      if (mp[node.source].find(dest) != mp[node.source].end()) {
        cout<<node.steps << '>' << dest <<endl;
        return node.value * mp[node.source][dest];
      }
      for (auto it : mp[node.source]) {
//queue all dest in mp from current source with calculated rate
        q.push(CurrencyNode(it.first, it.second * node.value, node.steps+ '>' +it.first));//node.steps+1
      }
  }
  return -1;
}

int main() {
  mp["EUR"]["USD"]=1.2;
  mp["USD"]["GBP"]=0.75;
  mp["GBP"]["JPY"]=150;
  mp["USD"]["JPY"]=120;
//inner hash map for key USD has (GBP,0.75) (JPY,120) 
  cout << convert("EUR", 100, "JPY") << endl;
}

//find single 1 in maze, 0 is path, -1 is wall, go only down and right
int m[4][4]={0,0,0,0,
             0,0,-1,0,
             0,0,-1,1,
             0,0,0,0};
vector<pair<int,int>> path;

bool find(int x, int y){
    if(x>3 || y>3 || m[x][y]<0){
        return false;
    }

    pair<int,int> p = make_pair(x,y);
    path.push_back(p);

    //if return if ~ if else 
    if (m[x][y]==1){
        for (auto p : path){
            cout<<p.first<< ',' <<p.second<<'\n';
        }
        return true;
    }
    //recursive search if m[x][y]==0
    if(find(x+1,y) || find(x,y+1))
         return true;
    else{
         path.pop_back();
         return false;
    }
    //return find(x+1,y) || find(x,y+1);
};

int main() {
    if(find(0,0))
        cout<<"ok";
    else
        cout<<"no”;
}

//max sum among all paths 
int maxPathDFS(int x, int y){
    int t=0;
//backtrack sum and pos 
    stack<pair<int,pair<int,int>>> s;
    pair<int,int> n=make_pair(x,y);
    s.push(make_pair(0,n));
    while(!s.empty()){
        auto p=s.top();
        s.pop();
        int v=p.first;
        pair<int,int> n=p.second;
        int l=n.first;
        int r=n.second;
        v+=m[l][r];
        if(l<3)
      s.push(make_pair(v,make_pair(l+1,r)));
        if(r<3)
      s.push(make_pair(v,make_pair(l,r+1)));
        if(r==3&&l==3)
            t=max(t,v);
    }
    return t;
}
//recursive
int maxPath(int x, int y){
    cout<<x<<':'<<y<<endl;
   if(x<3 && y<3){
//not x++ y++ affect following lines 
    return max(maxPath(x+1,y), maxPath(x,y+1)) +m[x][y];
  }
  if(x<3)
    return maxPath(x+1,y)+m[x][y];
  if(y<3)
    return maxPath(x,y+1)+m[x][y];
   //if(x==3 && y==3)
     return m[x][y];//good if input x|y< 3
   //return 0;
};

//common ancestor of tree node without value 
struct tnode{
  tnode* right;
  tnode* left;
  tnode():right(nullptr),left(nullptr){}
};
struct lnode{
  tnode* data;
  lnode* prev;
  lnode* next;
  lnode(tnode* tn):data(tn),prev(nullptr),next(nullptr){}
};
//vector<tnode*> va;vector<tnode*> vb;
bool find(tnode* root, tnode* aim, lnode* head){
    if (root==nullptr)
        return false;
    if (root==aim){//pointer argument is a copy of value is address of tnode
        cout<<"3: "<<root<<endl;
        return true;
    }
    cout<<"1: "<<root<<" "<<head->data<<endl;
    if(head->data!=root){//skip 1st 
        lnode* nd = new lnode(root);
    //stack nd[215|7684]->heap[7684|lnode]
        head->next=nd;
        //head->next=new lnode(root);
        nd->prev=head;
        head=head->next;
        cout<<"2+ "<<head->data<<endl;
    }
    //head[134]7684]->heap[7684|lnode], copied value(address) 7684 from nd which bytes will be reclaimed by out of scope }, 7684 memory won't leak
    if(find(root->left,aim,head)||find(root->right,aim,head)){
       return true;
    }else{
       cout<<"2- "<<head->data<<endl;
       lnode* on=head;
       head=head->prev;//back track
        head->next=nullptr;
        delete on;
        return false;
    }
}

int main() {
    tnode* root = new tnode();
    tnode* a = new tnode();
    tnode* b = new tnode();
    root->left=a;
    root->right=b;  
    lnode* ha=new lnode(root);
    lnode* hb=new lnode(root);
    lnode* prev=nullptr;
    cout<<"r: "<<root<<endl;
    cout<<"a: "<<a<<endl;
    cout<<"b: "<<b<<endl;
    //pass in ptr/address of the 1st lnode 
if(find(root,a,ha)&&find(root,b,hb)){
  while(ha!=nullptr && hb!=nullptr && ha->data==hb->data){//address of tnode
      cout<<"ha: "<<ha->data<<endl;
      cout<<"hb: "<<hb->data<<endl;
      prev=ha;
      ha=ha->next;
      hb=hb->next;
  }
  cout<<prev->data;//previous is the last common parent 
}else{
  cout<<"invalid";
}
//go thru ha hb to delete all lnode and tnode 
}

//if a sum can be 5 and 3 cents coin
int c3=0,c5=0;
bool coins(int s){
    if(s==0){
        cout<<c3<<":"<<c5<<endl;//9=3:0 10=0:2 11=2:1
        return true;
    }
    if(s<0)
        return false;
    c5++;
    if(coins(s-5))
        return true;   
    else
        c5--; //continue instead of return 
    
    c3++;
    if(coins(s-3)) //not else if
        return true;
    else
        c3--;

    return false;
}

//2 threads print odd even number separately 
mutex mu;
condition_variable cond;
int ct = 1;
int num = 11;
void PrintOddAndEven(bool isEven){
    while(ct < num){
        unique_lock<mutex> ul(mu);
// lock_guard<mutex> lg(mu) can not be used with cond
        if(ct % 2 != isEven)
            cond.wait(ul);
//cond.wait(ul, [&] {return (ct % 2 ==isEven); }); &|= to capture isEven ref|val, wait till return true

        cout<<ct<<" : printed by thread"<<this_thread::get_id()<<endl;
        ct++;
        ul.unlock();//must explicit before notify
        cond.notify_all();//notify_one()random
    }
}

int main() {
    thread t1(PrintOddAndEven, false);
    thread t2(PrintOddAndEven, true);

    t1.join();
    t2.join();
    return 0;
}

//simulate racers, output the 1st  
std::atomic<bool> ready (false);
//std::atomic<bool> winner (false);
thread_local int meter;
std::once_flag win;
void race(int id) {
  while (!ready) {//!ready.load()
    std::this_thread::yield();
  }
  while(meter< 100){
     meter+=1;
     this_thread::sleep_for (10ms);
  }
// if (!winner.exchange(true)) cout<<id; 
or if(winner.compare_exchange_weak(false,true), not affected by other threads intermediate value during atomic read-modify-write return previous value, false only to the first thread
   std::call_once (win,[=]{cout << id;});
}
int main ()
{
  thread racers[10]; //vector<thread> racers;
  for (int i=0; i<10; ++i)      racers[i]=thread(race,i);//racers.push_back(thread(race,i));

  ready = true;  //ready.store(true);            
  
  for (auto|thread& racer : racers) racer.join();
  return 0;
}

mutex mtx;
condition_variable cv;
int food = 0;
bool ready() {return food>0;}
void eat (int n) {
  //for (int i=0; i<2; ++i){
    unique_lock<mutex> lck(mtx);
    cout << n << '<'<< food <<'>';
    //while(!ready())
        //cv.wait(lck);
    //cv.wait(lck,ready);
    cv.wait(lck, []{return food>0;});
    food-=1;
    cout << n << '('<< food << ')';
  //}
}

int main ()
{
  thread customers[10];
  for (int i=0; i<10; ++i)
    customers[i] = thread(eat,i);
  //for (int i=0; i<2; ++i){
    //while (ready()) std::this_thread::yield();//current thread let other thread to advance
    unique_lock<mutex> lck(mtx);
    food+=10;
    cout<<'{'<<food<<'}'<<endl;
    lck.unlock();//must explicit before notify, otherwise it is still locked upon notify because implicit unlock later at } 
    cv.notify_all();//can notify without wait first, to wake up other threads at cv.wait 
  //}
  for (auto& customer : customers)//auto& instead of auto to reference 
     customer.join();
  
  return 0;
}
output without loop
0<0>1<0>2<0>3<0>4<0>5<0>6<0>7<0>8<0>{10}9<10>9(9)1(8)2(7)0(6)5(5)6(4)7(3)4(2)8(1)3(0)
0-8 thread(eat,i) enter lock and wait (wait release the lock and block the thread then acquire it again upon wake and unblock the thread), after main thread enter lock then unlock and notify, thread 9 enter lock pass wait, random thread wake up and exit wait at a time

output with loop
0<0>1<0>2<0>3<0>4<0>5<0>6<0>7<0>8<0>{10}
9<10>9(9)9<9>9(8)2(7)2<7>2(6)0(5)0<5>0(4)1(3)1<3>1(2)6(1)6<1>6(0){10}
7(9)7<9>7(8)3(7)3<7>3(6)8(5)8<5>8(4)5(3)5<3>5(2)4(1)4<1>4(0)
random thread wake and acquire lock to eat then release the lock , and the same or another random thread wake and acquire the lock …

//sum 10000 concurrently 1000 per thread 
atomic<int> s(0);
void sum(int b,int e){
int r=0;
for(int i=b;i<e;i++)
    r+=i;
s.fetch_add(r);
}

int main() {
    vector<thread> ts;
    for(int i=0; i<10000; i+=1000)
ts.push_back(thread(sum,i,i+1000));
//thread t();ts.push_back(t); compile err
    for(thread& t : ts)
      t.join();
    std::cout << s.load();
}

//stack by lock-free linked list
template<class T>
class LFStack {
public:
struct node
{
    T data;
    node<T>* next;
    node(const T& data) : data(data), next(nullptr) {}
};
void push(const T& val)
{
node<T>* new_node = new node<T>(val);//return a ptr to node
        new_node->next = _head.load();
//if _head is still the new_node-> next point to, then _head=new_node; otherwise keep setting new_node->next to current _head which has been changed by other threads
 while(!_head.compare_exchange_weak(new_node->next, new_node)); 
}

bool pop()
{
node<T>* old_node = _head.load();
node<T>* next_node = nullptr;
do {
if(old_node == nullptr)
return false;
next_node = old_node->next;
} while(!_head.compare_exchange_weak(old_node, next_node));
//if _head still the same, _head=next_node; otherwise, old_node = current _head updated by other thread
delete old_node;
return true;
}
private:
std::atomic<node<T>*> _head;//ptr to the 1st node
};

//least reusable item will be removed 
class LRUCache{
private:

struct ListNode{
  string key;
  int value;
  ListNode* prev;
  ListNode* next;
  ListNode(string k, int v):key(k),value(v),prev(nullptr),next(nullptr){}
};

    int capacity;
    unordered_map<int, ListNode*> mp;
    ListNode* head= nullptr;
    ListNode* tail= nullptr;
public:
    LRUCache(int cap):capacity(cap){};
    int get(string key)
    {
        if(mp.find(key)!=mp.end())
        {
            auto np=mp[key];//ListNode* np=mp.at(key);
            remove(np);
            setHead(np);
            return np->value;
        }
        return -1;
    }

    void set(string key, int value)
    {
        if(mp.find(key)!=mp.end())
        {
            ListNode* op=mp.at(key);
            op->value=value;
            remove(op);
            setHead(op);
        }
        else
        {
            //auto cp=unique_ptr<ListNode>(new ListNode(key,value)); //auto delete 
            ListNode* cp=new ListNode(key,value);
            if(mp.size()>=capacity)
            {
                mp.erase(tail->key);
                ListNode* td=tail;
                remove(tail);
                delete td; //manual delete node that tail used to point before move
                setHead(cp);
            }
            else
            {
                setHead(cp);
            }
            mp.insert(make_pair(key,cp));//mp[cp->key]=cp;
        }
    }
      void remove(ListNode* np)
    {
        if(np->prev!= nullptr)
            np->prev->next=np->next;
        else //np is tail
            tail=np->next;

        if(np->next!= nullptr)
            np->next->prev=np->prev;
        else //np is head
            head=np->prev;
        //dont delet np maybe setHead to it later
    }
    void setHead(ListNode* np)
    {
        np->prev=head;
        np->next=nullptr;
        if(head!=nullptr)
            head->next=np;
        head=np;
        if(tail==nullptr) //1st node
            tail=head;
    }
};

