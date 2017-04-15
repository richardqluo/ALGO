#include "marketData.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <unordered_set>
#include <limits>

using namespace std;

void marketDataProcessor::load(istream& in){

  string line;

  //incremental index for storing ticks indices in assets and tickers map
  int curIdx = 0;
  while(getline(in, line)){

	string temp = "TICK:";
    size_t posTick = line.find(temp);

    //populate struct Tick with TICK data, push into the vector ticks
    if (posTick!=string::npos){
	  line.erase(posTick, temp.size());
      struct Tick tick;
      populateTick(line, tick, curIdx);
      ticks.push_back(tick);
      curIdx++;
    }
	else {
		temp = "QUERY:";
		//populate vector Query with QUERY data
		size_t posQuery = line.find(temp);
		if (posQuery!=string::npos){
		  line.erase(posQuery, temp.size());
		  queries.push_back(line);
		}
		else {
			break;
		}
	}
  }
  //output any invalid ticks
  if (invalidTicks.size() > 0) { 
	cout<< "Tick parsing ERROR: The following ticks don't match the expected format:\n" ;
	for (auto it = invalidTicks.begin(); it != invalidTicks.end();it++){
		cout<< "TICK:"  << *it << "\n";
	}
  }
}

void marketDataProcessor::process(void){
  vector<string>::iterator it;

  for (it = queries.begin(); it != queries.end();it++){
    //for query allTickersForAsset
    size_t pos = (*it).find("allTickersForAsset");
    if (pos!=string::npos){
      //find the "asset"
	  string sub = parseString(*it, '"', '"');
	  if (sub.empty()){
		invalidQueries.push_back(*it);
		continue;
	  }
      vector<int> values = assets[sub];
	  if(values.empty()){
		cout<< "\nNo tick for asset "  << sub << "\n";
		continue;
	  }
      outputTickers(values, *it);
	  continue;
    }

    //for query allBanksForTicker
    pos = (*it).find("allBanksForTicker");
    if (pos!=string::npos){
      //find the "ticker"
	  string sub = parseString(*it, '"', '"');
	  if (sub.empty()){
		invalidQueries.push_back(*it);
		continue;
	  }
      vector<int> values = tickers[sub];
	  if(values.empty()){
		cout<< "\nNo tick for ticker "  << sub << "\n";
		continue;
	  }
      outputBanks(values, *it);
	  continue;
    }

    //for query bestReturnsForTickerInTimeRange
    pos = (*it).find("bestReturnsForTickerInTimeRange");
    if (pos!=string::npos){
	  string subs = parseString(*it, '(', ')');
	  if (subs.empty()){
		invalidQueries.push_back(*it);
		continue;
	  }
      //parse out ticker, startTime and endTime
      vector<string> tokens = splitString(subs, ',');

      //validate input
      if(tokens.size() != 3){
		invalidQueries.push_back(*it);
		continue;
      }

	  string sub = parseString(tokens[0], '"', '"');
      //string sub = tokens[0];
      //sub.erase(0,1);
      //sub.erase(sub.size()-1);

      vector<int> values = tickers[sub];
	  if(values.empty()){
		cout<< "\nNo ticks for asset "  << sub << "\n";
		continue;
	  }

      //parse out startTime and endTime
      string startTime = parseString(tokens[1], '"', '"');
      string endTime = parseString(tokens[2], '"', '"');

	  vector<int> temp;
	  //find all ticks during the time span
	  for (int i= 0; i < values.size(); i++){
        int cmps = timeCompare(ticks[values[i]].time, startTime);
		int cmpe = timeCompare(endTime, ticks[values[i]].time);
		if( (cmps == 1) && (cmpe == 1) ){
			temp.push_back(values[i]);
		}
		else if (cmps == 0 || cmpe == 0){
			invalidQueries.push_back(*it);
			continue;
		}
	  }
	  //given a time span, need at least 2 tick to buy and sell
	  if(temp.size() < 2){
		cout<< "\nNot enough ticks for asset "  << sub << " between given start and end time\n" ;
		continue;
	  }

      //calucalte max return during the time span
      vector<int> mr = maxReturn(temp);

	  outputReturn(mr, *it);
	  continue;
    }
  }

  //output any invalid ticks
  if (invalidQueries.size() > 0) { 
	cout<< "Query parsing ERROR: The following queries don't match the expected format:\n" ;
	for (auto it = invalidQueries.begin(); it != invalidQueries.end();it++){
		cout<< "QUERY:"  << *it << "\n";
	}
  }
}

void marketDataProcessor::populateTick(string str, struct Tick& tick, int curIdx){

  //Split string into tokens to populate tick
  vector<string> tokens = splitString(str, '|');

  if (tokens.size() != 6){
	invalidTicks.push_back(str);
	return;
  }
  string t = tokens.at(0);
  
  if (!validateTimeStamp(t)){
	  invalidTicks.push_back(str);
	  return;
  }

  tick.time = t;
  tick.ticker = tokens.at(1);
  tick.asset = tokens.at(2);
  tick.bank = tokens.at(3);
  tick.bid = atof(tokens.at(4).c_str());
  tick.ask = atof(tokens.at(5).c_str());

  tickers[tick.ticker].push_back(curIdx);
  assets[tick.asset].push_back(curIdx);
  
}

vector<int> marketDataProcessor::maxReturn(vector<int> values){

  float minAsk = ticks[values[0]].ask;
  //initialize maxProfit with buy at the first tick ask price and sell at the second tick bid price 
  float maxProfit = ticks[values[1]].bid - ticks[values[0]].ask;
  int minBuy = 0, maxSell = 1;
  int buyPos = 0;

  //foward through all ticks with O(n) complexity, update min ask price if it's lesser or more than
  //any previous one and recalculate profit and update max profit accordingly  
  for (int i = 1; i < values.size()-1; i++){
	//ask tick to buy must be ahead of bid tick to sell 
    float curBid = ticks[values[i+1]].bid;
    float curAsk = ticks[values[i]].ask;

    if(curAsk < minAsk){
      minAsk = curAsk;
      buyPos = i;
    }

    float profit = curBid - minAsk;
    if(profit > maxProfit){
      minBuy = buyPos;
      maxSell = i+1;
      maxProfit = profit;
    }

  }

  //return tick for min buy and max sell indices
  vector<int> temp;
  temp.push_back(values[minBuy]); temp.push_back(values[maxSell]);
  return temp;

}

void marketDataProcessor::outputReturn(vector<int> values,string query){

  Tick minBuy = ticks[values[0]];
  Tick maxSell = ticks[values[1]];
  cout<<"RESULT: "<<query<<" - ";
  if (maxSell.bid - minBuy.ask > 0){
  cout<< "BUY:"<<minBuy.time<<"|"<<minBuy.ask<<"|"<<minBuy.bank<<";";
  cout<< " SELL:"<<maxSell.time<<"|"<<maxSell.bid<<"|"<<maxSell.bank<<"\n";
  }
  else{
	  cout<< "No possible profit\n";
  }
}

void marketDataProcessor::outputTickers(vector<int> values, string query){

  vector<int>::iterator it = values.begin();
  unordered_set<string> unique;

  cout<<"RESULT: "<<query<<" - ";
  cout << ticks[*it].ticker;
  unique.insert(ticks[*it].ticker);
  ++it;

  while(it != values.end()){

    unordered_set<string>::const_iterator got = unique.find(ticks[*it].ticker);
    if(got == unique.end()){
		//only unique item is output and add to hashset
      cout << "," << ticks[*it].ticker;
      unique.insert(ticks[*it].ticker);
    }
    ++it;
  }
  cout<<"\n";
}

void marketDataProcessor::outputBanks(vector<int> values, string query){
  
  unordered_set<string> unique;
  vector<int>::iterator it = values.begin();

  cout<<"RESULT: "<<query<<" - ";
  cout << ticks[*it].bank;
  unique.insert(ticks[*it].bank);
  ++it;

  while(it != values.end()){

    unordered_set<string>::const_iterator got = unique.find(ticks[*it].bank);
    if(got == unique.end()){
		//only unique item is output and add to hashset   
      cout << "," << ticks[*it].bank;
      unique.insert(ticks[*it].bank);
    }
    ++it;
  }
  cout<<"\n";
}

bool validateTimeStamp(string timeStamp){

  vector<string> t_tokens = splitString(timeStamp, ':');

  if(t_tokens.size() != 3 ){
	return false;
  }
  vector<string>::iterator it;
  for (it = t_tokens.begin(); it != t_tokens.end();it++){
	if(it->size() != 2){
		return false;
	}
  }
  return true;
}

int timeCompare(string t1, string t2){

  if(!validateTimeStamp(t1) || !validateTimeStamp(t2)){
    return 0;
  }

  vector<string> t1_tokens = splitString(t1, ':');
  vector<string> t2_tokens = splitString(t2, ':');
  for(int i = 0; i < 3; i++){
    int v1 = atoi(t1_tokens[i].c_str());
    int v2 = atoi(t2_tokens[i].c_str());
    if(v1 > v2)
      return 1;
    if(v1 < v2)
      return -1;
  }
  return 1;

}

string parseString(string line, char start, char end){

  string r;
  size_t s = line.find(start);
  size_t e = line.find_last_of(end);
  if (s!=string::npos && e!=string::npos){
	r = line.substr(s + 1, e -s-1);
  }
  return r;
}

vector<string> splitString(string line, char delimiter){
  vector<string> tokens;

  if (line.at(0) == delimiter ||  line.at(line.length()-1) == delimiter)
		return tokens;

  stringstream ss(line);
  string temp;

  while (getline(ss, temp, delimiter)) {
    tokens.push_back(temp);
  }

  return tokens;
}

//int main (int argc, char **argv){
//
//  marketDataProcessor md;
//
//  //read the input data
//  md.load(cin);
//
//  //process each query 
//  md.process();
//}
