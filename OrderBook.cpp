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
    unsigned long long id;
    unsigned long time;
    string symbol;
    char type;
    char side;
    double price;
    unsigned long long shares;
    Order(unsigned long long i, unsigned long tm, string sb, char tp, char sd, double p, unsigned long long sh) : id(i),time(tm),symbol(sb),type(tp),side(sd),price(p),shares(sh){}
};

class Matcher{
private:
    unordered_map<string,map<double,queue<shared_ptr<Order>>>> asks;
    unordered_map<string,map<double,queue<shared_ptr<Order>>,greater<double>>> bids;
    unordered_map<unsigned long long,shared_ptr<Order>> orders;


public:
    void load(vector<Order>& O){
        for(Order o : O){
            auto spo = make_shared<Order>(o.id,o.time,o.symbol,o.type,o.side,o.price,o.shares);
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
                    map<double,queue<shared_ptr<Order>>,greater<double>> bm;
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
                    map<double,queue<shared_ptr<Order>>> am;
                    am.insert(make_pair(o.price, oq));
                    asks.insert(make_pair(o.symbol, am));
                }
            }
        }
    };

    void match(unsigned long T){
        //cross orders by the smaller map
        if(asks.size() < bids.size()){
            for(auto ita = asks.begin(); ita != asks.end(); ++ita){
                match(T, ita->first);
            }
        }else{
            for(auto itb = bids.begin(); itb != bids.end(); ++itb){
                match(T, itb->first);
            }
        }
    }

    void match(unsigned long T, string S){

        auto ita = asks.find(S);
        if(ita == asks.end()){
            return;
        }
        auto itb = bids.find(S);
        if(itb == bids.end()){
            return;
        }

        double bestBidPrice = itb->second.begin()->first;
        double bestAskPrice = ita->second.begin()->first;

        while(bestBidPrice>=bestAskPrice){
            auto spa = ita->second.begin()->second.front();
            auto spb = itb->second.begin()->second.front();
            if(spa->time > T || spb->time > T){
                break;
            }
            int size = spb->shares - spa->shares;
            if(size == 0){
                cout << spb->id << " : " << spa->id << " = " << spb->shares << '\n';
                itb->second.begin()->second.pop();
                ita->second.begin()->second.pop();
            }else if(size > 0){
                cout << spa->id << " = " << size << '\n';
                spb->shares = size;
                ita->second.begin()->second.pop();
            }else if(size < 0){
                cout << spb->id << " = " << size << '\n';
                spa->shares = -size;
                itb->second.begin()->second.pop();
            }
            if(ita->second.begin()->second.empty()){
                ita->second.erase(bestAskPrice);
                if(ita->second.empty()){
                    asks.erase(ita);
                    break;
                }
            }
            if(itb->second.begin()->second.empty()){
                itb->second.erase(bestBidPrice);
                if(itb->second.empty()){
                    bids.erase(itb);
                    break;
                }
            }

            bestBidPrice = itb->second.begin()->first;
            bestAskPrice = ita->second.begin()->first;
        }

        return;
    }
};


int main() {
    cout << "begin" << endl;

    Order o1(1,0000001,"ALN",'L','B',60.90,100);
    Order o2(11,0000002,"XYZ",'L','B',60.90,200);
    Order o3(110,0000003,"XYZ",'L','S',59.90,100);
    Order o4(112,0000004,"XYZ",'L','S',60.90,120);
    Order o5(10,0000006,"ALN",'L','S',60.90,100);
    vector<Order> vo;
    vo.push_back(o1);
    vo.push_back(o2);
    vo.push_back(o3);
    vo.push_back(o4);
    vo.push_back(o5);

    Matcher mc;
    mc.load(vo);
    mc.match(0000006);
    mc.match(0000006, "XYZ");
    cout << "end" << endl;
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    return 0;
}
