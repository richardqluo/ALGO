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
    unordered_map<unsigned long long,vector<shared_ptr<Order>>> orders;


public:
    void New(Order&od){
        auto po = make_shared<Order>(od.id, od.time, od.symbol, od.type, od.side, od.price, od.shares);
        orders[od.id].push_back(po);

        if(od.side == 'B'){
            auto ib = bids.find(od.symbol);
            if(ib != bids.end()){
                auto it = ib->second.find(od.price);
                if(it != ib->second.end()){
                    it->second.push(po);
                }else{
                    queue<shared_ptr<Order>> oq;
                    oq.push(po);
                    ib->second.insert(make_pair(od.price, oq));
                }
            }else{
                queue<shared_ptr<Order>> oq;
                oq.push(po);
                map<double,queue<shared_ptr<Order>>,greater<double>> bm;
                bm.insert(make_pair(od.price, oq));
                bids.insert(make_pair(od.symbol, bm));
            }
        }else if(od.side == 'S'){
            auto ia = asks.find(od.symbol);
            if(ia != asks.end()){
                auto it = ia->second.find(od.price);
                if(it != ia->second.end()){
                    it->second.push(po);
                }else{
                    queue<shared_ptr<Order>> oq;
                    oq.push(po);
                    ia->second.insert(make_pair(od.price, oq));
                }
            }else{
                queue<shared_ptr<Order>> oq;
                oq.push(po);
                map<double,queue<shared_ptr<Order>>> am;
                am.insert(make_pair(od.price, oq));
                asks.insert(make_pair(od.symbol, am));
            }
        }
    };

    void Amend(Order& od){
        auto io = orders.find(od.id);
        if(io != orders.end()){
            //invalidate the old order and reNew
            io->second.back()->shares = 0;
            auto spo = make_shared<Order>(od.id, od.time, od.symbol, od.type, od.side, od.price, od.shares);
            io->second.push_back(spo);
            New(od);
        }
    }

    void Cancel(Order& od){
        auto io = orders.find(od.id);
        if(io != orders.end()){
            //invalidate the old order and reNew
            io->second.back()->shares = 0;
        }
    }

    void Match(unsigned long tm){
        //cross orders by the smaller map
        if(asks.size() < bids.size()){
            for(auto ia = asks.begin(); ia != asks.end(); ++ia){
                Match(tm, ia->first);
            }
        }else{
            for(auto ib = bids.begin(); ib != bids.end(); ++ib){
                Match(tm, ib->first);
            }
        }
    }

    void Match(unsigned long tm, string sb){

        auto ia = asks.find(sb);
        if(ia == asks.end()){
            return;
        }
        auto ib = bids.find(sb);
        if(ib == bids.end()){
            return;
        }

        double highestBid = ib->second.begin()->first;
        double lowestAsk = ia->second.begin()->first;

        while(highestBid >= lowestAsk){
            auto pa = ia->second.begin()->second.front();
            auto pb = ib->second.begin()->second.front();
            //skip cancelled orders
            while(pa->shares == 0){
                ia->second.begin()->second.pop();
                if(ia->second.begin()->second.empty()){
                    ia->second.erase(lowestAsk);
                    if(ia->second.empty()){
                        asks.erase(ia);
                        break;
                    }
                }
                pa = ia->second.begin()->second.front();
            }
            while(pb->shares == 0){
                ib->second.begin()->second.pop();
                if(ib->second.begin()->second.empty()){
                    ib->second.erase(highestBid);
                    if(ib->second.empty()){
                        bids.erase(ib);
                        break;
                    }
                }
                pb = ib->second.begin()->second.front();
            }
            if(pa->time > tm || pb->time > tm){
                break;
            }
            int size = pb->shares - pa->shares;
            if(size == 0){
                cout << pb->id << " : " << pa->id << " = " << pb->shares << '\n';
                ib->second.begin()->second.pop();
                orders[pb->id].clear(); //the order is close
                ia->second.begin()->second.pop();
                orders[pa->id].clear();
            }else if(size > 0){
                cout << pa->id << " = " << size << '\n';
                pb->shares = size;
                ia->second.begin()->second.pop();
                orders[pa->id].clear();
            }else if(size < 0){
                cout << pb->id << " = " << size << '\n';
                pa->shares = -size;
                ib->second.begin()->second.pop();
                orders[pb->id].clear();
            }
            if(ia->second.begin()->second.empty()){
                ia->second.erase(lowestAsk);
                if(ia->second.empty()){
                    asks.erase(ia);
                    break;
                }
            }
            if(ib->second.begin()->second.empty()){
                ib->second.erase(highestBid);
                if(ib->second.empty()){
                    bids.erase(ib);
                    break;
                }
            }

            highestBid = ib->second.begin()->first;
            lowestAsk = ia->second.begin()->first;
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
    /*
    vector<Order> vo;
    vo.push_back(o1);
    vo.push_back(o2);
    vo.push_back(o3);
    vo.push_back(o4);
    vo.push_back(o5);
    */
    Matcher mc;
    mc.New(o1);
    mc.New(o2);
    mc.New(o3);
    mc.New(o4);
    mc.New(o5);
    mc.Match(0000006);
    mc.Match(0000006, "XYZ");
    cout << "end" << endl;
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    return 0;
}
