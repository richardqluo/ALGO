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
  {return lhs>rhs;} //default < std::less<int>
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
//while(b<=e) {
//  if it.begin<b { it.end = b-1;  (2,2)|(2,3):1
//                  if it.end > e (3,4) { dup=pair(b,e); dupCount=it.count+1; new=pair(e+1,it.end);  dup(3,4):2 new(5,5):1
//                  if it.end <=e (4,7) { dup=pair(b,it.end); dupCount=it.count+1; dup=(4,5):2
//                  b=it.end+1; it++; it=(7,9) b=6 break|continue
//                }
//  if it.begin==b{if it.end > e { it.end = e; it.count++;
//                 if it.end <=e { it.count++;
//                 b=it.end+1; it++; 
//  if it.begin>b {new=pair(b,it.begin-1); new(6,6):1
//                 if it.end > e (6,7) {dup=pair(it.begin, e); dupCount=it.count+1; it.begin = e+1;
//                 if it.end <=e {it.count++; 
//                 b=it.end+1; it++; 
//}
//for(auto new : news){map.insert(make_pair(new.first, pair(new.second,1))) re|insert after iterate thru node in range
//for(auto dup : dups){map.insert(make_pair(dup.first, pair(dup.second,dupCount))) 
//...
//it = map.lower_bound(rt); 4 > (3,4):2 | 6 > (6,6):1
//rc = it->second.count;
