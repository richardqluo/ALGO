#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <map>
#include <queue>
#include <functional>
#include <memory>

using namespace std;


struct Order{
public:
    unsigned long long id;
    unsigned long time;
    string symbol;
    char type;
    char side;
    unsigned long long shares;
    double price;
    Order(unsigned long long i, unsigned long tm, string sb, char tp, char sd, unsigned long long sh, double p) : id(i),time(tm),symbol(sb),type(tp),side(sd),shares(sh),price(p){};
};

class Matcher{
private:
    unordered_map<string,map<double,queue<shared_ptr<Order>>,greater<double>>> asks;
    unordered_map<string,map<double,queue<shared_ptr<Order>>>> bids;
    unordered_map<unsigned long long,shared_ptr<Order>> orders;


public:
    void load(vector<Order>& O){
        for(Order o : O){
            auto spo = make_shared<Order>(o.id,o.time,o.symbol,o.type,o.side,o.price);
            orders[o.id] = spo;

            if(o.side == 'B'){
                auto itb = bids.find(o.symbol);
                if(itb != bids.end()){
                    auto it = itb->second.find(o.price);
                    if(it != itb->second.end()){
                        it->second.push(spo);
                    }else{
                        queue<shared_ptr<Order>> oq;
                        oq.push(spo);
                        itb->second.insert(make_pair(o.price, oq));
                    }
                }else{
                    queue<shared_ptr<Order>> oq;
                    oq.push(spo);
                    map<double,queue<shared_ptr<Order>>> bm;
                    bm.insert(make_pair(o.price, oq));
                    bids.insert(make_pair(o.symbol, bm));
                }
            }else if(o.side == 'S'){
                auto ita = asks.find(o.symbol);
                if(ita != asks.end()){
                    auto it = ita->second.find(o.price);
                    if(it != ita->second.end()){
                        it->second.push(spo);
                    }else{
                        queue<shared_ptr<Order>> oq;
                        oq.push(spo);
                        ita->second.insert(make_pair(o.price, oq));
                    }
                }else{
                    queue<shared_ptr<Order>> oq;
                    oq.push(spo);
                    map<double,queue<shared_ptr<Order>>,greater<double>> am;
                    am.insert(make_pair(o.price, oq));
                    asks.insert(make_pair(o.symbol, am));
                }
            }
        }
    };
    vector<int> match(unsigned long T, string S){
        vector<int> r;

        auto itb = bids.find(S);
        if(itb == bids.end()){
            return r;
        }
        auto ita = asks.find(S);
        if(ita == asks.end()){
            return r;
        }

        double bestBidPrice = itb->second.begin()->first;
        double bestAskPrice = ita->second.begin()->first;

        while(bestBidPrice>=bestAskPrice){
            auto spa = ita->second.begin()->second.front();
            auto spb = itb->second.begin()->second.front();

            int size = spb->shares - spa->shares;
            if(size == 0){
                itb->second.begin()->second.pop();
                ita->second.begin()->second.pop();
            }else if(size > 0){
                spb->shares = size;
                ita->second.begin()->second.pop();
            }else if(size < 0){
                spa->shares = -size;
                itb->second.begin()->second.pop();
            }
            if(ita->second.begin()->second.empty()){
                ita->second.erase(bestAskPrice);
                if(ita->second.empty()){
                    asks.erase(ita);
                }
            }
            if(itb->second.begin()->second.empty()){
                itb->second.erase(bestBidPrice);
                if(itb->second.empty()){
                    bids.erase(itb);
                }
            }
            if(asks.empty() || bids.empty()){
                break;
            }
            bestBidPrice = itb->second.begin()->first;
            bestAskPrice = ita->second.begin()->first;
        }

        //cout << it->first << " : " << it->second << '\n';
        return r;
    }
};


int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    return 0;
}
