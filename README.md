# ALGO
//vector (array with consecutive memory) to replace queue (linked list with scattered memory) given size
class Vq{
private:
    vector<int> vt;
    int ct = 0;
public:
    Vq(int sz):size(sz){};
    const int size;

    void push(int p){
      if (vt.size() == size){
          if(ct > size){
              ct = 0;
          }
          vt[ct] = p;
      }else{
          vt.push_back(p);
          if (vt.size() == size){ //only once when it reach the given size
              vt.resize(size);//add/remove item to fit the same size
              //vt.reserve(size) //reallocate if size > orig capacity to make new capacity >= size to avoid multiple reallocations during grrowth 
          }
      }  
      ct++;  
    };
    
    int peek(int i){
        return vt.at(i); //check out_of_range exp before vt[i]
    };
};

int main() {
    Vq vq(2);
    vq.push(0);
    vq.push(1);
    vq.push(2);
    std::cout << vq.peek(0) << "\n";
}

#include <map>
#include <functional>
struct moreComp {
  bool operator() (const int& lhs, const int& rhs) const
  {return lhs>rhs;} //default < std::less<int>
};
class StkEx{
private:
    std::multimap<int,int,std::greater<int>> asks;
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
