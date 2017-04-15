#include <vector>
#include <unordered_map>
using namespace std;

//split a given string given a delimiter
vector<string> splitString(string line, char delimiter);

//parse out substring given start and end delimiter
string parseString(string line, char start, char end);

//validate timestamp
bool validateTimeStamp(string timeStamp);

//compare two time values as string, returns 1 if (t1 > t2), 0 otherwise.
int timeCompare(string t1, string t2);

struct Tick{
  string time;
  string ticker;
  string asset;
  string bank;
  float bid;
  float ask;
};

class marketDataProcessor
{
  private:
    //Vector to store the ticks given by the user
    vector<Tick> ticks;

    //Vector to store all the queries given by the user
    vector<string> queries;

	//Vector to store invalid ticks 
    vector<string> invalidTicks;

	//Vector to store invalid ticks 
    vector<string> invalidQueries;

    //HashMap contains asset value as key and vector of indices for ticks under that asset as value
    unordered_map< string, vector< int > > assets;

    //HashMap contains ticker value as key and vector of indices for ticks under that ticker as value
    unordered_map< string, vector< int > > tickers;

    //generate maximum return given a vector of Ticks, returns a vector contains buy and sell instances.
    vector<int> maxReturn(vector<int> );

	//output all unique ticker values for given asset in the query
    void outputTickers(vector<int>, string);

    //output all unique bank values for given ticker in the query
    void outputBanks(vector<int>, string);

    //output the best buy and sell time, bank and values for maximized profit
    void outputReturn(vector<int>, string);

  public:
    //populate struct Tick from each line of input data
    void populateTick(string str, struct Tick& tick, int count);

    //read the input data file (ticks and queries)
    void load(istream& in);

    //process queries 
    void process(void);
};

