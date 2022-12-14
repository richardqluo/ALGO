
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <float.h>
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

struct lessComp {
    bool operator() (const shared_ptr<Order>& lhs, const shared_ptr<Order>& rhs) const
    {return lhs->time > rhs->time;}
};

class Matcher{
private:
    unordered_map<string,map<double,priority_queue<shared_ptr<Order>,vector<shared_ptr<Order>>,lessComp>>> asks;
    unordered_map<string,map<double,priority_queue<shared_ptr<Order>,vector<shared_ptr<Order>>,lessComp>,greater<double>>> bids;
    unordered_map<unsigned long long,vector<shared_ptr<Order>>> orders;


public:
    void Add(shared_ptr<Order>& po){
        if(po->side == 'B'){
            auto ib = bids.find(po->symbol);
            if(ib != bids.end()){
                auto it = ib->second.find(po->price);
                if(it != ib->second.end()){
                    it->second.push(po);
                }else{
                    priority_queue<shared_ptr<Order>,vector<shared_ptr<Order>>,lessComp> oq;
                    oq.push(po);
                    ib->second.insert(make_pair(po->price, oq));
                }
            }else{
                priority_queue<shared_ptr<Order>,vector<shared_ptr<Order>>,lessComp> oq;
                oq.push(po);
                map<double,priority_queue<shared_ptr<Order>,vector<shared_ptr<Order>>,lessComp>,greater<double>> bm;
                bm.insert(make_pair(po->price, oq));
                bids.insert(make_pair(po->symbol, bm));
            }
        }else if(po->side == 'S'){
            auto ia = asks.find(po->symbol);
            if(ia != asks.end()){
                auto it = ia->second.find(po->price);
                if(it != ia->second.end()){
                    it->second.push(po);
                }else{
                    priority_queue<shared_ptr<Order>,vector<shared_ptr<Order>>,lessComp> oq;
                    oq.push(po);
                    ia->second.insert(make_pair(po->price, oq));
                }
            }else{
                priority_queue<shared_ptr<Order>,vector<shared_ptr<Order>>,lessComp> oq;
                oq.push(po);
                map<double,priority_queue<shared_ptr<Order>,vector<shared_ptr<Order>>,lessComp>> am;
                am.insert(make_pair(po->price, oq));
                asks.insert(make_pair(po->symbol, am));
            }
        }
    };
    
    void New(Order& od){
        double pc = od.price;
        if(od.type == 'M'){
            pc = od.side == 'S' ? 0 : DBL_MAX;
        }
        vector<shared_ptr<Order>> vo;
        auto po = make_shared<Order>(od.id, od.time, od.symbol, od.type, od.side, pc, od.shares);
        vo.push_back(po);
        orders[od.id] = vo;
        Add(po);
    }

    void Amend(Order& od){
        auto io = orders.find(od.id);
        if(io != orders.end()){
            if(io->second.back()->symbol != od.symbol ||
               io->second.back()->type != od.type ||
               io->second.back()->side != od.side){
                cout << "AmendReject" << '\n';
                return;
            }
            //invalidate the old order and reNew
            io->second.back()->shares = 0;
            auto po = make_shared<Order>(od.id, od.time, od.symbol, od.type, od.side, od.price, od.shares);
            io->second.push_back(po);
            Add(po);
        }else{
            cout << "AmendReject" << '\n';
        }
    }

    void Cancel(unsigned long long id, unsigned long tm){
        auto io = orders.find(id);
        if(io != orders.end()){
            if(io->second.back()->shares > 0){
                io->second.back()->shares = 0;
                io->second.clear();
                orders.erase(io);
            }else{
                cout << "CancelRected" << '\n';
            };
        }else{
            cout << "CancelRected" << '\n';
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
            auto pa = ia->second.begin()->second.top();
            auto pb = ib->second.begin()->second.top();
            //skip cancelled orders
            while(pa->shares == 0){
                ia->second.begin()->second.pop();
                if(ia->second.begin()->second.empty()){
                    ia->second.erase(lowestAsk);
                    if(ia->second.empty()){
                        asks.erase(ia);
                        return;
                    }
                }
                pa = ia->second.begin()->second.top();
            }
            while(pb->shares == 0){
                ib->second.begin()->second.pop();
                if(ib->second.begin()->second.empty()){
                    ib->second.erase(highestBid);
                    if(ib->second.empty()){
                        bids.erase(ib);
                        return;
                    }
                }
                pb = ib->second.begin()->second.top();
            }
            if(pa->time > tm || pb->time > tm){
                break;
            }
            int size = pb->shares - pa->shares;
            if(size == 0){
                cout << pb->id << " : " << pa->id << " = " << pb->shares << '\n';
                ib->second.begin()->second.pop();
                orders[pb->id].clear(); //the order is close
                orders.erase(pb->id);
                ia->second.begin()->second.pop();
                orders[pa->id].clear();
                orders.erase(pa->id);
            }else if(size > 0){
                cout << pa->id << " = " << pa->shares << '\n';
                pb->shares = size;
                ia->second.begin()->second.pop();
                orders[pa->id].clear();
                orders.erase(pa->id);
            }else if(size < 0){
                cout << pb->id << " = " << pb->shares << '\n';
                pa->shares = -size;
                ib->second.begin()->second.pop();
                orders[pb->id].clear();
                orders.erase(pb->id);
            }
            if(ia->second.begin()->second.empty()){
                ia->second.erase(lowestAsk);
                if(ia->second.empty()){
                    asks.erase(ia);
                    break;
                }
            }
            if(ib->second.begin()->second.empty()){
                ib->second.erase(highestBid);//ib->second.erase(ib->second.begin());
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
    Order o3(110,0000003,"XYZ",'L','S',60.90,100);

    Order a2(11,0000001,"XYZ",'L','S',60.90,150);
    Order a3(110,0000001,"XYZ",'L','S',59.53,150);

    Matcher mc;
    mc.New(o1);
    mc.New(o2);
    mc.New(o3);

    mc.Amend(a2);
    mc.Amend(a3);

    mc.Cancel(112, 0000004);

    mc.Match(0000007);
    mc.Match(0000006, "XYZ");
    
    string line;
    vector<string> vs;
    vs.reserve(5);
    Matcher mc;
    while (getline(cin, line)){
        size_t start = 0;
        size_t end = line.find(' ');
        while (end != string::npos) {
            vs.push_back(line.substr(start, end - start));
            start = end + 1;
            end = line.find(' ', start);
        }
        vs.push_back(line.substr(start, end - start));

        if(vs[0] == "BUY" || vs[0] == "SELL"){
            if(vs.size()==5) {
                Order o(vs[0], vs[1], stoi(vs[2]), stoi(vs[3]), vs[4]);
                mc.New(o);
            }
        }else if(vs[0] == "MODIFY"){
            if(vs.size()==5) {
                cout << vs.size() << endl;
                Order o(vs[2], "GFD", stoi(vs[3]), stoi(vs[4]), vs[1]);
                mc.Modify(o);
            }
        }else if(vs[0] == "CANCEL"){
            if(vs.size()==2) {
                cout << vs.size() << endl;
                mc.Cancel(vs[1]);
            }
        }else if(vs[0] == "PRINT"){
            mc.Print();
        }
        vs.clear();

        //cout << line <<endl;
    }
    cout << "end" << endl;
    return 0;
}
