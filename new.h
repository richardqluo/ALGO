struct custCmp{
    bool operator()(const pair<int,string>& l,const pair<int,string>& r){
        return l.first > r.first;//smallest on top, default < std::less largest on top for priority_queue
    }
};

class Vp{
private:
    unordered_map<string,pair<int,int>> mp;
    priority_queue<pair<int,string>,vector<pair<int,string>>,custCmp> pq;//vector needed with custom comparer
    priority_queue<pair<long,int>> tq;//add a similar custCmp
    const int k;
    const long n;
    int sm=0;
public:
    Vp(int sz, long tm):k(sz),n(tm){};
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
        if(pq.size()==k){
            if(vol>pq.top().first){
                pq.pop();
                pq.push(make_pair(vol,sym));//may has duplicate sym item
            }
        }else{
            pq.push(make_pair(vol,sym));
        }
    };
    void peek(){
        while(!pq.empty()){
            cout<<pq.top().second<<":"<<pq.top().first<<'\n';
            pq.pop();
        }
    };
    //avg price during given time window
    void push(long tm,int prc){
        if(tm-tq.top().first>n){
            int pre=tq.top().second;
            sm-=pre;
            tq.pop();
            tq.push(make_pair(tm,prc));
            sm+=prc;
        }else{
            tq.push(make_pair(tm,prc));
            sm+=prc;
        }
    };
    int avg(){
        return sm/tq.size();
    };
};

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
  {return lhs>rhs;} //default < std::less<int> .begin() is smallest
};
class StkEx{
private:
    std::multimap<int,int,std::greater<int>> asks; //.begin() is largest
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

